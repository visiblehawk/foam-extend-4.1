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
    PODecomposition

Author
    Hrvoje Jasak, Wikki Ltd.  All rights reserved.

Description
    Calculates proper orthogonal decomposition of a given field set

\*---------------------------------------------------------------------------*/

#include "fvCFD.H"
#include "PODOrthoNormalBases.H"
#include "VectorSpace.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

int main(int argc, char *argv[])
{

#   include "setRootCase.H"

#   include "createTime.H"

    // Get times list
    instantList Times = runTime.times();

    const label startTime = 1;
    const label endTime = Times.size();
    const label nSnapshots = Times.size() - 1;

    Info << "Number of snapshots: " << nSnapshots << endl;

    // Create a list of snapshots
    PtrList<volVectorField> fields(nSnapshots);

    runTime.setTime(Times[startTime], startTime);

#   include "createMesh.H"

    IOdictionary PODsolverDict
    (
        IOobject
        (
            "PODsolverDict",
            runTime.system(),
            mesh,
            IOobject::MUST_READ,
            IOobject::NO_WRITE
        )
    );

    scalar accuracy =
        readScalar
        (
            PODsolverDict.subDict("scalarTransportCoeffs").lookup("accuracy")
        );

    Info << "Seeking accuracy: " << accuracy << endl;

    word fieldName
    (
        PODsolverDict.subDict("scalarTransportCoeffs").lookup("velocity")
    );

    label snapI = 0;

    labelList timeIndices(nSnapshots);

    for (label i = startTime; i < endTime; i++)
    {
        runTime.setTime(Times[i], i);

        Info<< "Time = " << runTime.timeName() << endl;

        mesh.readUpdate();

        Info<< "    Reading " << fieldName << endl;
        fields.set
        (
            snapI,
            new volVectorField
            (
                IOobject
                (
                    fieldName,
                    runTime.timeName(),
                    mesh,
                    IOobject::MUST_READ
                ),
                mesh
            )
        );

        // Rename the field
        fields[snapI].rename(fieldName + name(i));
        timeIndices[snapI] = i;
        snapI++;

        Info<< endl;
    }

    timeIndices.setSize(snapI);

    vectorPODOrthoNormalBase eb(fields, accuracy);

    const RectangularMatrix<vector>& coeffs = eb.interpolationCoeffs();

    // Check all snapshots
    forAll (fields, fieldI)
    {
        runTime.setTime(Times[timeIndices[fieldI]], timeIndices[fieldI]);

        volVectorField reconstruct
        (
            IOobject
            (
                fieldName + "PODreconstruct",
                runTime.timeName(),
                mesh,
                IOobject::NO_READ
            ),
            mesh,
            dimensionedVector
            (
                "zero",
                fields[fieldI].dimensions(),
                vector::zero
            )
        );

        for (label baseI = 0; baseI < eb.baseSize(); baseI++)
        {
            reconstruct +=
                cmptMultiply
                (
                    eb.orthoField(baseI),
                    coeffs[fieldI][baseI]
                );
        }

        scalar sumFieldError =
            sumMag
            (
                reconstruct.internalField()
              - fields[fieldI].internalField()
            );

        scalar measure = sumMag(fields[fieldI].internalField()) + SMALL;

        Info<< "Field error: absolute = " << sumFieldError << " relative = "
            << sumFieldError/measure << " measure = " << measure
            << endl;

        reconstruct.write();
    }

    // Write out all fields
    runTime.setTime(Times[startTime], startTime);
    Info<< "Writing POD base for Time = " << runTime.timeName() << endl;

    for (label i = 0; i < eb.baseSize(); i++)
    {
        eb.orthoField(i).write();
    }

    Info << endl;

    Info<< "End\n" << endl;

    return(0);
}


// ************************************************************************* //
