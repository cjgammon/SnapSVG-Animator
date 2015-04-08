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
 * @file  ILinearColorGradient.h
 *
 * @brief This file contains the interface for ILinearColorGradient. This interface 
 *        represents linear color gradient.
 */

#ifndef ILINEAR_COLOR_GRADIENT_H_
#define ILINEAR_COLOR_GRADIENT_H_

#include "FCMPreConfig.h"
#include "FCMTypes.h"
#include "IFCMUnknown.h"
#include "Utils/DOMTypes.h"


/* -------------------------------------------------- Forward Decl */


/* -------------------------------------------------- Enums */


/* -------------------------------------------------- Macros / Constants */

namespace DOM
{
    namespace Utils
    {

        /**
         * @brief Defines the universally-unique Interface ID for ILinearColorGradient.
         *
         * @note  Textual Representation:  {39CE80B7-90A9-479E-A46C-D48FA6CFC923}
         */
        FCM::ConstFCMIID IID_ILINEAR_COLOR_GRADIENT =
            {0x39ce80b7, 0x90a9, 0x479e, {0xa4, 0x6c, 0xd4, 0x8f, 0xa6, 0xcf, 0xc9, 0x23}};
    }
}


/* -------------------------------------------------- Structs / Unions */


/* -------------------------------------------------- Class Decl */

namespace DOM
{
    namespace Utils
    {
        /**
         * @class ILinearColorGradient
         *
         * @brief This interface represents the linear color gradient.
         */
        BEGIN_DECLARE_INTERFACE(ILinearColorGradient, IID_ILINEAR_COLOR_GRADIENT)

            /**
             * @brief  This function gets the number of key points in the gradient ramp.
             *         Each key point contains a color and its position in the gradient.
             *
             * @param  nColors (OUT)
             *         Number of points in the color ramp
             *
             * @return On success, FCM_SUCCESS is returned; else an error code is returned.
             */
            virtual FCM::Result _FCMCALL GetKeyColorCount(FCM::U_Int8& nColors) = 0;


            /**
             * @brief  This function gets a specific key point in a color ramp.
             *         Each key point contains a color and its position in the gradient.
             *
             * @param  index (IN)
             *         The index of a particular key color. It can vary from 0 to 
             *         GetKeyColorCount() - 1
             *
             * @param  colorpoint (OUT)
             *         The value of a color and its position in the color ramp;
             *
             * @return On success, FCM_SUCCESS is returned; else an error code is returned.
             */
            virtual FCM::Result _FCMCALL GetKeyColorAtIndex(
                FCM::U_Int8 index, 
                GRADIENT_COLOR_POINT& colorpoint) = 0;

        END_DECLARE_INTERFACE
    }
}


/* -------------------------------------------------- Inline / Functions */


#include "FCMPostConfig.h"

#endif // ILINEAR_COLOR_GRADIENT_H_

