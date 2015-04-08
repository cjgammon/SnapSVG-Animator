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
 * @file  IDashedStrokeStyle.h
 *
 * @brief This file contains the interface for IDashedStrokeStyle. This interface 
 *        represents a dashed stroke (series of solid dashes followed by blanks) of an edge.
 */

#ifndef IDASHED_STROKE_STYLE_H_
#define IDASHED_STROKE_STYLE_H_

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
         *        IDashedStrokeStyle.
         *
         * @note  Textual Representation: {C442D351-DA7B-4982-A9DA-CE6682EAEC47}
         */
        FCM::ConstFCMIID IID_IDASHED_STROKE_STYLE =
           {0xc442d351, 0xda7b, 0x4982, {0xa9, 0xda, 0xce, 0x66, 0x82, 0xea, 0xec, 0x47}};
    }
}


/* -------------------------------------------------- Structs / Unions */


/* -------------------------------------------------- Class Decl */

namespace DOM
{
    namespace StrokeStyle
    {
        /**
         * @class IDashedStrokeStyle
         *
         * @brief Defines an interface that represents a "dashed" stroke of an edge. 
         */
        BEGIN_DECLARE_INTERFACE(IDashedStrokeStyle, IID_IDASHED_STROKE_STYLE)

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
             * @return On success, FCM_SUCCESS is returned, else an error code is returned.
             *
             * @see    DOM::FillStyle::ISolidFillStyle
             *
             * @see    DOM::FillStyle::IGradientFillStyle
             *
             * @see    DOM::FillStyle::IBitmapFillStyle
             */
            virtual FCM::Result _FCMCALL GetFillStyle(PIFCMUnknown& pFillStyle) = 0;


            /**
             * @brief  This function gets length of the solid parts of a dashed stroke.
             *
             * @param  solidDashLength (OUT)
             *         Length of the solid part of the dashed stroke.
             *
             * @return On success, FCM_SUCCESS is returned, else an error code is returned.
             */
            virtual FCM::Result _FCMCALL GetSolidDashLength(FCM::U_Int32& solidDashLength) = 0;


            /**
             * @brief  This function gets length of the blank part of a dashed stroke.
             *
             * @param  blankDashLength (OUT)
             *         Length of the blank part of the dashed stroke.
             *
             * @return On success, FCM_SUCCESS is returned, else an error code is returned.
             */
            virtual FCM::Result _FCMCALL GetBlankDashLength(FCM::U_Int32& blankDashLength) = 0;

        END_DECLARE_INTERFACE
    } 
}


/* -------------------------------------------------- Inline / Functions */


#include "FCMPostConfig.h"

#endif // IDASHED_STROKE_STYLE_H_

