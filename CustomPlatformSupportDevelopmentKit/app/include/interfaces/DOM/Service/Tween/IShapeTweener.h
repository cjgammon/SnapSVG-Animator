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
 * @file  IShapeTweener.h
 *
 * @brief This file contains interface for IShapeTweener. 
 *        IShapeTweener represents a shape tweener.
 */

#ifndef ISHAPE_TWEENER_H_
#define ISHAPE_TWEENER_H_

#include "FCMPreConfig.h"
#include "FCMPluginInterface.h"
#include "Utils/DOMTypes.h"


/* -------------------------------------------------- Forward Decl */

namespace DOM
{
    namespace FrameElement
    {
        FORWARD_DECLARE_INTERFACE(IShape);
    }
}


/* -------------------------------------------------- Enums */


/* -------------------------------------------------- Macros / Constants */

namespace DOM
{
    namespace Service
    {
        namespace Tween
        {
            /**
             * @brief Defines the universally-unique interface ID for 
             *        IShapeTweener.
             *
             * @note  Textual Representation: {7228A823-6A82-4C89-A60C-7CB8C2183EE4}
             */
            FCM::ConstFCMIID IID_ISHAPE_TWEENER =
                {0x7228a823, 0x6a82, 0x4c89, {0xa6, 0xc, 0x7c, 0xb8, 0xc2, 0x18, 0x3e, 0xe4}};
        }
    }
}


/* -------------------------------------------------- Structs / Unions */


/* -------------------------------------------------- Class Decl */

namespace DOM
{
    namespace Service
    {
        namespace Tween
        {
            /**
             * @class IShapeTweener
             *
             * @brief Defines an interface that represents shape tweener.
             *        Shape tweener can be used to obtain the shapes at 
             *        discrete intervals during shape tween.
             */
            BEGIN_DECLARE_INTERFACE(IShapeTweener, IID_ISHAPE_TWEENER)

                /**
                 * @brief  Returns the shape object at a specific point during shape animation.
                 *
                 * @param  pTween (IN)
                 *         Tween object
                 *
                 * @param  offset (IN)
                 *         The point in the frame span for which the shape is required. 
                 *         It can have values from 0 to (FrameDuration - 2). 
                 *         The value at (FrameDuration - 1) is available in the next keyframe.
                 *
                 * @param  pShape (OUT)
                 *         Contains the morphed shape at the specified offset.
                 *
                 * @return On success, FCM_SUCCESS is returned, else an error code is returned.
                 */ 
                virtual FCM::Result _FCMCALL GetShape(
                    PITween pTween, 
                    FCM::U_Int32 offset, 
                    DOM::FrameElement::PIShape& pShape) = 0;

            END_DECLARE_INTERFACE
        }
    }
};


/* -------------------------------------------------- Inline / Functions */


#include "FCMPostConfig.h"

#endif // ISHAPE_TWEENER_H_

