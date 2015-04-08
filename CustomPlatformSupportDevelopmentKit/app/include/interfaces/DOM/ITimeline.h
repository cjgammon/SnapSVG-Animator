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
 * @file  ITimeline.h
 *
 * @brief This file contains the interface for ITimeline. The ITimeline 
 *        interface represents the timeline. This represents the timelines for
 *        both main timeline and the movie-clip.
 */

#ifndef ITIMELINE_H_
#define ITIMELINE_H_

#include "FCMPreConfig.h"
#include "FCMPluginInterface.h"


/* -------------------------------------------------- Forward Decl */

namespace FCM
{
    FORWARD_DECLARE_INTERFACE(IFCMList);
}


/* -------------------------------------------------- Macros / Constants */

namespace DOM
{
    /**
     * @brief Defines the universally-unique interface ID for 
     *        ITimeline.
     *
     * @note  Textual Representation: {9AB0D86F-33AC-4185-9787-983B245367E1}
     */
    FCM::ConstFCMIID IID_ITIMELINE =
        {0x9ab0d86f, 0x33ac, 0x4185, {0x97, 0x87, 0x98, 0x3b, 0x24, 0x53, 0x67, 0xe1}};
}


/* -------------------------------------------------- Enums */


/* -------------------------------------------------- Structs / Unions */


/* -------------------------------------------------- Class Decl */

namespace DOM 
{
    /**
     * @class ITimeline
     *
     * @brief Defines the interface that represents the timeline.
     */
    BEGIN_DECLARE_INTERFACE(ITimeline, IID_ITIMELINE)

        /**
         * @brief  This function returns the name of the current timeline. For main
         *         timeline, it returns the name of the scene and for movie-clips 
         *         the name of the movie-clip is returned.
         *
         * @param  ppName (OUT)
         *         Name of the current timeline.
         *
         * @return On success, FCM_SUCCESS is returned, else an error 
         *         code is returned and "ppName" is set to NULL.
         *
         * @note   The memory allocated for 'ppName' in this function must be freed 
         *         by the caller using IFCMCalloc::Free().
         */        
        virtual FCM::Result _FCMCALL GetName(FCM::StringRep16* ppName) = 0;


        /**
         * @brief  This function returns a list of layers (ILayer) for the 
         *         timeline. 
         *
         * @param  pLayerList (OUT)
         *         List of layers. The objects in the layer lists can be queried
         *         for ILayer along with one of the following interfaces: 
         *         ILayerFolder, ILayerNormal, ILayerMask and ILayerGuide.
         *
         * @return On success, FCM_SUCCESS is returned, else an error code is returned.
         *
         * @see    DOM::ILayer
         *
         * @see    DOM::Layer::ILayerFolder
         *
         * @see    DOM::Layer::ILayerNormal
         *
         * @see    DOM::Layer::ILayerMask
         *
         * @see    DOM::Layer::ILayerGuide
         */
        virtual FCM::Result _FCMCALL GetLayers(FCM::PIFCMList& pLayerList) = 0;


        /**
         * @brief  This function returns number of frames in the timeline’s 
         *         longest layer.
         *
         * @param  frameCount (OUT)
         *         Number of frames in the timeline’s longest layer
         *
         * @return On success, FCM_SUCCESS is returned, else an error code is returned.
         */    
        virtual FCM::Result _FCMCALL GetMaxFrameCount(FCM::U_Int32& frameCount) = 0;

    END_DECLARE_INTERFACE 
};


/* -------------------------------------------------- Inline / Functions */


#include "FCMPostConfig.h"

#endif // ITIMELINE_H_

