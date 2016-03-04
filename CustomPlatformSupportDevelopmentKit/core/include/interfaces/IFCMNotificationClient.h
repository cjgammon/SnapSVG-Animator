/*************************************************************************
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
**************************************************************************/

/**
 * @file  IFCMNotificationClient.h
 *
 * @brief This file contains interface for IFCMNotificationClient. 
 *        The IFCMNotificationClient interface must be supported by any plugin that wishes to
 *        receive notification of specific certain event by either the host app, or by other 
 *        plugins that want to generate system-wide events.  A Plugin wanting notification will
 *        obtain an <tt>IMoaNotificationClient</tt> Interface to itself and an 
 *        <tt>IFCMNotificationService</tt> interface from the main callback object of the 
 *        application. It then registers the client interface with the callback interface.
*/ 

#ifndef IFCM_NOTIFICATION_CLIENT_H_
#define IFCM_NOTIFICATION_CLIENT_H_

#include "FCMPreConfig.h"
#include "FCMTypes.h"
#include "IFCMUnknown.h"


/* -------------------------------------------------- Forward Decl */


/* -------------------------------------------------- Macros / Constants */

namespace FCM 
{
    /**
     * @brief Defines the universally-unique interface ID for IFCMNotificationClient.
     *
     * @note  Textual Representation:  {44BA2FFC-2057-4C95-AA38-80D6EA007336}
     */
    FCM::ConstFCMIID FCMIID_IFCMNotificationClient =
        {0x44ba2ffc, 0x2057, 0x4c95, {0xaa, 0x38, 0x80, 0xd6, 0xea, 0x0, 0x73, 0x36}};
}


/* -------------------------------------------------- Enums */


/* -------------------------------------------------- Structs / Unions */


/* -------------------------------------------------- Class Decl */

namespace FCM
{ 
    /**
     * @class IFCMNotificationClient
     *
     * @brief Defines the interface that represents the notification client.
     */
    BEGIN_DECLARE_INTERFACE(IFCMNotificationClient, FCMIID_IFCMNotificationClient)

        /**
         * @brief  The host application calls the Notify method of a registered Notification Client
         *         whenever the event type that was passed in at the time of registration occurs.
         *         For each event type, additional data is passed in the pNotificationData
         *         parameter that further describes the event that occurred.
         *
         * @param  nid (IN)
         *         The type of the event that occurred.
         *
         * @param  pNotificationData (IN)
         *         Event-type specific data describing the event that occurred.
         *
         * @param  pRefCon (IN)
         *         The refcon value that was passed when the client was registered for this event.
         *
         * @return Return FCM_SUCCESS on success, else an error code.
         */
         virtual FCM::Result _FCMCALL Notify(
            ConstRefFCMNotifyID nid,
            FCM::PVoid pNotificationData,
            FCM::PVoid pRefCon) = 0;

    END_DECLARE_INTERFACE

};  // namespace FCM


/* -------------------------------------------------- Inline / Functions */


#include "FCMPostConfig.h"

#endif //IFCM_NOTIFICATION_CLIENT_H_