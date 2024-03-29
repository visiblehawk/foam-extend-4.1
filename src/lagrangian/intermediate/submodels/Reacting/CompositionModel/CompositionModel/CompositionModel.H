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
    Foam::CompositionModel

Description
    Templated reacting parcel composition model class
    Consists of carrier species (via thermo package), and additional liquids
    and solids

SourceFiles
    CompositionModel.C
    NewCompositionModel.C

\*---------------------------------------------------------------------------*/

#ifndef CompositionModel_H
#define CompositionModel_H

#include "IOdictionary.H"
#include "autoPtr.H"
#include "runTimeSelectionTables.H"

#include "PtrList.H"
#include "multiComponentMixture.H"

#include "liquidMixture.H"
#include "solidMixture.H"

#include "phasePropertiesList.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                       Class CompositionModel Declaration
\*---------------------------------------------------------------------------*/

template<class CloudType>
class CompositionModel
{
    // Private data

        //- The cloud dictionary
        const dictionary& dict_;

        //- Reference to the owner injection class
        CloudType& owner_;

        //- The coefficients dictionary
        const dictionary& coeffDict_;

        //- Reference to the multi-component carrier phase thermo
        multiComponentMixture<typename CloudType::thermoType>& mcCarrierThermo_;

        //- Global (additional) liquid properties data
        autoPtr<liquidMixture> liquids_;

        //- Global (additional) solid properties data
        autoPtr<solidMixture> solids_;

        //- List of phase properties
        phasePropertiesList phaseProps_;


public:

    //- Runtime type information
    TypeName("CompositionModel");

    //- Declare runtime constructor selection table
    declareRunTimeSelectionTable
    (
        autoPtr,
        CompositionModel,
        dictionary,
        (
            const dictionary& dict,
            CloudType& owner
        ),
        (dict, owner)
    );


    // Constructors

        //- Construct from dictionary
        CompositionModel
        (
            const dictionary& dict,
            CloudType& owner,
            const word& type
        );


    //- Destructor
    virtual ~CompositionModel();


    //- Selector
    static autoPtr<CompositionModel<CloudType> > New
    (
        const dictionary& dict,
        CloudType& owner
    );


    // Member Functions

        // Access

            //- Return the cloud object
            const CloudType& owner() const;

            //- Return the cloud dictionary
            const dictionary& dict() const;

            //- Return the coefficients dictionary
            const dictionary& coeffDict() const;

            //- Return the carrier phase thermo package
            const multiComponentMixture<typename CloudType::thermoType>&
                mcCarrierThermo() const;


            // Composition lists

                //- Return the global (additional) liquids
                const liquidMixture& liquids() const;

                //- Return the global (additional) solids
                const solidMixture& solids() const;

                //- Return the list of phase properties
                const phasePropertiesList& phaseProps() const;

                //- Return the number of phases
                label nPhase() const;


            // Phase properties

                //- Return the list of phase type names
                //  If only 1 phase, return the component names of that phase
                const wordList& phaseTypes() const;

                //- Return the list of state labels (s), (l), (g) etc.
                const wordList& stateLabels() const;

                //- Return the list of component names for phaseI
                const wordList& componentNames(const label phaseI) const;

                //- Return global id of component cmptName in carrier thermo
                label globalCarrierId(const word& cmptName) const;

                //- Return global id of component cmptName in phase phaseI
                label globalId(const label phaseI, const word& cmptName) const;

                //- Return global ids of for phase phaseI
                const labelList& globalIds(const label phaseI) const;

                //- Return local id of component cmptName in phase phaseI
                label localId(const label phaseI, const word& cmptName) const;

                //- Return global carrier id of component given local id
                label localToGlobalCarrierId
                (
                    const label phaseI,
                    const label id
                ) const;

                //- Return the list of phase phaseI mass fractions
                const scalarField& Y0(const label phaseI) const;

                //- Return the list of phase phaseI volume fractions fractions
                //  based on supplied mass fractions Y
                scalarField X
                (
                    const label phaseI,
                    const scalarField& Y
                ) const;


            // Mixture properties

                //- Return the list of mixture mass fractions
                //  If only 1 phase, return component fractions of that phase
                virtual const scalarField& YMixture0() const = 0;

                // Indices of gas, liquid and solid phases in phase properties
                // list - returns -1 if not applicable

                    //- Gas id
                    virtual label idGas() const = 0;

                    //- Liquid id
                    virtual label idLiquid() const = 0;

                    //- Solid id
                    virtual label idSolid() const = 0;


        // Evaluation

            //- Return total enthalpy for the phase phaseI
            virtual scalar H
            (
                const label phaseI,
                const scalarField& Y,
                const scalar p,
                const scalar T
            ) const;

            //- Return sensible enthalpy for the phase phaseI
            virtual scalar Hs
            (
                const label phaseI,
                const scalarField& Y,
                const scalar p,
                const scalar T
            ) const;

            //- Return chemical enthalpy for the phase phaseI
            virtual scalar Hc
            (
                const label phaseI,
                const scalarField& Y,
                const scalar p,
                const scalar T
            ) const;

            //- Return specific heat caoacity for the phase phaseI
            virtual scalar cp
            (
                const label phaseI,
                const scalarField& Y,
                const scalar p,
                const scalar T
            ) const;

            //- Return latent heat for the phase phaseI
            virtual scalar L
            (
                const label phaseI,
                const scalarField& Y,
                const scalar p,
                const scalar T
            ) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#define makeCompositionModel(CloudType)                                       \
                                                                              \
    defineNamedTemplateTypeNameAndDebug                                       \
    (                                                                         \
        CompositionModel<CloudType>,                                          \
        0                                                                     \
    );                                                                        \
                                                                              \
    defineTemplateRunTimeSelectionTable                                       \
    (                                                                         \
        CompositionModel<CloudType>,                                          \
        dictionary                                                            \
    );


#define makeCompositionModelThermoType(SS, CloudType, ParcelType, ThermoType) \
                                                                              \
    defineNamedTemplateTypeNameAndDebug                                       \
    (                                                                         \
        SS<CloudType<ParcelType<ThermoType> > >,                              \
        0                                                                     \
    );                                                                        \
                                                                              \
    CompositionModel<CloudType<ParcelType<ThermoType> > >::                   \
        adddictionaryConstructorToTable                                       \
            <SS<CloudType<ParcelType<ThermoType> > > >                        \
            add##SS##CloudType##ParcelType##ThermoType##ConstructorToTable_;


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
#   include "CompositionModel.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
