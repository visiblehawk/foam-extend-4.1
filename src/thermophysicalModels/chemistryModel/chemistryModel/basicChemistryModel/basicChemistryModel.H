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

Class
    Foam::basicChemistryModel

Description
    Base class for chemistry models

SourceFiles
    basicChemistryModelI.H
    basicChemistryModel.C

\*---------------------------------------------------------------------------*/

#ifndef basicChemistryModel_H
#define basicChemistryModel_H

#include "IOdictionary.H"
#include "Switch.H"
#include "volFields.H"
#include "volFieldsFwd.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// Forward declaration of classes
class fvMesh;

/*---------------------------------------------------------------------------*\
                   class basicChemistryModel Declaration
\*---------------------------------------------------------------------------*/

class basicChemistryModel
:
    public IOdictionary
{
    // Private Member Functions

        //- Construct as copy (not implemented)
        basicChemistryModel(const basicChemistryModel&);

        //- Disallow default bitwise assignment
        void operator=(const basicChemistryModel&);


protected:

    // Protected data

        //- Reference to the mesh database
        const fvMesh& mesh_;

        //- Chemistry activation switch
        Switch chemistry_;

        //- Latest estimation of integration step
        // HR 14.12.18: Upgraded to volScalarField in order to enable automatic
        // mapping/resizing after a topo change. Also needed for PLB.
        volScalarField deltaTChem_;


    // Protected member functions

        //- Return non-const access to the latest estimation of integration
        //  step, e.g. for multi-chemistry model
        scalarField& deltaTChem();


public:

    //- Runtime type information
    TypeName("basicChemistryModel");


    // Constructors

        //- Construct from mesh
        basicChemistryModel(const fvMesh& mesh, const objectRegistry&);


    //- Destructor
    virtual ~basicChemistryModel();


    // Member Functions

        //- Return const access to the mesh database
        inline const fvMesh& mesh() const;

        //- Chemistry activation switch
        inline Switch chemistry() const;

        //- Return the latest estimation of integration step
        inline const scalarField& deltaTChem() const;


        // Functions to be derived in derived classes

            // Fields

                //- Return const access to chemical source terms
                virtual tmp<volScalarField> RR(const label i) const = 0;


            // Chemistry solution

                //- Solve the reaction system for the given start time and
                //  timestep and return the characteristic time
                virtual scalar solve(const scalar t0, const scalar deltaT) = 0;

                //- Return the chemical time scale
                virtual tmp<volScalarField> tc() const = 0;

                //- Return source for enthalpy equation [kg/m/s3]
                virtual tmp<volScalarField> Sh() const = 0;

                //- Return the heat release, i.e. enthalpy/sec [m2/s3]
                virtual tmp<volScalarField> dQ() const = 0;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "basicChemistryModelI.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
