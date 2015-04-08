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
 * @file  IAdjustColorFilter.h
 *
 * @brief This file contains interface for IAdjustColorFilter. 
 *        This represents Adjust Color Filter.
 */

#ifndef IADJUST_COLOR_FILTER_H_
#define IADJUST_COLOR_FILTER_H_

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
         *        IAdjustColorFilter.
         *
         * @note  Textual Representation:  {4398CC6B-E5DF-4B97-918D-F8EE937685A5}
         */
        FCM::ConstFCMIID IID_IADJUST_COLOR_FILTER =
            {0x4398cc6b, 0xe5df, 0x4b97, {0x91, 0x8d, 0xf8, 0xee, 0x93, 0x76, 0x85, 0xa5}};
    }
}


/* -------------------------------------------------- Structs / Unions */


/* -------------------------------------------------- Class Decl */

namespace DOM
{
    namespace GraphicFilter
    {

        /**
         * @class IAdjustColorFilter
         *
         * @brief This interface defines the properties that can modify the color 
         *        filter applied to the object.
         */
        BEGIN_DECLARE_INTERFACE(IAdjustColorFilter, IID_IADJUST_COLOR_FILTER)

            /**
             * @brief  This function is used to get the brightness value of the object to which 
             *         the Adjust Color filter is applied.
             *
             * @param  brightness (OUT) 
             *         Value of the brightness property for the object in context.
             *
             * @return On success, FCM_SUCCESS is returned, else an error code is returned.
             */
            virtual FCM::Result _FCMCALL GetBrightness(FCM::Double& brightness) = 0;


            /**
             * @brief  This function is used to get the contrast value of the object to which 
             *         the Adjust Color filter is applied.
             *
             * @param  contrast (OUT) 
             *         Value of the contrast property for the object in context.
             *
             * @return On success, FCM_SUCCESS is returned, else an error code is returned.
             */
            virtual FCM::Result _FCMCALL GetContrast(FCM::Double& contrast) = 0;


            /**
             * @brief  This function is used to get the saturation value of the object to which 
             *         the Adjust Color filter is applied.
             *
             * @param  saturation (OUT) 
             *         Value of the saturation property for the object in context.
             *
             * @return On success, FCM_SUCCESS is returned, else an error code is returned.
             */
            virtual FCM::Result _FCMCALL GetSaturation(FCM::Double& saturation) = 0;


            /**
             * @brief  This function is used to get the hue value of the object to which 
             *         the Adjust Color filter is applied.
             *
             * @param  hue (OUT) 
             *         Value of the hue property for the object in context.
             *
             * @return On success, FCM_SUCCESS is returned, else an error code is returned.
             */
            virtual FCM::Result _FCMCALL GetHue(FCM::Double& hue) = 0;

            
            /**
             * @brief  This function is used to get the color Matrix of the object.
             *
             * @param  colorMatrix (OUT) 
             *         A 2D matrix which defines the color filter applied.
             *
             * @return On success, FCM_SUCCESS is returned, else an error code is returned.
             */
            virtual FCM::Result _FCMCALL GetColorMatrix(Utils::COLOR_MATRIX& colorMatrix) = 0;


            /**
             * @brief  This function is used to determine whether or not the 
             *         filter is enabled for the object.
             *
             * @param  enabled (OUT) 
             *         Boolean value is set to true when the filter is enabled 
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

#endif // IADJUST_COLOR_FILTER_H_
