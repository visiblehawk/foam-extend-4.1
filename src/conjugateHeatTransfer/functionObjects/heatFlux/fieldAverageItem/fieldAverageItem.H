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
    Foam::fieldAverageItem

Description
    Helper class to describe what form of averaging to apply. A set will be
    applied to each base field in Foam::fieldAverage, of the form:

    \verbatim
        {
            mean            on;
            prime2Mean      on;
            base            time; // iteration
        }
    \endverbatim

SourceFiles
    fieldAverageItem.C
    fieldAverageItemIO.C

\*---------------------------------------------------------------------------*/

#ifndef fieldAverageItem_H
#define fieldAverageItem_H

#include "NamedEnum.H"
#include "Switch.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// Forward declaration of classes
class Istream;
class Ostream;

// Forward declaration of friend functions and operators
class fieldAverageItem;
Istream& operator>>(Istream&, fieldAverageItem&);
Ostream& operator<<(Ostream&, const fieldAverageItem&);


/*---------------------------------------------------------------------------*\
                      Class fieldAverageItem Declaration
\*---------------------------------------------------------------------------*/

class fieldAverageItem
{
public:

    // Public data

        //- Enumeration defining the averaging base type
        enum baseType
        {
            ITER,
            TIME
        };


private:

    // Private data

        //- Field name
        word fieldName_;

        //- Compute mean flag
        Switch mean_;

        //- Compute prime-squared mean flag
        Switch prime2Mean_;

        //- Averaging base type names
        static const NamedEnum<baseType, 2> baseTypeNames_;

        //- Averaging base type
        baseType base_;


public:

    // Constructors

        //- Construct null
        fieldAverageItem();

        //- Construct from Istream
        fieldAverageItem(Istream&);

        //- Construct as copy
        fieldAverageItem(const fieldAverageItem&);


    //- Destructor
    ~fieldAverageItem();


    // Member Functions

        // Access

            //- Return const access to the field name
            const word& fieldName() const
            {
                return fieldName_;
            }

            //- Return const access to the mean flag
            const Switch& mean() const
            {
                return mean_;
            }

            //- Return const access to the prime-squared mean flag
            const Switch& prime2Mean() const
            {
                return prime2Mean_;
            }

            //- Return averaging base type name
            const word base() const
            {
                return baseTypeNames_[base_];
            }

            //- Return true if base is ITER
            Switch ITERBase() const
            {
                return base_ == ITER;
            }

            //- Return true if base is time
            Switch timeBase() const
            {
                return base_ == TIME;
            }


    // Member Operators

        void operator=(const fieldAverageItem&);


    // Friend Operators

        friend bool operator==
        (
            const fieldAverageItem& a,
            const fieldAverageItem& b
        )
        {
            return
                a.fieldName_ == b.fieldName_
             && a.mean_ == b.mean_
             && a.prime2Mean_ == b.prime2Mean_
             && a.base_ == b.base_;
        }

        friend bool operator!=
        (
            const fieldAverageItem& a,
            const fieldAverageItem& b
        )
        {
            return !(a == b);
        }


    // IOstream Operators

        friend Istream& operator>>(Istream&, fieldAverageItem&);
        friend Ostream& operator<<(Ostream&, const fieldAverageItem&);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
