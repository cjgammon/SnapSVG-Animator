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
 * @file  IPropertyIDs.h
 *
 * @brief This file contains the identifiers of properties that can be tweened. 
 */

#ifndef IPROPERTY_IDS_H_
#define IPROPERTY_IDS_H_

#include "FCMPreConfig.h"
#include "FCMPluginInterface.h"
#include "Utils/DOMTypes.h"


/* -------------------------------------------------- Forward Decl */


/* -------------------------------------------------- Enums */


/* -------------------------------------------------- Macros / Constants */

namespace DOM
{
    namespace Service
    {
        namespace Tween
        {
            /**
             * @brief Defines the property for "shape."
             */
            #define kDOMShapeProperty  "com.adobe.shape"


            /**
             * @var   SHAPE_PROPERTY_ID
             *
             * @brief Defines the property ID for "shape."
             */
            FCM::ConstFCMIID SHAPE_PROPERTY_ID =
                {0xf8dfae9, 0x2aa3, 0x49fa, {0x83, 0x25, 0xcb, 0x4e, 0x2, 0xca, 0xe, 0x88}};


            /**
             * @brief Defines the property for "geometry."
             */
            #define kDOMGeometricProperty  "com.adobe.geometry"


            /**
             * @var   GEOMETRIC_PROPERTY_ID
             *
             * @brief Defines ID for geometric property.
             */
            FCM::ConstFCMIID GEOMETRIC_PROPERTY_ID =
                {0x65b6b2bf, 0x359f, 0x4471, {0x99, 0x3, 0x74, 0x6f, 0xa6, 0x9, 0xce, 0x4f}};


            /**
             * @brief Defines the property for "color."
             */
            #define kDOMColorProperty  "com.adobe.color"


            /**
             * @var   COLOR_PROPERTY_ID
             *
             * @brief Defines ID for color property.
             */
            FCM::ConstFCMIID COLOR_PROPERTY_ID =
                {0x85fc70ac, 0xce7a, 0x4be3, {0x87, 0x5a, 0xb6, 0x20, 0xc9, 0x9c, 0xe0, 0x8d}};


            /**
             * @brief Defines the property for "filter."
             */
            #define kDOMFilterProperty  "com.adobe.filter"


            /**
             * @var   FILTER_PROPERTY_ID
             *
             * @brief Defines ID for filter property.
             */
            FCM::ConstFCMIID FILTER_PROPERTY_ID =
                {0xdfbf19f4, 0x650f, 0x4fdb, {0xa9, 0x6d, 0xf3, 0xee, 0x30, 0x9e, 0xeb, 0x7}};
        }
    }
}


/* -------------------------------------------------- Structs / Unions */


/* -------------------------------------------------- Class Decl */


/* -------------------------------------------------- Inline / Functions */


#include "FCMPostConfig.h"

#endif // IPROPERTY_IDS_H_

