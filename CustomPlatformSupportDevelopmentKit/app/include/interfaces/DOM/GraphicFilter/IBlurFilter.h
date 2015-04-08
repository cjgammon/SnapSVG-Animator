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
 * @file  IBlurFilter.h
 *
 * @brief This file contains interface for IBlurFilter. 
 *        This represents Blur Filter.
 */

#ifndef IBLUR_FILTER_H_
#define IBLUR_FILTER_H_

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
         *        IBlurFilter.
         *
         * @note  Textual Representation:  {B3C424E6-9F46-4FB3-889A-103AD516DA21}
         */
        FCM::ConstFCMIID IID_IBLUR_FILTER =
            {0xb3c424e6, 0x9f46, 0x4fb3, {0x88, 0x9a, 0x10, 0x3a, 0xd5, 0x16, 0xda, 0x21}};
    }
}


/* -------------------------------------------------- Structs / Unions */


/* -------------------------------------------------- Class Decl */

namespace DOM
{
    namespace GraphicFilter
    {
        /**
         * @class IBlurFilter
         *
         * @brief This interface defines the Blur filter applied to the object.
         */
        BEGIN_DECLARE_INTERFACE(IBlurFilter, IID_IBLUR_FILTER)

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
             * @brief  This function is used to determine whether or not the 
             *         filter is enabled for the object.
             *
             * @param  enabled (OUT) 
             *         Boolean value is set to true when filter is enabled 
             *         for the object in context, else it is set to false.
             *
             * @return On success, FCM_SUCCESS is returned, else an error code is returned.
             */
            virtual FCM::Result _FCMCALL IsEnabled(FCM::Boolean& enabled) = 0;
            
        END_DECLARE_INTERFACE
    }
}


/* -------------------------------------------------- Inline / Functions */


#include "FCMPostConfig.h"

#endif // IBLUR_FILTER_H_

