/******************************************************************************
* ADOBE CONFIDENTIAL
* ___________________
*
*  Copyright [2015] Adobe Systems Incorporated
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
 * @file  IButton.h
 *
 * @brief This file contains the interface for IButton. 
 *        IButton interface represents a "Button" instance on the 
 *        stage.
 */

#ifndef IBUTTON_INSTANCE_H_
#define IBUTTON_INSTANCE_H_

#include "FCMPreConfig.h"
#include "FCMPluginInterface.h"


/* -------------------------------------------------- Enums */

namespace DOM
{
    namespace FrameElement
    {
        /**
         * @enum ButtonTrackMode
         *
         * @brief Defines the different tracking option availabe in Flash for a button instance.
         */
        enum ButtonTrackMode
        {
            /** Track as button */
            TRACK_AS_BUTTON,
            
            /** Track as menu item */
            TRACK_AS_MENU_ITEM,
        };        
    }
}


/* -------------------------------------------------- Macros / Constants */

namespace DOM
{
    namespace FrameElement
    {
        /**
         * @brief Defines the universally-unique interface ID for 
         *        IButton
         *
         * @note  Textual Representation: {1E104164-6508-4CF9-B537-E5057A3F3C8B}
         */
        FCM::ConstFCMIID IID_IBUTTON_INSTANCE =
            {0x1e104164, 0x6508, 0x4cf9, {0xb5, 0x37, 0xe5, 0x05, 0x7a, 0x3f, 0x3c, 0x8b}};
    }
}


/* -------------------------------------------------- Structs / Unions */


/* -------------------------------------------------- Class Decl */

namespace DOM 
{
    namespace FrameElement
    {
        /**
         * @class IButton
         *
         * @brief Defines an interface that represents a "Button" instance  
         *        on the stage.
         */
        BEGIN_DECLARE_INTERFACE(IButton, IID_IBUTTON_INSTANCE)

            /**
             * @brief  This function gets the tracking option that is set for
             *         the button instance.
             *
             * @param  trackMode (OUT)
             *         Tracking option that is set for the button instance.
             *
             * @return Returns FCM_SUCCESS on success, else returns an error code.
             */
            virtual FCM::Result _FCMCALL GetTrackingMode(ButtonTrackMode& trackMode) = 0;

        END_DECLARE_INTERFACE
    }
}


/* -------------------------------------------------- Inline / Functions */


#include "FCMPostConfig.h"

#endif // IBUTTON_INSTANCE_H_

