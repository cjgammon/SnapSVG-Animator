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
 * @file  IPublisher.h
 *
 * @brief This file contains the interface for IPublisher. The IPublisher 
 *        interface needs to be implemented by a "Publisher" plugin.
 *        It contains methods to publish a FLA document and a single timeline.
 *        FlashPro would invoke methods in this interface in the test-movie 
 *        and publish workflows.
 */

#ifndef IPUBLISHER_H_
#define IPUBLISHER_H_

#include "FCMPreConfig.h"
#include "FCMPluginInterface.h"

#include "FCMPreConfig.h"
#include "FCMTypes.h"
#include "FCMPluginInterface.h"
#include "IFCMDictionary.h"
#include "ITimeline.h"
#include "IFLADocument.h"
#include "Exporter/Service/IFrameCommandGenerator.h"


/* -------------------------------------------------- Forward Decl */


/* -------------------------------------------------- Macros / Constants */

namespace Publisher
{
    /**
    * @brief Defines the universally-unique Interface ID for IPublisher.
    *
    * @note Textual Representation: {3ACF3427-4FD-44EF-9FBA-3EBB62F044A2}
    */
    FCM::ConstFCMIID FCMIID_IPublisher =
        {0x3acf3427, 0x4fd, 0x44ef, {0x9f, 0xba, 0x3e, 0xbb, 0x62, 0xf0, 0x44, 0xa2}};
}


/* -------------------------------------------------- Enums */


/* -------------------------------------------------- Structs / Unions */


/* -------------------------------------------------- Class Decl */

namespace Publisher
{
    /**
     * @class IPublisher
     *
     * @brief Defines an interface that represents publisher. This interface
     *        must be implemented by a publisher plugin.
     */
    BEGIN_DECLARE_INTERFACE(IPublisher, FCMIID_IPublisher)

        /**
         * @brief  This function is "typically" called by FlashPro in the Publish/Test-Movie 
         *         workflow. The implementor of this function must publish the entire Fla Document
         *         on invocation of this function.
         *
         * @param  pFlaDocument (IN)
         *         Document to be published.
         *
         * @param  pDictPublishSettings (IN) 
         *         Dictionary containing values set in the publish settings dialog.
         *
         * @param  pDictConfig (IN) 
         *         Publish configuration parameters. These are controlled by 
         *         FlashPro and the plugins have no control over them. For e.g.,
         *           pDictConfig[kPublishSettingsKey_PreviewNeeded] = "true" or "false";
         *
         * @return On success, FCM_SUCCESS is returned, else an error code is returned.
         */
        virtual FCM::Result _FCMCALL Publish(
            DOM::PIFLADocument pFlaDocument, 
            const PIFCMDictionary pDictPublishSettings, 
            const PIFCMDictionary pDictConfig) = 0;
        
        
        /**
         * @brief  This function is "typically" called by FlashPro in the Test-Scene/Test-Movie 
         *         workflow. The implementor of this function must publish the timeline on invocation 
         *         of this function.
         *
         * @param  pFlaDocument (IN)
         *         Document containing the timeline.
         *
         * @param  pTimeline (IN)
         *         Timeline to be published. It may correspond to a movie-clip or
         *         a scene itself. This will be never be NULL.
         *
         * @param  frameRange (IN) 
         *         Range of frames to be published.
         *
         * @param  pDictPublishSettings (IN) 
         *         Dictionary containing values set in the publish settings dialog.
         *
         * @param  pDictConfig (IN) 
         *         Publish configuration parameters. These are controlled by 
         *         FlashPro and the plugins have no control over them. For e.g.,
         *           pDictConfig[kPublishSettingsKey_PreviewNeeded] = "true" or "false";
         *
         * @return On success, FCM_SUCCESS is returned, else an error code is returned.
         */
        virtual FCM::Result _FCMCALL Publish(
            DOM::PIFLADocument pFlaDocument, 
            DOM::PITimeline pTimeline, 
            const Exporter::Service::RANGE& frameRange, 
            const PIFCMDictionary pDictPublishSettings, 
            const PIFCMDictionary pDictConfig) = 0;
        

        /**
         * @brief  On invocation of this function, the publisher must clear its cache (if 
         *         it maintains one).
         *
         * @return On success, FCM_SUCCESS is returned, else an error code is returned.
         */
        virtual FCM::Result _FCMCALL ClearCache() = 0;

    END_DECLARE_INTERFACE
};


/* -------------------------------------------------- Inline / Functions */

#include "FCMPostConfig.h"

#endif //IPUBLISHER_H_

