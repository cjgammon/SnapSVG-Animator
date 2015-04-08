/*************************************************************************
* ADOBE CONFIDENTIAL
* ___________________
*
*  Copyright [2013] Adobe Systems Incorporated
*  All Rights Reserved.
*
* NOTICE:  All information contained herein is, and remains
* the property of Adobe Systems Incorporated and its suppliers,
* if any.  The intellectual and technical concepts contained
* herein are proprietary to Adobe Systems Incorporated and its
* suppliers and are protected by all applicable intellectual 
* property laws, including trade secret and copyright laws.
* Dissemination of this information or reproduction of this material
* is strictly forbidden unless prior written permission is obtained
* from Adobe Systems Incorporated.
**************************************************************************/

/**
 * @file  IFCMDictionary.h
 *
 * @brief This file contains interface for IFCMDictionary. 
 *        The IFCMDictionary is an interface for generic container for storing data.
 */ 

#ifndef IFCM_DICTIONARY_H_
#define IFCM_DICTIONARY_H_

#include "FCMPreConfig.h"
#include "FCMTypes.h"
#include "IFCMUnknown.h"


/* -------------------------------------------------- Forward Decl */


/* -------------------------------------------------- Macros / Constants */

namespace FCM 
{
    /**
     * @brief Defines the universally-unique interface ID for IFCMDictionary.
     *
     * @note  Textual Representation:  {8BB58643-057E-4158-8876-6FF334E5ECE6}
     */
    FCM::ConstFCMIID FCMIID_IFCMDictionary =
        {0x8bb58643, 0x57e, 0x4158, {0x88, 0x76, 0x6f, 0xf3, 0x34, 0xe5, 0xec, 0xe6}};


    /**
     * @def   FCMDICT_RUNTIME_KEY_PREFIX  
     * @brief By convention, all keys beginning with these characters will never be written to
     *        persistent storage; they are runtime-only keys.
     */
    #define FCMDICT_RUNTIME_KEY_PREFIX    "_rt_"
}


/* -------------------------------------------------- Enums */

namespace FCM
{
    /**
     * @brief This defines different types of dictionary IDs.
     */
    typedef enum
    {
        /** illegal */
        kFCMDictType_Invalid = 0,

        /** FCMDict. Put does AddRef; Remove does Release. */
        kFCMDictType_Dict = 1,

        /** Returns a PIFCMUnknown. Put does AddRef; Remove does Release. */
        kFCMDictType_PIFCMUnknown,

        /** Returns a FCM::S_Int32. */
        kFCMDictType_Long,
        
        /** Returns a 32 bit IEEE single precision floating point number. */
        kFCMDictType_Float,
        
        /** FCM::Boolean */
        kFCMDictType_Bool,
        
        /** FCMGUID */
        kFCMDictType_FCMGUID,

        /** Arbitrary bytes */
        kFCMDictType_Bytes,
        
        /** Null-terminated StringRep8 string */
        kFCMDictType_StringRep8,

        /** FCM::Double */
        kFCMDictType_Double,

        /* Last */
        _kFCMDictType_Last_

    } FCMDictRecTypeID;

};  // namespace FCM


/* -------------------------------------------------- Structs / Unions */


/* -------------------------------------------------- Class Decl */

namespace FCM
{ 
    /**
     * @class IFCMDictionary
     *
     * @brief Defines the interface that represents the dictionary.
     */
    BEGIN_DECLARE_INTERFACE(IFCMDictionary, FCMIID_IFCMDictionary)

        /**
         * @brief  Inserts a value for a given key into a dictionary.
         *
         * @param  pKey (IN)
         *         The key for the value being stored.
         *
         * @param  type (IN)
         *         The type of value being stored and it should be of type FCMDictRecTypeID.
         *
         * @param  value (IN)
         *         The value being stored.
         *
         * @param  valueLen (IN)
         *         The length of the value being stored.
         *
         * @return FCM_SUCCESS is returned for success, else an error code is returned.
         */
         virtual FCM::Result _FCMCALL Add(
            CStringRep8 pKey,
            FCMDictRecTypeID type,
            FCM::PVoid value,
            FCM::U_Int32 valueLen) = 0;

    
        /**
         * @brief  Create a dictionary and places it inside another under a given key.
         *         Optionally returns the new dict (if ppDict is not NULL). The dict 
         *         returned is "owned" by outer dict and does not need to be released by the caller.
         *
         * @param  pKey (IN)
         *         The key for the dictionary being created.
         *
         * @param  ppDict (OUT)
         *         Set with the new dictionary created.
         *
         * @return FCM_SUCCESS is returned for success, else an error code is returned.
         */
         virtual FCM::Result _FCMCALL AddLevel(CStringRep8 pKey, PIFCMDictionary& ppDict) = 0;


