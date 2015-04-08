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
 * @file  IStrokeWidth.h
 *
 * @brief This file contains the interface for IStrokeWidth. This represents the
 *        stroke width associated with the stroke.
 */

#ifndef ISTROKE_WIDTH_H_
#define ISTROKE_WIDTH_H_

#include "FCMPreConfig.h"
#include "FCMPluginInterface.h"
#include "FCMTypes.h"
#include "Utils/DOMTypes.h"


/* -------------------------------------------------- Forward Decl */


/* -------------------------------------------------- Enums */


/* -------------------------------------------------- Macros / Constants */

namespace DOM
{
    namespace StrokeStyle
    {
        /**
         * @brief Defines the universally-unique interface ID for 
         *        IStrokeWidth.
         *
         * @note  Textual Representation: {7F877445-E2B3-4E39-A25F-294A22ED98D3}
         */
        FCM::ConstFCMIID IID_ISTROKE_WIDTH =
            {0x7f877445, 0xe2b3, 0x4e39, {0xa2, 0x5f, 0x29, 0x4a, 0x22, 0xed, 0x98, 0xd3}};
    }
}


/* -------------------------------------------------- Structs / Unions */


/* -------------------------------------------------- Class Decl */

namespace DOM
{
    namespace StrokeStyle
    {
        /**
         * @class IStrokeWidth
         *
         * @brief Defines an interface that represents a stroke width associated with the stroke.
         */
        BEGIN_DECLARE_INTERFACE(IStrokeWidth, IID_ISTROKE_WIDTH)


        END_DECLARE_INTERFACE
    }
}


/* -------------------------------------------------- Inline / Functions */


#include "FCMPostConfig.h"

#endif // ISTROKE_WIDTH_H_

