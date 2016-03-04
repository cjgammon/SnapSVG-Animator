/******************************************************************************
* ADOBE CONFIDENTIAL
* ___________________
*
*  Copyright [2015] Adobe Systems Incorporated
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
 * @file  ITimelineBuilder2.h
 *
 * @brief This file contains interface for ITimelineBuilder2. 
 *        ITimelineBuilder2 represents a builder to build a timeline.
 */

#ifndef ITIMELINE_BUILDER_2_H_
#define ITIMELINE_BUILDER_2_H_

#include "FCMPreConfig.h"
#include "FCMPluginInterface.h"
#include "ITimelineBuilder.h"


/* -------------------------------------------------- Forward Decl */

/* -------------------------------------------------- Enums */


/* -------------------------------------------------- Macros / Constants */

namespace Exporter
{
    namespace Service
    {
        /**
         * @brief Defines the universally-unique interface ID for 
         *        ITimelineBuilder2.
         *
         * @note  Textual Representation:  {5C126281-A7C3-4515-AD55-068D5B7A5F56}
         */
        FCM::ConstFCMIID IID_ITIMELINE_BUILDER_2 =
            {0x5c126281, 0xa7c3, 0x4515, { 0xad, 0x55, 0x6, 0x8d, 0x5b, 0x7a, 0x5f, 0x56 }};
    }
}


/* -------------------------------------------------- Structs / Unions */


/* -------------------------------------------------- Class Decl */

namespace Exporter
{
    namespace Service
    {
        /**
         * @class ITimelineBuilder2
         *
         * @brief This interface represents a builder for a timeline. ITimelineBuilder2
         *        contains all the methods inherited from ITimelineBuilder and the new methods 
         *        mentioned in this file that are invoked by the FrameCommandGenerator 
         *        service (implemented in 'Adobe Animate CC') to add frame commands for 
         *        a timeline.
         *
         * @note  This interface is available in version 1.1.0 of the SDK and above.
         */
        BEGIN_DECLARE_INTERFACE_INHERIT(ITimelineBuilder2, IID_ITIMELINE_BUILDER_2, ITimelineBuilder)
            
            /**  
             * @brief  This function is invoked while updating the masking behavior of an object.
             *
             * @param  maskObjectId (IN)
             *         Object Identifier of the mask object
             *
             * @param  lowestMaskedObjectId (IN)
             *         Object Id of the lowest object that is masked by the mask object. The mask will   
             *         have effect on all the objects after the mask object till the object with the id
             *         lowestMaskedObjectId. If lowestMaskedObjectId is same as maskObjectId,
             *         then the mask has no effect.
             *
             * @return On success, FCM_SUCCESS is returned, else an error code is returned.
             */
            virtual FCM::Result _FCMCALL UpdateMask(
                FCM::U_Int32 maskObjectId, 
                FCM::U_Int32 lowestMaskedObjectId) = 0;
            
        END_DECLARE_INTERFACE

    }
}


/* -------------------------------------------------- Inline / Functions */


#include "FCMPostConfig.h"

#endif // ITIMELINE_BUILDER_2_H_

