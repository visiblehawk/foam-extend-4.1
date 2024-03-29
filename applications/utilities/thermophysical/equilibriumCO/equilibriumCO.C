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

Description
    Calculates the equilibrium level of carbon monoxide

\*---------------------------------------------------------------------------*/

#include "argList.H"
#include "objectRegistry.H"
#include "foamTime.H"
#include "dictionary.H"
#include "IFstream.H"
#include "OSspecific.H"
#include "IOmanip.H"

#include "specieThermo.H"
#include "janafThermo.H"
#include "perfectGas.H"
#include "SLPtrList.H"

using namespace Foam;

typedef specieThermo<janafThermo<perfectGas> > thermo;

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
// Main program:

int main(int argc, char *argv[])
{

#   include "setRootCase.H"

#   include "createTime.H"

    Info<< nl << "Reading Burcat data IOdictionary" << endl;

    IOdictionary CpData
    (
        IOobject
        (
            "BurcatCpData",
            runTime.constant(),
            runTime,
            IOobject::MUST_READ,
            IOobject::NO_WRITE,
            false
        )
    );



    scalar T = 3000.0;

    SLPtrList<thermo> EQreactions;

    EQreactions.append
    (
        new thermo
        (
            thermo(CpData.lookup("CO2"))
         ==
            thermo(CpData.lookup("CO"))
          + 0.5*thermo(CpData.lookup("O2"))
        )
    );

    EQreactions.append
    (
        new thermo
        (
            thermo(CpData.lookup("O2"))
         ==
            2.0*thermo(CpData.lookup("O"))
        )
    );

    EQreactions.append
    (
        new thermo
        (
            thermo(CpData.lookup("H2O"))
         ==
            thermo(CpData.lookup("H2"))
          + 0.5*thermo(CpData.lookup("O2"))
        )
    );

    EQreactions.append
    (
        new thermo
        (
            thermo(CpData.lookup("H2O"))
         ==
            thermo(CpData.lookup("H"))
          + thermo(CpData.lookup("OH"))
        )
    );


    for
    (
        SLPtrList<thermo>::iterator EQreactionsIter = EQreactions.begin();
        EQreactionsIter != EQreactions.end();
        ++EQreactionsIter
    )
    {
        Info<< "Kc(EQreactions) = " << EQreactionsIter().Kc(T) << endl;
    }


    Info<< nl << "end" << endl;

    return 0;
}


// ************************************************************************* //

