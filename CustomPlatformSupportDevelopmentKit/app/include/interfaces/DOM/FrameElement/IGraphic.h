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
 * @file  IGraphic.h
 *
 * @brief This file contains the interface for IGraphic. 
 *        IGraphic interface represents a "graphic" symbol on the stage.
 */

#ifndef IGRAPHICINSTANCE_H_
#define IGRAPHICINSTANCE_H_

#include "FCMPreConfig.h"
#include "FCMPluginInterface.h"
#include "ISymbolInstance.h"


/* -------------------------------------------------- Forward Decl */


/* -------------------------------------------------- Enums */

namespace DOM
{
    namespace FrameElement
    {
         /**
         * @enum AnimationLoopMode
         *
         * @brief This defines different modes of playing animation for graphic instance.
         */
        enum AnimationLoopMode
        {
            /** Loop */
            ANIMATION_LOOP,

            /** Play once */
            ANIMATION_PLAY_ONCE,

            /** Single frame */
            ANIMATION_SINGLE_FRAME
        }; 
    }
} 


/* -------------------------------------------------- Macros / Constants */

namespace DOM
{
    namespace FrameElement
    {
        /**
         * @brief Defines the universally-unique interface ID for 
         *        IGraphic
         *
         * @note  Textual Representation: {E8E5694B-749B-411E-9A1C-5035F37D1032}
         */
        FCM::ConstFCMIID IID_IGRAPHIC_INSTANCE =
            {0xe8e5694b, 0x749b, 0x411e, {0x9a, 0x1c, 0x50, 0x35, 0xf3, 0x7d, 0x10, 0x32}};
    }
}


/* -------------------------------------------------- Structs / Unions */


/* -------------------------------------------------- Class Decl */

namespace DOM
{
    namespace FrameElement
    {
        /**
         * @class IGraphic
         *
         * @brief Defines an interface that represents an instance of a "graphic"
         *        symbol present on the stage.
         */
        BEGIN_DECLARE_INTERFACE(IGraphic, IID_IGRAPHIC_INSTANCE)

            /**
             * @brief  This function returns the loop mode of animation for the graphic
             *         instance.
             *
             * @param  loopMode (OUT)
             *         Loop mode of animation for the graphic instance.
             *
             * @return On success, FCM_SUCCESS is returned, else an error code is returned.
             */
            virtual FCM::Result _FCMCALL GetLoopMode(AnimationLoopMode& loopMode) = 0;

            
            /**
             * @brief  This function returns the first frame index of the animation 
             *         to display.
             *
             * @param  firstFrameIndex (OUT)
             *         This specifies the first frame to display.
             *
             * @return On success, FCM_SUCCESS is returned, else an error code is returned.
             */
            virtual FCM::Result _FCMCALL GetFirstFrameIndex(FCM::U_Int32& firstFrameIndex) = 0;
            
        END_DECLARE_INTERFACE
    }
}


/* -------------------------------------------------- Inline / Functions */


#include "FCMPostConfig.h"

#endif // IGRAPHICINSTANCE_H_

