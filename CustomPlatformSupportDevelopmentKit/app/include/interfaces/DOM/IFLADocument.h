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
 * @file  IFLADocument.h
 *
 * @brief This file contains the interface for IFLADocument. The IFLADocument 
 *        interface represents a FLA document.
 */

#ifndef IFLA_DOCUMENT_H_
#define IFLA_DOCUMENT_H_

#include "FCMPreConfig.h"
#include "FCMPluginInterface.h"
#include "Utils/DOMTypes.h"

/* -------------------------------------------------- Forward Decl */

namespace FCM
{
   
    FORWARD_DECLARE_INTERFACE(IFCMList);
}


namespace DOM
{        
    FORWARD_DECLARE_INTERFACE(ITimeLine);
    FORWARD_DECLARE_INTERFACE(ILibraryItem);
}


/* -------------------------------------------------- Enums */


/* -------------------------------------------------- Macros / Constants */

namespace DOM
{
    /**
     * @brief Defines the universally-unique interface ID for 
     *        IFLADocument.
     *
     * @note  Textual Representation: {9F54B497-9471-4C13-985B-7CB1D4576030}
     */
    FCM::ConstFCMIID IID_IFLA_DOCUMENT =
        {0x9f54b497, 0x9471, 0x4c13, {0x98, 0x5b, 0x7c, 0xb1, 0xd4, 0x57, 0x60, 0x30}};
}


/* -------------------------------------------------- Structs / Unions */


/* -------------------------------------------------- Class Decl */

namespace DOM 
{

    /**
     * @class IFLADocument
     *
     * @brief Defines the interface that represents an FLA document.
     */
    BEGIN_DECLARE_INTERFACE(IFLADocument, IID_IFLA_DOCUMENT)

        /**
         * @brief  This function provides the background color of the stage.
         *
         * @param  color (OUT)
         *         Background color of the stage.
         *
         * @return On success, FCM_SUCCESS is returned, else an error code is returned.
         */
        virtual FCM::Result _FCMCALL GetBackgroundColor(Utils::COLOR& color) = 0;


        /**
         * @brief  This function provides the frame-rate when the published file 
         *         is played back.
         *
         * @param  frameRate (OUT)
         *         Frame rate - Number of frames displayed per second.
         *
         * @return On success, FCM_SUCCESS is returned, else an error code is returned.
         */    
        virtual FCM::Result _FCMCALL GetFrameRate(FCM::Double& frameRate) = 0;


        /**
         * @brief  This function returns the stage height (in pixels).
         *
         * @param height (OUT)
         *        Height of the stage.
         *
         * @return On success, FCM_SUCCESS is returned, else an error code is returned.
         */    
        virtual FCM::Result _FCMCALL GetStageHeight(FCM::U_Int32& height) = 0;


        /**
         * @brief  This function returns the stage width (in pixels).
         *
         * @param  width (OUT)
         *         Width of the stage.
         *
         * @return On success, FCM_SUCCESS is returned, else an error code is returned.
         */        
        virtual FCM::Result _FCMCALL GetStageWidth(FCM::U_Int32& width) = 0;


        /**
         * @brief  This returns the items which are part of the Library.
         *
         * @param  pLibItemsList (OUT)
         *         List of library items. Each item in the list can queried for 
         *         following depending on its type: 
         *          Folder - ILibraryItem, IFolder
         *          Font - ILibraryItem, IFont
         *          Sound/Bitmap - ILibraryItem, IMediaItem
         *          MovieClip/Graphic - ILibraryItem, ISymbolItem
         *
         * @return On success, FCM_SUCCESS is returned, else an error code is returned.
         *
         * @see    DOM::ILibraryItem
         *
         * @see    DOM::LibraryItem::IFolderItem
         *
         * @see    DOM::LibraryItem::IFontItem
         *
         * @see    DOM::LibraryItem::IMediaItem
         *
         * @see    DOM::LibraryItem::ISymbolItem
         */    
        virtual FCM::Result _FCMCALL GetLibraryItems(FCM::PIFCMList& pLibItemsList) = 0;
    

        /**
         * @brief  This function finds the library item from its path.
         *
         * @param  pPath (IN)
         *         The full path of the item in the library.
         *
         * @param  pLibItem (OUT)
         *         Library item. The library item can queried for 
         *         following depending on its type: 
         *          Folder - ILibraryItem, IFolder
         *          Font - ILibraryItem, IFont
         *          Sound/Bitmap - ILibraryItem, IMediaItem
         *          MovieClip/Graphic - ILibraryItem, ISymbolItem
         *
         * @return On success, FCM_SUCCESS is returned, else an error code is returned.
         */    
        virtual FCM::Result _FCMCALL GetLibraryItemByPath(
            FCM::CStringRep16 pPath, 
            PILibraryItem& pLibItem) = 0;


        /**
         * @brief  This function returns the list of timelines (scenes).
         *
         * @param  pTimelineList (OUT)
         *         List of timelines or scenes (ITimeline).
         *
         * @return On success, FCM_SUCCESS is returned, else an error code is returned.
         *
         * @see    DOM::ITimeline
         */    
        virtual FCM::Result _FCMCALL GetTimelines(FCM::PIFCMList& pTimelineList) = 0;
        
        
        /**
         * @brief  This function returns the document type identification of 
         *         the FLA document.
         *
         * @param  docId (OUT)
         *         Id to identify the document type.
         *
         * @return On success, FCM_SUCCESS is returned, else an error code is returned.
         */
        virtual FCM::Result _FCMCALL GetTypeId(FCM::FCMGUID& docId) = 0;


        /**
         * @brief  This function returns the path of the FLA document.
         *         If the document is never been saved, NULL will be returned.
         *
         * @param  ppPath (OUT)
         *         Path of the document.
         *
         * @return On success, FCM_SUCCESS is returned, else an error code is returned.
         *
         * @note   Memory allocated for ppPath must be released by the caller using 
         *         FCM::IFCMCalloc.
         */
        virtual FCM::Result _FCMCALL GetPath(FCM::StringRep16* ppPath) = 0;

    END_DECLARE_INTERFACE 
}; 


/* -------------------------------------------------- Inline / Functions */


#include "FCMPostConfig.h"

#endif // IFLA_DOCUMENT_H_

