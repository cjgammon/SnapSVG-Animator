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
 * @file  ILayer.h
 *
 * @brief This file contains the interface for ILayer. The ILayer 
 *        interface represents a layer in the timeline. 
 */

#ifndef ILAYER_H_
#define ILAYER_H_

#include "FCMPreConfig.h"
#include "FCMPluginInterface.h"
#include "IFCMUnknown.h"

/* -------------------------------------------------- Forward Decl */

namespace DOM
{
    FORWARD_DECLARE_INTERFACE(IFrame);
}


/* -------------------------------------------------- Enums */


/* -------------------------------------------------- Macros / Constants */

namespace DOM 
{
    /**
     * @brief Defines the universally-unique interface ID for 
     *        ILayer.
     *
     * @note  Textual Representation: {926CB1E2-CE8A-40EC-84DC-805E117084BC}
     */
    FCM::ConstFCMIID IID_ILAYER =
       {0x926cb1e2, 0xce8a, 0x40ec, {0x84, 0xdc, 0x80, 0x5e, 0x11, 0x70, 0x84, 0xbc}};
}


namespace DOM
{
    namespace Layer
    {
        /**
         * @brief Defines the universally-unique interface ID for 
         *        ILayerFolder.
         *
         * @note  Textual Representation: {404D03B5-B3A8-4AA6-B496-4669384F3691}
         */
        FCM::ConstFCMIID IID_ILAYER_FOLDER =
            {0x404d03b5, 0xb3a8, 0x4aa6, {0xb4, 0x96, 0x46, 0x69, 0x38, 0x4f, 0x36, 0x91}};
    }
}


namespace DOM
{
    namespace Layer
    {
        /**
         * @brief Defines the universally-unique interface ID for 
         *        ILayerNormal.
         *
         * @note  Textual Representation: {4254C006-B3A8-4AA6-B496-4669384F3691}
         */
        FCM::ConstFCMIID IID_ILAYER_NORMAL =
            {0x4254c006, 0x17ce, 0x497c, {0xa6, 0x2c, 0x4, 0xf5, 0xd6, 0x9c, 0x87, 0x5}};
    }
}


namespace DOM
{
    namespace Layer
    {
        /**
         * @brief Defines the universally-unique interface ID for  
         *        ILayerMask.
         *
         * @note  Textual Representation: {55886E53-CBA5-4708-AFA7-60C1398F8E2}
         */
        FCM::ConstFCMIID IID_ILAYER_MASK =
            {0x55886e53, 0xcba5, 0x4708, {0xaf, 0xa7, 0x60, 0xc1, 0x39, 0x8, 0xf8, 0xe2}};
    }
}


namespace DOM
{
    namespace Layer
    {  
        /**
         * @brief Defines the universally-unique interface ID for 
         *        ILayerGuide.
         *
         * @note  Textual Representation: {D34CE64-23A2-4FF0-BFCA-368EDE7D414C}
         */
        FCM::ConstFCMIID IID_ILAYER_GUIDE =
            {0xd34ce64, 0x23a2, 0x4ff0, {0xbf, 0xca, 0x36, 0x8e, 0xde, 0x7d, 0x41, 0x4c}};
    }
}


/* -------------------------------------------------- Structs / Unions */


/* -------------------------------------------------- Class Decl */

namespace DOM 
{

    /**
    * @class ILayer
    *
    * @brief Defines the interface that represents a layer in the timeline.
    */
    BEGIN_DECLARE_INTERFACE(ILayer, IID_ILAYER)

        /**
         * @brief  Returns the name of the layer.
         * 
         * @param  ppName (OUT)
         *         Name of the layer
         *
         * @return On success, FCM_SUCCESS is returned, else an error code is returned.
         *
         * @note   The memory allocated for 'ppName' in this function must be freed 
         *         by the caller using IFCMCalloc::Free().
         */
        virtual FCM::Result _FCMCALL GetName(FCM::StringRep16* ppName) = 0;


        /**
         * @brief  Tests if the layer is visible or not.
         * 
         * @param  isVisible (OUT)
         *         True if all objects in the layer are visible;false if
         *         any is hidden.
         *
         * @return On success, FCM_SUCCESS is returned, else an error code is returned.
         */     
        virtual FCM::Result _FCMCALL IsVisible(FCM::Boolean& isVisible) = 0;


        /**
         * @brief  Get Parent layer for the current layer.
         * 
         * @param  pParentLayer (OUT)
         *         Parent Layer for the current layer Otherwise it is set to NULL
         *
         * @return On success, FCM_SUCCESS is returned, else an error code is returned.
         */
        virtual FCM::Result _FCMCALL GetParentLayer(PILayer& pParentLayer) = 0;
        

