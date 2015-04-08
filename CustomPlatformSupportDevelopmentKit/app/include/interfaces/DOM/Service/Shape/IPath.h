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
 * @file  IPath.h
 *
 * @brief This file contains the interface for IPath. IPath interface 
 *        represents a connected path.
 */

#ifndef IPATH_H_
#define IPATH_H_

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
             *        IPath.
             *
             * @note  Textual Representation: {FE5B89A9-9A9C-41E4-8818-E9A73B57D0DB}
             */
            FCM::ConstFCMIID IID_IPATH =
                {0xfe5b89a9, 0x9a9c, 0x41e4, {0x88, 0x18, 0xe9, 0xa7, 0x3b, 0x57, 0xd0, 0xdb}};
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
             * @class IPath
             *
             * @brief Defines the interface that represents a connected path.
             */
            BEGIN_DECLARE_INTERFACE(IPath, IID_IPATH)

                /**
                 * @brief  This function gets all the edges that defines a connected path. 
                 *
                 * @param  pEdgeList (OUT)
                 *         List of edges (IEdge) that define a connected path.
                 *
                 * @return On success, FCM_SUCCESS is returned, else an error code is returned.
                 *
                 * @see    DOM::Service::Shape::IEdge
                 */
                virtual FCM::Result _FCMCALL GetEdges(PIFCMList& pEdgeList) = 0;

            END_DECLARE_INTERFACE
        }
    }
}


/* -------------------------------------------------- Inline / Functions */


#include "FCMPostConfig.h"

#endif // IPATH_H_

