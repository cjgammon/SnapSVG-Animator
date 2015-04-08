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
 * @file  IGradientGlowFilter.h
 *
 * @brief This file contains interface for IGradientGlowFilter. 
 *        This represents Gradient glow Filter.
 */

#ifndef IGRADIENT_GLOW_FILTER_H_
#define IGRADIENT_GLOW_FILTER_H_

#include "FCMPreConfig.h"
#include "FCMPluginInterface.h"



/* -------------------------------------------------- Forward Decl */


/* -------------------------------------------------- Enums */


/* -------------------------------------------------- Macros / Constants */

namespace DOM
{
    namespace GraphicFilter
    {
        /**
         * @brief Defines the universally-unique interface ID for 
         *        IGradientGlowFilter
         *
         * @note  Textual Representation:  {AB4017B5-6994-4D76-A6D4-01F16F1D9290}
         */
        FCM::ConstFCMIID IID_IGRADIENT_GLOW_FILTER =
            {0xab4017b5, 0x6994, 0x4d76, {0xa6, 0xd4, 0x1, 0xf1, 0x6f, 0x1d, 0x92, 0x90}};
    }
}


/* -------------------------------------------------- Structs / Unions */


/* -------------------------------------------------- Class Decl */

namespace DOM
{
    namespace GraphicFilter
    {
        /**
         * @class IGradientGlowFilter
         *
         * @brief This interface defines the Gradient Glow filter applied to the object.
         */
        BEGIN_DECLARE_INTERFACE(IGradientGlowFilter, IID_IGRADIENT_GLOW_FILTER)

            /**
             * @brief  This function is used to get the blur value along the X axis for the object 
             *         in context.
             *
             * @param  blurX (OUT) 
             *         Value of the blur-X property for the object.
             *
             * @return On success, FCM_SUCCESS is returned, else an error code is returned.
             */
            virtual FCM::Result _FCMCALL GetBlurX(FCM::Double& blurX) = 0;


            /**
              * @brief  This function is used to get the blur value along the Y axis for the object 
              *         in context.
              *
              * @param  blurY (OUT) 
              *         Value of the blur-Y property for the object.
              *
              * @return On success, FCM_SUCCESS is returned, else an error code is returned.
              */
            virtual FCM::Result _FCMCALL GetBlurY(FCM::Double& blurY) = 0;


            /**
             * @brief  This function is used to get the strength value of the filter applied 
             *          to the object.
             *
             * @param  strength (OUT) 
             *         Value of the strength property for the object in context.
             *
             * @return On success, FCM_SUCCESS is returned, else an error code is returned.
             */
            virtual FCM::Result _FCMCALL GetStrength(FCM::S_Int32& strength) = 0;

            
            /**
             * @brief  This function is used to get the type of quality of the filter applied 
             *          to the object.
             *
             * @param  qualityType (OUT) 
             *         Quality type of the filter(High, Low, or Medium).
             *
             * @return On success, FCM_SUCCESS is returned, else an error code is returned.
             */
            virtual FCM::Result _FCMCALL GetQuality(Utils::FilterQualityType& qualityType) = 0;


            /**
             * @brief  This function is used to get the angle at which filter
             *         effects are applied.
             *
             * @param  angle (OUT) 
             *         Angle at which the filter effects are applied(in Radians).
             *
             * @return On success, FCM_SUCCESS is returned, else an error code is returned.
             */
            virtual FCM::Result _FCMCALL GetAngle(FCM::Double& angle) = 0;


            /**
             * @brief  This function is used to get the distance at which filter 
             *         effects are applied.
             *
             * @param  distance (OUT) 
             *         Distance at which the filter effects are applied.
             *
             * @return On success, FCM_SUCCESS is returned, else an error code is returned.
             */
            virtual FCM::Result _FCMCALL GetDistance(FCM::Double& distance) = 0;


            /**
             * @brief  This function is used to determine if knockout is enabled or disabled.
             *
             * @param  knockOut (OUT) 
             *         Boolean variable which is set to true when Knock out property is enabled.
             *
             * @return On success, FCM_SUCCESS is returned, else an error code is returned.
             */
            virtual FCM::Result _FCMCALL GetKnockout(FCM::Boolean& knockOut) = 0;


            /**
             * @brief  This function is used to get the filter type.
             *
             * @param  filterType (OUT) 
             *         This parameter can any on of the three types - Inner, Outer or Full.
             *
             * @return On success, FCM_SUCCESS is returned, else an error code is returned.
             */
            virtual FCM::Result _FCMCALL GetFilterType(Utils::FilterType& filterType) = 0;

            
            /**
             * @brief  This function is used to get the gradient color.
             *
             * @param  pGradient (OUT) 
             *         This parameter used to get key color count and color at index 
             *         required to make up gradient. Depending on type of gradient, this 
             *         can be either queried for ILinearColorGradient or IRadialColorGradient.
             *
             * @return On success, FCM_SUCCESS is returned, else an error code is returned.
             *
             * @see    DOM::Utils::ILinearColorGradient
             *
             * @see    DOM::Utils::IRadialColorGradient
             */
            virtual FCM::Result _FCMCALL GetGradient(FCM::PIFCMUnknown& pGradient) = 0;

            
            /**
             * @brief  This function is used to determine whether or not the 
             *         filter is enabled for the object.
             *
             * @param  enabled (OUT) 
             *         Boolean value which is set to true when filter is enabled 
             *         for the object in context else set to false.
             *
             * @return On success, FCM_SUCCESS is returned, else an error code is returned.
             */
            virtual FCM::Result _FCMCALL IsEnabled(FCM::Boolean& enabled) = 0;

        END_DECLARE_INTERFACE

    }
}


/* -------------------------------------------------- Inline / Functions */


#include "FCMPostConfig.h"

#endif // IGRADIENT_GLOW_FILTER_H_

