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
 * @file  IBevelFilter.h
 *
 * @brief This file contains interface for IBevelFilter. 
 *        This represents Bevel Filter.
 */

#ifndef IBEVEL_FILTER_H_
#define IBEVEL_FILTER_H_

#include "FCMPreConfig.h"
#include "FCMPluginInterface.h"
#include "Utils/DOMTypes.h"


/* -------------------------------------------------- Forward Decl */


/* -------------------------------------------------- Enums */


/* -------------------------------------------------- Macros / Constants */

namespace DOM
{
    namespace GraphicFilter
    {
        /**
         * @brief Defines the universally-unique interface ID for 
         *        IBevelFilter.
         *
         * @note  Textual Representation:  {B6B6702D-D4BD-4B28-B916-FA713EE10E70}
         */
        FCM::ConstFCMIID IID_IBEVEL_FILTER =
            {0xb6b6702d, 0xd4bd, 0x4b28, {0xb9, 0x16, 0xfa, 0x71, 0x3e, 0xe1, 0xe, 0x70}};
    }
}


/* -------------------------------------------------- Structs / Unions */


/* -------------------------------------------------- Class Decl */

namespace DOM
{
    namespace GraphicFilter
    {
        /**
         * @class IBevelFilter
         *
         * @brief This interface defines the Bevel filter applied to the object.
         */
        BEGIN_DECLARE_INTERFACE(IBevelFilter, IID_IBEVEL_FILTER)

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
             *         Quality type of the filter(High or Low or Medium).
             *
             * @return On success, FCM_SUCCESS is returned, else an error code is returned.
             */
            virtual FCM::Result _FCMCALL GetQuality(Utils::FilterQualityType& qualityType) = 0;

            
            /**
             * @brief  This function is used to get the color of the shadow applied 
             *          to the object.
             *
             * @param  color (OUT) 
             *         Color of the shadow applied to the object(in RGBA form).
             *
             * @return On success, FCM_SUCCESS is returned, else an error code is returned.
             */
            virtual FCM::Result _FCMCALL GetShadowColor(Utils::COLOR& color) = 0;

            
            /**
             * @brief  This function is used to get the highlight color.
             *
             * @param  color (OUT) 
             *         Highlight color of the filter applied to the object(in RGBA form).
             *
             * @return On success, FCM_SUCCESS is returned, else an error code is returned.
             */
            virtual FCM::Result _FCMCALL GetHighlightColor(Utils::COLOR& color) = 0;
            

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
             *         Boolean variable is set to true when Knock out property is enabled.
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
             * @brief  This function is used to determine whether or not the 
             *         filter is enabled for the object.
             *
             * @param  enabled (OUT) 
             *         Boolean value is set to true when filter is enabled 
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

#endif // IBEVEL_FILTER_H_

