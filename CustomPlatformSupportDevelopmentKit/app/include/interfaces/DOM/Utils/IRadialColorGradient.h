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
 * @file  IRadialColorGradient.h
 *
 * @brief This file contains the interface for IRadialColorGradient. This interface
 *        represents a radial color gradient.
 */

#ifndef IRADIAL_COLOR_GRADIENT_H_
#define IRADIAL_COLOR_GRADIENT_H_

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
         * @brief Defines the universally-unique Interface ID for IRadialColorGradient.
         *
         * @note  Textual Representation:  {28DC1E66-066F-4ABA-92A0-AA9DB74B9853}
         */
        FCM::ConstFCMIID IID_IRADIAL_COLOR_GRADIENT =
           {0x28dc1e66, 0x66f, 0x4aba, {0x92, 0xa0, 0xaa, 0x9d, 0xb7, 0x4b, 0x98, 0x53}};
    }
}


/* -------------------------------------------------- Structs / Unions */


/* -------------------------------------------------- Class Decl */

namespace DOM
{
    namespace Utils
    {
        /**
         * @class IRadialColorGradient
         *
         * @brief This interface represents the radial color gradient.
         */
        BEGIN_DECLARE_INTERFACE(IRadialColorGradient, IID_IRADIAL_COLOR_GRADIENT)
     
            /**
             * @brief  This function gets the number of key points in the gradient ramp.
             *         Each key point contains a color and its position in the gradient.
             *
             * @param  nColors (OUT)
             *         Number of points in the color ramp
             *
             * @return On success, FCM_SUCCESS is returned, else an error code is returned.
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
             *         The value of a color and its position in the color ramp.
             *
             * @return On success, FCM_SUCCESS is returned, else an error code is returned.
             */
            virtual FCM::Result _FCMCALL GetKeyColorAtIndex(
                FCM::U_Int8 index, 
                GRADIENT_COLOR_POINT& colorpoint) = 0;


            /**
             * @brief  This function gets the focal point (meaningful only for radial 
             *         interpolation). Focal point specifies the horizontal offset of 
             *         the focal point from the transformation point.
             *
             * @param  focalPoint (OUT)
             *         Horizontal offset of the focal point from the transformation
             *         point.
             *
             * @return On success, FCM_SUCCESS is returned, else an error code is returned.
             */
            virtual Result _FCMCALL GetFocalPoint(FCM::S_Int32& focalPoint) = 0;
            
        END_DECLARE_INTERFACE
    }
}


/* -------------------------------------------------- Inline / Functions */


#include "FCMPostConfig.h"

#endif // IRADIAL_COLOR_GRADIENT_H_

