/*************************************************************************
* ADOBE CONFIDENTIAL
* ___________________
*
*  Copyright [2014] Adobe Systems Incorporated
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
 * @file  IFCMStringUtils.h
 *
 * @brief This file contains interface for IFCMStringUtils. 
 *        The IFCMStringUtils interface is supported by any FCM Application that wants to provide
 *        string utility functions to plug-ins. This Interface can be obtained by GetService of the
 *        main Application callback Interface.
 */

#ifndef IFCM_STRING_UTILS_H_
#define IFCM_STRING_UTILS_H_

#include "FCMPreConfig.h"
#include "FCMTypes.h"
#include "IFCMUnknown.h"


/* -------------------------------------------------- Forward Decl */


/* -------------------------------------------------- Macros / Constants */

namespace FCM 
{
    /**
     * @brief Defines the universally-unique interface ID for IFCMStringUtils.
     *
     * @note  Textual Representation:  {974EFCC9-B1E4-4BFA-8EA3-B9C41F2A450F}
     */
    FCM::ConstFCMIID FCMIID_IFCMStringUtils =
        {0x974efcc9, 0xb1e4, 0x4bfa, {0x8e, 0xa3, 0xb9, 0xc4, 0x1f, 0x2a, 0x45, 0xf}};
}


/* -------------------------------------------------- Enums */


/* -------------------------------------------------- Structs / Unions */


/* -------------------------------------------------- Class Decl */

namespace FCM
{
    /**
     * @class IFCMStringUtils
     *
     * @brief Defines the interface for string conversions between StringRep8 and StringRep16.
     */
    BEGIN_DECLARE_INTERFACE(IFCMStringUtils, FCMIID_IFCMStringUtils)

        /**
         * @brief  Converts a <tt>FCM::StringRep8</tt> to <tt>FCM::StringRep16</tt>.
         *
         * @param  pStrSrc (IN) 
         *         The source string pointer.  
         *
         * @param  pStrDest (OUT)
         *         The destination string pointer, which will be allocated and the content of the
         *         pStrSrc will be copied to it.
         *
         * @return FCM_SUCCESS is returned for success, else an error code is returned.
         */
        virtual FCM::Result _FCMCALL ConvertStringRep8to16(CStringRep8 pStrSrc,FCM::StringRep16& pStrDest)=0;


        /**
         * @brief  Converts a <tt>FCM::StringRep16</tt> to <tt>FCM::StringRep8</tt>.
         *
         * @param  pStrSrc (IN) 
         *         The source string pointer.  
         *
         * @param  pStrDest (OUT)
         *         The destination string pointer, which will be allocated and the content of the
         *         pStrSrc will be copied to it.
         *
         * @return FCM_SUCCESS is returned for success, else an error code is returned.
         */
        virtual FCM::Result _FCMCALL ConvertStringRep16to8(CStringRep16 pStrSrc,FCM::StringRep8& pStrDest)=0;

    END_DECLARE_INTERFACE

}; // namespace FCM


/* -------------------------------------------------- Inline / Functions */


#include "FCMPostConfig.h"

#endif //IFCM_STRING_UTILS_H_
