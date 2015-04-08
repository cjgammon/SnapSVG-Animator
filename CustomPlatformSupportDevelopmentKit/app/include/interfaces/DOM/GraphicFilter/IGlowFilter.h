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
 * @file  IGlowFilter.h
 *
 * @brief This file contains interface for IGlowFilter. 
 *        This represents Glow Filter
 */

#ifndef IGLOW_FILTER_H_
#define IGLOW_FILTER_H_

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
         *        IGlowFilter.
         *
         * @note  Textual Representation:  {473CC4CD-CD5B-48D0-B34E-59513A708451}
         */
        FCM::ConstFCMIID IID_IGLOW_FILTER =
            {0x473cc4cd, 0xcd5b, 0x48d0, {0xb3, 0x4e, 0x59, 0x51, 0x3a, 0x70, 0x84, 0x51}};
    }
}


/* -------------------------------------------------- Structs / Unions */


/* -------------------------------------------------- Class Decl */

namespace DOM
{
    namespace GraphicFilter
    {
        /**
         * @class IGlowFilter
         *
         * @brief This interface defines the Glow filter applied to the object.
         */
        BEGIN_DECLARE_INTERFACE(IGlowFilter, IID_IGLOW_FILTER)

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
                 * @brief  This function is used to determine if knockout is enabled or disabled.
                 *
                 * @param  knockOut (OUT) 
                 *         Boolean variable which is set to true when Knock out property is enabled.
                 *
                 * @return On success, FCM_SUCCESS is returned, else an error code is returned.
                 */
                virtual FCM::Result _FCMCALL GetKnockout(FCM::Boolean& knockOut) = 0;

                
                /**
                 * @brief  This function is used to determine if innerShadow is enabled or disabled.
                 *
                 * @param  innerShadow (OUT) 
                 *         Boolean variable which is set to true when innerShadow property is enabled.
                 *
                 * @return On success, FCM_SUCCESS is returned, else an error code is returned.
                 */
                virtual FCM::Result _FCMCALL GetInnerShadow(FCM::Boolean& innerShadow) = 0;

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

#endif // IGLOW_FILTER_H_

