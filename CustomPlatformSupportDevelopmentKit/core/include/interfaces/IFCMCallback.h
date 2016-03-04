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
 * @file  IFCMCallback.h
 *
 * @brief This file contains interface for IFCMCallback. 
 *        The IFCMCallback interface is a glue for the objects created to access the FCM. Each object
 *        of an interface implementation contains pointer IFCMCallback object, which can be used
 *        to create a new object of any classID and interfaceID and also can be used to register
 *        a service which can be used by FCM Application or Plug-in. By default, the IFCMCalloc
 *        and IFCMNotificationService are registered with IFCMCallback by FCM Application.
 */

#ifndef IFCM_CALLBACK_H_
#define IFCM_CALLBACK_H_

#include "FCMPreConfig.h"
#include "FCMTypes.h"
#include "IFCMUnknown.h"


/* -------------------------------------------------- Forward Decl */


/* -------------------------------------------------- Macros / Constants */

namespace FCM 
{
    /**
     * @brief Defines the universally-unique interface ID for IFCMCallback.
     *
     * @note  Textual Representation:  {DB438EAB-F457-491D-8EF2-A1B867707E3E}
     */
    FCM::ConstFCMIID FCMIID_IFCMCallback =
        {0xdb438eab, 0xf457, 0x491d, {0x8e, 0xf2, 0xa1, 0xb8, 0x67, 0x70, 0x7e, 0x3e}};
}


/* -------------------------------------------------- Enums */


/* -------------------------------------------------- Structs / Unions */


/* -------------------------------------------------- Class Decl */

namespace FCM 
{
    /**
     * @class IFCMCallback
     *
     * @brief Defines the interface that represents the callback.
     */
    BEGIN_DECLARE_INTERFACE(IFCMCallback, FCMIID_IFCMCallback)

        /**
         * @brief  Creates a single object of the class associated with the specified classID.
         *
         * @param  pUnkOuter (IN) 
         *         If NULL, indicates that the object is not being created as part of  
         *         an aggregate.If non-NULL, pointer to the aggregate object's IUnknown interface.
         * 
         * @param  classID (IN)
         *         The CLSID associated with the data and code that will be used to
         *         create the object.
         *
         * @param  interfaceID (IN)
         *         A reference to the identifier of the interface to be used to
         *         communicate with the object.
         *
         * @param  ppvObj (OUT)
         *         Address of pointer variable that receives the interface pointer 
         *         requested in interfaceID.Upon successful return, *ppvObj contains the
         *         requested interface pointer, which the caller should release after the use.
         *         Upon failure, *ppvObj contains NULL.
         *
         * @return FCM_SUCCESS is returned for success, else an error code is returned.
         */
        virtual FCM::Result _FCMCALL CreateInstance(
            PIFCMUnknown pUnkOuter,
            ConstRefFCMCLSID classID,
            ConstRefFCMIID interfaceID,
            FCM::PPVoid ppvObj) = 0;


        /**
         * @brief  Gets the object implementing a service with its serviceId.
         *
         * @param  serviceId (IN)
         *         Service ID associated with the service.
         * 
         * @param  serviceRef (OUT)
         *         The service object, which the caller should release after the use.
         *
         * @return FCM_SUCCESS is returned for success, else an error code is returned.
         */
        virtual FCM::Result _FCMCALL GetService(
            const FCM::SRVCID& serviceId,
            PIFCMUnknown& serviceRef) = 0;

    END_DECLARE_INTERFACE

}; // namespace FCM


/* -------------------------------------------------- Inline / Functions */


#include "FCMPostConfig.h"

#endif //IFCM_CALLBACK_H_