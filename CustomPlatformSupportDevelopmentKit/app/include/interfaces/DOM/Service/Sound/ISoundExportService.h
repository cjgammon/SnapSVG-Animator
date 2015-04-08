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
 * @file  ISoundExportService.h
 *
 * @brief This file contains the interface for ISoundExportService. The ISoundExportService 
 *        is used to export the Sound library items to different audio formats.
 */

#ifndef ISOUND_EXPORT_SERVICE_H_
#define ISOUND_EXPORT_SERVICE_H_

#include "FCMPreConfig.h"

#include "FCMPluginInterface.h"
#include "Utils/DOMTypes.h"
#include "LibraryItem/IMediaItem.h"


/* -------------------------------------------------- Forward Decl */


/* -------------------------------------------------- Enums */


/* -------------------------------------------------- Macros / Constants */

namespace DOM 
{
    namespace Service
    {
        namespace Sound
        {
            /**
             * @brief Defines the Interface ID (which is universally unique) for 
             *        ISoundExportService
             *
             * @note  Textual Representation: {DEAD57CE-C4DC-44D2-B140-E82374272CF5}
             */
            FCM::ConstFCMIID IID_ISOUND_EXPORT_SERVICE = 
                {0xdead57ce, 0xc4dc, 0x44d2, {0xb1, 0x40, 0xe8, 0x23, 0x74, 0x27, 0x2c, 0xf5}};
        }
    }
}


/* -------------------------------------------------- Structs / Unions */


/* -------------------------------------------------- Class Decl */

namespace DOM 
{
    namespace Service
    {
        namespace Sound
        {
            /**
             * @class ISoundExportService
             *
             * @brief Defines the interface for converting sound library item into different 
             *        formats.
             */
            BEGIN_DECLARE_INTERFACE(ISoundExportService, IID_ISOUND_EXPORT_SERVICE)

               /**
                * @brief  This function exports a sound to a ".wav" or ".mp3" format.
                *
                * @param  pSound (IN)
                *         An IMediaItem of a sound in libary.
                *
                * @param  pFilePath (IN)
                *         A string, that specifies the absolute path (including name) 
                *         of the exported file. The parent directory should exist.
                *         The file extension must be ".WAV" or ".MP3".
                * 
                * @return On success, FCM_SUCCESS is returned, else an error code is returned.
                */
                virtual FCM::Result _FCMCALL ExportToFile(
                    const LibraryItem::PIMediaItem pSound, 
                    FCM::CStringRep16 pFilePath) = 0;

            END_DECLARE_INTERFACE 
        }
    }
}
 

/* -------------------------------------------------- Inline / Functions */


#include "FCMPostConfig.h"

#endif // ISOUND_EXPORT_SERVICE_H_

