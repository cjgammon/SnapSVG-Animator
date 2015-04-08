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
 * @file  IRaggedStrokeStyle.h
 *
 * @brief This file contains the interface for IRaggedStrokeStyle. This interface 
 *        represents a ragged stroke of an edge.
 */

#ifndef IRAGGED_STROKE_STYLE_H_
#define IRAGGED_STROKE_STYLE_H_

#include "FCMPreConfig.h"
#include "FCMPluginInterface.h"


/* -------------------------------------------------- Forward Decl */


/* -------------------------------------------------- Enums */

namespace DOM
{
    namespace StrokeStyle
    {
        /**
         * @enum  RaggedStrokePattern
         *
         * @brief Specifies the pattern of a ragged stroke.
         */
        enum RaggedStrokePattern
        {
            /** Solid pattern  */
            RAGGED_SOLID_PATTERN,

            /** Simple pattern  */
            RAGGED_SIMPLE_PATTERN,

            /** Random pattern  */
            RAGGED_RANDOM_PATTERN,

            /** Dotted pattern  */
            RAGGED_DOTTED_PATTERN,

            /** Randomly dotted pattern  */
            RAGGED_RANDOM_DOTTED_PATTERN,

            /** Triple dotted pattern  */
            RAGGED_TRIPLE_DOTTED_PATTERN,

            /** Randomly triple dotted pattern  */
            RAGGED_RANDOM_TRIPLE_DOTTED_PATTERN,
        };


        /**
         * @enum  RaggedWaveHeight
         *
         * @brief Specifies the wave height of the pattern within the ragged stroke.
         */
        enum RaggedWaveHeight
        {
            /** Flat  */
            RAGGED_WAVEHEIGHT_FLAT,

            /** Wavy  */
            RAGGED_WAVEHEIGHT_WAVY,

            /** Very Wavy */
            RAGGED_WAVEHEIGHT_VERYWAVY,

            /** Wild  */
            RAGGED_WAVEHEIGHT_WILD,
        };

        /**
         * @enum  RaggedWaveLength
         *
         * @brief Specifies the wave length of the pattern within the ragged stroke.
         */
        enum RaggedWaveLength
        {
            /** Very short  */
            RAGGED_WAVELENGTH_VERYSHORT,

            /** short  */
            RAGGED_WAVELENGTH_SHORT,

            /** Medium  */
            RAGGED_WAVELENGTH_MEDIUM,

            /** Long  */
            RAGGED_WAVELENGTH_LONG
        };
    }
}


/* -------------------------------------------------- Macros / Constants */

namespace DOM
{
    namespace StrokeStyle
    {
        /**
         * @brief Defines the universally-unique interface ID for 
         *        IRaggedStrokeStyle.
         *
         * @note  Textual Representation: {FCAA3162-1C2D-4C2B-91CA-6F64C39D3928}
         */
        FCM::ConstFCMIID IID_IRAGGED_STROKE_STYLE =
            {0xfcaa3162, 0x1c2d, 0x4c2b, {0x91, 0xca, 0x6f, 0x64, 0xc3, 0x9d, 0x39, 0x28}};
    }
}


/* -------------------------------------------------- Structs / Unions */


/* -------------------------------------------------- Class Decl */

namespace DOM
{
    namespace StrokeStyle
    {
        /**
         * @class IRaggedStrokeStyle
         *
         * @brief Defines an interface that represents a "ragged" stroke of an edge. 
         */
        BEGIN_DECLARE_INTERFACE(IRaggedStrokeStyle, IID_IRAGGED_STROKE_STYLE)

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
             * @brief  This functions gets the pattern of a ragged line.
             *
             * @param  strokePattern (OUT)
             *         Stroke pattern of the ragged line.
             *
             * @return On success, FCM_SUCCESS is returned, else an error code is returned.
             */
            virtual FCM::Result _FCMCALL GetPattern(RaggedStrokePattern& strokePattern) = 0;


            /**
             * @brief  This functions gets the wave height of a ragged line.
             *
             * @param  waveHeight (OUT)
             *         Wave height
             *
             * @return On success, FCM_SUCCESS is returned, else an error code is returned.
             */
            virtual FCM::Result _FCMCALL GetWaveHeight(RaggedWaveHeight& waveHeight) = 0;


            /**
             * @brief  This functions gets the wave length of a ragged line.
             *
             * @param  waveLength (OUT)
             *         Wave length
             *
             * @return On success, FCM_SUCCESS is returned, else an error code is returned.
             */
            virtual FCM::Result _FCMCALL GetWaveLength(RaggedWaveLength& waveLength) = 0;

        END_DECLARE_INTERFACE
    }
}


/* -------------------------------------------------- Inline / Functions */


#include "FCMPostConfig.h"

#endif // IRAGGED_STROKE_STYLE_H_

