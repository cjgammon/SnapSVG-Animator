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
 * @file  IDropShadowFilter.h
 *
 * @brief This file contains interface for IDropShadowFilter. 
 *        This represents Drop Shadow Filter.
 */

#ifndef IDROP_SHADOW_FILTER_H_
#define IDROP_SHADOW_FILTER_H_

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
         *        IDropShadowFilter.
         *
         * @note  Textual Representation:  {2AEF9C73-153E-4CAF-8064-A840266E8E83}
         */
        FCM::ConstFCMIID IID_IDROP_SHADOW_FILTER =
            {0x2aef9c73, 0x153e, 0x4caf, {0x80, 0x64, 0xa8, 0x40, 0x26, 0x6e, 0x8e, 0x83}};
    }
}


/* -------------------------------------------------- Structs / Unions */


/* -------------------------------------------------- Class Decl */

namespace DOM
{
    namespace GraphicFilter
    {
        /**
         * @class IDropShadowFilter
         *
         * @brief This interface defines the Drop Shadow filter applied to the object.
         */
        BEGIN_DECLARE_INTERFACE(IDropShadowFilter, IID_IDROP_SHADOW_FILTER)

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
             *         Angle at which the filter effects are applied (in Radians).
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
             * @brief  This function is used to determine whether knockout is enabled or disabled.
             *
             * @param  knockOut (OUT) 
             *         Boolean variable which is set to true when Knock out property is enabled.
             *
             * @return On success, FCM_SUCCESS is returned, else an error code is returned.
             */
            virtual FCM::Result _FCMCALL GetKnockout(FCM::Boolean& knockOut) = 0;

            
            /**
             * @brief  This function is used to determine whether innerShadow is enabled or disabled.
             *
             * @param  innerShadow (OUT) 
             *         Boolean variable which is set to true when innerShadow property is enabled.
             *
             * @return On success, FCM_SUCCESS is returned, else an error code is returned.
             */
            virtual FCM::Result _FCMCALL GetInnerShadow(FCM::Boolean& innerShadow) = 0;


            /**
             * @brief  This function is used to determine whether the hideObject property is 
             *         enabled or disabled.
             *
             * @param  hideObject (OUT) 
             *         Boolean variable is set to true when hideObject property is enabled.
             *
             * @return On success, FCM_SUCCESS is returned, else an error code is returned.
             */
            virtual FCM::Result _FCMCALL GetHideObject(FCM::Boolean& hideObject) = 0;


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
             * @brief  This function is used to determine whether or not the 
             *         filter is enabled for the object.
             *
             * @param  enabled (OUT) 
             *         Boolean value which is set to true when filter is enabled 
             *         for the object in context, else set to false.
             *
             * @return On success, FCM_SUCCESS is returned, else an error code is returned.
             */
            virtual FCM::Result _FCMCALL IsEnabled(FCM::Boolean& enabled) = 0;

        END_DECLARE_INTERFACE
    }
}


/* -------------------------------------------------- Inline / Functions */


#include "FCMPostConfig.h"

#endif // IDROP_SHADOW_FILTER_H_