        /**  
         * @brief  Removes an entry for the given key from a dictionary. For reference counted
         *         types, count will simply be decremented.
         *
         * @param  pKey (IN)
         *         The key for value being removed.
         *
         * @return FCM_SUCCESS is returned for success, else an error code is returned.
         */
         virtual FCM::Result _FCMCALL Remove(CStringRep8    pKey) = 0;


        /**  
         * @brief  Returns a value from a dictionary given its key. The param value should be
         *         allocated and the allocation length should be passed in param valueLen.
         *         If the allocated buffer is less than the value stored in the dictionary then
         *         FCM_DICT_ERR_BUFFERTOOSMALL is returned. If the allocated buffer is greater 
         *         than (or) equal to the length of value in dictionary then value in dictionary
         *         is coped into value param and the valueLen param is set to the length of value
         *         in dictionary.
         *
         * @param  pKey (IN)
         *         The key for value being looked for.
         *
         * @param  type (IN)
         *         The type of value being looked for and it should be of type FCMDictRecTypeID.
         *
         * @param  pValue (OUT)
         *         The value corresponding to the key is copied into this parameter.
         *
         * @param  valueLen (IN-OUT)
         *         The length of buffer allocated for value param.In successful case would conatin
         *         the actual length of value in the dictionary.
         *
         * @return FCM_SUCCESS is returned for success, else an error code is returned.
         */
         virtual FCM::Result _FCMCALL Get(
             CStringRep8 pKey,
             FCMDictRecTypeID type,
             FCM::PVoid pValue,
             FCM::U_Int32& valueLen) = 0;


        /**
         * @brief  Removes all the values in dictionary.
         *
         * @return FCM_SUCCESS is returned for success. Otherwise an error code is returned.
         */
         virtual FCM::Result _FCMCALL Clear() = 0;


        /**  
         * @brief  Fetches the number of entries in the dictionary.
         *
         * @param  count (OUT)
         *         Populated with the number of entries in the dictionary.
         *
         * @return FCM_SUCCESS is returned for success, else an error code is returned.
         */
         virtual FCM::Result _FCMCALL Count(FCM::U_Int32& count) = 0;


        /**  
         * @brief  Returns info on the nth entry in a dictionary.
         *
         * @param  index (IN)
         *         The location of the entry in dictionary for which we need info. Index should 
         *         range from 1 to Count.
         *
         * @param  pKey (OUT)
         *         The key of the nth entry is copied into this parameter. Should be freed using 
         *         <tt>IFCMCalloc</tt> interface.
         *
         * @param  type (OUT)
         *         The type of the nth entry is copied into this parameter.
         *
         * @param  valueLen (OUT)
         *         The length of the nth entry is copied into this parameter.
         *
         * @return FCM_SUCCESS is returned for success. Otherwise an error code is returned.
         */
         virtual FCM::Result _FCMCALL GetNth(
             FCM::U_Int32 index,
             StringRep8 & pKey,
             FCMDictRecTypeID& type,
             FCM::U_Int32& valueLen) = 0;


        /**  
         * @brief  Returns info on the entry with the given key in a dictionary.
         *
         * @param  pKey (IN)
         *         The key of the entry being looked for.
         *
         * @param  type (OUT)
         *         The type of the entry with the given key is copied into this parameter.
         *
         * @param  valueLen (OUT)
         *         The length of the entry with the given key is copied into this parameter.
         *
         * @return FCM_SUCCESS is returned for success, else an error code is returned.
         */
         virtual FCM::Result _FCMCALL GetInfo(
             CStringRep8 pKey,
             FCMDictRecTypeID& type,
             FCM::U_Int32& valueLen) = 0;

    END_DECLARE_INTERFACE

};  // namespace FCM


/* -------------------------------------------------- Inline / Functions */


#include "FCMPostConfig.h"

#endif //IFCM_DICTIONARY_H_