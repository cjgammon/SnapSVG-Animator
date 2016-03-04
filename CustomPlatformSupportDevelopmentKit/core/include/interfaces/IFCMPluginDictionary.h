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
 * @file  IFCMPluginDictionary.h
 *
 * @brief This file contains interface for IFCMPluginDictionary. 
 *        The IFCMPluginDictionary interface provides a generic mechanism for exchanging plug-in
 *        data with FCM Application. This interface provides a list of registry dictionaries. Each
 *        registry dictionary corresponds to dictionary for an interface implementation in plug-in.
 *        <tt>IFCMPluginDictionary</tt> aggregates <tt>IFCMDictionary</tt>. The FCM Application
 *        can define a set of keys for an interface and data corresponding to these keys will be 
 *        populated by the plug-in in the registry dictionary and this data can be used by the FCM
 *        Application.
 */

#ifndef IFCM_PLUGIN_DICTIONARY_H_
#define IFCM_PLUGIN_DICTIONARY_H_

#include "FCMPreConfig.h"
#include "FCMPluginInterface.h"
#include "IFCMDictionary.h"


/* -------------------------------------------------- Forward Decl */


/* -------------------------------------------------- Macros / Constants */

namespace FCM 
{
    /**
     * @brief Defines the universally-unique interface ID for IFCMPluginDictionary.
     *
     * @note  Textual Representation:  {6DC4BA91-3BF2-4DD7-85B6-8D6C3E765520}
     */
    FCM::ConstFCMIID FCMIID_IFCMPluginDictionary =
        {0x6dc4ba91, 0x3bf2, 0x4dd7, {0x85, 0xb6, 0x8d, 0x6c, 0x3e, 0x76, 0x55, 0x20}};
}


/* -------------------------------------------------- Enums */


/* -------------------------------------------------- Structs / Unions */


/* -------------------------------------------------- Class Decl */

namespace FCM
{ 
    /**
     * @class IFCMPluginDictionary
     *
     * @brief Defines the interface that represents the Plugin Dictionary.
     */
    BEGIN_DECLARE_INTERFACE(IFCMPluginDictionary, FCMIID_IFCMPluginDictionary)

        /**
         * @brief  Create a registry dictionary and adds it to the list of registry dictionaries.
         *         Optionally return the new dict (if ppRegistryDict is not NULL).The registry dict
         *         returned by CreateRegistryEntry is owned by the cache, and you MUST NOT call
         *         Release() on it (unless you also call AddRef() on it).
         *
         * @param  classID (IN)
         *         The class Id of the registry dict.
         *
         * @param  interfaceID (IN)
         *         The interface Id of the registry dict.
         *
         * @param  ppRegistryDict (OUT)
         *         Set with the new dictionary created.
         *
         * @return FCM_SUCCESS is returned for success, else an error code is returned.
         */
        virtual FCM::Result _FCMCALL CreateRegistryEntry(
            ConstRefFCMCLSID classID,
            ConstRefFCMIID interfaceID,
            PIFCMDictionary* ppRegistryDict) = 0;


        /**
         * @brief  Fetches the number of registry dictionaries.
         *
         * @param  count (OUT)
         *         Populated with the number of registry dictionaries.
         *
         * @return FCM_SUCCESS is returned for success, else an error code is returned.
         */
        virtual FCM::Result _FCMCALL GetRegistryEntriesCount(FCM::U_Int32& count) = 0;


        /**  
         * @brief  Returns the nth registry dictionary. 
         *
         * @param  index (IN)
         *         The location of the registry dictionary in the list of registry dictionaries for
         *         which we need info. index should range from 1 and Count.
         *
         * @param  classID (OUT)
         *         Set with the class Id of the registry dict.
         *
         * @param  interfaceID (OUT)
         *         Set with the interface Id of the registry dict.
         *
         * @param  pRegistryDict (OUT)
         *         Set with the nth registry dictionary.
         *
         * @return FCM_SUCCESS is returned for success, else an error code is returned.
         */
        virtual FCM::Result _FCMCALL GetNthRegistryEntry(
            FCM::U_Int32 index,
            FCMCLSID& classID,
            FCMIID& interfaceID,
            PIFCMDictionary& pRegistryDict) = 0;
    
    END_DECLARE_INTERFACE

};  // namespace FCM


/* -------------------------------------------------- Inline / Functions */


#include "FCMPostConfig.h"

#endif //IFCM_PLUGIN_DICTIONARY_H_