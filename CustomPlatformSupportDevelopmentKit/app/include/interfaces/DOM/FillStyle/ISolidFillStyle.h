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
 * @file  ISolidFillStyle.h
 *
 * @brief This file contains the interface for ISolidFillStyle. The ISolidFillStyle 
 *        represents a "solid" fill style.
 */

#ifndef ISOLID_FILL_STYLE_H_
#define ISOLID_FILL_STYLE_H_

#include "FCMPreConfig.h"
#include "FCMPluginInterface.h"
#include "Utils/DOMTypes.h"


/* -------------------------------------------------- Forward Decl */


/* -------------------------------------------------- Enums */


/* -------------------------------------------------- Macros / Constants */

namespace DOM
{    
    namespace FillStyle
    {
        /**
         * @brief Defines the universally-unique interface ID  for 
         *        ISolidFillStyle
         *
         * @note  Textual Representation: {199BF9B9-35B4-4530-BAB2-AC94FE42478F}
         */
        FCM::ConstFCMIID IID_ISOLID_FILL_STYLE =
            {0x199bf9b9, 0x35b4, 0x4530, {0xba, 0xb2, 0xac, 0x94, 0xfe, 0x42, 0x47, 0x8f}};
    }
}


/* -------------------------------------------------- Structs / Unions */


/* -------------------------------------------------- Class Decl */

namespace DOM
{    
    namespace FillStyle
    {
        /**
         * @class ISolidFillStyle
         *
         * @brief Defines an interface that represents a "solid" fill style of a filled region.
         */
        BEGIN_DECLARE_INTERFACE(ISolidFillStyle, IID_ISOLID_FILL_STYLE)

            /**
             * @brief  This function gets the solid "color" used in the fill.
             *
             * @param  color (OUT)
             *         Color of the fill (in RGBA form)
             *
             * @return On success, FCM_SUCCESS is returned, else an error code is returned.
             */
            virtual FCM::Result _FCMCALL GetColor(Utils::COLOR& color) = 0;

        END_DECLARE_INTERFACE
        
    }
}


/* -------------------------------------------------- Inline / Functions */


#include "FCMPostConfig.h"

#endif // ISOLID_FILL_STYLE_H_

