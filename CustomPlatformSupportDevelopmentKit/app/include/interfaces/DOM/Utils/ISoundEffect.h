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
 * @file  ISoundEffect.h
 *
 * @brief This file defines ISoundEffect that represents effects that can be 
 *        applied on a sound object.
 */

#ifndef ISOUND_EFFECT_H_
#define ISOUND_EFFECT_H_

#include "FCMPreConfig.h"
#include "FCMTypes.h"
#include "IFCMUnknown.h"
#include "DOMTypes.h"


/* -------------------------------------------------- Forward Decl */


/* -------------------------------------------------- Enums */


/* -------------------------------------------------- Macros / Constants */


namespace DOM
{
    namespace Utils
    {
        /**
         * @brief Defines the universally-unique Interface ID for ISoundEffect.
         *
         * @note  Textual Representation:  {DDF122A4-F880-4444-93BF-4E5B236AC90A}
         */
        FCM::ConstFCMIID IID_ISOUND_EFFECT =
            {0xddf122a4, 0xf880, 0x4444, {0x93, 0xbf, 0x4e, 0x5b, 0x23, 0x6a, 0xc9, 0xa}};
    }
}


/* -------------------------------------------------- Structs / Unions */

namespace DOM
{
    namespace Utils
    {
        /**
         * @struct ENVELOPE_POINT
         *
         * @brief  This structure defines a sound envelope.
         */
        struct ENVELOPE_POINT
        {
            /** 
             * Size of this structure. This must be set by the client/caller to 
             * sizeof(ENVELOPE_POINT).
             */
            FCM::U_Int32 structSize;

            /** 
             * Position of envelope point as a number of 44 kHz samples. 
             * Multiply accordingly if using a sampling rate less than 44 kHz.
             */ 
            FCM::S_Int32 pos44;

            /** 
             * For stereo, volumeLeft represents the left channel; whereas for mono, 
             * it represents the volume.
             */
            FCM::Double volumeLeft;

            /** 
             * For stereo, volumeRight represents the right channel; whereas for mono, 
             * it is not used.
             */
            FCM::Double volumeRight;
        };
    }
}


/* -------------------------------------------------- Class Decl */

namespace DOM
{
    namespace Utils
    {
        /**
         * @class ISoundEffect
         *
         * @brief ISoundEffect represents the effect that can be applied on a sound
         *        object. Sound Effect provides a "sound envelope" that allows 
         *        time based volume control.
         */
        BEGIN_DECLARE_INTERFACE(ISoundEffect, IID_ISOUND_EFFECT)

            /**
             * @brief  This function gets a "sound envelope".
             *
             * @param  soundEnvelope (OUT)
             *         Value of a sound envelope.
             *
             * @return On success, FCM_SUCCESS is returned; else an error code is 
             *         returned.
             *
             * @note   Caller of this function must set soundEnvelope.structSize to 
             *         sizeof(ENVELOPE_POINT).
             */
            virtual FCM::Result _FCMCALL GetEnvelopePoint(ENVELOPE_POINT& soundEnvelope) = 0;
            
        END_DECLARE_INTERFACE
    }
}


/* -------------------------------------------------- Inline / Functions */


#include "FCMPostConfig.h"

#endif // ISOUND_EFFECT_H_

