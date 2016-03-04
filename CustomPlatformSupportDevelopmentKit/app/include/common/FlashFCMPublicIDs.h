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
 * @file  FlashFCMPublicIDs.h
 *
 * @brief This file contains the dictionary keys and GUIDs for all the required 
 *        interfaces that help you access the services that the interface provides.
 *
 * @deprecated This file is deprecated. Please use ApplicationFCMPublicIDs.h.
 */

#ifndef FLASH_FCM_PUBLIC_IDS_H_
#define FLASH_FCM_PUBLIC_IDS_H_

#include "FCMPreConfig.h"
#include "ApplicationFCMPublicIDs.h"


/* -------------------------------------------------- Forward Decl */


/* -------------------------------------------------- Enums */


/* -------------------------------------------------- Macros / Constants */



namespace Application
{
    /**
     * @def   kFlashCategoryKey_DocType
     *
     * @brief Doc type category key. The value for this key should be a dictionary
     *        that defines a document type.
     *
     * @deprecated This macro is deprecated. Please use kApplicationCategoryKey_DocType.
     */
    #define kFlashCategoryKey_DocType            "Flash.Component.Category.DocType"


    /**
     * @def   kFlashCategoryKey_Publisher
     *
     * @brief Publisher category Key. The value for this key should be a dictionary
     *        that defines a publisher.
     *
     * @deprecated This macro is deprecated. Please use kApplicationCategoryKey_Publisher.
     */
    #define kFlashCategoryKey_Publisher            "Flash.Component.Category.Publisher"


    /**
     * @def   kFlashCategoryKey_Name
     *
     * @brief The name key of the category. The value for this key should 
     *        have the name of the category of the component.
     *        A component may belong to many categories. Publisher and DocType 
     *        are examples of categories.
     *
     * @deprecated This macro is deprecated. Please use kApplicationCategoryKey_Name.
     */
    #define kFlashCategoryKey_Name                "Flash.Component.Category.Name"


    /**
     * @def   kFlashCategoryKey_UniversalName
     *
     * @brief The universal name key of the category. The value for this key should
     *        have the universal name of the category of the component.
     *        A component may belong to many categories.
     *
     * @deprecated This macro is deprecated. Please use kApplicationCategoryKey_UniversalName.
     */
    #define kFlashCategoryKey_UniversalName    "Flash.Component.Category.UniversalName"


    /**
     * @def   kFlashDocTypeKey_Desc
     *
     * @brief DocType description key. The value for this key should have the description about the DocType.
     *
     * @deprecated This macro is deprecated. Please use kApplicationDocTypeKey_Desc.
     */
    #define kFlashDocTypeKey_Desc                "Flash.Component.DocType.Desc"

    
    /**
     * @def   kFlashPublisherKey_UI
     *
     * @brief The ui key of the Publisher. The value is the ID of the publish settings UI
     *
     * @deprecated This macro is deprecated. Please use kApplicationPublisherKey_UI.
     */
    #define kFlashPublisherKey_UI                "Flash.Component.Publisher.UI"


    /**
     * @def   kFlashPublisherKey_TargetDocs
     *
     * @brief The target docs key of the Publisher. The value is a dictionary with 
     *        keys as the target doc IDs supported by the Publisher. The values 
     *        in the dictionary are ignored.
     *
     * @deprecated This macro is deprecated. Please use kApplicationPublisherKey_TargetDocs.
     */
    #define kFlashPublisherKey_TargetDocs        "Flash.Component.Publisher.TargetDocs"


    namespace Service
    {
        /**
         * @brief Defines the universally-unique service ID for a flash application service. 
         *        Use this service ID to get a pointer to the service interface 
         *        "IFlashApplicationService".
         *
         * @note  Textual Representation: {656F39DD-8F8F-4E2F-B0B0-1257E00D501B}
         *
         * @deprecated This constant is deprecated. Please use APP_SERVICE.
         */
        static const FCM::SRVCID FLASHAPP_SERVICE = 
            {0x656f39dd, 0x8f8f, 0x4e2f, {0xb0, 0xb0, 0x12, 0x57, 0xe0, 0xd, 0x50, 0x18}};

        /**
         * @brief Defines the universally-unique service ID for a output console service. 
         *        Use this service ID to get a pointer to the service interface 
         *        "IOutputConsoleService". 
         *
         * @note  Textual Representation: {FCA7CC58-26E5-4A9F-865D-27E2DA47B07A}
         *
         * @deprecated This constant is deprecated. Please use APP_OUTPUT_CONSOLE_SERVICE.
         */
        static const FCM::SRVCID FLASHAPP_OUTPUT_CONSOLE_SERVICE = APP_OUTPUT_CONSOLE_SERVICE;
    }
};




/* -------------------------------------------------- Structs / Unions */


/* -------------------------------------------------- Class Decl */


/* -------------------------------------------------- Inline / Functions */


#include "FCMPostConfig.h"

#endif // FLASH_FCM_PUBLIC_IDS_H_

