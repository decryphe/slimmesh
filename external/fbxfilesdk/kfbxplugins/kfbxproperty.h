/*!  \file kfbxproperty.h
 */
#ifndef FBXFILESDK_KFBXPLUGINS_KFBXPROPERTY_H
#define FBXFILESDK_KFBXPLUGINS_KFBXPROPERTY_H

/**************************************************************************************

 Copyright (C) 2001 - 2009 Autodesk, Inc. and/or its licensors.
 All Rights Reserved.

 The coded instructions, statements, computer programs, and/or related material 
 (collectively the "Data") in these files contain unpublished information 
 proprietary to Autodesk, Inc. and/or its licensors, which is protected by 
 Canada and United States of America federal copyright law and by international 
 treaties. 
 
 The Data may not be disclosed or distributed to third parties, in whole or in
 part, without the prior written consent of Autodesk, Inc. ("Autodesk").

 THE DATA IS PROVIDED "AS IS" AND WITHOUT WARRANTY.
 ALL WARRANTIES ARE EXPRESSLY EXCLUDED AND DISCLAIMED. AUTODESK MAKES NO
 WARRANTY OF ANY KIND WITH RESPECT TO THE DATA, EXPRESS, IMPLIED OR ARISING
 BY CUSTOM OR TRADE USAGE, AND DISCLAIMS ANY IMPLIED WARRANTIES OF TITLE, 
 NON-INFRINGEMENT, MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE OR USE. 
 WITHOUT LIMITING THE FOREGOING, AUTODESK DOES NOT WARRANT THAT THE OPERATION
 OF THE DATA WILL BE UNINTERRUPTED OR ERROR FREE. 
 
 IN NO EVENT SHALL AUTODESK, ITS AFFILIATES, PARENT COMPANIES, LICENSORS
 OR SUPPLIERS ("AUTODESK GROUP") BE LIABLE FOR ANY LOSSES, DAMAGES OR EXPENSES
 OF ANY KIND (INCLUDING WITHOUT LIMITATION PUNITIVE OR MULTIPLE DAMAGES OR OTHER
 SPECIAL, DIRECT, INDIRECT, EXEMPLARY, INCIDENTAL, LOSS OF PROFITS, REVENUE
 OR DATA, COST OF COVER OR CONSEQUENTIAL LOSSES OR DAMAGES OF ANY KIND),
 HOWEVER CAUSED, AND REGARDLESS OF THE THEORY OF LIABILITY, WHETHER DERIVED
 FROM CONTRACT, TORT (INCLUDING, BUT NOT LIMITED TO, NEGLIGENCE), OR OTHERWISE,
 ARISING OUT OF OR RELATING TO THE DATA OR ITS USE OR ANY OTHER PERFORMANCE,
 WHETHER OR NOT AUTODESK HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH LOSS
 OR DAMAGE. 

**************************************************************************************/
#include <fbxfilesdk/components/kbaselib/kaydaradef_h.h>
// FBX includes
#include <fbxfilesdk/components/kfcurve/kfcurvenode.h>
#include <fbxfilesdk/fbxcore/kfbxevaluationinfo.h>
#include <fbxfilesdk/kfbxplugins/kfbxdatatypes.h>
#include <fbxfilesdk/fbxcore/kfbxpropertydef.h>

// FBX namespace
#include <fbxfilesdk/fbxfilesdk_nsbegin.h>

