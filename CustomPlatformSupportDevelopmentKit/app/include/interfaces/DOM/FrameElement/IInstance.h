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
 * @file  IInstance.h
 *
 * @brief This file contains the interface for IInstance. IInstance interface 
 *        represents the elements on the stage that are instances of a library
 *        item.
 */

#ifndef IINSTANCE_H_
#define IINSTANCE_H_

#include "FCMPreConfig.h"
#include "FCMPluginInterface.h"
#include "IFCMUnknown.h"
#include "FrameElement/IFrameDisplayElement.h"


/* -------------------------------------------------- Forward Decl */

namespace DOM
{
    FORWARD_DECLARE_INTERFACE(ILibraryItem);
}


/* -------------------------------------------------- Enums */


/* -------------------------------------------------- Macros / Constants */

namespace DOM 
{
    namespace FrameElement
    {
        /**
         * @brief Defines the universally-unique interface ID for 
         *        IInstance.
         *
         * @note  Textual Representation: {FAAAC715-FA2B-43E2-8B33-62A1C4059D1E}
         */
        FCM::ConstFCMIID IID_IINSTANCE =
            {0xfaaac715, 0xfa2b, 0x43e2, {0x8b, 0x33, 0x62, 0xa1, 0xc4, 0x5, 0x9d, 0x1e}};
    }
}


/* -------------------------------------------------- Structs / Unions */


/* -------------------------------------------------- Class Decl */

namespace DOM 
{
    namespace FrameElement
    {
        /**
         * @class IInstance
         *
         * @brief Defines an interface that represents an instance of a library
         *        item that is present on the stage.
         */
        BEGIN_DECLARE_INTERFACE(IInstance, IID_IINSTANCE)

            /**
             * @brief  This functions gets the library item that is used to 
             *         instantiate this instance.
             *
             * @param  pLibItem (OUT)
             *         Library Item. Each library item can be further queried for following
             *         interfaces: IFolderItem, IFontItem, IMediaItem and ISymbolItem.
             *
             * @return On success, FCM_SUCCESS is returned, else an error code is returned.
             *
             * @see    DOM::LibraryItem::IFolderItem
             *
             * @see    DOM::LibraryItem::IFontItem
             *
             * @see    DOM::LibraryItem::IMediaItem
             *
             * @see    DOM::LibraryItem::ISymbolItem
             */
            virtual FCM::Result _FCMCALL GetLibraryItem(PILibraryItem& pLibItem) = 0;

        END_DECLARE_INTERFACE
    }
}


/* -------------------------------------------------- Inline / Functions */


#include "FCMPostConfig.h"

#endif // IINSTANCE_H_

