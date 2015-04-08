/******************************************************************************
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
******************************************************************************/

/**
 * @file  FCMPublicIDs.h
 *
 * @brief This file contains all the public IDs used in FCM.
 */

#ifndef FCM_PUBLIC_IDS_H_
#define FCM_PUBLIC_IDS_H_

#include "FCMPreConfig.h"
#include "FCMTypes.h"


/* -------------------------------------------------- Forward Decl */


/* -------------------------------------------------- Enums */


/* -------------------------------------------------- Macros / Constants */

namespace FCM
{
    /**
     * @def   kFCMDictKey_ServiceDict
     *
     * @brief Service Dictionary Key. The value for this key should be a dictionary
     *        that defines services the plugin provides.
     */
    #define kFCMDictKey_ServiceDict                "FCM.Service.Dictionary"


    /**
     * @def   kFCMDictType_ServiceDict
     *
     * @brief Service Dictionary Type 
     */
    #define kFCMDictType_ServiceDict             kFCMDictType_Dict


    /**
     * @def   kFCMComponent
     *
     * @brief FCM component key. The value for this key should be a dictionary
     *        that defines the components in the plugin.
     */
    #define kFCMComponent                    "FCM.Component"


    /**
     * @brief Defines the universally-unique ID for
     *        the memory service.
     *
     * @note  Textual Representation: {3E3AC427-4F7F-4AA4-B7BB-E13D45DB73DB}
     */
    const FCM::SRVCID SRVCID_Core_Memory = 
        {0x3e3ac427, 0x4f7f, 0x4aa4, {0xb7, 0xbb, 0xe1, 0x3d, 0x45, 0xdb, 0x73, 0xdb}};


     /**
      * @brief Defines the universally-unique ID for 
      *        the Notification service.
      *
      * @note  Textual Representation: {F30EA34-403B-4112-99ED-C5A87FF1B163}
      */
    const FCM::SRVCID SRVCID_Core_Notification = 
        {0xf30ea374, 0x403b, 0x4112, {0x99, 0xed, 0xc5, 0xa8, 0x7f, 0xf1, 0xb1, 0x63}};
    

    /**
      * @brief Defines the universally-unique ID for 
      *        a service to convert strings between StringRep8 and StringRep16.
      *
      * @note  Textual Representation: {A04640CB-9148-4C4D-826E-E5BAEA88EFDE}
      */
    const FCM::SRVCID SRVCID_Core_StringUtils =
        {0xa04640cb, 0x9148, 0x4c4d, {0x82, 0x6e, 0xe5, 0xba, 0xea, 0x88, 0xef, 0xde}};
    

    /**
     * @brief Defines the universally-unique class ID to
     *        create object that implements IFCMDictionary dictionary interface.
     *
     * @note  Textual Representation: {01E262C7-886C-4F8C-872C-A0FD70672180}
     */
    const FCMCLSID CLSID_IFCMDictionary =
         {0x1e262c7, 0x886c, 0x4f8c, {0x87, 0x2c, 0xa0, 0xfd, 0x70, 0x67, 0x21, 0x80}};


     /**
     * @brief Defines the universally-unique ID for 
     *        the shutdown notification.
     *
     * @note  Textual Representation: {E6FE94C6-6184-4859-91A6-5729CE4465E6}
     */
    const FCMNotifyID NOTIFYID_FCM_SHUTDOWN =
        {0xe6fe94c6, 0x6184, 0x4859, {0x91, 0xa6, 0x57, 0x29, 0xce, 0x44, 0x65, 0xe6}};
    
};


/* -------------------------------------------------- Structs / Unions */


/* -------------------------------------------------- Class Decl */


/* -------------------------------------------------- Inline / Functions */


#include "FCMPostConfig.h"

#endif //FCM_PUBLIC_IDS_H_