        /**
         * @brief  Gets the type associated with this layer. The type determines whether
         *         it is a folder, mask, guide, or normal.
         *
         * @param  pLayerType (OUT)
         *         The type of the layer. This can be queried for one of the following
         *         interfaces: ILayerFolder, ILayerNormal, ILayerMask and ILayerGuide.
         *
         * @return On success, FCM_SUCCESS is returned, else an error code is returned.
         *
         * @see    DOM::Layer::ILayerFolder
         *
         * @see    DOM::Layer::ILayerNormal
         *
         * @see    DOM::Layer::ILayerMask
         *
         * @see    DOM::Layer::ILayerGuide
         */
        virtual FCM::Result _FCMCALL GetLayerType(FCM::PIFCMUnknown& pLayerType) = 0;

    END_DECLARE_INTERFACE 
}


namespace DOM
{
    namespace Layer
    {
        /**
         * @class ILayerFolder
         *
         * @brief Defines the interface that represents a folder layer in the timeline.
         */
        BEGIN_DECLARE_INTERFACE(ILayerFolder, IID_ILAYER_FOLDER)
        
            /**
             * @brief  Returns the list of child layers.
             * 
             * @param  pLayerList (OUT)
             *         List of ILayer objects 
             *
             * @return On success, FCM_SUCCESS is returned, else an error code is returned.
             *
             * @see    DOM::ILayer
             */    
            virtual FCM::Result _FCMCALL GetChildren(PIFCMList& pLayerList) = 0;
        
        END_DECLARE_INTERFACE 
    }
}

namespace DOM
{
    namespace Layer
    {
        /**
         * @class ILayerNormal
         *
         * @brief Defines the interface that represents a normal layer in the timeline.
         */
        BEGIN_DECLARE_INTERFACE(ILayerNormal, IID_ILAYER_NORMAL)
        
            /**
             * @brief  Returns the list of frames.
             * 
             * @param  pKeyFrameList (OUT)
             *         List of frames (IFrame)
             *
             * @return On success, FCM_SUCCESS is returned, else an error code is returned.
             *
             * @see    DOM::IFrame
             */    
            virtual FCM::Result _FCMCALL GetKeyFrames(PIFCMList& pKeyFrameList) = 0;


            /**
             * @brief  Return frame at index, index can be any value between 0 to 
             *         duration returned by GetTotalDuration.
             * 
             * @param  index (IN)
             *         Index at which frame is needed    
             *
             * @param  pFrame (OUT)  
             *         Frame at specified index.
             *
             * @return On success, FCM_SUCCESS is returned, else an error code is returned.
             */    
            virtual FCM::Result _FCMCALL GetFrameAtIndex(FCM::U_Int32 index, PIFrame& pFrame) = 0;


            /**
             * @brief  Returns the duration of all the key Frames.
             * 
             * @param  duration (OUT)
             *         Layer duration
             *
             * @return On success, FCM_SUCCESS is returned, else an error code is returned.
             */
            virtual FCM::Result _FCMCALL GetTotalDuration(FCM::U_Int32& duration) = 0;
        
        END_DECLARE_INTERFACE 
    }
}


namespace DOM
{
    namespace Layer
    {
        /**
         * @class ILayerMask
         *
         * @brief Defines the interface that represents a mask layer in the timeline.
         */
        BEGIN_DECLARE_INTERFACE(ILayerMask, IID_ILAYER_MASK)

            /**
             * @brief  Returns the list of child layers.
             * 
             * @param  pLayerList (OUT)
             *         List of layer objects (ILayer)
             *
             * @return On success, FCM_SUCCESS is returned, else an error code is returned.
             *
             * @see    DOM::ILayer
             */    
            virtual FCM::Result _FCMCALL GetChildren(PIFCMList& pLayerList) = 0;
            
        END_DECLARE_INTERFACE 
    }
}


namespace DOM
{
    namespace Layer
    {
        /**
         * @class ILayerGuide
         *
         * @brief Defines the interface that represents a guide layer in the timeline.
         */
        BEGIN_DECLARE_INTERFACE(ILayerGuide, IID_ILAYER_GUIDE)

            /**
             * @brief  Returns the list of child layers.
             * 
             * @param  pLayerList (OUT)
             *         List of layer objects (ILayer)
             *
             * @return On success, FCM_SUCCESS is returned, else an error code is returned.
             *
             * @see    DOM::ILayer
             */    
            virtual FCM::Result _FCMCALL GetChildren(PIFCMList& pLayerList) = 0;
            
        END_DECLARE_INTERFACE
    }
}


/* -------------------------------------------------- Inline / Functions */


#include "FCMPostConfig.h"

#endif // ILAYER_H_

