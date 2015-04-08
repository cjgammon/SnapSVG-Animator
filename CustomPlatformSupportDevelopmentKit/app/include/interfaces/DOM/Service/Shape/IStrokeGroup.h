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
 * @file  IStrokeGroup.h
 *
 * @brief This file contains interface for IStrokeGroup. IStrokeGroup represents
 *        a group of strokes that have the same stroke style.
 */

#ifndef ISTROKE_GROUP_H_
#define ISTROKE_GROUP_H_

#include "FCMPreConfig.h"
#include "FCMPluginInterface.h"


/* -------------------------------------------------- Forward Decl */

namespace FCM
{
    FORWARD_DECLARE_INTERFACE(IFCMList);
}



/* -------------------------------------------------- Enums */


/* -------------------------------------------------- Macros / Constants */

namespace DOM
{
    namespace Service
    {
        namespace Shape
        {
            /**
             * @brief Defines the universally-unique interface ID for 
             *        IStrokeGroup.
             *
             * @note  Textual Representation:  {56820F87-EF8A-485B-B27A-B484E19FC762}
             */
            FCM::ConstFCMIID IID_ISTROKE_GROUP =
                {0x56820f87, 0xef8a, 0x485b, {0xb2, 0x7a, 0xb4, 0x84, 0xe1, 0x9f, 0xc7, 0x62}};
        }
    }
}


/* -------------------------------------------------- Structs / Unions */


/* -------------------------------------------------- Class Decl */

namespace DOM
{
    namespace Service
    {
        namespace Shape
        {
            /**
             * @class IStrokeGroup
             *
             * @brief Defines a stroke group that represents a group of strokes that have the same
             *        stroke style. 
             */
            BEGIN_DECLARE_INTERFACE(IStrokeGroup, IID_ISTROKE_GROUP)

                /**
                 * @brief  This functions gets the stroke style of the group of edges.
                 *
                 * @param  pStrokeStyle (OUT)
                 *         Stroke style of the stroke group. This can be queried for the following
                 *         interfaces: IDashedStrokeStyle, IDottedStrokeStyle, IHatchedStrokeStyle,
                 *         IRaggedStrokeStyle, ISolidStrokeStyle and IStippleStrokeStyle.
                 *
                 * @return On success, FCM_SUCCESS is returned, else an error code is returned.
                 *
                 * @see    DOM::StrokeStyle::IDashedStrokeStyle
                 *
                 * @see    DOM::StrokeStyle::IDottedStrokeStyle
                 *
                 * @see    DOM::StrokeStyle::IHatchedStrokeStyle
                 *
                 * @see    DOM::StrokeStyle::IRaggedStrokeStyle
                 *
                 * @see    DOM::StrokeStyle::ISolidStrokeStyle
                 *
                 * @see    DOM::StrokeStyle::IStippleStrokeStyle
                 */
                virtual FCM::Result _FCMCALL GetStrokeStyle(FCM::PIFCMUnknown& pStrokeStyle) = 0;


                /**
                 * @brief  This function returns a stroke list. The list is ordered as per their rendering 
                 *         order.
                 *
                 * @param  pPathList
                 *         Path - List of paths (IPath) having the same stroke style. This list is ordered 
                 *         as per the rendering order with the first in the list being the one 
                 *         that needs to be rendered first.
                 *
                 * @return On success, FCM_SUCCESS is returned, else an error code is returned.
                 *
                 * @see    DOM::Service::Shape::IPath
                 */
                virtual FCM::Result _FCMCALL GetPaths(PIFCMList& pPathList) = 0;

            END_DECLARE_INTERFACE
        }
    }
}


/* -------------------------------------------------- Inline / Functions */


#include "FCMPostConfig.h"

#endif // ISTROKE_GROUP_H_

