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
 * @file  ISymbolItem.h
 *
 * @brief This file contains interface for ISymbolItem. 
 *        This represents a symbol item in Library.
 */

#ifndef ISYMBOL_ITEM_H_
#define ISYMBOL_ITEM_H_

#include "FCMPreConfig.h"
#include "FCMPluginInterface.h"
#include "ITimeline.h"


/* -------------------------------------------------- Forward Decl */


/* -------------------------------------------------- Enums */


/* -------------------------------------------------- Macros / Constants */

namespace DOM
{
    namespace LibraryItem
    {
        /**
         * @brief Defines the Interface ID (which is universally unique) for 
         *        ISymbolItem.
         *
         * @note  Textual Representation: {596975D9-32F0-412D-8332-C5270CAF2CF3}
         */
        FCM::ConstFCMIID IID_ISYMBOL_ITEM =
            {0x596975d9, 0x32f0, 0x412d, {0x83, 0x32, 0xc5, 0x27, 0xc, 0xaf, 0x2c, 0xf3}};
    }
}


/* -------------------------------------------------- Structs / Unions */


/* -------------------------------------------------- Class Decl */

namespace DOM
{
    namespace LibraryItem
    {
        /**
        * @class ISymbolItem
        *
        * @brief This interface represents a symbol item.
        */
        BEGIN_DECLARE_INTERFACE(ISymbolItem, IID_ISYMBOL_ITEM)
        
            /**
             * @brief  This function returns timeline.
             *
             * @param  pTimeline (OUT)
             *         Timeline object.
             *
             * @return On success, FCM_SUCCESS is returned, else an error code is returned.
             */    
            virtual FCM::Result _FCMCALL GetTimeLine(PITimeline& pTimeline) = 0;
            
        END_DECLARE_INTERFACE
    }
}


/* -------------------------------------------------- Inline / Functions */


#include "FCMPostConfig.h"

#endif // ISYMBOL_ITEM_H_

