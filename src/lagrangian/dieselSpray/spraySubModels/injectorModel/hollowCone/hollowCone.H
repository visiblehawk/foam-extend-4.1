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
    Foam::hollowConeInjector

Description
    A hollow cone injector with randomly distributed parcels between
    the inner and outer cone angle.

    Set inner angle to zero for solid cone.

\*---------------------------------------------------------------------------*/

#ifndef hollowConeInjector_H
#define hollowConeInjector_H

#include "injectorModel.H"
#include "scalarList.H"
#include "pdf.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
/*---------------------------------------------------------------------------*\
                           Class hollowConeInjector Declaration
\*---------------------------------------------------------------------------*/

class hollowConeInjector
:
    public injectorModel
{

private:

    // Private data

        dictionary hollowConeDict_;

        autoPtr<pdf> dropletPDF_;
        scalarList innerAngle_;
        scalarList outerAngle_;

public:

    //- Runtime type information
        TypeName("hollowConeInjector");


    // Constructors

        //- Construct from components
        hollowConeInjector
        (
            const dictionary& dict,
            spray& sm
        );


    // Destructor

        ~hollowConeInjector();


    // Member Functions

        //- Return the injected droplet diameter
        scalar d0(const label injector, const scalar time) const;

        //- Return the spray angle of the injector
        vector direction
        (
            const label injector,
            const label hole,
            const scalar time,
            const scalar d
        ) const;

        scalar velocity
        (
            const label i,
            const scalar time
        ) const;

        scalar averageVelocity
        (
            const label i
        ) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
