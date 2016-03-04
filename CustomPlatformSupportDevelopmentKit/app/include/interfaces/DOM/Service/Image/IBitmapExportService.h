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
 * @file  IBitmapExportService.h
 *
 * @brief This file contains the interface for IBitmapExportService. The IBitmapExportService 
 *        is used to export the Bitmap library items to different file formats.
 */

#ifndef IBITMAP_EXPORT_SERVICE_H_
#define IBITMAP_EXPORT_SERVICE_H_

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
        namespace Image 
        {
            /**
             * @brief Defines the universally-unique interface ID for 
             *        IBitmapExportService.
             *
             * @note  Textual Representation: {B795A4B4-4A08-4DE0-9DFA-5B0E8198065F}
             */
            FCM::ConstFCMIID IID_IBITMAP_EXPORT_SERVICE = 
                {0xb795a4b4, 0x4a08, 0x4de0, {0x9d, 0xfa, 0x5b, 0xe, 0x81, 0x98, 0x6, 0x5f}};
        }
    }
}


/* -------------------------------------------------- Structs / Unions */


/* -------------------------------------------------- Class Decl */

namespace DOM 
{
    namespace Service
    {
        namespace Image 
        {
            /**
             * @class IBitmapExportService
             *
             * @brief Defines the interface for converting bitmap library item into different 
             *        file formats.
             */
            BEGIN_DECLARE_INTERFACE(IBitmapExportService, IID_IBITMAP_EXPORT_SERVICE)

                /**
                 * @brief  This function is used to export a bitmap to a file.
                 *
                 * @param  pBitmap (IN)
                 *         An IMediaItem of a bitmap in library.
                 *
                 * @param  pFilePath (IN)
                 *         A string that specifies the absolute path (including name) 
                 *         of the exported file. The parent directory should exist.
                 *         The file extension can be '.JPG' or '.PNG'.
                 * 
                 * @param  quality (IN)
                 *         A number, from 1-100, which determines the quality of the exported 
                 *         image file. A higher number indicates higher quality.
                 *
                 * @return On success, FCM_SUCCESS is returned, else an error code is returned.
                 */
                virtual FCM::Result _FCMCALL ExportToFile(
                    const LibraryItem::PIMediaItem pBitmap, 
                    FCM::CStringRep16 pFilePath, 
                    FCM::U_Int32 quality) = 0;

           END_DECLARE_INTERFACE 
        }
    }
}


/* -------------------------------------------------- Inline / Functions */


#include "FCMPostConfig.h"

#endif // IBITMAP_EXPORT_SERVICE_H_

