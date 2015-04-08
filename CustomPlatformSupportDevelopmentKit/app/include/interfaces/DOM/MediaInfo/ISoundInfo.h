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
 * @file  ISoundInfo.h
 *
 * @brief This file contains the definition for ISoundInfo interface. This 
 *        interface represents the information related to a sound item.
 */

#ifndef ISOUND_INFO_H_
#define ISOUND_INFO_H_

#include "FCMPreConfig.h"
#include "FCMPluginInterface.h"
#include "Utils/DOMTypes.h"


/* -------------------------------------------------- Forward Decl */


/* -------------------------------------------------- Enums */


/* -------------------------------------------------- Macros / Constants */

namespace DOM
{
    namespace MediaInfo
    {
        /**
         * @brief Defines the universally-unique interface ID for 
         *        ISoundInfo
         *
         * @note  Textual Representation: {9FA0514D-45F6-462D-B2F4-0C541E54C457}
         */
        FCM::ConstFCMIID IID_ISOUND_INFO =
            {0x9fa0514d, 0x45f6, 0x462d, {0xb2, 0xf4, 0xc, 0x54, 0x1e, 0x54, 0xc4, 0x57}};
    }
}


/* -------------------------------------------------- Structs / Unions */


/* -------------------------------------------------- Class Decl */

namespace DOM
{
    namespace MediaInfo
    {
        /**
         * @class ISoundInfo
         *
         * @brief Marker interface that represents "sound information". 
         */
        BEGIN_DECLARE_INTERFACE(ISoundInfo, IID_ISOUND_INFO)

        
        END_DECLARE_INTERFACE
    }
};


/* -------------------------------------------------- Inline / Functions */


#include "FCMPostConfig.h"

#endif // ISOUND_INFO_H_

