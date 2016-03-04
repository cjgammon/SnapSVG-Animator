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
 * @file  IFrameCommandGenerator.h
 *
 * @brief This file contains interface for IFrameCommandGenerator. 
 *        IFrameCommandGenerator provides service related to lifetime management
 *        of the frame objects. 
 */

#ifndef IFRAME_COMMAND_GENERATOR_H_
#define IFRAME_COMMAND_GENERATOR_H_

#include "FCMPreConfig.h"
#include "FCMPluginInterface.h"
#include "Utils/DOMTypes.h"


/* -------------------------------------------------- Forward Decl */

namespace DOM
{
    FORWARD_DECLARE_INTERFACE(ITimeline);
}


namespace Exporter
{
    namespace Service
    {
        FORWARD_DECLARE_INTERFACE(IResourcePalette);
        FORWARD_DECLARE_INTERFACE(ITimelineBuilderFactory);
        FORWARD_DECLARE_INTERFACE(ITimelineBuilder);
    }
}

/* -------------------------------------------------- Enums */


/* -------------------------------------------------- Macros / Constants */

namespace Exporter
{
    namespace Service
    {
        /**
         * @brief Defines the universally-unique interface ID for 
         *        IFrameCommandGenerator.
         *
         * @note  Textual Representation:  {440D57C4-E12C-4AAE-9A68-D3611191FA3E}
         */
        FCM::ConstFCMIID IID_IFRAME_COMMAND_GENERATOR =
            {0x440d57c4, 0xe12c, 0x4aae, {0x9a, 0x68, 0xd3, 0x61, 0x11, 0x91, 0xfa, 0x3e}};
    }
}


/* -------------------------------------------------- Structs / Unions */

namespace Exporter
{
    namespace Service
    {
        /**
         * @struct RANGE
         *
         * @brief  Defines a range of numbers.
         */
        struct RANGE
        {
            /** Minimum value of the range */
            FCM::U_Int32 min;

            /** Maximum value of the range */
            FCM::U_Int32 max;
        };
    }
}


/* -------------------------------------------------- Class Decl */

namespace Exporter
{
    namespace Service
    {
        /**
         * @class IFrameCommandGenerator
         *
         * @brief Defines an interface that generates frame commands for the defined 
         *        frame range.
         */
        BEGIN_DECLARE_INTERFACE(IFrameCommandGenerator, IID_IFRAME_COMMAND_GENERATOR)

            /**  
             * @brief This function generates frame commands for the specified frame range.
             *
             * @param pTimeline (IN)
             *        Timeline object for which frame commands need to be generated.
             *
             * @param frameRange (IN)
             *        Range of frames.
             *
             * @param pDictPublishSettings (IN)
             *        Dictionary containing publish settings. Only setting key that is recognized   
             *        by this service is Exporter::Service::kPublishSettingsKey_IncludeInvisibleLayer.
             *        If this key is not set or an invalid value is set, default value of "true" 
             *        is used. All other settings in the dictionary are ignored.
             *
             * @param pResourcePalette (IN)
             *        Resource palette object. Definition of resources are added to 
             *        the resource palette.
             *
             * @param pTimelineBuilderFactory (IN)
             *        Factory interface to build timeline for each symbol (movie-clip/graphic).
             *        This interface is invoked every time a symbol is encountered during 
             *        DOM parsing.
             *
             * @param pTimelineBuilder (OUT)
             *        Root(or main) timeline builder.
             *
             * @return On success, FCM_SUCCESS is returned, else an error code is returned.
             */
            virtual FCM::Result _FCMCALL GenerateFrameCommands(
                DOM::PITimeline pTimeline, 
                const RANGE& frameRange, 
                const PIFCMDictionary pDictPublishSettings, 
                PIResourcePalette pResourcePalette, 
                PITimelineBuilderFactory pTimelineBuilderFactory, 
                PITimelineBuilder& pTimelineBuilder) = 0;

        END_DECLARE_INTERFACE 
    }
}


/* -------------------------------------------------- Inline / Functions */


#include "FCMPostConfig.h"

#endif // IFRAME_COMMAND_GENERATOR_H_

