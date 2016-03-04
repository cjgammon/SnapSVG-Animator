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
 * @file  IFCMNoAggregationUnknown.h
 *
 * @brief This file contains interface for IFCMNoAggregationUnknown and 
 *        needed to support "Aggregation".
 *        This interface is used by FCM internally. It is not meant to 
 *        be used by external clients.
 */

#ifndef IFCM_NO_AGGREGATION_UNKNOWN_H_
#define IFCM_NO_AGGREGATION_UNKNOWN_H_

#include "FCMPreConfig.h"
#include "FCMTypes.h"
#include "FCMErrors.h"


/* -------------------------------------------------- Forward Decl */


/* -------------------------------------------------- Macros / Constants */

namespace FCM 
{
    /**
     * @brief Defines the universally-unique interface ID for IFCMNoAggregationUnknown.
     *
     * @note  Textual Representation:  {F04D68CF-9429-4393-980A-A2215F050A17}
     */
    FCM::ConstFCMIID FCMIID_IFCMNoAggregationUnknown =
        {0xf04d68cf, 0x9429, 0x4393, {0x98, 0xa, 0xa2, 0x21, 0x5f, 0x5, 0xa, 0x17}};
}


/* -------------------------------------------------- Enums */


/* -------------------------------------------------- Structs / Unions */


/* -------------------------------------------------- Class Decl */

namespace FCM 
{
    /**
     * @class IFCMNoAggregationUnknown
     *
     * @brief Defines the interface that represents the NoAggregationUnknown.
     */
    BEGIN_DECLARE_BASE_INTERFACE(IFCMNoAggregationUnknown,FCMIID_IFCMNoAggregationUnknown)
        
        /**
         * @brief  Return pointers to supported interfaces on given object.
         *
         * @param  pInterfaceID (IN)
         *         Pointer to identifier interface.
         *
         * @param  ppvObj (OUT)
         *         pointer to requested interface object. 
         *
         * @return FCM_SUCCESS is returned for success, else an error code is returned.
         */
        virtual FCM::Result _FCMCALL MainQueryInterface(ConstRefFCMIID pInterfaceID, FCM::PPVoid ppvObj)=0;


        /**
         * @brief  Decrements the reference count on the given object.
         *
         * @return Returns the new reference count.
         */
        virtual FCM::U_Int32 _FCMCALL NoAggregationAddRef() = 0;
        
        
        /**
         * @brief  Decrements the reference count on the given object.
         *
         * @return Returns the new reference count.
         */
        virtual FCM::U_Int32 _FCMCALL NoAggregationRelease() = 0;

    END_DECLARE_INTERFACE

};  // namespace FCM


/* -------------------------------------------------- Inline / Functions */


#include "FCMPostConfig.h"

#endif //IFCM_NO_AGGREGATION_UNKNOWN_H_