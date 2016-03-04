/******************************************************************************
* ADOBE CONFIDENTIAL
* ___________________
*
*  Copyright [2015] Adobe Systems Incorporated
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
 * @file  IApplicationService.h
 *
 * @brief This file contains interface for IApplicationService.
 *        IApplicationService provides "Adobe Animate CC" application level services.
 *        The definitions in this file are valid for CPSDK version 1.3 and above
 *        (or for 'Adobe Animate CC').
 */

#ifndef IAPPLICATION_SERVICE_H_
#define IAPPLICATION_SERVICE_H_

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
         * @brief Defines the universally-unique Interface ID for IApplicationService.
         *
         * @note  Textual Representation: {9688268F-961D-4811-A8C3-DF7E06494B89}
         */
        FCM::ConstFCMIID IID_IAPPLICATION_SERVICE =
            {0x9688268f, 0x961d, 0x4811, {0xa8, 0xc3, 0xdf, 0x7e, 0x6, 0x49, 0x4b, 0x89}};
    }
}


/* -------------------------------------------------- Structs / Unions */


/* -------------------------------------------------- Class Decl */

namespace Application 
{
    namespace Service
    {
        /**
         * @class IApplicationService
         *
         * @brief This service provides "Adobe Animate CC" application-level services.
         *
         * @note  This interface is supported for CPSDK version 1.3 and above 
         *        (or for 'Adobe Animate CC').
         */
        BEGIN_DECLARE_INTERFACE(IApplicationService, IID_IAPPLICATION_SERVICE)

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
             * @brief  This function returns the application version.
             *
             * @param  version (OUT)
             *         Application Version
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
             * @brief  This function returns the locale of the application user interface.
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

#endif // IAPPLICATION_SERVICE_H_