// Forward declarations
class KFbxObject;
class KFbxColor;
class KFbxCriteria;

    /** \brief Class to hold user properties.
      * \nosubgrouping
      */
    class KFBX_DLL KFbxProperty : private FbxPropertyFlags
    {
        /**
          * \name Constructor and Destructor.
          */
        //@{
        public:
            using FbxPropertyFlags::eFbxPropertyFlags;
            using FbxPropertyFlags::eNO_FLAG;
            using FbxPropertyFlags::eANIMATABLE;
            using FbxPropertyFlags::eUSER;
            using FbxPropertyFlags::eTEMPORARY;
            using FbxPropertyFlags::ePUBLISHED;
            using FbxPropertyFlags::ePSTATIC;

            using FbxPropertyFlags::eNOT_SAVABLE;
            using FbxPropertyFlags::eHIDDEN;

            using FbxPropertyFlags::eUI_DISABLED;
            using FbxPropertyFlags::eUI_GROUP;
            using FbxPropertyFlags::eUI_BOOLGROUP;
            using FbxPropertyFlags::eUI_EXPANDED;
            using FbxPropertyFlags::eUI_NOCAPTION;
            using FbxPropertyFlags::eUI_PANEL;

            typedef FbxPropertyFlags::eFbxPropertyFlags EFlags;

            /** Creates a property.
              * \param pCompoundProperty The parent property of this property.
              * \param pName The property name. 
              * \param pDataType The data type of this property.
              * \param pLabel The label of this property.
              * \param pCheckForDuplicate If \c true, parent property checks if it has a child property already with pName, if \c false, the new property is created. 
              * \param pWasFound If pCheckForDuplicate is \c true, this flag is set to indicate whether the pCompoundProperty already has a child property with pName.  
              */
            static KFbxProperty Create(KFbxProperty const &pCompoundProperty, char const* pName, KFbxDataType const &pDataType=KFbxDataType(), char const* pLabel="",bool pCheckForDuplicate=true, bool* pWasFound=NULL);

            /** Creates a property.
              * \param pParentProperty The parent property of this property.
              * \param pName The property name. 
              * \param pDataType The data type of this property.
              * \param pValue The value of this property.
              * \param pFlags The property flag.
              * \param pCheckForDuplicate If \c true, parent property checks whether it already has a child property with pName, if \c false, the new property is created. 
              * \param pForceSet If \c true, the value will be set forcibly, if \c false the value is not set when it is equal to the default value. 
              */
            template<typename T> inline static KFbxProperty Create(KFbxProperty const &pParentProperty, char const* pName,KFbxDataType const &pDataType,T const &pValue,eFbxPropertyFlags pFlags=eNO_FLAG,bool pCheckForDuplicate=true,bool pForceSet=true)
            {
                if( !pCheckForDuplicate )
                {
                    KFbxProperty lProperty = Create(pParentProperty, pName, pDataType, "", pCheckForDuplicate);
                    lProperty.ModifyFlag(pFlags, true); // modify the flags before we set the value
                    lProperty.Set(pValue);
                    return lProperty;
                }

                // First check for a duplicate
                KFbxProperty lProperty = pParentProperty.Find(pName);

                // only set if we are forcing the set, or we are actually creating the property
                // (as opposed to returning an existing one)
                //bool lSetValue = pForceSet ? true : !lProperty.IsValid();

                if( !lProperty.IsValid() )
                    lProperty = Create(pParentProperty, pName, pDataType, "", false); // don't check because we already did

                lProperty.ModifyFlag(pFlags, true); // modify the flags before we set the value
                //if( lSetValue )
                //  lProperty.Set(pValue);
                lProperty.Set( &pValue,FbxTypeOf(pValue),!pForceSet);
                return lProperty;
            }

            /** Creates a dynamic property.
              * \param pObject The object that contains this property.
              * \param pName The property name.
              * \param pDataType The data type of this property.
              * \param pLabel The label of this property.
              * \param pCheckForDuplicate If \c true, pObject checks whether it already has a property with pName, if \c false, a new property is created. 
              * \param pWasFound If pCheckForDuplicate is \c true, this flag is set to indicate whether the pObject already has a child property with pName.  
              */
            static KFbxProperty Create(KFbxObject* pObject, char const* pName, KFbxDataType const &pDataType=KFbxDataType(), char const* pLabel="",bool pCheckForDuplicate=true,bool* pWasFound=NULL);

            /** Creates a dynamic property from another property.
              * \param pObject The object that contains this property.
              * \param pFromProperty The property copied by this property. 
              * \param pCheckForDuplicate If \c true, pObject checks if it already has a property with pName, if \c false, a new property is created. 
              */
            static KFbxProperty Create(KFbxObject* pObject, KFbxProperty& pFromProperty, bool pCheckForDuplicate=true);

            /** Creates a dynamic property from another property.
              * \param pCompoundProperty The parent property of this property.
              * \param pFromProperty The property copied by this property. 
              * \param pCheckForDuplicate If \c true, parent property checks if it already has a child property with pName, if \c false, a new property is created. 
              */
            static KFbxProperty Create(KFbxProperty const& pCompoundProperty, KFbxProperty& pFromProperty, bool pCheckForDuplicate = true);

            /** Destroys a dynamic property.
              * \param pRecursive 
              * \param pDependents
              * \remarks Before destroying, disconnect it from all the source and destination properties, then remove it from the object containing it and destroy it.
              *          The two parameters are useless in this function.
              */
            void Destroy(bool pRecursive = true, bool pDependents = false);

            /** Destroys a dynamic property and its children
            * \remarks Destroy all children of current property, and current property will also be destroyed.
            */
            void DestroyRecursively();

            /** Destroys children of a dynamic property.
            * \remarks Destroy all children of current property, and current property will not be destroyed.
            */
            void DestroyChildren();

            /** Static property constructor.
              */
            KFbxProperty();

            /** Copy constructor for properties.
              * \param pProperty The property copied to this one.
              */
            KFbxProperty(KFbxProperty const &pProperty);

            /** Copy constructor for properties.
              * \param pPropertyHandle The property handle copied to this property handle. 
              */
            KFbxProperty(KFbxPropertyHandle const &pPropertyHandle);

            /** Static property destructor.
              */
            ~KFbxProperty();

        public:
        //@}

        /**
          * \name Property Identification.
          */
        //@{
        public:
            /** Returns the property data type. 
              * \return The property data type.
              */
            KFbxDataType        GetPropertyDataType() const;

            /** Returns the internal name of the property. 
              * \return Property internal name string.
              */
            KString         GetName() const;

            /** Returns the hierarchical name of the property.
              * \return Property hierarchical name string.
              */
            KString             GetHierarchicalName() const;

            /** Returns the property label.
              * \param pReturnNameIfEmpty If \c true, lets this method return the internal name if the label is empty. 
              * \return The property label if set, or the property internal name if the pReturnNameIfEmpty
              *         flag is set to \c true and the label has not been defined.
              * \remarks Some applications may ignore the label field and work uniquely with the
              *          internal name. Therefore, it should not be taken for granted that a label exists. Also, remember
              *          that the label does not get saved in the FBX file. It only exists while the property object is
              *          in memory.
              */
            KString         GetLabel(bool pReturnNameIfEmpty=true);


            /** Sets a label for the property.
              * \param pLabel Label string.
              */
            void                SetLabel(KString pLabel);

            /** Returns the object that contains the property.
              * \return The property object owner (or null if the property is an orphan).
              */
            KFbxObject*         GetFbxObject() const;

        //@}

        /**
          * \name User data
          */
        //@{
            /** Sets the user tag.
              * \param pTag The user tag to be set.
              */
            void                SetUserTag(int pTag);
                
            //! Gets the user tag.
            int                 GetUserTag();

            /** Sets the user data pointer.
              * \param pUserData The user data pointer.
              */            
            void  SetUserDataPtr(void* pUserData);

            /** Gets the user data pointer.
              * \return The user data pointer.
              */
            void* GetUserDataPtr();
        //@}

        /**
          * \name Property Flags.
          */
        //@{
            /** Changes the property attribute.
              * \param pFlag Property attribute identifier.
              * \param pValue New state of pFlag.
              */
            void ModifyFlag(eFbxPropertyFlags pFlag, bool pValue);

            /** Returns the state of the property attribute. 
              * \param pFlag Property attribute identifier.
              * \return The state of the property attribute(pFlag).
              */
            bool GetFlag(eFbxPropertyFlags pFlag);

            /** Returns the inheritance type of the given flag, similar to GetValueInheritType().
              * \param pFlag The flag to be queried.
              * \return The inheritance type of the specific flag.
              */
            KFbxInheritType GetFlagInheritType( eFbxPropertyFlags pFlag ) const;

            /** Sets the inheritance type for the specific flag, similar to SetValueInheritType().
              * \param pFlag The flag to be set.
              * \param pType The inheritance type to be set.
              * \return \c True on success, \c false otherwise.
              */
            bool SetFlagInheritType( eFbxPropertyFlags pFlag, KFbxInheritType pType );

            /** Checks if the property flag has been modified from its default value.
              * \param pFlag The flag to be queried.
              * \return \c True if the value of this property has changed, \c false otherwise
              */
            bool ModifiedFlag( eFbxPropertyFlags pFlag ) const;
        //@}

        /**
          * \name Assignment and comparison operators
          */
        //@{

            /** Assignment operator.
              * \param pKProperty The property assigned to this property.
              * \return This property.
              */
            KFbxProperty &      operator=  (KFbxProperty const &pKProperty);

            /** Equivalence operator.
              * \param pKProperty The property compared to this property.
              * \return \c True if equal, \c false otherwise.
              */            
            bool                operator== (KFbxProperty const &pKProperty) const;

            /** Non-equivalence operator.
              * \param pKProperty The property compared to this property.
              * \return \c True if unequal, \c false otherwise.
              */    
            bool                operator!= (KFbxProperty const &pKProperty) const;

            /** Equivalence operator.
              * \param pValue The value compared to this property.
              * \return \c True if this property is valid and pValue doesn't equal zero, or this property is invalid and pValue equals zero, \c false otherwise.
              */    
            inline bool         operator== (int pValue) const { return pValue==0 ? !IsValid() : IsValid(); }

            /** Non-equivalence operator.
              * \param pValue The value compared to this property.
              * \return \c True if this property is valid and pValue equals zero, or this property is invalid and pValue doesn't equal zero, \c false otherwise.
              */    
            inline bool         operator!= (int pValue) const { return pValue!=0 ? !IsValid() : IsValid(); }

            /** Compares this property's value to another property's value.
              * \param pProp The property whose value is compared with this property's value.
              * \return \c True if equal, \c false otherwise.
              */
            bool CompareValue(KFbxProperty const& pProp) const;
        //@}

        /** Copies the value of a property.
          * \param pProp The property from which to derive the value.
          * \return \c True if value has been copied successfully, \c false otherwise.
          */
        bool CopyValue(KFbxProperty const& pProp);

        /**
          * \name Value management.
          */
        //@{
        public:

            /** Judges the property�s validity.
              * \return \c True if this property is valid, \c false otherwise. 
              */
            bool            IsValid() const;

            /** Checks if the specified property's value has changed from its default value.
              * \param pProperty Property that is tested.
              * \return \c True if the property value is still the default, \c false otherwise.
              * \remarks If the inheritance type of pProperty's value is eFbxOverride, pProperty's value should
              *          have been modified, so pProperty doesn't have the default value.
              *          If the inheritance type of pProperty's value is eFbxInherit, that means pProperty's value inherits
              *          the referenced object's property value, so pProperty has the default value. 
              */
            static bool HasDefaultValue( KFbxProperty& pProperty );

            /** Sets the value of this property. 
              * \param pValue Pointer to the new value
              * \param pValueType The data type of the new value
              * \param pCheckForValueEquality If \c true, the value is not set when it is equal to the default value.
              * \return \c True if type is compatible and the value is set successfully, \c false otherwise.
              */
            bool Set(void const *pValue,EFbxType pValueType, bool pCheckForValueEquality);

            /** Sets the value of this function
              * \param pValue Pointer to the new value
              * \param pValueType The data type of the new value
              * \return \c True if type is compatible and the value is set successfully, \c false otherwise.
              */
            inline bool Set(void const *pValue,EFbxType pValueType) { return Set( pValue, pValueType, true ); }

            /** Returns the value of the property.
              * \param pValue Pointer to the value 
              * \param pValueType The data type of the value.
              * \return \c True if type is compatible and value is returned successfully, \c false otherwise.
              */
            bool Get(void *pValue,EFbxType pValueType) const;

            /** Evaluates the KFCurve value of the property at the specific time, then returns the value of the property .
              * \param pValue Pointer to the value. 
              * \param pValueType The data type of the value.
              * \param pEvaluateInfo Contains the time of evaluation.
              * \return \c True if type is compatible and value is returned successfully, \c false otherwise.
              * \remarks When evaluated, the property value is set as the KFCurve value. 
              */
            bool Get(void *pValue,EFbxType pValueType,KFbxEvaluationInfo const *pEvaluateInfo);

            /** Sets the value of the property.
              * \param pValue The new value
              * \return \c True if type is compatible and the value is set successfully, \c false otherwise.
              */
            template <class T> inline bool  Set( T const &pValue )  { return Set( &pValue,FbxTypeOf(pValue), true ); }

            /** Gets the value of the property.
              * \param pFBX_TYPE The data type of the value.
              * \return The property value.
              */
            template <class T> inline T     Get( T const *pFBX_TYPE) const { T lValue; Get( &lValue,FbxTypeOf(lValue) ); return lValue; }

            /** Evaluates the KFCurve value of the property at the specific time, then returns the value of the property.
              * \param pFBX_TYPE The data type of the value.
              * \param pEvaluateInfo Contains the time of evaluation.
              * \return The value of the property after evaluation.
              * \remarks When evaluated, the property value is set as the KFCurve value. 
              */
            template <class T> inline T     Get( T const *pFBX_TYPE,KFbxEvaluationInfo const *pEvaluateInfo) { T lValue; Get( &lValue,FbxTypeOf(lValue),pEvaluateInfo ); return lValue; }

            /** Evaluates the KFCurve value of the property at the specific time, then returns the value of the property.
              * \param pEvaluateInfo Contains the time of evaluation.
              * \return The value of the property after evaluation.
              * \remarks When evaluated, the property value is set as the KFCurve value. 
              */
            template <class T> inline T     Get( KFbxEvaluationInfo const *pEvaluateInfo) { T lValue; Get( &lValue,FbxTypeOf(lValue),pEvaluateInfo ); return lValue; }
           
            /** Evaluates the KFCurve value of the property at the specific time, then returns the value of the property .
              * \param pValue Pointer to the value. 
              * \param pValueType The data type of the value.
              * \param pEvaluateInfo Contains the time of evaluation.
              * \return \c True if type is compatible and value is returned successfully.
              * \remarks When evaluated, the property value is set as the KFCurve value. 
              */
            bool Get(void *pValue,EFbxType pValueType,KFbxEvaluationInfo *pEvaluateInfo) const;

            /** Queries the inheritance type of the property.
              * Use this method to determine if this property's value is overridden from the default
              * value, or from the referenced object, if this object is a clone.
              * \return The inheritance type of the property.
              */
            KFbxInheritType GetValueInheritType() const;

            /** Sets the inheritance type of the property.
              * Use the method to explicitly override the default value of the property,
              * or the referenced object's property value, if this object is a clone.
              *
              * You can also use this to explicitly inherit the default value of the property,
              * or the referenced object's property value, if this object is a clone.
              *
              * \param pType The new inheritance type.
              * \return \c True on success, \c false otherwise.
              */
            bool SetValueInheritType( KFbxInheritType pType );

            /** Checks if the property's value has been modified from its default value.
              * \return \c True if the value of the property has changed, \c false otherwise.
              * \remarks If the inheritance type of the property's value is eFbxOverride, the property's value should
              *          have been modified, it returns \c true.
              *          If the inheritance type of the property's value is eFbxInherit, that means the property's value inherits
              *          the referenced object's property value, so the property's value is not modified and it returns \c false. 
              */
            bool Modified() const;

        //@}

        /**
          * \name Property Limits.
          * Property limits are provided for convenience if some applications desire to
          * bound the range of possible values for a given type property. 
          * Note: These limits are meaningless for the boolean type. It is the responsibility of the
          * calling application to implement the necessary instructions to limit the property of boolean type.
          */
        //@{
        public:
            /** Sets a minimum property value limit.
              * \param pMin Minimum value allowed.
              */
            void                SetMinLimit(double pMin);

         /** Returns whether a minimum limit exists, if it returns false, 
           * calling GetMinLimit() produces undefined behavior.
           * \return \c True when a minimum limit exists, \c false otherwise.
           */
         bool           HasMinLimit() const;

            /** Returns the minimum property value limit.
              * \return The minimum value limit.
              */
            double          GetMinLimit() const;

         /** Returns whether a maximum limit exists, if it returns false, 
           * calling GetMaxLimit() produces undefined behavior.
           * \return \c True when a maximum limit exists, \c false otherwise.
           */
         bool           HasMaxLimit() const;

            /** Sets a maximum property value limit.
              * \param pMax Maximum value allowed.
              */
            void                SetMaxLimit(double pMax);

            /** Returns the maximum property value.
              * \return The maximum value limit.
              */
            double          GetMaxLimit() const;

            /** Sets the minimum and maximum value limit of the property. 
              * \param pMin Minimum value allowed.
              * \param pMax Maximum value allowed.
              */
            void                SetLimits(double pMin, double pMax);
        //@}

        /**
          * \name Enum and property list
          */
        //@{
        public:
            /** Adds a string value at the end of the enumeration list.
              * \param pStringValue The string value to be added.
              * \return The index in the list where the string is added.
              * \remarks This function is only valid if the property type is eENUM.
              * Empty strings are not allowed.
              */
            int                 AddEnumValue(char const *pStringValue);

            /** Inserts a string value at the specific index.
              * \param pIndex Zero bound index.
              * \param pStringValue The string value to be inserted.
              * \remarks This function is only valid if the property type is eENUM.
              * pIndex must be in the range [0, ListValueGetCount()].
              * Empty strings are not allowed.
              */
            void                InsertEnumValue(int pIndex, char const *pStringValue);

            /** Returns the number of elements in the enumeration list.
              * \return The number of elements in the enumeration list.
              * \remarks This function returns 0 if the property type is not eENUM.
              */
            int                 GetEnumCount();

            /** Sets a string value at the specific index.
              * \param pIndex Zero bound index.
              * \param pStringValue The string value at the specific index.
              * \remarks This function is only valid if the property type is eENUM.
              * The function assigns the string value to the specific index.
              * A string value must exist at the specific index in order to be changed.
              * Empty strings are not allowed.
              */
            void                SetEnumValue(int pIndex, char const *pStringValue);

            /** Removes the string value at the specified index.
              * \param pIndex Index of the string value to be removed.
              * \remarks This function is only valid if the property type is eENUM.
              */
            void                RemoveEnumValue(int pIndex);

            /** Returns a string value at the specified index
              * \param pIndex Zero bound index.
              * \remarks This function is only valid if the property type is eENUM.
              */
            char *              GetEnumValue(int pIndex);
        //@}

        /**
          * \name Hierarchical properties
          */
        //@{
            /** Judges if this property is the root property. 
              * \return \c True when this property is a root property, \c false otherwise.
              */
            inline bool                 IsRoot() const                                          { return mPropertyHandle.IsRoot(); }

            /** Judges whether this property is a child of the specified property.
              * \param pParent The specified property.
              * \return \c True when this property is a child of the specified property, \c false otherwise.
              */
            inline bool                 IsChildOf(KFbxProperty  const & pParent) const          { return mPropertyHandle.IsChildOf(pParent.mPropertyHandle); }

            /** Judges whether this property is a descendant of the specified property.
              * \param pAncestor The specified property.
              * \return \c True when this property is a descendant of the specified property, \c false otherwise.
              */
            inline bool                 IsDescendentOf(KFbxProperty const & pAncestor) const    { return mPropertyHandle.IsDescendentOf(pAncestor.mPropertyHandle); }

            /** Returns the parent property of this property.
              * \return The parent of this property.
              */
            inline KFbxProperty         GetParent() const                                       { return KFbxProperty(mPropertyHandle.GetParent());  }

            /** Sets the parent for this property (this function has not been implemented, so it always return \c false).
              * \param pOther The parent to be set.
              * \return \c True on success, \c false otherwise.
              */
            bool                        SetParent( const KFbxProperty& pOther );

            /** Returns the first child of this property.
              * \return The first child of this property, if there is none, an invalid property is returned.
              */
            inline KFbxProperty         GetChild() const                                        { return KFbxProperty(mPropertyHandle.GetChild());   }

            /** Returns the sibling of this property.
              * \return The sibling of this property, if there is none, an invalid property is returned.
              */
            inline KFbxProperty         GetSibling() const                                      { return KFbxProperty(mPropertyHandle.GetSibling()); }

            /** Returns the first property that is a descendant of this property. 
              * \return The first descendant of this property, if there is none, an invalid property is returned.
              */
            inline KFbxProperty         GetFirstDescendent() const                              { return KFbxProperty(mPropertyHandle.GetFirstDescendent());   }

            /** Returns the property that follows pProperty that is a descendant of this property. 
              * \param pProperty The last found descendant.
              * \return The property that follows pProperty, if there is none, an invalid property is returned.
              */
            inline KFbxProperty         GetNextDescendent(KFbxProperty const &pProperty) const  { return KFbxProperty(mPropertyHandle.GetNextDescendent(pProperty.mPropertyHandle)); }

            /** Searches a property using its name.
              * \param pName The name of the property as a \c NULL terminated string.
              * \param pCaseSensitive Whether the name is case-sensitive.
              * \return A valid KFbxProperty if the property is found, else
              *         an invalid KFbxProperty. See KFbxProperty::IsValid()
              */
            inline KFbxProperty         Find (char const *pName,bool pCaseSensitive = true) const { return KFbxProperty(mPropertyHandle.Find(pName,pCaseSensitive));  }

            /** Searches a property using its name and data type.
              * \param pName The name of the property as a \c NULL terminated string.
              * \param pDataType The data type of the property.
              * \param pCaseSensitive Whether the name is case-sensitive.
              * \return A valid KFbxProperty if the property is found, else
              *         an invalid KFbxProperty. See KFbxProperty::IsValid()
              */
            inline KFbxProperty         Find (char const *pName,KFbxDataType const &pDataType, bool pCaseSensitive = true) const { return KFbxProperty(mPropertyHandle.Find(pName,pDataType.GetTypeInfoHandle(),pCaseSensitive));  }

            /** Searches a property using its full name.
              * \param pName The full name of the property as a \c NULL terminated string.
              * \param pCaseSensitive whether the name is case-sensitive.
              * \return A valid KFbxProperty if the property is found, else
              *         an invalid KFbxProperty. See KFbxProperty::IsValid()
              */
            inline KFbxProperty         FindHierarchical (char const *pName,bool pCaseSensitive = true) const { return KFbxProperty(mPropertyHandle.Find(pName,sHierarchicalSeparator,pCaseSensitive));  }

            /** Searches a property using its full name and data type.
              * \param pName The full name of the property as a \c NULL terminated string.
              * \param pDataType The data type of the property.
              * \param pCaseSensitive whether the name is case-sensitive.
              * \return A valid KFbxProperty if the property is found, else
              *         an invalid KFbxProperty. See KFbxProperty::IsValid()
              */
            inline KFbxProperty         FindHierarchical (char const *pName,KFbxDataType const &pDataType, bool pCaseSensitive = true) const { return KFbxProperty(mPropertyHandle.Find(pName,sHierarchicalSeparator,pDataType.GetTypeInfoHandle(),pCaseSensitive));  }

        //@}

        /**
          * \name Optimizations
          */
        //@{
            //! Internal function for building a property name map.
            inline void     BeginCreateOrFindProperty() { mPropertyHandle.BeginCreateOrFindProperty();  }

            //! Internal function for clearing the property name map. 
            inline void     EndCreateOrFindProperty()   { mPropertyHandle.EndCreateOrFindProperty();    }

            //!This is an internal structure that you can use to build and clear the name map of properties. You can use the name map to speed up searching for property names.
         struct KFbxPropertyNameCache
         {
            /** Constructor, the name map is created in the constructor.
               * \param prop Property for building and clearing the name map.
               */
            KFbxPropertyNameCache(const KFbxProperty& prop) :
               mProp(const_cast<KFbxProperty&>(prop))
            {
               mProp.BeginCreateOrFindProperty();
            }

            //! Destructor, the name map is destroyed in destructor.
            ~KFbxPropertyNameCache()
            {
               mProp.EndCreateOrFindProperty();
            }

         private:
            KFbxProperty & mProp;

            KFbxPropertyNameCache& operator=(const KFbxPropertyNameCache &other) { mProp = other.mProp; mProp.BeginCreateOrFindProperty(); return *this; }
         };
        //@}

        /**
          * \name Array Management
          */
        //@{
            /** Sets the array size(not implemented).
              * \param pSize
              * \param pVariableArray
              */
            bool            SetArraySize( int pSize, bool pVariableArray );

            //! Returns the array size(not implemented).
            int             GetArraySize() const;

            /** Returns the (pIndex)th array item.
              * \param pIndex The item index.
              * \return The (pIndex)th array item.
              */
            KFbxProperty    GetArrayItem(int pIndex) const;

            /** Returns the (pIndex)th array item.
              * \param pIndex The item index.
              * \return The (pIndex)th array item.
              */
            inline KFbxProperty operator[](int pIndex) const { return GetArrayItem(pIndex); }
        //@}

        /**
          * \name FCurve Management
          */
        //@{
            /** Creates a KFCurveNode on a take.
              * \param pTakeName Name of the take to create the KFCurveNode on.
              */
            KFCurveNode* CreateKFCurveNode(const char* pTakeName=NULL);

            /** Returns the KFCurveNode of the property from a take.
              * \param pTakeName Name of the take from which to get the KFCurveNode.
              * \param pCreateAsNeeded Creates a KFCurveNode if not found.
              * \return Pointer to the KFCurveNode of the property on the given take.
              * \remark The returned pointer is not persistent and should not be stored for
              *         later use.
              */
            KFCurveNode* GetKFCurveNode(bool pCreateAsNeeded=false, const char* pTakeName=NULL);

            /** Tries to get the KFCurve of the specified channel from the current take.
              * \param pChannel Name of the KFCurve channel we are looking for.
              * \return Pointer to the KFCurve of the specified channel if found, NULL in any other case.
              * \remark This method fails if the KFCurveNode does not exist.
              *         If the pChannel is left NULL, this method retrieves the KFCurve directly from the KFCurveNode of the property,
              *         Otherwise it searches recursively to find the KFCurve of the specified channel.
             */
            KFCurve* GetKFCurve(const char* pChannel = NULL);

            /** Tries to get the KFCurve of the specified channel from the given take.
              * \param pChannel Name of the KFCurve channel we are looking for.
              * \param pTakeName Name of the take from which to get the KFCurve.
              * \return Pointer to the KFCurve if found, NULL in any other case.
              * \remark This method fails if the KFCurveNode does not exist.
              *         If pTakeName is NULL, this function searches in the current take.
              *         If the pChannel is left NULL, this method retrieves the KFCurve directly from the KFCurveNode of the property.
              *         Otherwise it searches recursively to find the KFCurve of the specified channel.
             */
            KFCurve* GetKFCurve(const char* pChannel, const char* pTakeName);
        //@}

        /**
          * \name Evaluation management
          */
        //@{
            /** Evaluates the KFCurve value of this property at the specified time, then sets the property value as the KFCurve value.
              * \param pEvaluationInfo Contains the time of evaluation.
              * \return \c True on success, \c false otherwise.
              * \remarks When evaluated, the property value is set as the KFCurve value. 
              */
            bool    Evaluate(KFbxEvaluationInfo const *pEvaluationInfo);
        //@}

        /**
          * \name General Object Connection and Relationship Management
          */
        //@{
        public:
            // SrcObjects
            /** Connects this property to one source object.
              * \param pObject The source object to which this property connects.
              * \param pType The connection type between the property and the object.
              * \return \c True on success, \c false otherwise.
              */
            bool ConnectSrcObject       (KFbxObject* pObject,kFbxConnectionType pType=eFbxConnectionNone);

            /** Judges whether this property connects with the source object.
              * \param pObject The source object.
              * \return \c True if this property connects with the source object, \c false otherwise.
              */
            bool IsConnectedSrcObject   (const KFbxObject* pObject) const;

            /** Disconnects this property from one source object.
              * \param pObject The source object from which this property will be disconnected.
              * \return \c True on success, \c false otherwise.
              */
            bool DisconnectSrcObject    (KFbxObject* pObject);

            /** Disconnects this property from all the source objects.
              * \return \c True if it disconnects all the source objects successfully, \c false otherwise.
              */
            bool DisconnectAllSrcObject();

            /** Disconnects this property from all source objects that satisfy a given criteria. 
              * \param pCriteria The given criteria.
              * \return \c True if it disconnects all the source objects successfully, \c false otherwise.
              */
            bool DisconnectAllSrcObject(KFbxCriteria const &pCriteria);

            /** Disconnects this property from all the source objects of a specific class type.
              * \param pClassId The specific class type.
              * \return \c True if it disconnects all the source objects successfully, \c false otherwise.
              */
            bool DisconnectAllSrcObject(const kFbxClassId& pClassId);

            /** Disconnects this property from all the source objects of a specific class type that satisfy the given criteria.
              * \param pClassId The specific class type.
              * \param pCriteria The given criteria.
              * \return \c True if it disconnects all the source objects successfully, \c false otherwise.
              */
            bool DisconnectAllSrcObject(const kFbxClassId& pClassId,KFbxCriteria const &pCriteria);

            /** Returns the number of source objects with which this property connects.
              * \return The number of source objects with which this property connects. 
              */
            int GetSrcObjectCount()const;

            /** Returns the number of source objects that satisfy the given criteria with which this property connects.
              * \param pCriteria The given criteria.
              * \return The number of source objects that satisfy the given criteria with which this property connects.
              */
            int GetSrcObjectCount(KFbxCriteria const &pCriteria)const;

            /** Returns the number of source objects of the specific class type with which this property connects. 
              * \param pClassId The specific class type.
              * \return The number of source objects of the specific class type with which this property connects.
              */
            int GetSrcObjectCount(const kFbxClassId& pClassId)const;

            /** Returns the number of source objects which are of the specified class type and satisfy the given criteria with which this property connects.
              * \param pClassId The specified class type.
              * \param pCriteria The given criteria.
              * \return The number of source objects.
              */
            int GetSrcObjectCount(const kFbxClassId& pClassId,KFbxCriteria const &pCriteria)const;

            /** Returns the source object at the specified index with which this property connects.
              * \param pIndex The specified index whose default value is 0.
              * \return The source object at the specified index, NULL if not found.
              */
            KFbxObject* GetSrcObject(int pIndex=0) const;

            /** Returns the source object that satisfies the criteria at the specified index with which this property connects.
              * \param pCriteria The given criteria.
              * \param pIndex The specified index whose default value is 0.
              * \return The source object that satisfies the given criteria at the specified index, NULL if not found.
              */
            KFbxObject* GetSrcObject(KFbxCriteria const &pCriteria,int pIndex=0) const;

            /** Returns the source object of the specified class type at the specified index with which this property connects.
              * \param pClassId The specified class type.
              * \param pIndex The specified index whose default value is 0.
              * \return The source object of the specified class type at the specified index, NULL if not found.
              */
            KFbxObject* GetSrcObject(const kFbxClassId& pClassId,int pIndex=0) const;

            /** Returns the source object which is of the specified class type and satisfies the given criteria at the specified index.
              * \param pClassId The specified class type.
              * \param pCriteria The given criteria.
              * \param pIndex The specified index whose default value is 0.
              * \return The source object which is of the specified class type and satisfies the given criteria at the specified index, NULL if not found.
              */
            KFbxObject* GetSrcObject(const kFbxClassId& pClassId,KFbxCriteria const &pCriteria,int pIndex=0) const;

            /** Searches the source object with the specified name, starting with the specified index.
              * \param pName The object name.
              * \param pStartIndex The start index.
              * \return The source object with the name, NULL if not found.
              */
            KFbxObject* FindSrcObject(const char *pName,int pStartIndex=0) const;

            /** Searches the source object with the specified name which satisfies the given criteria, starting with the specified index.
              * \param pCriteria The given criteria.
              * \param pName The object name.
              * \param pStartIndex The start index.
              * \return The source object with the name, NULL if not found.
              */
            KFbxObject* FindSrcObject(KFbxCriteria const &pCriteria,const char *pName,int pStartIndex=0) const;

            /** Searches the source object with the specified name which is of the specified class type, starting with the specified index.
              * \param pClassId The specified class type.
              * \param pName The object name.
              * \param pStartIndex The start index.
              * \return The source object with the name, NULL if not found.
              */
            KFbxObject* FindSrcObject(const kFbxClassId& pClassId,const char *pName,int pStartIndex=0) const;

            /** Searches the source object with the specified name which is of the specified class type and satisfies the given criteria, starting with the specified index.
              * \param pClassId The specified class type.
              * \param pCriteria The given criteria.
              * \param pName The object name.
              * \param pStartIndex The start index.
              * \return The source object with the name, NULL if not found.
              */
            KFbxObject* FindSrcObject(const kFbxClassId& pClassId,KFbxCriteria const &pCriteria,const char *pName,int pStartIndex=0) const;

            /** Disconnects this property from all source objects of the specified class type.
              * \param pFBX_TYPE The specified class type.
              * \return \c True if it disconnects all source objects successfully, \c false otherwise.
              */
            template < class T > inline bool DisconnectAllSrcObject (T const *pFBX_TYPE){ return DisconnectAllSrcObject(T::ClassId);}

            /** Disconnects this property from all source objects which are of the specified class type and satisfy the given criteria.
              * \param pFBX_TYPE The specified class type.
              * \param pCriteria The given criteria.
              * \return \c True if it disconnects all source objects successfully, \c false otherwise.
              */
            template < class T > inline bool DisconnectAllSrcObject (T const *pFBX_TYPE,KFbxCriteria const &pCriteria)  { return DisconnectAllSrcObject(T::ClassId,pCriteria);  }

            /** Returns the number of source objects of a specific class type with which this property connects. 
              * \param pFBX_TYPE The specified class type.
              * \return The number of source objects of the specified class type with which this property connects. 
              */
            template < class T > inline int  GetSrcObjectCount(T const *pFBX_TYPE) const{ return GetSrcObjectCount(T::ClassId); }

            /** Returns the number of source objects which are of the specified class type and satisfy the given criteria with which this property connects. 
              * \param pFBX_TYPE The specified class type.
              * \param pCriteria The given criteria.
              * \return The number of source objects which are of the specified class type and satisfy the given criteria.
              */
            template < class T > inline int  GetSrcObjectCount(T const *pFBX_TYPE,KFbxCriteria const &pCriteria) const { return GetSrcObjectCount(T::ClassId,pCriteria); }

            /** Returns the source object of the specified class type at the specified index.
              * \param pFBX_TYPE The specified class type.
              * \param pIndex The specified index whose default value is 0.
              * \return The source object of a specified class type at the specified index, NULL if not found.
              */
            template < class T > inline T*   GetSrcObject(T const *pFBX_TYPE,int pIndex=0) const { return (T*)GetSrcObject(T::ClassId,pIndex); }

            /** Returns the source object which is of the specified class type and satisfies the given criteria at the specified index.
              * \param pFBX_TYPE The specified class type.
              * \param pCriteria The given criteria.
              * \param pIndex The specified index whose default value is 0.
              * \return The source object which is of the specified class type and satisfies the given criteria at the specified index, NULL if not found.
              */
            template < class T > inline T*   GetSrcObject(T const *pFBX_TYPE,KFbxCriteria const &pCriteria,int pIndex=0) const { return (T*)GetSrcObject(T::ClassId,pCriteria,pIndex); }

            /** Searches the source object with the specified name which is of the specified class type, starting with the specified index.
              * \param pFBX_TYPE The specified class type.
              * \param pName The object name.
              * \param pStartIndex The start index.
              * \return The source object with the name, NULL if not found.
              */
            template < class T > inline T*   FindSrcObject(T const *pFBX_TYPE,const char *pName,int pStartIndex=0) const { return (T*)FindSrcObject(T::ClassId,pName,pStartIndex); }

            /** Searches the source object with the specified name which is of the specified class type and satisfies the given criteria, starting with the specified index.
              * \param pFBX_TYPE The specified class type.
              * \param pCriteria The given criteria.
              * \param pName The object name.
              * \param pStartIndex The start index.
              * \return The source object with the name, NULL if not found.
              */
            template < class T > inline T*   FindSrcObject(T const *pFBX_TYPE,KFbxCriteria const &pCriteria,const char *pName,int pStartIndex=0) const { return (T*)FindSrcObject(T::ClassId,pCriteria,pName,pStartIndex); }

            // DstObjects
            /** Connects this property to one destination object.
              * \param pObject The destination object with which this property connects.
              * \param pType The connection type between this property and the object.
              * \return \c True on success, \c false otherwise.
              */
            bool ConnectDstObject       (KFbxObject* pObject,kFbxConnectionType pType=eFbxConnectionNone);

            /** Judges whether this property connects with the destination object.
              * \param pObject The destination object.
              * \return \c True if this property connects with the destination object, \c false otherwise.
              */
            bool IsConnectedDstObject   (const KFbxObject* pObject) const;

            /** Disconnects this property from the destination object.
              * \param pObject The destination object from which this property disconnects from.
              * \return \c True on success, \c false otherwise.
              */
            bool DisconnectDstObject    (KFbxObject* pObject);

            /** Disconnects this property from all the destination objects.
              * \return \c True if it disconnects all the destination objects successfully, \c false otherwise.
              */
            bool DisconnectAllDstObject();

            /** Disconnects this property from all the destination objects that satisfy given criteria.
              * \param pCriteria The given criteria.
              * \return \c True if it disconnects all the destination objects successfully, \c false otherwise.
              */
            bool DisconnectAllDstObject(KFbxCriteria const &pCriteria);

            /** Disconnects this property from all the destination objects of the specified class type.
              * \param pClassId The specified class type.
              * \return \c True if it disconnects all the destination objects successfully, \c false otherwise.
              */
            bool DisconnectAllDstObject(const kFbxClassId& pClassId);

            /** Disconnects this property from all the destination objects which are of the specified class type and satisfy the given criteria.
              * \param pClassId The specified class type.
              * \param pCriteria The given criteria.
              * \return \c True if it disconnects all the destination objects successfully, \c false otherwise.
              */
            bool DisconnectAllDstObject(const kFbxClassId& pClassId,KFbxCriteria const &pCriteria);

            /** Returns the number of destination objects with which this property connects. 
              * \return The number of destination objects with which this property connects. 
              */
            int GetDstObjectCount() const;

            /** Returns the number of destination objects that satisfy the given criteria with which this property connects. 
              * \param pCriteria The given criteria.
              * \return The number of destination objects that satisfy given criteria with which this property connects. 
              */
            int GetDstObjectCount(KFbxCriteria const &pCriteria) const;

            /** Returns the number of destination objects of the specified class type with which this property connects. 
              * \param pClassId The specified class type.
              * \return The number of destination objects of the specified class type with which this property connects. 
              */
            int GetDstObjectCount(const kFbxClassId& pClassId) const;

            /** Returns the number of destination objects which are of the specified class type and satisfy the given criteria with which this property connects. 
              * \param pClassId The specified class type.
              * \param pCriteria The given criteria.
              * \return The number of destination objects which are of the specified class type and satisfy the given criteria.
              */
            int GetDstObjectCount(const kFbxClassId& pClassId,KFbxCriteria const &pCriteria) const;

            /** Returns the destination object at the specified index with which this property connects.
              * \param pIndex The specified index whose default value is 0.
              * \return The destination object at the specified index, NULL if not found.
              */
            KFbxObject* GetDstObject(int pIndex=0) const;

            /** Returns the destination object that satisfies given criteria at the specified index with which this property connects.
              * \param pCriteria The given criteria.
              * \param pIndex The specified index whose default value is 0.
              * \return The destination object that satisfies given criteria at the specified index, NULL if not found.
              */
            KFbxObject* GetDstObject(KFbxCriteria const &pCriteria,int pIndex=0) const;

            /** Returns the destination object of the specified class type at the specified index with which this property connects.
              * \param pClassId The specified class type.
              * \param pIndex The specified index whose default value is 0.
              * \return The destination object of the specified class type at the specified index, NULL if not found.
              */
            KFbxObject* GetDstObject(const kFbxClassId& pClassId,int pIndex=0) const;

            /** Returns the destination object which is of the specified class type and satisfies the given criteria at the specified index with which this property connects.
              * \param pClassId The specified class type.
              * \param pCriteria The given criteria.
              * \param pIndex The specified index whose default value is 0.
              * \return The destination object which is of the specified class type and satisfies the given criteria at the specified index, NULL if not found.
              */
            KFbxObject* GetDstObject(const kFbxClassId& pClassId,KFbxCriteria const &pCriteria,int pIndex=0)const;

            /** Searches the destination object with the specified name, starting with the specified index.
              * \param pName The object name.
              * \param pStartIndex The start index.
              * \return The destination object with the name, NULL if not found.
              */
            KFbxObject* FindDstObject(const char *pName,int pStartIndex=0) const;

            /** Searches the destination object with the specified name which satisfies the given criteria, starting with the specified index.
              * \param pCriteria The given criteria.
              * \param pName The object name.
              * \param pStartIndex The start index.
              * \return The destination object with the name, NULL if not found.
              */
            KFbxObject* FindDstObject(KFbxCriteria const &pCriteria,const char *pName,int pStartIndex=0) const;

            /** Searches the destination object with the specified name which is of the specified class type, starting with the specified index.
              * \param pClassId The specified class type.
              * \param pName The object name.
              * \param pStartIndex The start index.
              * \return The destination object with the name, NULL if not found.
              */
            KFbxObject* FindDstObject(const kFbxClassId& pClassId,const char *pName,int pStartIndex=0) const;

            /** Searches the destination object with the specified name which is of the specified class type and satisfies the given criteria, starting with the specified index.
              * \param pClassId The specified class type.
              * \param pCriteria The given criteria.
              * \param pName The object name.
              * \param pStartIndex The start index.
              * \return The destination object with the name, NULL if not found.
              */
            KFbxObject* FindDstObject(const kFbxClassId& pClassId,KFbxCriteria const &pCriteria,const char *pName,int pStartIndex=0) const;

            /** Disconnects this property from all the destination objects of the specified class type.
              * \param pFBX_TYPE The specified class type.
              * \return \c True if it disconnects all the destination objects successfully, \c false otherwise.
              */
            template < class T > inline bool DisconnectAllDstObject (T const *pFBX_TYPE){ return DisconnectAllDstObject(T::ClassId);    }

            /** Disconnects this property from all the destination objects which are of the specified class type and satisfy the given criteria.
              * \param pFBX_TYPE The specified class type.
              * \param pCriteria The given criteria.
              * \return \c True if it disconnects all the destination objects successfully, \c false otherwise.
              */
            template < class T > inline bool DisconnectAllDstObject (T const *pFBX_TYPE,KFbxCriteria const &pCriteria)  { return DisconnectAllDstObject(T::ClassId,pCriteria);  }

            /** Returns the number of destination objects of the specified class type with which this property connects. 
              * \param pFBX_TYPE The specified class type.
              * \return The number of destination objects of the specified class type with which this property connects. 
              */
            template < class T > inline int  GetDstObjectCount(T const *pFBX_TYPE) const { return GetDstObjectCount(T::ClassId); }

            /** Returns the number of destination objects which are of the specified class type and satisfy the given criteria with which this property connects. 
              * \param pFBX_TYPE The specified class type.
              * \param pCriteria The given criteria.
              * \return The number of destination objects which are of the specified class type and satisfy the given criteria with which this property connects. 
              */
            template < class T > inline int  GetDstObjectCount(T const *pFBX_TYPE,KFbxCriteria const &pCriteria) const { return GetDstObjectCount(T::ClassId,pCriteria); }

            /** Returns the destination object of the specified class type at the specified index with which this property connects.
              * \param pFBX_TYPE The specified class type.
              * \param pIndex The specified index whose default value is 0.
              * \return The destination object of the specified class type at the specified index, NULL if not found.
              */
            template < class T > inline T*   GetDstObject(T const *pFBX_TYPE,int pIndex=0) const { return (T*)GetDstObject(T::ClassId,pIndex); }

            /** Returns the destination object which is of the specified class type and satisfies the given criteria at the specified index with which this property connects.
              * \param pFBX_TYPE The specified class type.
              * \param pCriteria The given criteria.
              * \param pIndex The specified index whose default value is 0.
              * \return The destination object which is of the specified class type and satisfies the given criteria at the specified index, NULL if not found.
              */
            template < class T > inline T*   GetDstObject(T const *pFBX_TYPE,KFbxCriteria const &pCriteria,int pIndex=0) const { return (T*)GetDstObject(T::ClassId,pCriteria,pIndex); }

            /** Searches the destination object with the specified name which is of the specified class type, starting with the specified index.
              * \param pFBX_TYPE The specified class type.
              * \param pName The object name.
              * \param pStartIndex The start index.
              * \return The source object with the name, NULL if not found.
              */
            template < class T > inline T*   FindDstObject(T const *pFBX_TYPE,const char *pName,int pStartIndex=0) const { return (T*)FindDstObject(T::ClassId,pName,pStartIndex); }

            /** Searches the destination object with the specified name which is of the specified class type and satisfies the given criteria, starting with the specified index.
              * \param pFBX_TYPE The specified class type.
              * \param pCriteria The given criteria.
              * \param pName The object name.
              * \param pStartIndex The start index.
              * \return The source object with the name, NULL if not found.
              */
            template < class T > inline T*   FindDstObject(T const *pFBX_TYPE,KFbxCriteria const &pCriteria,const char *pName,int pStartIndex=0) const { return (T*)FindDstObject(T::ClassId,pCriteria,pName,pStartIndex); }
        //@}

        /**
          * \name General Property Connection and Relationship Management
          */
        //@{
        public:
            // Properties
            /** Connects this property to a source property.
              * \param pProperty The source property with which this property connects.
              * \return \c True on success, \c false otherwise.
              */
            bool            ConnectSrcProperty      (KFbxProperty const & pProperty);

            /** Judges whether this property connects with the specified source property.
              * \param pProperty The specified source property.
              * \return \c True if this property connects with the specified source property, \c false otherwise.
              */
            bool            IsConnectedSrcProperty  (KFbxProperty const & pProperty);

            /** Disconnects this property from the specified source property.
              * \param pProperty The specified source property.
              * \return \c True on success, \c false otherwise.
              */
            bool            DisconnectSrcProperty   (KFbxProperty const & pProperty);

            /** Returns the number of source properties with which this property connects. 
              * \return The number of source properties with which this property connects. 
              */
            int             GetSrcPropertyCount     () const;

            /** Returns the source property at the specified index with which this property connects. 
              * \param pIndex The specified index.
              * \return The source property at the specified index. 
              */
            KFbxProperty    GetSrcProperty          (int pIndex=0) const;

            /** Searches the source property with the specified name, starting with the specified index with which this property connects. 
              * \param pName The specified property name.
              * \param pStartIndex The start index.
              * \return The source property with the specified name. 
              */
            KFbxProperty    FindSrcProperty         (const char *pName,int pStartIndex=0) const;

            /** Connects this property to a destination property.
              * \param pProperty The destination property with which this property connects.
              * \return \c True on success, \c false otherwise.
              */
            bool            ConnectDstProperty      (KFbxProperty const & pProperty);

            /** Judges if this property connects with the specified destination property.
              * \param pProperty The specified destination property.
              * \return \c True if this property connects with the specified destination property, \c false otherwise.
              */
            bool            IsConnectedDstProperty  (KFbxProperty const & pProperty);

            /** Disconnects this property from the specified destination property.
              * \param pProperty The specified destination property.
              * \return \c True on success, \c false otherwise.
              */
            bool            DisconnectDstProperty   (KFbxProperty const & pProperty);

            /** Returns the number of destination properties with which this property connects. 
              * \return The number of destination properties with which this property connects. 
              */
            int             GetDstPropertyCount     () const;

            /** Returns the destination property at the specified index with which this property connects. 
              * \param pIndex The specified index.
              * \return The destination property at the specified index. 
              */
            KFbxProperty    GetDstProperty          (int pIndex=0) const;

            /** Searches the destination property with the specified name, starting with the specified index with which this property connects. 
              * \param pName The specified property name.
              * \param pStartIndex The start index.
              * \return The destination property with the specified name. 
              */
            KFbxProperty    FindDstProperty         (const char *pName,int pStartIndex=0) const;

            //!Clears the connection cache of this property, this cache is used to store the connections that satisfy the given criteria. 
            void            ClearConnectCache();

        //@}

            //! Hierarchical separator of properties.
        static const char* sHierarchicalSeparator;

        ///////////////////////////////////////////////////////////////////////////////
        //  WARNING!
        //  Anything beyond these lines may not be documented accurately and is
        //  subject to change without notice.
        ///////////////////////////////////////////////////////////////////////////////
        #ifndef DOXYGEN_SHOULD_SKIP_THIS

        protected:
            //! Constructor / Destructor
            KFbxProperty(KFbxObject* pObject, char const* pName, KFbxDataType const &pDataType=KFbxDataType(), char const* pLabel="");
            KFbxProperty(KFbxProperty const & pParent, char const* pName, KFbxDataType const &pDataType, char const* pLabel);

        // General Property Connection and Relationship Management
        private:
            bool            ConnectSrc      (KFbxProperty const &pProperty,kFbxConnectionType pType=eFbxConnectionNone);
            bool            DisconnectSrc   (KFbxProperty const &pProperty);
            bool            DisconnectAllSrc();
            bool            DisconnectAllSrc(KFbxCriteria const &pCriteria);
            bool            IsConnectedSrc  (KFbxProperty const &pProperty) const;
            int             GetSrcCount     () const;
            int             GetSrcCount     (KFbxCriteria const &pCriteria) const;
            KFbxProperty    GetSrc          (int pIndex=0) const;
            KFbxProperty    GetSrc          (KFbxCriteria const &pCriteria,int pIndex=0) const;
            KFbxProperty    FindSrc         (KFbxCriteria const &pCriteria,const char *pName,int pStartIndex=0) const;

            bool            ConnectDst      (KFbxProperty const &pProperty,kFbxConnectionType pType=eFbxConnectionNone);
            bool            DisconnectDst   (KFbxProperty const &pProperty);
            bool            DisconnectAllDst();
            bool            DisconnectAllDst(KFbxCriteria const &pCriteria);
            bool            IsConnectedDst  (KFbxProperty const &pProperty) const;
            int             GetDstCount     () const;
            int             GetDstCount     (KFbxCriteria const &pCriteria) const;
            KFbxProperty    GetDst          (int pIndex=0) const;
            KFbxProperty    GetDst          (KFbxCriteria const &pCriteria,int pIndex=0) const;
            KFbxProperty    FindDst         (KFbxCriteria const &pCriteria,const char *pName,int pStartIndex=0) const;
        private:
            //! Internal management
            mutable KFbxPropertyHandle  mPropertyHandle;

            friend class KFbxObject;
        #endif // #ifndef DOXYGEN_SHOULD_SKIP_THIS
    };

    typedef KFbxProperty* HKFbxProperty;

    template <class T> inline bool  KFbxSet( KFbxProperty &pProperty,T const &pValue, bool pCheckValueEquality = true ) { return pProperty.Set( &pValue,FbxTypeOf(pValue),pCheckValueEquality ); }
    template <class T> inline bool  KFbxGet( KFbxProperty const& pProperty,T &pValue ) { return pProperty.Get( &pValue,FbxTypeOf(pValue) ); }
    template <class T> inline bool  KFbxGet( KFbxProperty& pProperty, T &pValue, KFbxEvaluationInfo const* pInfo)  { return pProperty.Get( &pValue, FbxTypeOf(pValue), pInfo ); }
    template <class T> inline T     KFbxGet( KFbxProperty const& pProperty)            { T pValue; pProperty.Get( &pValue,FbxTypeOf(pValue) ); return pValue; }


    /** \brief This template class is used to contain user properties of specific data types.
      * \nosubgrouping
      */
    template <class T> class KFbxTypedProperty : public KFbxProperty {
        public:

        /**
          * \name Constructor and Destructor.
          */
        //@{
            //! Constructor.
            inline KFbxTypedProperty() : KFbxProperty()                                         {}

            /** Copy constructor.
              * \param pProperty The property copied to this one.
              */
            inline KFbxTypedProperty(KFbxProperty const &pProperty) : KFbxProperty(pProperty)   {}

            //! Destructor.
            inline ~KFbxTypedProperty() {}

        //@}


        /**
          * \name Static initialization.
          */
        //@{
            /** Creates a property and initializes it using a specific flag.
              * \param pObject The object that contains this property.
              * \param pName   The name of the property.
              * \param pFlags  The property flag.
              */
            inline KFbxProperty const & StaticInit(KFbxObject* pObject, char const* pName,eFbxPropertyFlags pFlags=eNO_FLAG)
            {
                *this = Create(pObject, pName, GetFbxDataType(FbxTypeOf(*((T *)0))), "");
                ModifyFlag(pFlags, true);
                return *this;
            }

            /** Creates a property and initializes it using a specific value and flag.
              * \param pObject The object that contains this property.
              * \param pName   The name of the property.
              * \param pValue  The value of the property.
              * \param pForceSet If \c true, the value is forcibly set, if \c false the value is not set when it equals the default value. 
              * \param pFlags  The property flag.
              */
            inline KFbxProperty const & StaticInit(KFbxObject* pObject, char const* pName,T const &pValue,bool pForceSet=true,eFbxPropertyFlags pFlags=eNO_FLAG)
            {
                bool lWasFound = false;
                *this = Create(pObject, pName, GetFbxDataType(FbxTypeOf(*((T *)0))), "",true,&lWasFound);

                if( pForceSet || !lWasFound )
                {
                    ModifyFlag(pFlags, true); // modify the flags before we set the value
                    Set(pValue,false);
                }

                return *this;
            }

            /** Creates a property and initializes it using a specific flag.
              * \param pObject The object that contains this property.
              * \param pName   The name of the property.
              * \param pDataType  The property data type.
              * \param pFlags  The property flag.
              */
            inline KFbxProperty const & StaticInit(KFbxObject* pObject, char const* pName, KFbxDataType const &pDataType,eFbxPropertyFlags pFlags=eNO_FLAG)
            {
                *this = Create(pObject, pName, pDataType, "");
                ModifyFlag(pFlags, true);
                return *this;
            }

            /** Creates a property and initializes it using a specific value and flag.
              * \param pObject The object that contains this property.
              * \param pName   The name of the property.
              * \param pDataType  The property data type.
              * \param pValue  The property value.
              * \param pForceSet If \c true, the value is forcibly set, if \c false the value is not set when it equals the default value. 
              * \param pFlags  The property flag.
              */
            inline KFbxProperty const & StaticInit(KFbxObject* pObject, char const* pName, KFbxDataType const &pDataType,T const &pValue, bool pForceSet=true, eFbxPropertyFlags pFlags=eNO_FLAG)
            {
                bool lWasFound = false;
                *this = Create(pObject, pName, pDataType, "",true,&lWasFound);

                if( pForceSet || !lWasFound )
                {
                    ModifyFlag(pFlags, true); // modify the flags before we set the value
                    // since we will trigger callbacks in there!
                    Set(pValue,false);
                }

                return *this;
            }

            /** Creates a property and initializes it using a specific value and flag.
              * \param pCompound The parent property of this property.
              * \param pName   The name of the property.
              * \param pDataType  The property data type.
              * \param pValue  The property value.
              * \param pForceSet If \c true, the value is forcibly set, if \c false the value is not set when it equals to the default value. 
              * \param pFlags  The property flag.
              */
            inline KFbxProperty const & StaticInit(KFbxProperty pCompound, char const* pName, KFbxDataType const &pDataType,T const &pValue, bool pForceSet=true, eFbxPropertyFlags pFlags=eNO_FLAG)
            {
                bool lWasFound = false;
                *this = Create(pCompound, pName, pDataType, "",true,&lWasFound);

                if( pForceSet || !lWasFound )
                {
                    ModifyFlag(pFlags, true); // modify the flags before we set the value
                    // since we will trigger callbacks in there!
                    Set(pValue,false);
                }

                return *this;
            }

        //@}
        public:

            /** Assignment operator.
              * \param pValue The value assigned to this property.
              * \return This property.
              */
            KFbxTypedProperty &operator=(T const &pValue)      { KFbxSet(*this,pValue); return *this; }

            /** Sets the value of this property.
              * \param pValue The new value
              * \param pCheckValueEquality If \c true, the value is not set if it equals the default value.
              * \return \c True if the value is set successfully, \c false otherwise.
              */
            bool     Set(T const &pValue, bool pCheckValueEquality )    { return KFbxSet(*this,pValue,pCheckValueEquality); }

            /** Sets the value of this property.
              * \param pValue The new value
              * \return \c True if the value is set successfully, \c false otherwise.
              */
            bool     Set(T const &pValue )  { return KFbxSet(*this,pValue,true); }

            /** Returns the value of this property.
              * \return The value of the property.
              */
            T        Get() const            { T lValue; KFbxGet(*this,lValue); return lValue; }

            /** Evaluates the KFCurve value of the property at the specific time, then returns the value of the property.
              * \param pInfo Contains the time of evaluation.
              * \return The value of the property after evaluation.
              * \remarks When evaluated, the property value is set as the KFCurve value. 
              */
            T        Get( KFbxEvaluationInfo const* pInfo ) { T lValue; KFbxGet( *this, lValue, pInfo); return lValue; }

        friend class KFbxObject;
    };

    // For use with deprecated type functions
    //KFBX_DLL KFbxDataType                       EUserPropertyTypeToDataType(KFbxProperty::EUserPropertyType);
    //KFBX_DLL KFbxProperty::EUserPropertyType        DataTypeToEUserPropertyType(const KFbxDataType &pDataType);

    /** \brief This template class is specialized using the pointer of class fbxReference to hold user properties whose data type are fbxReference.
      * \nosubgrouping
      */
    template <> class KFbxTypedProperty<fbxReference*> : public KFbxProperty
    {
    public:
        /**
          * \name Constructor and Destructor.
          */
        //@{

        //! Constructor.
        inline KFbxTypedProperty() : KFbxProperty()
        {}

        /** Copy constructor.
          * \param pProperty The property copied to this one.
          */
        inline KFbxTypedProperty(KFbxProperty const &pProperty)
            : KFbxProperty(pProperty)
        {}

        //! Destructor.
        inline ~KFbxTypedProperty()
        {}
        //@}

        /**
          * \name Static initialization.
          */
        //@{
        /** Creates a property and initializes it using a specific flag.
          * \param pObject The object that contains this property.
          * \param pName   The name of the property.
          * \param pFlags  The property flag.
          */
        inline KFbxProperty const & StaticInit(KFbxObject* pObject,char const* pName,eFbxPropertyFlags pFlags=eNO_FLAG)
        {
            *this = KFbxProperty::Create(pObject, pName, GetFbxDataType(FbxTypeOf(*((fbxReference* *)0))), "");
            ModifyFlag(pFlags, true);
            return *this;
        }

        /** Creates a property and initializes it using a specific value and flag.
          * \param pObject The object that contains this property.
          * \param pName   The name of the property.
          * \param pValue  The value of the property.
          * \param pForceSet If \c true, the value is set forcibly, if \c false the value is not set when it equals the default value. 
          * \param pFlags  The property flag.
          */
        inline KFbxProperty const & StaticInit(KFbxObject* pObject,
                                               char const* pName,
                                               fbxReference* const &pValue,
                                               bool pForceSet=true,
                                               eFbxPropertyFlags pFlags=eNO_FLAG
                                               )
        {
            bool lWasFound = false;
            *this = KFbxProperty::Create(pObject, pName, GetFbxDataType(FbxTypeOf(*((fbxReference* *)0))), "",true, &lWasFound);
            if( pForceSet || !lWasFound )
            {
                ModifyFlag(pFlags, true);
                Set(pValue,false);
            }

            return *this;
        }


        /** Creates a property and initializes it using a specific flag.
          * \param pObject The object that contains this property.
          * \param pName   The name of the property.
          * \param pDataType  The data type of the property.
          * \param pFlags  The property flag.
          */
        inline KFbxProperty const & StaticInit(KFbxObject* pObject,
                                               char const* pName,
                                               KFbxDataType const &pDataType,
                                               eFbxPropertyFlags pFlags=eNO_FLAG)
        {
            *this = KFbxProperty::Create(pObject, pName, pDataType, "");
//          KFbxProperty::StaticInit(pObject, pName, pDataType, "");
            ModifyFlag(pFlags, true);
            return *this;
        }

        /** Creates a property and initializes it using a specific value and flag.
          * \param pObject The object that contains this property.
          * \param pName   The name of the property.
          * \param pDataType  The data type of the property.
          * \param pValue  The value of the property.
          * \param pForceSet If \c true, the value is forcibly set, if \c false the value is not set when it equals the default value. 
          * \param pFlags  The property flag.
          */
        inline KFbxProperty const & StaticInit(KFbxObject* pObject,
                                               char const* pName,
                                               KFbxDataType const &pDataType,
                                               fbxReference* const &pValue,
                                               bool pForceSet=true,
                                               eFbxPropertyFlags pFlags=eNO_FLAG
                                               )
        {
            bool lWasFound = false;
            *this = KFbxProperty::Create(pObject, pName, pDataType, "",true,&lWasFound);

            if( pForceSet || !lWasFound )
            {
                ModifyFlag(pFlags, true);
                Set(pValue,false);
            }

            return *this;
        }
    //@}

    public:

        /** Assignment operator.
          * \param pValue The value assigned to this property.
          * \return This property.
          */
        KFbxTypedProperty &operator=(fbxReference* const &pValue)
        {
            KFbxSet(*this,pValue);
            return *this;
        }

        /** This function connects this property to the source object.
          * \param pValue The pointer of the source object.
          * \return \c True on success, \c false otherwise.
          * \remarks This function is different from the set value function of KFbxProperty.
          *          It connects this property to the source object whose pointer is pValue.
                     Before connecting, it disconnects all source objects.
          */
        inline bool Set(fbxReference* const &pValue ) { return Set(pValue, true); }

        /** This function connects this property to the source object.
          * \param pValue The pointer of the source object.
          * \param pCheckValueEquality 
          * \return \c True on success, \c false otherwise.
          * \remarks This function is different from the set value function of KFbxProperty.
          *          It connects this property to the source object whose pointer is pValue.
                     Before connecting, it disconnects all source objects.
          */
        bool Set(fbxReference* const &pValue, bool pCheckValueEquality )
        {
            KFbxObject* lValue = reinterpret_cast<KFbxObject*>(pValue);
            DisconnectAllSrcObject();
            if (lValue) {
                return ConnectSrcObject(lValue);
            }

            return false;
        }

        /** Returns the source object.
          * \return The pointer of the source object. 
          */
        fbxReference* Get() const
        {
            KFbxObject* lValue = GetSrcObjectCount() > 0 ? GetSrcObject(0) : NULL;
            return reinterpret_cast<fbxReference*>(lValue);
        }

        friend class KFbxObject;
    };


    typedef KFbxTypedProperty<fbxBool1>         KFbxPropertyBool1;
    typedef KFbxTypedProperty<fbxInteger1>      KFbxPropertyInteger1;
    typedef KFbxTypedProperty<fbxDouble1>       KFbxPropertyDouble1;
    typedef KFbxTypedProperty<fbxDouble3>       KFbxPropertyDouble3;
    typedef KFbxTypedProperty<fbxDouble4>       KFbxPropertyDouble4;
    typedef KFbxTypedProperty<fbxString>        KFbxPropertyString;
    typedef KFbxTypedProperty<fbxReference*>    KFbxPropertyReference;

    /** \enum eFbxConnectEventType      Connection event type.
      * - \e eFbxConnectRequest         
      * - \e eFbxConnect
      * - \e eFbxConnected
      * - \e eFbxDisconnectRequest
      * - \e eFbxDisconnect
      * - \e eFbxDisconnected
      */
    enum eFbxConnectEventType {
        eFbxConnectRequest,
        eFbxConnect,
        eFbxConnected,
        eFbxDisconnectRequest,
        eFbxDisconnect,
        eFbxDisconnected
    };

    /** \enum eFbxConnectEventDirection      Connection event direction.
      * - \e eConnectEventSrc         
      * - \e eConnectEventDst
      */
    enum eFbxConnectEventDirection {
        eConnectEventSrc,
        eConnectEventDst
    };

    /** Class handles Connection events.
      * \nosubgrouping
      */
    class KFBX_DLL KFbxConnectEvent
    {
        /**
          * \name Constructor and Destructor.
          */
        //@{
        public:

            /** Constructor.
              * \param pType The connection event type.
              * \param pDir  The direction of connection event.
              * \param pSrc  The source property.
              * \param pDst  The destination property.
              */
            inline KFbxConnectEvent(eFbxConnectEventType pType,eFbxConnectEventDirection pDir,KFbxProperty *pSrc,KFbxProperty *pDst)
                : mType(pType)
                , mDirection(pDir)
                , mSrc(pSrc)
                , mDst(pDst)
            {
            }
        //@}

        /**
          * \name Data Access.
          */
        //@{
        public:

            //! Returns the connection event type.
            inline eFbxConnectEventType GetType() const { return mType; }

            //! Returns the direction of connection event.
            inline eFbxConnectEventDirection GetDirection() const { return mDirection; }

            //! Returns the source property.
            inline KFbxProperty &GetSrc()  const    { return *mSrc;  }

            //! Returns the destination property.
            inline KFbxProperty &GetDst()  const    { return *mDst;  }

            /** Returns the object of the specified class type that contains the source property.
              * \param pFBX_TYPE The specified class type.
              * \return The object of the specified class type that contains the source property, \c NUll if this property is not a root property or no object of the specified class type contains this property.
              */
            template < class T > inline T*  GetSrcIfObject(T const *pFBX_TYPE) const    { return mSrc->IsRoot() ? KFbxCast<T>(mSrc->GetFbxObject()) : (T*)0; }

            /** Returns the object of the specified class type that contains the destination property.
              * \param pFBX_TYPE The specified class type.
              * \return The object of the specified class type that contains the destination property, \c NUll if this property is not a root property or no object of the specified class type contains this property.
              */
            template < class T > inline T*  GetDstIfObject(T const *pFBX_TYPE) const    { return mDst->IsRoot() ? KFbxCast<T>(mDst->GetFbxObject()) : (T*)0; }
        //@}

        private:
            eFbxConnectEventType        mType;
            eFbxConnectEventDirection   mDirection;
            KFbxProperty*               mSrc;
            KFbxProperty*               mDst;
    };



#include <fbxfilesdk/fbxfilesdk_nsend.h>

#endif // FBXFILESDK_KFBXPLUGINS_KFBXPROPERTY_H

