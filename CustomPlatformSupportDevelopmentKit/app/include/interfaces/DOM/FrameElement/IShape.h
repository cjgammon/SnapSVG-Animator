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
 * @file  IShape.h
 *
 * @brief This file contains the interface for IShape. IShape interface 
 *        can be used in shape services to extract the geometry of the shape.
 */

#ifndef ISHAPE_H_
#define ISHAPE_H_

#include "FCMPreConfig.h"
#include "FCMPluginInterface.h"
#include "FrameElement/IFrameDisplayElement.h"


/* -------------------------------------------------- Forward Decl */

namespace FCM
{
    FORWARD_DECLARE_INTERFACE(IFCMList);
}


/* -------------------------------------------------- Enums */


/* -------------------------------------------------- Macros / Constants */

namespace DOM 
{
    namespace FrameElement
    {
        /**
         * @brief Defines the universally-unique interface ID for 
         *        IShape
         *
         * @note  Textual Representation: {2F540DEF-C6DB-45A2-A254-7128AE2D8A1E}
         */
        FCM::ConstFCMIID IID_ISHAPE =
            {0x2f540def, 0xc6db, 0x45a2, {0xa2, 0x54, 0x71, 0x28, 0xae, 0x2d, 0x8a, 0x1e}};
    }
}


/* -------------------------------------------------- Structs / Unions */


/* -------------------------------------------------- Class Decl */

namespace DOM 
{
    namespace FrameElement
    {
        /**
         * @class IShape
         *
         * @brief Defines an interface that represents a shape.
         */
        BEGIN_DECLARE_INTERFACE(IShape, IID_ISHAPE)

        END_DECLARE_INTERFACE
    }
}


/* -------------------------------------------------- Inline / Functions */


#include "FCMPostConfig.h"

#endif // ISHAPE_H_

