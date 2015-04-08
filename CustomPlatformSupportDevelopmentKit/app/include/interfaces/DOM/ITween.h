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
 * @file  ITween.h
 *
 * @brief This interface represents a tween.
 */

#ifndef ITWEEN_H_
#define ITWEEN_H_

#include "FCMPreConfig.h"
#include "FCMPluginInterface.h"


/* -------------------------------------------------- Forward Decl */

namespace DOM
{
    FORWARD_DECLARE_INTERFACE(IFrame);
};


namespace FCM
{
    FORWARD_DECLARE_INTERFACE(IDictionary);
};


/* -------------------------------------------------- Macros / Constants */

namespace DOM
{
    /**
     * @brief Defines the universally-unique interface ID for ITween.
     *
     * @note  Textual Representation: {1308CEB5-52FB-44E0-854B-20BC694362DF}
     */
    FCM::ConstFCMIID IID_ITWEEN =
        {0x1308ceb5, 0x52fb, 0x44e0, {0x85, 0x4b, 0x20, 0xbc, 0x69, 0x43, 0x62, 0xdf}};
}


/* -------------------------------------------------- Structs / Unions */


/* -------------------------------------------------- Class Decl */

namespace DOM
{
    /**
    * @class ITween
    *
    * @brief Defines a interface that represents a tween object.
    */
    BEGIN_DECLARE_INTERFACE(ITween, IID_ITWEEN)
            
        /**
         * @brief  This function gets the tweened properties.
         * 
         * @param  pDictionary (OUT)
         *         Dictionary of the tweened properties. Each entry of a dictionary is 
         *         represented by a pair of <propertyName , propertyID>. The propertyName
         *         can take following values: kDOMShapeProperty, kDOMColorProperty,
         *         kDOMFilterProperty and kDOMGeometricProperty.
         *         For example, tweening of a color transform would contain the 
         *         following entry: <kDOMColorProperty, GUID>. 
         *
         * @return On success, FCM_SUCCESS is returned, else an error code is returned.
         *
         * @see    kDOMShapeProperty
         *
         * @see    kDOMColorProperty
         *
         * @see    kDOMFilterProperty 
         *
         * @see    kDOMGeometricProperty
         */
        virtual FCM::Result _FCMCALL GetTweenedProperties(FCM::PIFCMDictionary& pDictionary) = 0;

    END_DECLARE_INTERFACE
};


/* -------------------------------------------------- Inline / Functions */


#include "FCMPostConfig.h"

#endif // ITWEEN_H_

