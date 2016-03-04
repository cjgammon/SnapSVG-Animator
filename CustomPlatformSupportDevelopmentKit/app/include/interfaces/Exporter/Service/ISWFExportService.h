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
 * @file  ISWFExportService.h
 *
 * @brief This file contains interface for ISWFExportService. 
 *        ISWFExportService provides service to export an FLA file to SWF format.
 */

#ifndef ISWF_EXPORT_SERVICE_H_
#define ISWF_EXPORT_SERVICE_H_

#include "FCMPreConfig.h"
#include "FCMPluginInterface.h"
#include "Utils/DOMTypes.h"
#include "IFCMDictionary.h"


/* -------------------------------------------------- Forward Decl */

namespace DOM
{
    FORWARD_DECLARE_INTERFACE(IFLADocument);
    FORWARD_DECLARE_INTERFACE(ITimeline);
}


/* -------------------------------------------------- Enums */


/* -------------------------------------------------- Macros / Constants */

namespace Exporter
{
    namespace Service
    {
        /**
         * @brief Defines the universally-unique interface ID for 
         *        ISWFExportService.
         *
         * @note  Textual Representation:  {B4C11F4F-982A-4D24-A5C4-BF9CA0437BF5}
         */
        FCM::ConstFCMIID IID_ISWF_EXPORT_SERVICE =
            {0xb4c11f4f, 0x982a, 0x4d24, {0xa5, 0xc4, 0xbf, 0x9c, 0xa0, 0x43, 0x7b, 0xf5}};
    }
}


/* -------------------------------------------------- Structs / Unions */


/* -------------------------------------------------- Class Decl */

namespace Exporter
{
    namespace Service
    {
        /**
         * @class ISWFExportService
         *
         * @brief Defines an interface that exports an FLA document to SWF format.
         */
        BEGIN_DECLARE_INTERFACE(ISWFExportService, IID_ISWF_EXPORT_SERVICE)

            /**
             * @brief  This function exports a FLA document to SWF format.
             *
             * @param  pFlaDocument (IN)
             *         FLA Document
             *
             * @param  pFilePath (IN)
             *         A string that specifies the absolute path (including name) 
             *         of the exported "SWF" file. The parent directory should exist.
             * 
             * @return On success, FCM_SUCCESS is returned, on cancellation FCM_OPERATION_CANCELLED
             *         is returned. For any other failure, a non-zero error code is returned.
             *
             * @note   It uses the publish settings set through the publish setting dialog using
             *         "com.adobe.events.flash.extension.savestate" event.
             */
            virtual FCM::Result _FCMCALL ExportToFile(
                DOM::PIFLADocument pFlaDocument,  
                FCM::CStringRep16 pFilePath) = 0;


            /**
             * @brief  This function exports a timeline to a SWF format.
             *
             * @param  pTimeline (IN)
             *         Timeline to be exported
             *
             * @param  pFilePath (IN)
             *         A string that specifies the absolute path (including name) 
             *         of the exported "SWF" file. The parent directory should exist.
             * 
             * @return On success, FCM_SUCCESS is returned. On cancellation FCM_OPERATION_CANCELLED
             *         is returned. For any other failure, a non-zero error code is returned.
             *
             * @note   It uses the publish settings set through the publish setting dialog using
             *         "com.adobe.events.flash.extension.savestate" event.
             */
            virtual FCM::Result _FCMCALL ExportToFile(
                DOM::PITimeline pTimeline,  
                FCM::CStringRep16 pFilePath) = 0;

        END_DECLARE_INTERFACE 
    }
}


/* -------------------------------------------------- Inline / Functions */


#include "FCMPostConfig.h"

#endif // ISWF_EXPORT_SERVICE_H_

