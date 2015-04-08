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
 * @file  IRegionGeneratorService.h
 *
 * @brief This file contains interface for IRegionGeneratorService. IRegionGeneratorService 
 *        contains methods to generate filled regions and strokes for a shape.
 */

#ifndef IREGION_GENERATOR_SERVICE_H_
#define IREGION_GENERATOR_SERVICE_H_

#include "FCMPreConfig.h"
#include "FCMPluginInterface.h"


/* -------------------------------------------------- Forward Decl */

namespace FCM
{
    FORWARD_DECLARE_INTERFACE(IFCMList);
};


namespace DOM
{
    namespace FrameElement
    {
        FORWARD_DECLARE_INTERFACE(IShape);
    };
};


/* -------------------------------------------------- Enums */


/* -------------------------------------------------- Macros / Constants */

namespace DOM
{
    namespace Service
    {
        namespace Shape
        {
            /**
             * @brief Defines the universally-unique interface ID for 
             *        IRegionGeneratorService.
             *
             * @note  Textual Representation:  {EC97BC1D-0C7F-419E-836C-6997529671A9}
             */
            FCM::ConstFCMIID IID_IREGION_GENERATOR_SERVICE =
                {0xec97bc1d, 0xc7f, 0x419e, {0x83, 0x6c, 0x69, 0x97, 0x52, 0x96, 0x71, 0xa9}};
        }
    }
}


/* -------------------------------------------------- Structs / Unions */


/* -------------------------------------------------- Class Decl */

namespace DOM
{
    namespace Service
    {
        namespace Shape
        {
            /**
             * @class IRegionGeneratorService
             *
             * @brief Service to generate filled regions and strokes for a shape.
             */
            BEGIN_DECLARE_INTERFACE(IRegionGeneratorService, IID_IREGION_GENERATOR_SERVICE)

                /**
                 * @brief  Generates a list of filled regions.
                 *
                 * @param  pShape (IN)
                 *         Shape whose filled regions needs to be extracted.
                 *
                 * @param  pFilledRegionList (OUT)
                 *         List of filled regions (IFilledRegion). Each item in the list can be 
                 *         queried for IFilledRegion.
                 *
                 * @return On success, FCM_SUCCESS is returned, else an error code is returned.
                 *
                 * @see   DOM::Service::Shape::IFilledRegion
                 */
                virtual FCM::Result _FCMCALL GetFilledRegions(
                    DOM::FrameElement::PIShape pShape, 
                    PIFCMList& pFilledRegionList) = 0;


                /**
                 * @brief  This function returns a list of stroke grouped as per their style. 
                 *         The group list is ordered as per their rendering order.
                 *
                 * @param  pShape (IN)
                 *         Shape whose stroke list needs to be extracted.
                 *
                 * @param  pStrokeGroupList (OUT)
                 *         List of stroke group (IStrokeGroup). Each stroke group contains 
                 *         strokes with the same stroke style. Each item in the list can be 
                 *         queried for IStrokeGroup.
                 *
                 * @return On success, FCM_SUCCESS is returned, else an error code is returned.
                 *
                 * @see    DOM::Service::Shape::IStrokeGroup
                 */
                virtual FCM::Result _FCMCALL GetStrokeGroups(
                    DOM::FrameElement::PIShape pShape, 
                    PIFCMList& pStrokeGroupList) = 0;

            END_DECLARE_INTERFACE
        }
    }
}


/* -------------------------------------------------- Inline / Functions */


#include "FCMPostConfig.h"

#endif // IREGION_GENERATOR_SERVICE_H_

