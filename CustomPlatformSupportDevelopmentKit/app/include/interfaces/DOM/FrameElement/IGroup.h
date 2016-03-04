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
 * @file  IGroup.h
 *
 * @brief This file contains the interface for IGroup. This represents
 *        a "Group" of frame display elements.
 */

#ifndef IGROUP_H_
#define IGROUP_H_

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
         * @brief Defines the Interface ID (which is universally unique) for 
         *        IGroup.
         *
         * @note  Textual Representation: {E43E72E4-B425-46A4-9123-F3363AA59A37}
         */
        FCM::ConstFCMIID IID_IGROUP =
            {0xe43e72e4, 0xb425, 0x46a4, {0x91, 0x23, 0xf3, 0x36, 0x3a, 0xa5, 0x9a, 0x37}};
    }
}


/* -------------------------------------------------- Structs / Unions */


/* -------------------------------------------------- Class Decl */

namespace DOM 
{
    namespace FrameElement
    {
        /**
         * @class IGroup
         *
         * @brief  Defines an interface that represents a Group.
         */
        BEGIN_DECLARE_INTERFACE(IGroup, IID_IGROUP)

             /**
              * @brief  This function returns an array of FrameDisplayElements in Group.
              *         The raw shapes in the group will be returned as part of
              *         an array of FrameDisplayElements.
              *
              * @param  pMembers (OUT)
              *         List of FrameDisplayElements. Each item in the list can be queried 
              *         for IFrameDisplayElement.
              *
              * @return On success, FCM_SUCCESS is returned, else an error code is returned.
              *
              * @see    DOM::FrameElement::IFrameDisplayElement
              */
            virtual FCM::Result _FCMCALL GetMembers(PIFCMList& pMembers) = 0;

        END_DECLARE_INTERFACE
    }
}


/* -------------------------------------------------- Inline / Functions */


#include "FCMPostConfig.h"

#endif // IGROUP_H_

