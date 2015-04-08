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
 * @file  ISolidStrokeStyle.h
 *
 * @brief This file contains the interface for ISolidStrokeStyle. This interface 
 *        represents a solid stroke style of an edge.
 */

#ifndef ISOLID_STROKE_STYLE_H_
#define ISOLID_STROKE_STYLE_H_

#include "FCMPreConfig.h"
#include "FCMPluginInterface.h"


/* -------------------------------------------------- Forward Decl */

namespace DOM
{
    namespace StrokeStyle
    {
        FORWARD_DECLARE_INTERFACE(IStrokeWidth);
    };
};


/* -------------------------------------------------- Enums */


/* -------------------------------------------------- Macros / Constants */

namespace DOM
{
    namespace StrokeStyle
    {
        /**
         * @brief Defines the universally-unique interface ID for 
         *        ISolidStrokeStyle.
         *
         * @note  Textual Representation: {6A896201-FD1D-46DB-8AE2-BE5B9AB25529}
         */
        FCM::ConstFCMIID IID_ISOLID_STROKE_STYLE = 
            {0x6a896201, 0xfd1d, 0x46db, {0x8a, 0xe2, 0xbe, 0x5b, 0x9a, 0xb2, 0x55, 0x29}};
    }
}


/* -------------------------------------------------- Structs / Unions */

namespace DOM
{
    namespace StrokeStyle
    {
        /**
         * @struct MITER_JOIN_PROP
         *
         * @brief  Defines the properties for the miter join.
         */
        struct MITER_JOIN_PROP
        {
            /** 
             * Specifies the angle above which the tip of the miter will be 
             * truncated by a segment. That means the miter is truncated only if 
             * the miter angle is greater than the value of miterLimit.
             */
            FCM::Double miterLimit;
        };


        /**
         * @struct CAP_STYLE
         *
         * @brief  Defines cap style.
         */
        struct CAP_STYLE
        {
            /** 
             * Caller must ensure that this member is set to the sizeof this 
             * structure. That is, it must be set to sizeof(CAP_STYLE).
             */
            size_t structSize;


            /** Type of cap */
            Utils::CapType type;
        };


        /**
         * @struct JOIN_STYLE
         *
         * @brief  Defines a join style
         */
        struct JOIN_STYLE
        {
            /** 
             * Caller must ensure that this member is set to the size of this 
             * structure. That is, it must be set to sizeof(JOIN_STYLE).
             */
            size_t structSize;


            /** Type of join */
            Utils::JoinType type;


            union 
            {
                /** Property for a miter join */
                MITER_JOIN_PROP miterJoinProp;
            };
        };
    }
}


/* -------------------------------------------------- Class Decl */

namespace DOM
{
    namespace StrokeStyle
    {
        /**
         * @class ISolidStrokeStyle
         *
         * @brief Defines an interface that represents a "solid" stroke style of an edge. 
         */
        BEGIN_DECLARE_INTERFACE(ISolidStrokeStyle, IID_ISOLID_STROKE_STYLE)

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
             * @brief  This function gets the join style of the stroke. Join style 
             *         contains information about the join type and any other 
             *         property associated with the join type.
             *
             * @param  joinStyle (OUT)
             *         Join style
             *
             * @return On success, FCM_SUCCESS is returned; else an error code is returned.
             *
             * @note   Caller of this function must set joinStyle.structSize to 
             *         sizeof(JOIN_STYLE).
             */
            virtual FCM::Result _FCMCALL GetJoinStyle(JOIN_STYLE& joinStyle) = 0;


            /**
             * @brief  This function gets the cap style of the stroke. Cap style 
             *         contains information about the cap type and any other 
             *         property associated with the cap type.
             *
             * @param  capStyle (OUT)
             *         Cap style
             *
             * @return On success, FCM_SUCCESS is returned, else an error code is returned.
             *
             * @note   Caller of this function must set capStyle.structSize to 
             *         sizeof(CAP_STYLE).
             */
            virtual FCM::Result _FCMCALL GetCapStyle(CAP_STYLE& capStyle) = 0;


            /**
             * @brief  This function gets the type of scale to be applied to the 
             *         stroke. The ScaleType returned will be normal for variable 
             *         width stroke.
             *
             * @param  scaleType (OUT)
             *         Type of scale
             *
             * @return On success, FCM_SUCCESS is returned; else an error code is returned.
             */
            virtual FCM::Result _FCMCALL GetScaleType(Utils::ScaleType& scaleType) = 0;


            /**
             * @brief  This function should be used to find whether stroke hinting 
             *         is set on the stroke. Stroke hinting will be returned 'false' for variable 
             *         width stroke.
             *
             * @param  strokeHinting (OUT)
             *         Set to true if stroke hinting is on; otherwise false is set.
             *
             * @return On success, FCM_SUCCESS is returned, else an error code is returned.
             */
            virtual FCM::Result _FCMCALL GetStrokeHinting(FCM::Boolean& strokeHinting) = 0;


            /**
             * @brief  This function returns stroke width associated with the stroke.
             *         The stroke width is returned for variable width strokes only. For 
             *         uniform width profile, "pStrokeWidth" is set to NULL.
             *
             * @param  pStrokeWidth (OUT)
             *         Stroke width associated with the stroke.
             *
             * @return On success, FCM_SUCCESS is returned, else an error code is returned.
             */
            virtual FCM::Result _FCMCALL GetStrokeWidth(DOM::StrokeStyle::PIStrokeWidth& pStrokeWidth) = 0;

        END_DECLARE_INTERFACE
    }
}


/* -------------------------------------------------- Inline / Functions */


#include "FCMPostConfig.h"

#endif // ISOLID_STROKE_STYLE_H_

