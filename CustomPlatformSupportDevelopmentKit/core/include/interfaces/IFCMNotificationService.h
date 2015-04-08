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
 * @file  IFCMNotificationService.h
 *
 * @brief This file contains interface for IFCMNotificationService. 
 *        The IFCMNotificationService interface is supported by any FCM Application that wants to
 *        provide system wide notification hooks to plugins.  These notifications are mainly 
 *        provided by the application, but can also be provided by specific plugins. This Interface
 *        can be obtained by GetService of the main Application callback Interface.
 */

#ifndef IFCM_NOTIFICATION_SERVICE_H_
#define IFCM_NOTIFICATION_SERVICE_H_

#include "FCMPreConfig.h"
#include "FCMTypes.h"
#include "IFCMUnknown.h"
#include "IFCMNotificationClient.h"


/* -------------------------------------------------- Forward Decl */


/* -------------------------------------------------- Macros / Constants */

namespace FCM 
{
    /**
     * @brief Defines the universally-unique interface ID for IFCMNotificationService.
     *
     * @note  Textual Representation:  {4488009E-53CA-43EF-8890-50482CDE31AA}
     */
    FCM::ConstFCMIID FCMIID_IFCMNotificationService =
        {0x4488009e, 0x53ca, 0x43ef, {0x88, 0x90, 0x50, 0x48, 0x2c, 0xde, 0x31, 0xaa}};
}


/* -------------------------------------------------- Enums */


/* -------------------------------------------------- Structs / Unions */


/* -------------------------------------------------- Class Decl */

namespace FCM
{ 
    /**
     * @class IFCMNotificationService
     *
     * @brief Defines the interface that represents a notification service.
     */
    BEGIN_DECLARE_INTERFACE(IFCMNotificationService, FCMIID_IFCMNotificationService)

        /**
         * @brief  Registers a <tt>IFCMNotificationClient</tt> interface to receive notification  
         *         whenever a specific type of event occurs.  The type of event is specified by nid.
         *
         * @param  pClient (IN)
         *         The client to be notified when the event occurs.
         *
         * @param  nid (IN)
         *         The id for the event for which the the client wants to be notified for.
         *
         * @param  pRefCon (IN)
         *         A refcon pointer that will be passed to the client at the time of notification.
         *
         * @return FCM_SUCCESS is returned for success, else an error code is returned.
         */
         virtual FCM::Result _FCMCALL RegisterNotificationClient(
            PIFCMNotificationClient pClient,
            ConstRefFCMNotifyID nid,
            const FCM::PVoid pRefCon) = 0;


        /**
         * @brief  Unregisters an <tt>IFCMNotificationClient</tt> interface that was previously
         *         registered by a call to <tt>RegisterNotificationClient</tt>.  The type of the
         *         event to unregister for is specified by nid.  
         *
         * @param  pClient (IN)
         *         The client to be unregistered.
         *
         * @param  nid (IN)
         *         The id of the event to unregister for.
         *
         * @return FCM_SUCCESS is returned for success, else an error code is returned.
         */
         virtual FCM::Result _FCMCALL UnregisterNotificationClient(
             PIFCMNotificationClient pClient,
             ConstRefFCMNotifyID nid) = 0;


        /**
         * @brief  Sends a notification call to all <tt>IFCMNotificationClients</tt> registered for
         *         the event specified by nid. Any data passed in the pNotificationData parameter 
         *         will be passed on to the client interfaces in the pNotificationData parmaeter of
         *         the <tt>Notify</tt> method. 
         *
         * @param  pNotificationData (IN)
         *         This data will be passed on to all client interfaces.
         *
         * @param  nid (IN)
         *         The id for the event to be sent to all clients.
         *
         * @return FCM_SUCCESS is returned for success, else an error code is returned.
         */
         virtual FCM::Result _FCMCALL SendNotification(
             ConstRefFCMNotifyID nid,
             const FCM::PVoid pNotificationData) = 0;

    END_DECLARE_INTERFACE

};  // namespace FCM


/* -------------------------------------------------- Inline / Functions */


#include "FCMPostConfig.h"

#endif //IFCM_NOTIFICATION_SERVICE_H_