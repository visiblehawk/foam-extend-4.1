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

Description
    Create intermediate mesh files from PROSTAR files

\*---------------------------------------------------------------------------*/

#include "starMesh.H"
#include "foamTime.H"
#include "wallPolyPatch.H"
#include "cyclicPolyPatch.H"
#include "symmetryPolyPatch.H"
#include "preservePatchTypes.H"
#include "IFstream.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

void starMesh::readBoundary()
{
    label nPatches = 0, nFaces = 0;
    labelList nPatchFaces(1000);

    label lineIndex, starLabel;
    label starRegion, configNumber;

    labelList pointLabels(4);
    labelList pointLabelsTri(3);

    labelList patchLabels(1000, label(-1));

    word patchType;
    patchTypes_.setSize(1000);
    patchNames_.setSize(1000);

    fileName boundaryFileName(casePrefix_ + ".bnd");

    {
        IFstream boundaryFile(boundaryFileName);

        // Collect no. of faces (nFaces),
        // no. of patches (nPatches)
        // and for each of these patches the number of faces
        // (nPatchFaces[patchLabel])
        // and a conversion table from Star regions to (Foam) patchLabels

        if (boundaryFile.good())
        {
            forAll(nPatchFaces, faceLabel)
            {
                nPatchFaces[faceLabel] = 0;
            }

            while ((boundaryFile >> lineIndex).good())
            {
                nFaces++;

                // Skip point numbers
                for (int i=0; i<4; i++)
                {
                    boundaryFile >> starLabel;
                }

                boundaryFile >> starRegion;
                boundaryFile >> configNumber;
                boundaryFile >> patchType;

                // Build translation table to convert star patch to foam patch
                label patchLabel = patchLabels[starRegion];
                if (patchLabel == -1)
                {
                    patchLabel = nPatches;
                    patchLabels[starRegion] = patchLabel;
                    patchTypes_[patchLabel] = patchType;
                    patchNames_[patchLabel] = patchType + name(starRegion);

                    nPatches++;

                    Info<< "Star region " << starRegion
                        << " with type " << patchType
                        << " is now Foam patch " << patchLabel << endl;

                }

                nPatchFaces[patchLabel]++;
            }


            Info<< nl
                << "Setting size of boundary to " << nPatches
                << nl << endl;

            nPatchFaces.setSize(nPatches);
        }
        else
        {
            FatalErrorIn("starMesh::readBoundary()")
                << "Cannot read file "
                << boundaryFileName
                << abort(FatalError);
        }
    }

    if (nPatches > 0)
    {
        boundary_.setSize(nPatchFaces.size());
        patchTypes_.setSize(nPatchFaces.size());
        patchNames_.setSize(nPatchFaces.size());

        // size the lists and reset the counters to be used again
        forAll(boundary_, patchLabel)
        {
            boundary_[patchLabel].setSize(nPatchFaces[patchLabel]);

            nPatchFaces[patchLabel] = 0;
        }

        IFstream boundaryFile(boundaryFileName);

        for (label faceI=0; faceI<nFaces; faceI++)
        {
            boundaryFile >> lineIndex;

            for (int i = 0; i < 4; i++)
            {
                boundaryFile >> starLabel;

                // convert Star label to Foam point label
                // through lookup-list starPointLabelLookup_
                pointLabels[i] = starPointLabelLookup_[starLabel];

                if (pointLabels[i] < 0)
                {
                    Info<< "Boundary file not consistent with vertex file\n"
                        << "Star vertex number " << starLabel
                        << " does not exist\n";
                }

            }

            boundaryFile >> starRegion;
            label patchLabel = patchLabels[starRegion];

            boundaryFile >> configNumber;
            boundaryFile >> patchType;

            if   // Triangle
            (
                pointLabels[2] == pointLabels[3]
            )
            {
                //Info<< "Converting collapsed quad into triangle"
                //    << " for face " << faceI
                //    << " in Star boundary " << lineIndex << endl;

                pointLabelsTri[0] = pointLabels[0];
                pointLabelsTri[1] = pointLabels[1];
                pointLabelsTri[2] = pointLabels[2];

                boundary_[patchLabel][nPatchFaces[patchLabel]]
                    = face(pointLabelsTri);
            }
            else
            {
                boundary_[patchLabel][nPatchFaces[patchLabel]]
                    = face(pointLabels);
            }

            // Increment counter of faces in current patch
            nPatchFaces[patchLabel]++;
        }

        forAll(boundary_, patchLabel)
        {
            word patchType = patchTypes_[patchLabel];

            if (patchType == "SYMP")
            {
                patchTypes_[patchLabel] = symmetryPolyPatch::typeName;
            }
            else if (patchType == "WALL")
            {
                patchTypes_[patchLabel] = wallPolyPatch::typeName;
            }
            else if (patchType == "CYCL")
            {
                // incorrect. should be cyclicPatch but this
                // requires info on connected faces.
                patchTypes_[patchLabel] = cyclicPolyPatch::typeName;
            }
            else
            {
                patchTypes_[patchLabel] = polyPatch::typeName;
            }

            Info<< "Foam patch " << patchLabel
                << " is of type " << patchTypes_[patchLabel]
                << " with name " << patchNames_[patchLabel] << endl;
        }
    }
    else
    {
        WarningIn("void starMesh::readBoundary()")
            << "no boundary faces in file "
            << boundaryFileName
            << endl;
    }

    patchPhysicalTypes_.setSize(patchTypes_.size());

    PtrList<dictionary> patchDicts;

    preservePatchTypes
    (
        runTime_,
        runTime_.constant(),
        polyMesh::meshSubDir,
        patchNames_,
        patchDicts,
        defaultFacesName_,
        defaultFacesType_
    );

    forAll(patchDicts, patchI)
    {
        if (patchDicts.set(patchI))
        {
            const dictionary& dict = patchDicts[patchI];
            dict.readIfPresent("type", patchTypes_[patchI]);
            dict.readIfPresent("physicalType", patchPhysicalTypes_[patchI]);
        }
    }
}


// ************************************************************************* //
