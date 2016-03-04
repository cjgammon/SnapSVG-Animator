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
 * @file  IRect.h
 *
 * @brief This file contains the interface for IRect. This interface 
 *        represents a rectangle.
 */

#ifndef IRECT_H_
#define IRECT_H_

#include "FCMPreConfig.h"
#include "FCMTypes.h"
#include "IFCMUnknown.h"
#include "DOMTypes.h"


/* -------------------------------------------------- Forward Decl */


/* -------------------------------------------------- Enums */


/* -------------------------------------------------- Macros / Constants */

namespace DOM
{
    namespace Utils
    {

        /**
         * @brief Defines the universally-unique Interface ID for IRect.
         *
         * @note  Textual Representation:  {340DA6FD-EDEF-4969-9E66-EE9D7FF13099}
         */
        FCM::ConstFCMIID IID_IRECT =
            {0x340da6fd, 0xedef, 0x4969, {0x9e, 0x66, 0xee, 0x9d, 0x7f, 0xf1, 0x30, 0x99}};
    }
}


/* -------------------------------------------------- Structs / Unions */


/* -------------------------------------------------- Class Decl */

namespace DOM
{
    namespace Utils
    {
        /**
         * @class IRect
         *
         * @brief This interface represents a rectangle.
         */
        BEGIN_DECLARE_INTERFACE(IRect, IID_IRECT)

            /**
             * @brief  This function gets the bounds of the rectangle.
             *
             * @param  rect (OUT)
             *         Rectangular bounds
             *
             * @return On success, FCM_SUCCESS is returned, else an error code is returned.
             */
            virtual FCM::Result _FCMCALL GetRect(RECT& rect) = 0;
            
        END_DECLARE_INTERFACE
    }
}


/* -------------------------------------------------- Inline / Functions */


#include "FCMPostConfig.h"

#endif // IRECT_H_

