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
 * @file  IColorTweener.h
 *
 * @brief This file contains interface for IColorTweener. IColorTweener 
 *        represents a color tweener. 
 */

#ifndef ICOLOR_TWEENER_H_
#define ICOLOR_TWEENER_H_

#include "FCMPreConfig.h"
#include "FCMPluginInterface.h"
#include "Utils/DOMTypes.h"


/* -------------------------------------------------- Forward Decl */


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
             *        IColorTweener.
             *
             * @note  Textual Representation: {BA44C10E-6EAA-4AEE-A9E9-F14E9BF83321}
             */
            FCM::ConstFCMIID IID_ICOLOR_TWEENER =
                {0xba44c10e, 0x6eaa, 0x4aee, {0xa9, 0xe9, 0xf1, 0x4e, 0x9b, 0xf8, 0x33, 0x21}};
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
             * @class IColorTweener
             *
             * @brief Defines an interface that represents a color tweener.
             *        Color tweener can be used to obtain interpolated color
             *        values at discrete intervals during a motion/classic tween.
             */
            BEGIN_DECLARE_INTERFACE(IColorTweener, IID_ICOLOR_TWEENER)

                /**
                 * @brief  Returns the color transform (in the form of a matrix) within the frame span duration.
                 *
                 * @param  pTween (IN)
                 *         Tween object
                 *
                 * @param  offset (IN)
                 *         The point in the frame span for which color transform is required. 
                 *         It can have values from 0 to (FrameDuration - 2). 
                 *         The value at (FrameDuration - 1) is available in the next keyframe.
                 *
                 * @param  colorMatrix (OUT)
                 *         The color transform (in the form of a matrix) containing the color 
                 *         transform at the offset.
                 *
                 * @return On success, FCM_SUCCESS is returned, else an error code is returned.
                 */ 
                virtual FCM::Result _FCMCALL GetColorMatrix(
                    PITween pTween, 
                    FCM::U_Int32 offset, 
                    Utils::COLOR_MATRIX& colorMatrix) = 0;

            END_DECLARE_INTERFACE
        }
    }
};


/* -------------------------------------------------- Inline / Functions */


#include "FCMPostConfig.h"

#endif // ITWEENER_H_

