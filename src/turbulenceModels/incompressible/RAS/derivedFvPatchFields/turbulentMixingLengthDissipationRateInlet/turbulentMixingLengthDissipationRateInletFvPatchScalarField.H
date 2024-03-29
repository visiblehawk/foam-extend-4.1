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
    Foam::incompressible::
        turbulentMixingLengthDissipationRateInletFvPatchScalarField

Description
    Calculate epsilon via the mixing length [m]

    Example of the boundary condition specification:
    \verbatim
        inlet
        {
            type            turbulentMixingLengthDissipationRateInlet;
            mixingLength    0.005;         // 5 mm
            value           uniform 200;   // placeholder
        }
    \endverbatim

SourceFiles
    turbulentMixingLengthDissipationRateInletFvPatchScalarField.C

\*---------------------------------------------------------------------------*/

#ifndef incompressibleturbulentMixingLengthDissipationRateInlet_H
#define incompressibleturbulentMixingLengthDissipationRateInlet_H

#include "inletOutletFvPatchFields.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace incompressible
{

/*---------------------------------------------------------------------------*\
       Class turbulentMixingLengthDissipationRateInletFvPatch Declaration
\*---------------------------------------------------------------------------*/

class turbulentMixingLengthDissipationRateInletFvPatchScalarField
:
    public inletOutletFvPatchScalarField
{
    // Private data

        //- turbulent length scale
        scalar mixingLength_;

        //- Name of the flux field
        word phiName_;

        //- Name of the turbulent kinetic energy field
        word kName_;


public:

    //- Runtime type information
    TypeName("turbulentMixingLengthDissipationRateInlet");


    // Constructors

        //- Construct from patch and internal field
        turbulentMixingLengthDissipationRateInletFvPatchScalarField
        (
            const fvPatch&,
            const DimensionedField<scalar, volMesh>&
        );

        //- Construct from patch, internal field and dictionary
        turbulentMixingLengthDissipationRateInletFvPatchScalarField
        (
            const fvPatch&,
            const DimensionedField<scalar, volMesh>&,
            const dictionary&
        );

        //- Construct by mapping given
        //  turbulentMixingLengthDissipationRateInletFvPatchScalarField
        //  onto a new patch
        turbulentMixingLengthDissipationRateInletFvPatchScalarField
        (
            const turbulentMixingLengthDissipationRateInletFvPatchScalarField&,
            const fvPatch&,
            const DimensionedField<scalar, volMesh>&,
            const fvPatchFieldMapper&
        );

        //- Construct as copy
        turbulentMixingLengthDissipationRateInletFvPatchScalarField
        (
            const turbulentMixingLengthDissipationRateInletFvPatchScalarField&
        );

        //- Construct and return a clone
        virtual tmp<fvPatchScalarField> clone() const
        {
            return tmp<fvPatchScalarField>
            (
                new turbulentMixingLengthDissipationRateInletFvPatchScalarField
                (
                    *this
                )
            );
        }

        //- Construct as copy setting internal field reference
        turbulentMixingLengthDissipationRateInletFvPatchScalarField
        (
            const turbulentMixingLengthDissipationRateInletFvPatchScalarField&,
            const DimensionedField<scalar, volMesh>&
        );

        //- Construct and return a clone setting internal field reference
        virtual tmp<fvPatchScalarField> clone
        (
            const DimensionedField<scalar, volMesh>& iF
        ) const
        {
            return tmp<fvPatchScalarField>
            (
                new turbulentMixingLengthDissipationRateInletFvPatchScalarField
                (
                    *this,
                    iF
                )
            );
        }


    // Member functions

        //- Update the coefficients associated with the patch field
        virtual void updateCoeffs();

        //- Write
        virtual void write(Ostream&) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace incompressible
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
