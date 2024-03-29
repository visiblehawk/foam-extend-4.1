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
    Foam::writeFunctions

Description
    Various functions for collecting and writing binary data.

SourceFiles
    writeFunctions.C

\*---------------------------------------------------------------------------*/

#ifndef writeFunctions_H
#define writeFunctions_H

#include "labelList.H"
#include "scalarList.H"
#include "floatScalar.H"
#include "OFstream.H"
#include "dynamicLabelList.H"
#include "point.H"

using namespace Foam;

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{


/*---------------------------------------------------------------------------*\
                           Class writeFuns Declaration
\*---------------------------------------------------------------------------*/

class writeFuns
{
    // Private member functions

        //- Swap halves of word
        static void swapWord(int32_t& word32);

        //- Swap halves of word
        static void swapWords(const label nWords, int32_t* words32);


public:

    //- Write ascii or binary. If binary optionally in-place swaps argument
    static void write(std::ostream&, const bool, DynamicList<floatScalar>&);
    static void write(std::ostream&, const bool, dynamicLabelList&);

    //- Write ascii or binary. If binary optionally in-place swaps argument
    static void write(std::ostream&, const bool, List<floatScalar>&);
    static void write(std::ostream&, const bool, labelList&);

    //- Append point to DynamicList
    static void insert(const point&, DynamicList<floatScalar>& dest);

    //- Append elements of labelList to DynamicList
    static void insert(const labelList&, dynamicLabelList&);
    //- Append elements of scalarList to DynamicList
    static void insert(const scalarList&, DynamicList<floatScalar>&);

    //- Append elements of scalarList to DynamicList using map
    static void insert
    (
        const labelList& map,
        const scalarList& source,
        DynamicList<floatScalar>&
    );

    //- Append points to DynamicList of floats
    static void insert(const List<point>& source, DynamicList<floatScalar>&);
    //- As above but using map
    static void insert
    (
        const labelList& map,
        const List<point>& source,
        DynamicList<floatScalar>&
    );

};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
