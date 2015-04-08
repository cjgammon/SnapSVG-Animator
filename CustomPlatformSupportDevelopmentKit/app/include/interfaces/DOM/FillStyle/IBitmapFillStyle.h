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
 * @file  IBitmapFillStyle.h
 *
 * @brief This file contains the interface for IBitmapFillStyle. The IBitmapFillStyle 
 *        represents a "bitmap" fill style.
 */

#ifndef IBITMAP_FILL_STYLE_H_
#define IBITMAP_FILL_STYLE_H_

#include "FCMPreConfig.h"
#include "FCMPluginInterface.h"
#include "Utils/DOMTypes.h"

    
/* -------------------------------------------------- Forward Decl */
    
namespace DOM
{    
    namespace LibraryItem
    {
        FORWARD_DECLARE_INTERFACE(IMediaItem);
    }
}


/* -------------------------------------------------- Enums */


/* -------------------------------------------------- Macros / Constants */

namespace DOM
{
    namespace FillStyle
    {

        /**
         * @brief Defines the Interface ID (which is universally unique) for 
         *        IBitmapFillStyle
         *
         * @note  Textual Representation: {49C1053B-256E-4031-B35E-6B410B21CB09}
         */
        FCM::ConstFCMIID ID_IBITMAP_FILL_STYLE =
            {0x49c1053b, 0x256e, 0x4031, {0xb3, 0x5e, 0x6b, 0x41, 0xb, 0x21, 0xcb, 0x9}};
    }
}


/* -------------------------------------------------- Structs / Unions */


/* -------------------------------------------------- Class Decl */

namespace DOM
{
    namespace FillStyle
    {
        /**
         * @class IBitmapFillStyle
         *
         * @brief Defines an interface that represents a "bitmap" fill style.
         */
        BEGIN_DECLARE_INTERFACE(IBitmapFillStyle, ID_IBITMAP_FILL_STYLE)

            /**
             * @brief  This function gets the library item corresponding to the bitmap 
             *         used in the fill.
             *
             * @param  pBitmap (OUT)
             *         Bitmap library item - This can successfully queried for ILibraryItem
             *         and IMediaItem.
             *
             * @return On success, FCM_SUCCESS is returned; else an error code is returned.
             *
             * @see    LibraryItem::IMediaItem
             *
             * @see    LibraryItem::ILibraryItem
             */
            virtual FCM::Result _FCMCALL GetBitmap(LibraryItem::PIMediaItem& pBitmap) = 0;


            /**
             * @brief  This function can be used to check if the bitmap is larger than
             *         the fill area. If the bitmap is larger than the fill area,  
             *         the bitmap is clipped; else it is repeated.
             *
             * @param  clipped (OUT)
             *         Set to true if the bitmap is larger than the fill area and the 
             *         bitmap is clipped, else it is set to false.
             *
             * @return On success, FCM_SUCCESS is returned, else an error code is returned.
             */
            virtual FCM::Result _FCMCALL IsClipped(FCM::Boolean& clipped) = 0;
        

            /**
             * @brief  This function gets the transformation matrix that defines the 
             *         placement, orientation, and the scale for the bitmap fill.
             *
             * @param  matrix (OUT)
             *         Transformation matrix
             *
             * @return On success, FCM_SUCCESS is returned; else an error code is returned.
             */
            virtual FCM::Result _FCMCALL GetMatrix(Utils::MATRIX2D& matrix) = 0;

        END_DECLARE_INTERFACE
    } 
} 


/* -------------------------------------------------- Inline / Functions */


#include "FCMPostConfig.h"

#endif // IBITMAP_FILL_STYLE_H_

