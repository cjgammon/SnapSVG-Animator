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
 * @file  IGradientFillStyle.h
 *
 * @brief This file contains the interface for IGradientFillStyle. The IGradientFillStyle 
 *        represents a "gradient" fill style.
 */

#ifndef IGRADIENT_FILL_STYLE_H_
#define IGRADIENT_FILL_STYLE_H_

#include "FCMPreConfig.h"
#include "FCMPluginInterface.h"
#include "Utils/DOMTypes.h"


/* -------------------------------------------------- Forward Decl */


/* -------------------------------------------------- Macros / Constants */

namespace DOM
{
    namespace FillStyle
    {
        /**
         * @brief Defines the universally-unique interface ID for 
         *        IGradientFillStyle
         *
         * @note  Textual Representation: {82567309-5FAD-4948-9B83-CD8F4C0BF77C}
         */
        FCM::ConstFCMIID IID_IGRADIENT_FILL_STYLE =
            {0x82567309, 0x5fad, 0x4948, {0x9b, 0x83, 0xcd, 0x8f, 0x4c, 0xb, 0xf7, 0x7c}};
    }
}


/* -------------------------------------------------- Enums */

namespace DOM
{
    namespace FillStyle
    {
        /**
         * @enum  GradientSpread
         *
         * @brief This defines the behavior of a gradient's overflow.
         */
        enum GradientSpread
        {
            /** Extended spread */
            GRADIENT_SPREAD_EXTEND,

            /** Reflect spread */
            GRADIENT_SPREAD_REFLECT,

            /** Repeat spread : Gradient is repeated */
            GRADIENT_SPREAD_REPEAT
        };
        
    }

}


/* -------------------------------------------------- Structs / Unions */


/* -------------------------------------------------- Class Decl */


namespace DOM
{
    namespace FillStyle
    {
        /**
         * @class IGradientFillStyle
         *
         * @brief Defines an interface that represents a "gradient" fill style.
         */
        BEGIN_DECLARE_INTERFACE(IGradientFillStyle, IID_IGRADIENT_FILL_STYLE)

            /**
             * @brief  This function gets the transformation matrix that defines the 
             *         placement, orientation, and scales for the gradient fill.
             *
             * @param  matrix (OUT)
             *         Transformation matrix
             *
             * @return On success, FCM_SUCCESS is returned, else an error code is returned.
             */    
            virtual FCM::Result _FCMCALL GetMatrix(Utils::MATRIX2D& matrix) = 0;


            /**
             * @brief  This function gets a color gradient - it can be either linear gradient 
             *         (ILinearColorGradientor) or a radial gradient(IRadialColorGradient).
             *
             * @param  pColorGradient (OUT)
             *         Color gradient (ILinearColorGradientor or IRadialColorGradient)
             *
             * @return On success, FCM_SUCCESS is returned; else an error code is returned.
             *
             * @see    FCM::IFCMUnknown
             *
             * @see    DOM::Utils::ILinearColorGradient
             *
             * @see    DOM::Utils::IRadialColorGradient
             */
            virtual FCM::Result _FCMCALL GetColorGradient(FCM::PIFCMUnknown& pColorGradient) = 0;


            /**
             * @brief  This function gets the spread type.
             *
             * @param  spread (OUT)
             *         GradientSpread type set for this fill
             *
             * @return On success, FCM_SUCCESS is returned, else an error code is returned.
             */
            virtual FCM::Result _FCMCALL GetSpread(GradientSpread& spread) = 0;

        END_DECLARE_INTERFACE
    }
}


/* -------------------------------------------------- Inline / Functions */


#include "FCMPostConfig.h"

#endif // IGRADIENT_FILL_STYLE_H_

