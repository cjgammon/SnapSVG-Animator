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
 * @file  IFrameDisplayElement.h
 *
 * @brief This file contains the interface for IFrameDisplayElement. The 
 *        IFrameDisplayElement interface represents anything that appears on the stage. 
 */

#ifndef IFRAME_DISPLAY_ELEMENT_H_
#define IFRAME_DISPLAY_ELEMENT_H_

#include "FCMPreConfig.h"
#include "FCMPluginInterface.h"
#include "Utils/DOMTypes.h"


/* -------------------------------------------------- Forward Decl */


/* -------------------------------------------------- Enums */


/* -------------------------------------------------- Macros / Constants */

namespace DOM 
{
    namespace FrameElement
    {
        /**
         * @brief Defines the universally-unique interface ID for 
         *        IFrameDisplayElement
         *
         * @note  Textual Representation: {800F6FF6-59C5-4FE3-ABBB-CEE26261A6B8}
         */
        FCM::ConstFCMIID IID_IFRAME_DISPLAY_ELEMENT =
            {0x800f6ff6, 0x59c5, 0x4fe3, {0xab, 0xbb, 0xce, 0xe2, 0x62, 0x61, 0xa6, 0xb8}};
    }
}


/* -------------------------------------------------- Structs / Unions */


/* -------------------------------------------------- Class Decl */

namespace DOM 
{
    namespace FrameElement
    {
        /**
         * @class IFrameDisplayElement
         *
         * @brief Defines the interface that represents a display item on the stage.
         */
        BEGIN_DECLARE_INTERFACE(IFrameDisplayElement, IID_IFRAME_DISPLAY_ELEMENT)

            /**
             * @brief  This function returns the depth of Display item in the frame.
             *
             * @param  depth (OUT)
             *         Depth of Display item.
             *
             * @return On success, FCM_SUCCESS is returned, else an error code is returned.
             */    
            virtual FCM::Result _FCMCALL GetDepth(FCM::U_Int32& depth) = 0;
        

            /**
             * @brief  This function returns the object space bounding rect of Display 
             *         item in the frame.
             *
             * @param  rect (OUT)
             *         Object space bounding rect of Display item.
             *
             * @return On success, FCM_SUCCESS is returned, else an error code is returned.
             */
            virtual FCM::Result _FCMCALL GetObjectSpaceBounds(Utils::RECT& rect) = 0;


            /**
             * @brief  This function returns the axis aligned bounding rect of Display 
             *         item in the frame.
             *
             * @param  rect (OUT)
             *         Bounding rect of Display item.
             *
             * @return On success, FCM_SUCCESS is returned, else an error code is returned.
             */
            virtual FCM::Result _FCMCALL GetBounds(Utils::RECT& rect) = 0;


            /**
             * @brief  This function returns the geometric matrix of Display item in the frame.
             *
             * @param  matrix (OUT)
             *         geometric matrix of Display item.
             *
             * @return On success, FCM_SUCCESS is returned, else an error code is returned.
             */
            virtual FCM::Result _FCMCALL GetMatrix(Utils::MATRIX2D& matrix) = 0;


            /**
             * @brief  This function returns the Pivot/Transformation Point of Display item 
             *         in the frame.
             *
             * @param  pivot (OUT)
             *         Pivot/Transformation Point of Display item.
             *
             * @return On success, FCM_SUCCESS is returned, else an error code is returned.
             */
            virtual FCM::Result _FCMCALL GetPivot(Utils::POINT2D& pivot) = 0;
        
        END_DECLARE_INTERFACE 
    }
}


/* -------------------------------------------------- Inline / Functions */


#include "FCMPostConfig.h"

#endif // IFRAME_DISPLAY_ELEMENT_H_

