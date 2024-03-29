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
    Foam::incompressible::LESModels::dynMixedSmagorinsky

Description
    The Mixed Isochoric Smagorinsky Model for incompressible flows.

    The mixed model is a linear combination of an eddy viscosity model
    with a scale similarity model.
    @verbatim
          B = (L + C) + R = (F(v*v) - F(v)*F(v)) + R
    @endverbatim

    The algebraic eddy viscosity SGS model is founded on the assumption
    that local equilibrium prevails, hence
    @verbatim
        R = 2/3*rho*k*I - 2*nuEff*dev(D)
    where
        k = cI*delta^2*||D||^2
        nuEff = ck*sqrt(k)*delta + nu
    @endverbatim

    The Leonard and cross contributions are incorporated
    by adding,
    @verbatim
        + div(((filter(U*U) - filter(U)*filter(U)) -
          0.333*I*tr(filter(U*U) - filter(U)*filter(U))))
        + div((filter(U*epsilon) - filter(U)*filter(epsilon)))
    @endverbatim
    to the rhs. of the equations.  This version implements filtering to
    evaluate the coefficients in the model.

SourceFiles
    dynMixedSmagorinsky.C

\*---------------------------------------------------------------------------*/

#ifndef dynMixedSmagorinsky_H
#define dynMixedSmagorinsky_H

#include "dynSmagorinsky.H"
#include "scaleSimilarity.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace incompressible
{
namespace LESModels
{

/*---------------------------------------------------------------------------*\
                           Class dynMixedSmagorinsky Declaration
\*---------------------------------------------------------------------------*/

class dynMixedSmagorinsky
:
    public scaleSimilarity,
    public dynSmagorinsky
{
    // Private Member Functions

        // Disallow default bitwise copy construct and assignment
        dynMixedSmagorinsky(const dynMixedSmagorinsky&);
        dynMixedSmagorinsky& operator=(const dynMixedSmagorinsky&);

public:

    //- Runtime type information
    TypeName("dynMixedSmagorinsky");

    // Constructors

        //- Constructors from components
        dynMixedSmagorinsky
        (
            const volVectorField& U,
            const surfaceScalarField& phi,
            transportModel& transport,
            const word& turbulenceModelName = turbulenceModel::typeName,
            const word& modelName = typeName
        );


    // Destructor

        ~dynMixedSmagorinsky()
        {}


    // Member Functions

        //- Return SGS kinetic energy
        tmp<volScalarField> k() const;

        //- Return sub-grid disipation rate
        tmp<volScalarField> epsilon() const;

        //- Return the sub-grid stress tensor.
        tmp<volSymmTensorField> B() const;

        //- Return the effective sub-grid turbulence stress tensor
        //  including the laminar stress
        tmp<volSymmTensorField> devBeff() const;

        //- Returns div(B).
        // This is the additional term due to the filtering of the NSE.
        tmp<fvVectorMatrix> divDevBeff() const;

        //- Correct Eddy-Viscosity and related properties
        void correct(const tmp<volTensorField>& gradU);

        //- Read LESProperties dictionary
        bool read();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace LESModels
} // End namespace incompressible
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
