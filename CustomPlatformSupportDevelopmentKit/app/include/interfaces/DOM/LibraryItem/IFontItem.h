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
 * @file  IFontItem.h
 *
 * @brief This file contains interface for IFontItem. 
 *        This represents a font symbol in Library.
 */

#ifndef IFONT_ITEM_H_
#define IFONT_ITEM_H_

#include "FCMPreConfig.h"
#include "FCMPluginInterface.h"
#include "Utils/DOMTypes.h"


/* -------------------------------------------------- Forward Decl */


/* -------------------------------------------------- Enums */


/* -------------------------------------------------- Macros / Constants */

namespace DOM
{
    namespace LibraryItem
    {
        /**
         * @brief Defines the universally-unique interface ID for 
         *        IFontItem
         *
         * @note  Textual Representation: {731BCB32-50DB-4A6C-9750-9E6CEF8DF1AF}
         */
        FCM::ConstFCMIID IID_IFONT_ITEM =
            {0x731bcb32, 0x50db, 0x4a6c, {0x97, 0x50, 0x9e, 0x6c, 0xef, 0x8d, 0xf1, 0xaf}};
    }
}


/* -------------------------------------------------- Structs / Unions */


/* -------------------------------------------------- Class Decl */

namespace DOM
{
    namespace LibraryItem
    {
        /**
        * @class IFontItem
        *
        * @brief Defines the interface that represents a font symbol in Library.
        */
        BEGIN_DECLARE_INTERFACE(IFontItem, IID_IFONT_ITEM)
        
            /**
             * @brief  This function determines name of the device font that should be associated with this Font Item.
             *
             * @param  ppFontName (OUT)
             *         The name of the device font that should be associated with this font item is returned.
             *
             * @return On success, FCM_SUCCESS is returned, else an error code is returned.
             *
             * @note   The memory allocated for 'ppFontName' in this function must be freed 
             *         by the caller using IFCMCalloc::Free().
             */
            virtual FCM::Result _FCMCALL GetFontName(FCM::StringRep16* ppFontName) = 0;
        
            
            /**
             * @brief  This function returns the font style.
             *
             * @param  ppFontStyle (OUT)
             *         The font style is returned.
             *
             * @return On success, FCM_SUCCESS is returned, else an error code is returned.
             *
             * @note   The memory allocated for 'ppFontStyle' in this function must be freed 
             *         by the caller using IFCMCalloc::Free().
             */
            virtual FCM::Result _FCMCALL GetFontStyle(FCM::StringRep8* ppFontStyle) = 0;
            

            /**
             * @brief  This function determines the point size of the font.
             *
             * @param  size (OUT)
             *         The point size of the font is returned.
             *
             * @return On success, FCM_SUCCESS is returned, else an error code is returned.
             */
            virtual FCM::Result _FCMCALL GetFontSize(FCM::U_Int16& size) = 0;
            

            /**
             * @brief  This function determines characters embedded in the font symbol.
             *
             * @param  ppEmbeddedCharacters (OUT)
             *         The characters embedded in the font symbol are returned.
             *
             * @return On success, FCM_SUCCESS is returned, else an error code is returned. 
             *
             * @note   The memory allocated for 'ppEmbeddedCharacters' in this function must 
             *         be freed by the caller using IFCMCalloc::Free().
             */
            virtual FCM::Result _FCMCALL GetEmbeddedCharacters(
                FCM::StringRep16* ppEmbeddedCharacters) = 0;
            

            /**
             * @brief  This function returns a series of delimited integers which correspond to 
             *         the items that are selected in the font embedding dialog. 
             *
             * @param  ppEmbeddedRanges (OUT)
             *         The delimited integers which correspond to the embedded ranges that are selected 
             *         in the font embedding dialog are returned.
             *
             * @return On success, FCM_SUCCESS is returned, else an error code is returned.
             *
             * @note   The memory allocated for 'ppEmbeddedRanges' in this function must 
             *         be freed by the caller using IFCMCalloc::Free().
             */            
            virtual FCM::Result _FCMCALL GetEmbeddedRanges(FCM::StringRep16* ppEmbeddedRanges) = 0;
            
        END_DECLARE_INTERFACE
    }
};


/* -------------------------------------------------- Inline / Functions */


#include "FCMPostConfig.h"

#endif // IFONT_ITEM_H_

