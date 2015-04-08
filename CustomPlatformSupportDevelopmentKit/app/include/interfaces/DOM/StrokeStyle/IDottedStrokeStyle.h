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
 * @file  IDottedStrokeStyle.h
 *
 * @brief This file contains the interface for IDottedStrokeStyle. This interface 
 *        represents a dotted stroke of an edge.
 */

#ifndef IDOTTED_STROKE_STYLE_H_
#define IDOTTED_STROKE_STYLE_H_

#include "FCMPreConfig.h"
#include "FCMPluginInterface.h"


/* -------------------------------------------------- Forward Decl */


/* -------------------------------------------------- Enums */


/* -------------------------------------------------- Macros / Constants */

namespace DOM
{
    namespace StrokeStyle
    {
        /**
         * @brief Defines the universally-unique interface ID for 
         *        IDottedStrokeStyle.
         *
         * @note  Textual Representation: {B686E0AD-C9B4-43DC-9D0D-73D506A9660E}
         */
        FCM::ConstFCMIID IID_IDOTTED_STROKE_STYLE =
            {0xb686e0ad, 0xc9b4, 0x43dc, {0x9d, 0xd, 0x73, 0xd5, 0x6, 0xa9, 0x66, 0xe}};
    }
}


/* -------------------------------------------------- Structs / Unions */


/* -------------------------------------------------- Class Decl */

namespace DOM
{
    namespace StrokeStyle
    {
        /**
         * @class IDottedStrokeStyle
         *
         * @brief Defines an interface that represents a "dotted" stroke of an edge. 
         */
        BEGIN_DECLARE_INTERFACE(IDottedStrokeStyle, IID_IDOTTED_STROKE_STYLE)

            /**
             * @brief  This function provides the stroke thickness.
             *
             * @param  thickness (OUT)
             *         Thickness of the stroke.
             *
             * @return On success, FCM_SUCCESS is returned, else an error code is returned.
             */
            virtual FCM::Result _FCMCALL GetThickness(FCM::Double& thickness) = 0;


            /**
             * @brief  This function gets the fill style settings of the stroke.
             *
             * @param  pFillStyle (OUT)
             *         Fill style of the stroke (ISolidFillStyle or IGradientFillStyle
             *         or IBitmapFillStyle).
             *
             * @return On success, FCM_SUCCESS is returned; otherwise an error code is returned.
             *
             * @see    DOM::FillStyle::ISolidFillStyle
             *
             * @see    DOM::FillStyle::IGradientFillStyle
             *
             * @see    DOM::FillStyle::IBitmapFillStyle
             */
            virtual FCM::Result _FCMCALL GetFillStyle(PIFCMUnknown& pFillStyle) = 0;


            /**
             * @brief  This function gets the spacing between dots in a dotted line.
             *
             * @param  dotSpace (OUT)
             *         Spacing between dots in a dotted line.
             *
             * @return On success, FCM_SUCCESS is returned; otherwise an error code is returned.
             */
            virtual FCM::Result _FCMCALL GetDotSpace(FCM::U_Int32& dotSpace) = 0;

        END_DECLARE_INTERFACE
    }
}


/* -------------------------------------------------- Inline / Functions */

#include "FCMPostConfig.h"

#endif // IDOTTED_STROKE_STYLE_H_

