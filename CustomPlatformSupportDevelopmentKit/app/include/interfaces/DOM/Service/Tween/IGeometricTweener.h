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
 * @file  IGeometricTweener.h
 *
 * @brief This file contains interface for IGeometricTweener. IGeometricTweener 
 *        represents a geometric tweener. 
 */

#ifndef IGEOMETRIC_TWEENER_H_
#define IGEOMETRIC_TWEENER_H_

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
             *        IGeometricTweener.
             *
             * @note  Textual Representation: {3D45198B-1ECD-4731-A71C-A8C4897F1B1C}
             */
            FCM::ConstFCMIID IID_IGEOMETRIC_TWEENER =
                {0x3d45198b, 0x1ecd, 0x4731, {0xa7, 0x1c, 0xa8, 0xc4, 0x89, 0x7f, 0x1b, 0x1c}};
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
             * @class IGeometricTweener
             *
             * @brief Defines an interface that represents a geometric tweener(Transform).
             *        Geometric tweener can be used to obtain 2D transformation matrix
             *        at discrete intervals during a motion/classic tween.
             */
            BEGIN_DECLARE_INTERFACE(IGeometricTweener, IID_IGEOMETRIC_TWEENER)

                /**
                 * @brief  Returns the geometric transform within the frame span duration.
                 *
                 * @param  pTween (IN)
                 *         Tween object
                 *
                 * @param  offset (IN)
                 *         The point in the frame span for which geometric transform is required. 
                 *         It can have values from 0 to (FrameDuration - 2). 
                 *         The value at (FrameDuration - 1) is available in the next keyframe.
                 *
                 * @param  geomMatrix (OUT)
                 *         Matrix containing the geometric transform at the offset.
                 *
                 * @return On success, FCM_SUCCESS is returned, else an error code is returned.
                 */ 
                virtual FCM::Result _FCMCALL GetGeometricTransform(
                    PITween pTween, 
                    FCM::U_Int32 offset, 
                    Utils::MATRIX2D& geomMatrix) = 0;

            END_DECLARE_INTERFACE
        }
    }
};


/* -------------------------------------------------- Inline / Functions */


#include "FCMPostConfig.h"

#endif // IGEOMETRIC_TWEENER_H_

