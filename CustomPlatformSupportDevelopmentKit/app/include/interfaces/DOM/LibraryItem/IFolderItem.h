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
 * @file  IFolderItem.h
 *
 * @brief This file contains the definition for IFolderItem interface. This 
 *        interface represents the information related to a folder item.
 */

#ifndef IFOLDER_ITEM_H_
#define IFOLDER_ITEM_H_

#include "FCMPreConfig.h"
#include "FCMPluginInterface.h"


/* -------------------------------------------------- Forward Decl */


/* -------------------------------------------------- Enums */


/* -------------------------------------------------- Macros / Constants */


namespace DOM
{
    namespace LibraryItem
    {
        /**
         * @brief Defines the universally-unique interface ID for 
         *        IFolderItem.
         *
         * @note  Textual Representation: {20733731-4EDF-4F5A-97C6-04FAA0E48C52}
         */
        FCM::ConstFCMIID IID_IFOLDER_ITEM =
            {0x20733731, 0x4edf, 0x4f5a, {0x97, 0xc6, 0x4, 0xfa, 0xa0, 0xe4, 0x8c, 0x52}};
    }
}


/* -------------------------------------------------- Structs / Unions */


/* -------------------------------------------------- Class Decl */

namespace DOM
{
    namespace LibraryItem
    {
        /**
        * @class IFolderItem
        *
        * @brief This interface represents the folder item in the library.
        */
        BEGIN_DECLARE_INTERFACE(IFolderItem, IID_IFOLDER_ITEM)

            /**
             * @brief  This function gets the children of the folder item in the library.
             *
             * @param  pChildList (OUT)
             *         List of items within the folder. Each of the item in the list can be 
             *         queried for ILibraryItem.
             *
             * @return On success, FCM_SUCCESS is returned, else an error code is returned.
             *
             * @see    DOM::ILibraryItem
             */
            virtual FCM::Result _FCMCALL GetChildren(PIFCMList& pChildList) = 0;
        
        END_DECLARE_INTERFACE

    }
};


/* -------------------------------------------------- Inline / Functions */


#include "FCMPostConfig.h"

#endif // IFOLDER_ITEM_H_

