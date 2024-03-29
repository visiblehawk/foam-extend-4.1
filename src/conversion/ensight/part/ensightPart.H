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
    Foam::ensightPart

Description
    Base class for ensightPartCells and ensightPartFaces

SourceFiles
    ensightPart.C
    ensightPartIO.C
    ensightPartI.H

\*---------------------------------------------------------------------------*/

#ifndef ensightPart_H
#define ensightPart_H

#include "ensightFile.H"
#include "ensightGeoFile.H"
#include "typeInfo.H"
#include "labelList.H"
#include "polyMesh.H"
#include "Field.H"
#include "IOPtrList.H"
#include "IOstream.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// Forward declaration of friend functions and operators
class ensightPart;

Ostream& operator<<(Ostream&, const ensightPart&);
ensightGeoFile& operator<<(ensightGeoFile&, const ensightPart&);


/*---------------------------------------------------------------------------*\
                           Class ensightPart Declaration
\*---------------------------------------------------------------------------*/

class ensightPart
{
    // Private data

        // Static data members
        static wordList elemTypes_;


protected:

    // Protected data

        //- part number
        label number_;

        //- part name (or description)
        string name_;

        //- simple labelList with a name
        labelListList elemLists_;

        //- start offset for elemLists_
        label offset_;

        //- number of elements in this part
        label size_;

        //- cell or face data
        bool isCellData_;

        //- material id (numeric)
        label matId_;

        //- mesh reference used
        const polyMesh* meshPtr_;


    // Protected Classes

        //- track the points used by the part and map global to local indices
        class localPoints
        {
        public:
            //- number of points used
            label nPoints;

            //- map global to local indices
            labelList list;

            // null constructor
            localPoints()
            :
                nPoints(0),
                list(0)
            {}

            // construct for mesh points
            localPoints(const polyMesh& pMesh)
            :
                nPoints(0),
                list(pMesh.points().size(), -1)
            {}
        };


    // Protected Member Functions

        //- reconstruct contents from Istream
        void reconstruct(Istream&);

        //- check for fully defined fields
        bool isFieldDefined(const scalarList&) const;

        //- write the part header
        void writeHeader(ensightFile&, bool withDescription=false) const;

        //- write a scalar field for idList
        void writeFieldList
        (
            ensightFile& os,
            const scalarList& field,
            const labelList& idList
        ) const;

        //- track points used
        virtual localPoints calcLocalPoints() const
        {
            return localPoints();
        }

        //- write connectivities
        virtual void writeConnectivity
        (
            ensightGeoFile& os,
            const string& key,
            const labelList& idList,
            const labelList& pointMap
        ) const
        {}


public:

    //- Runtime type information
    TypeName("ensightPart");


    // Constructors

        //- Construct null
        ensightPart();

        //- Construct empty part with number and description
        ensightPart(label partNumber, const string& partDescription);

        //- Construct empty part with number and description
        ensightPart
        (
            label partNumber,
            const string& partDescription,
            const polyMesh& pMesh
        );

        //- Construct as copy
        ensightPart(const ensightPart&);


    // Selectors

        // Declare run-time constructor selection table
        declareRunTimeSelectionTable
        (
            autoPtr,
            ensightPart,
            istream,
            (
                Istream& is
            ),
            (is)
        );

        //- Construct and return clone
        autoPtr<ensightPart> clone() const
        {
            return autoPtr<ensightPart>(new ensightPart(*this));
        };

        //- Construct on freestore from Istream
        static autoPtr<ensightPart> New(Istream& is);


    //- Destructor
    virtual ~ensightPart();


    // Static members

        virtual wordList const& elementTypes() const
        {
            return elemTypes_;
        }


    // Access

        //- number of elements in this part
        label size() const
        {
            return size_;
        }

        //- represents cell data
        bool isCellData() const
        {
            return isCellData_;
        }

        //- represents face data
        bool isFaceData() const
        {
            return !isCellData_;
        }

        //- part number
        label number() const
        {
            return number_;
        }

        //- part name or description
        const string& name() const
        {
            return name_;
        }

        //- material id
        label materialId() const
        {
            return matId_;
        }

        //- non-const access
        void name(const string& value)
        {
            name_ = value;
        }

        void materialId(const label value)
        {
            matId_ = value;
        }

        //- offset for element ids
        label offset() const
        {
            return offset_;
        }


    // Edit

        //- renumber elements
        void renumber(labelList const&);

        //- write summary information about the object
        bool writeSummary(Ostream&) const;

        //- write reconstruction information for the object
        bool writeData(Ostream&) const;

        //- write geometry
        void writeGeometry(ensightGeoFile&) const;

        //- write scalar field
        void writeScalarField
        (
            ensightFile&,
            const scalarList& field
        ) const;

        //- write vector field components
        void writeVectorField
        (
            ensightFile&,
            const scalarList& field0,
            const scalarList& field1,
            const scalarList& field2
        ) const;


        //- write generalized field components
        template <class Type>
        void writeField
        (
            ensightFile&,
            const Field<Type>&
        ) const;


    // Member Operators

        //- Disallow default bitwise assignment
        void operator=(const ensightPart&)
        {
            notImplemented("ensightPart::operator=(const ensightPart&)");
        }


    // IOstream Operators

        //- write data (reconstruction information)
        friend Ostream& operator<<(Ostream&, const ensightPart&);

        //- write geometry
        friend ensightGeoFile& operator<<
        (
            ensightGeoFile&,
            const ensightPart&
        );
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
#   include "ensightPartI.H"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
