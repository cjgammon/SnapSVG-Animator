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
 * @file  IFlashApplicationService.h
 *
 * @brief This file contains interface for IFlashApplicationService.
 *        IFlashApplicationService provides Flash Application level services.
 */

#ifndef IFLASH_APPLICATION_SERVICE_H_
#define IFLASH_APPLICATION_SERVICE_H_

#include "FCMPreConfig.h"
#include "FCMTypes.h"
#include "IFCMUnknown.h"
#include "Utils/DOMTypes.h"

/* -------------------------------------------------- Forward Decl */

namespace DOM
{
    FORWARD_DECLARE_INTERFACE(IFLADocument);
};


/* -------------------------------------------------- Enums */


/* -------------------------------------------------- Macros / Constants */

namespace Application 
{
    namespace Service
    {
        /**
         * @brief Defines the universally-unique Interface ID for IFlashApplicationService.
         *
         * @note  Textual Representation: {CF97A574-E99B-43AB-931A-1C3ABB2A98E6}
         */
        FCM::ConstFCMIID IID_IFLASH_APPLICATION_SERVICE =
            {0xcf97a574, 0xe99b, 0x43ab, {0x93, 0x1a, 0x1c, 0x3a, 0xbb, 0x2a, 0x98, 0xe6}};
    }
}


/* -------------------------------------------------- Structs / Unions */


/* -------------------------------------------------- Class Decl */

namespace Application 
{
    namespace Service
    {
        /**
         * @class IFlashApplicationService
         *
         * @brief This service provides Flash application-level services.
         */
        BEGIN_DECLARE_INTERFACE(IFlashApplicationService, IID_IFLASH_APPLICATION_SERVICE)

            /**
             * @brief  This function returns the active FLA document.
             *
             * @param  pDocument (OUT)
             *         Active FLA document
             *
             * @return On success, FCM_SUCCESS is returned; else an error code is returned.
             */
            virtual FCM::Result _FCMCALL GetActiveFLADocument(DOM::PIFLADocument& pDocument) = 0;


            /**
             * @brief  This function returns the Flash Professional version.
             *
             * @param  version (OUT)
             *         FlashPro Version
             *
             * @note   "version" is a 32-bit integer. Version can be converted into form 
             *         Major.Minor.Maintenance.Build, by doing the following:
             *
             *         ((version >> 24) & 0xFF) gives the Major number.
             *         ((version >> 16) & 0xFF) gives the Minor number.
             *         ((version >>  8) & 0xFF) gives the Maintenance number.
             *         ((version      ) & 0xFF) gives the Build number.
             *
             * @return On success, FCM_SUCCESS is returned; else an error code is returned.
             */
            virtual FCM::Result _FCMCALL GetVersion(FCM::U_Int32& version) = 0;


            /**
             * @brief  This function returns the locale of the Flash Professional user interface.
             *
             * @param  ppLanguageCode (OUT)
             *         Five character code identifying the locale/language of the application’s user 
             *         interface. Following language codes are supported:
             *         "en_US", "ja_JP", "zh_CN", "zh_TW", "ko_KR", "nl_NL", "fr_FR", "de_DE",
             *         "it_IT", "es_ES", "sv_SE", "pt_BR", "cs_CZ", "pl_PL", "ru_RU", "tr_TR"
             *
             * @return On success, FCM_SUCCESS is returned; else an error code is returned.
             *
             * @note   The caller of this function must release the 
             *         memory for 'ppLanguageCode' using IFCMCalloc::Free().
             */
            virtual FCM::Result _FCMCALL GetLanguageCode(StringRep8* ppLanguageCode) = 0;

        END_DECLARE_INTERFACE 
    }
}; 


/* -------------------------------------------------- Inline / Functions */


#include "FCMPostConfig.h"

#endif // IFLASH_APPLICATION_SERVICE_H_

