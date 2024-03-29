/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | foam-extend: Open Source CFD
   \\    /   O peration     | Version:     4.1
    \\  /    A nd           | Web:         http://www.foam-extend.org
     \\/     M anipulation  | For copyright notice see file Copyright
-------------------------------------------------------------------------------
License
    This file is part of foam-extend.

    foam-extend is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the
    Free Software Foundation, either version 3 of the License, or (at your
    option) any later version.

    foam-extend is distributed in the hope that it will be useful, but
    WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with foam-extend.  If not, see <http://www.gnu.org/licenses/>.

Application
    Mach

Description
    Calculates and optionally writes the local Mach number from the velocity
    field U at each time.

    The -nowrite option just outputs the max value without writing the field.

\*---------------------------------------------------------------------------*/

#include "calc.H"
#include "basicPsiThermo.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

void Foam::calc(const argList& args, const Time& runTime, const fvMesh& mesh)
{
    bool writeResults = !args.optionFound("noWrite");

    IOobject Uheader
    (
        "U",
        runTime.timeName(),
        mesh,
        IOobject::MUST_READ
    );

    IOobject Theader
    (
        "T",
        runTime.timeName(),
        mesh,
        IOobject::MUST_READ
    );

    // Check U and T exists
    if (Uheader.headerOk() && Theader.headerOk())
    {
        autoPtr<volScalarField> MachPtr;

        volVectorField U(Uheader, mesh);

        if (isFile(runTime.constantPath()/"thermophysicalProperties"))
        {
            // thermophysical Mach
            autoPtr<basicPsiThermo> thermo
            (
                basicPsiThermo::New(mesh)
            );

            volScalarField Cp = thermo->Cp();
            volScalarField Cv = thermo->Cv();

            MachPtr.set
            (
                new volScalarField
                (
                    IOobject
                    (
                        "Ma",
                        runTime.timeName(),
                        mesh
                    ),
                    mag(U)/(sqrt((Cp/Cv)*(Cp - Cv)*thermo->T()))
                )
            );
        }
        else
        {
            // thermodynamic Mach
            IOdictionary thermoProps
            (
                IOobject
                (
                    "thermodynamicProperties",
                    runTime.constant(),
                    mesh,
                    IOobject::MUST_READ,
                    IOobject::NO_WRITE
                )
            );

            dimensionedScalar R(thermoProps.lookup("R"));
            dimensionedScalar Cv(thermoProps.lookup("Cv"));

            volScalarField T(Theader, mesh);

            MachPtr.set
            (
                new volScalarField
                (
                    IOobject
                    (
                        "Ma",
                        runTime.timeName(),
                        mesh
                    ),
                    mag(U)/(sqrt(((Cv + R)/Cv)*R*T))
                )
            );
        }

        Info<< "Mach max : " << max(MachPtr()).value() << endl;

        if (writeResults)
        {
            MachPtr().write();
        }
    }
    else
    {
        Info<< "    Missing U or T" << endl;
    }
}


// ************************************************************************* //
