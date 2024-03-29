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

\*---------------------------------------------------------------------------*/

//extern "C"
//{
    #include "MASTER.h"
    #include "GLOBAL.h"
//}

#include "tecplotWriter.H"

#include "fvc.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template<class Type>
void Foam::tecplotWriter::writeField(const Field<Type>& fld) const
{
    for (direction cmpt = 0; cmpt < pTraits<Type>::nComponents; cmpt++)
    {
        scalarField cmptFld(fld.component(cmpt));

        // Convert to float
        Field<float> floats(cmptFld.size());
        forAll(cmptFld, i)
        {
            floats[i] = float(cmptFld[i]);
        }

        INTEGER4 size = INTEGER4(floats.size());
        INTEGER4 IsDouble = 0;  //float

        //Pout<< "Writing component:" << cmpt << " of size:" << size
        //    << " floats." << endl;

        if (!TECDAT112(&size, floats.begin(), &IsDouble))
        {
//            FatalErrorIn("tecplotWriter::writeField(..) const")
//                << "Error in TECDAT112." << exit(FatalError);
        }
    }
}


template<class Type>
Foam::tmp<Field<Type> > Foam::tecplotWriter::getPatchField
(
    const bool nearCellValue,
    const GeometricField<Type, fvPatchField, volMesh>& vfld,
    const label patchI
) const
{
    if (nearCellValue)
    {
        return vfld.boundaryField()[patchI].patchInternalField();
    }
    else
    {
        return vfld.boundaryField()[patchI];
    }
}


template<class Type>
Foam::tmp<Field<Type> > Foam::tecplotWriter::getFaceField
(
    const GeometricField<Type, fvsPatchField, surfaceMesh>& sfld,
    const labelList& faceLabels
) const
{
    const polyBoundaryMesh& patches = sfld.mesh().boundaryMesh();

    tmp<Field<Type> > tfld(new Field<Type>(faceLabels.size()));
    Field<Type>& fld = tfld();

    forAll(faceLabels, i)
    {
        label faceI = faceLabels[i];

        label patchI = patches.whichPatch(faceI);

        if (patchI == -1)
        {
            fld[i] = sfld[faceI];
        }
        else
        {
            label localFaceI = faceI - patches[patchI].start();
            fld[i] = sfld.boundaryField()[patchI][localFaceI];
        }
    }

    return tfld;
}


template<class GeoField>
Foam::wordList Foam::tecplotWriter::getNames
(
    const PtrList<GeoField>& flds
)
{
    wordList names(flds.size());
    forAll(flds, i)
    {
        names[i] = flds[i].name();
    }
    return names;
}


template<class Type>
void Foam::tecplotWriter::getTecplotNames
(
    const wordList& names,
    const INTEGER4 loc,
    string& varNames,
    DynamicList<INTEGER4>& varLocation
)
{
    forAll(names, i)
    {
        if (!varNames.empty())
        {
            varNames += " ";
        }

        direction nCmpts = pTraits<Type>::nComponents;

        if (nCmpts == 1)
        {
            varNames += names[i];
            varLocation.append(loc);
        }
        else
        {
            for
            (
                direction cmpt = 0;
                cmpt < nCmpts;
                cmpt++
            )
            {
                string fldName =
                    (cmpt != 0 ? " " : string::null)
                  + names[i]
                  + "_"
                  + pTraits<Type>::componentNames[cmpt];
                varNames += fldName;
                varLocation.append(loc);
            }
        }
    }
}


template<class GeoField>
void Foam::tecplotWriter::getTecplotNames
(
    const PtrList<GeoField>& flds,
    const INTEGER4 loc,
    string& varNames,
    DynamicList<INTEGER4>& varLocation
)
{
    getTecplotNames<typename GeoField::value_type>
    (
        getNames(flds),
        loc,
        varNames,
        varLocation
    );
}


// ************************************************************************* //
