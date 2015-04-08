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
 * @file  IBitmapInstance.h
 *
 * @brief This file contains the interface for IBitmapInstance. IBitmapInstance
 *        interface represents a bitmap on the stage.
 */

#ifndef IBITMAP_INSTANCE_H_
#define IBITMAP_INSTANCE_H_

#include "FCMPreConfig.h"
#include "FCMPluginInterface.h"
#include "IInstance.h"


/* -------------------------------------------------- Forward Decl */


/* -------------------------------------------------- Enums */


/* -------------------------------------------------- Macros / Constants */

namespace DOM 
{
    namespace FrameElement
    {
        /**
         * @brief Defines the universally-unique interface ID for 
         *        IBitmapInstance
         *
         * @note  Textual Representation: {E45B963E-A428-4D20-B91A-E8910684D8AB}
         */
        FCM::ConstFCMIID IID_IBITMAP_INSTANCE =
            {0xe45b963e, 0xa428, 0x4d20, {0xb9, 0x1a, 0xe8, 0x91, 0x6, 0x84, 0xd8, 0xab}};
    }
}


/* -------------------------------------------------- Structs / Unions */


/* -------------------------------------------------- Class Decl */

namespace DOM 
{
    namespace FrameElement
    {
        /**
         * @class IBitmapInstance
         *
         * @brief Defines an interface that represents a bitmap instance 
         *        present on the stage.
         */
        BEGIN_DECLARE_INTERFACE(IBitmapInstance, IID_IBITMAP_INSTANCE)

            
        END_DECLARE_INTERFACE
    }
}


/* -------------------------------------------------- Inline / Functions */

#include "FCMPostConfig.h"

#endif // IBITMAP_INSTANCE_H_

