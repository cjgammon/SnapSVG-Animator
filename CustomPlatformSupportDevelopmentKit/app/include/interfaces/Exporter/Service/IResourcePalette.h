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
 * @file  IResourcePalette.h
 *
 * @brief This file contains interface for IResourcePalette. 
 *        IResourcePalette provides methods to add different resources 
 *        to a resource palette.
 */

#ifndef IRESOURCE_PALETTE_H_
#define IRESOURCE_PALETTE_H_

#include "FCMPreConfig.h"
#include "FCMPluginInterface.h"
#include "Utils/DOMTypes.h"


/* -------------------------------------------------- Forward Decl */

namespace DOM
{    
    namespace FrameElement
    {
        FORWARD_DECLARE_INTERFACE(IShape);
        FORWARD_DECLARE_INTERFACE(IClassicText);
    }

    namespace LibraryItem
    {
        FORWARD_DECLARE_INTERFACE(IMediaItem);
    }
}


namespace Exporter
{
    namespace Service
    {
        FORWARD_DECLARE_INTERFACE(ITimelineBuilder);
    }
}

/* -------------------------------------------------- Enums */


/* -------------------------------------------------- Macros / Constants */


namespace Exporter
{
    namespace Service
    {
        /**
         * @brief Defines the universally-unique interface ID for 
         *        IResourcePalette.
         *
         * @note  Textual Representation:  {BC49188C-B9F7-4BCA-AA58-ED3CD9BCFCC7}
         */
        FCM::ConstFCMIID IID_IRESOURCE_PALETTE =
            {0xbc49188c, 0xb9f7, 0x4bca, {0xaa, 0x58, 0xed, 0x3c, 0xd9, 0xbc, 0xfc, 0xc7}};
    }
}


/* -------------------------------------------------- Structs / Unions */


/* -------------------------------------------------- Class Decl */

namespace Exporter
{
    namespace Service
    {
        /**
         * @class IResourcePalette
         *
         * @brief An interface that defines a resource palette to hold all the resources 
         *        encountered in the DOM traversal.
         */
        BEGIN_DECLARE_INTERFACE(IResourcePalette, IID_IRESOURCE_PALETTE)

            /**  
             * @brief  This function is invoked to add a symbol to the resource palette.
             *
             * @param  resourceId (IN)
             *         Resource Id associated with the symbol which is getting 
             *         added to the palette.
             *
             * @param  pName (IN)
             *         Name of the symbol which is getting added to the palette. For graphic
             *         symbols, pName can be NULL.
             *
             * @param  pTimelineBuilder (IN)
             *         TimelineBuilder object for the symbol. Contains all the timeline commands.
             *
             * @return On success, FCM_SUCCESS is returned, else an error code is returned.
             */
            virtual FCM::Result _FCMCALL AddSymbol(
                FCM::U_Int32 resourceId, 
                FCM::StringRep16 pName, 
                Exporter::Service::PITimelineBuilder pTimelineBuilder) = 0;


            /**  
             * @brief  This function is invoked to add a shape to the resource palette.
             *
             * @param  resourceId (IN)
             *         Resource Id associated with the shape, which is getting 
             *         added to the palette.
             *
             * @param  pShape (IN) 
             *         Shape object associated with resourceId via which shape information
             *         is extracted.
             *
             * @return On success, FCM_SUCCESS is returned, else an error code is returned.
             *
             * @note   Shape services (FLA_REGION_GENERATOR_SERVICE) can be used to extract 
             *         the geometry information (such as the boundary and holes) along with 
             *         fill and stroke style information.
             */
            virtual FCM::Result _FCMCALL AddShape(
                FCM::U_Int32 resourceId, 
                DOM::FrameElement::PIShape pShape) = 0;


            /**  
             * @brief  This function is invoked to add a sound object to the resource palette.
             *
             * @param  resourceId (IN)
             *         Resource Id associated with the sound that is getting 
             *         added to the palette.
             *
             * @param  pMediaItem (IN) 
             *         Library object associated with sound through which sound information
             *         is extracted. This parameter can also be successfully queried for 
             *         DOM::ILibraryItem.
             *
             * @return On success, FCM_SUCCESS is returned, otherwise an error code is returned.
             *
             * @note   Sound service (FLA_SOUND_SERVICE) can be used to export the media item to a 
             *         known sound format.
             *
             * @see    ILibraryItem
             */
            virtual FCM::Result _FCMCALL AddSound(
                FCM::U_Int32 resourceId, 
                DOM::LibraryItem::PIMediaItem pMediaItem) = 0;


            /**  
             * @brief  This function is invoked to add a bitmap object to the resource palette.
             *
             * @param  resourceId (IN)
             *         Resource Id associated with the bitmap which is getting 
             *         added to the palette.
             *
             * @param  pMediaItem (IN) 
             *         Library object associated with bitmap through which bitmap information
             *         is extracted. This parameter can also be successfully queried for 
             *         DOM::ILibraryItem.
             *
             * @return On success, FCM_SUCCESS is returned, else an error code is returned.
             *
             * @note   Bitmap service (FLA_BITMAP_SERVICE) can be used to export the media item 
             *         to a known bitmap format.
             */
            virtual FCM::Result _FCMCALL AddBitmap(
                FCM::U_Int32 resourceId, 
                DOM::LibraryItem::PIMediaItem pMediaItem) = 0;


            /**  
             * @brief  This function is invoked to add a text object to the resource palette.
             *
             * @param  resourceId (IN)
             *         Resource Id associated with the text which is getting 
             *         added to the palette.
             *
             * @param  pClassicText (IN) 
             *         Text object which contains the text related information.
             *
             * @return On success, FCM_SUCCESS is returned, otherwise an error code is returned.
             */
            virtual FCM::Result _FCMCALL AddClassicText(
                FCM::U_Int32 resourceId, 
                DOM::FrameElement::PIClassicText pClassicText) = 0;


            /**  
             * @brief  This function is invoked every time a resource is encountered during the 
             *         DOM traversal  to check if a resource is already added in the
             *         resource palette.
             *
             * @param  resourceId (IN)
             *         Resource Id .
             *
             * @param  hasResource (OUT) 
             *         This param must be updated to true if the resource is already added to the
             *         the resource palette.
             *
             * @return On success, FCM_SUCCESS is returned, else an error code is returned.
             */
            virtual FCM::Result _FCMCALL HasResource(
                FCM::U_Int32 resourceId, 
                FCM::Boolean& hasResource) = 0;

        END_DECLARE_INTERFACE 
    }
}


/* -------------------------------------------------- Inline / Functions */


#include "FCMPostConfig.h"

#endif // IRESOURCE_PALETTE_H_

