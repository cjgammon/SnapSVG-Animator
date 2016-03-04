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
 * @file  ISound.h
 *
 * @brief This file contains the interface for ISound. The ISound
 *        interface represents a sound object attached to a frame. 
 */

#ifndef ISOUND_ELEMENT_H_
#define ISOUND_ELEMENT_H_

#include "FCMPreConfig.h"
#include "FCMPluginInterface.h"
#include "Utils/DOMTypes.h"


/* -------------------------------------------------- Forward Decl */

namespace DOM
{
    namespace Utils
    {
        FORWARD_DECLARE_INTERFACE(ISoundEffect);
    }

    namespace LibraryItem
    {
        FORWARD_DECLARE_INTERFACE(IMediaItem);
    }
}


/* -------------------------------------------------- Enums */

namespace DOM
{
    namespace FrameElement
    {
        /**
         * @enum  SoundRepeatMode
         *
         * @brief This enum defines the repetition mode of a sound.
         */
        enum SoundRepeatMode
        {
            /** Sound is looped infinitely */
            SOUND_REPEAT_INFINITE,


            /** Sound is looped for a finite number of times */
            SOUND_REPEAT_FINITE
        }; 


        /**
         * @enum  SoundSyncMode
         *
         * @brief This enum defines sync mode of a sound.
         */
        enum SoundSyncMode
        {
            /**
             * Synchronizes the sound to the occurrence of an event. An event 
             * sound plays when its starting keyframe first appears and  
             * plays in its entirety, independently of the playhead in the
             * Timeline. An event sound continues playing until explicitly 
             * stopped.
             *
             * If an event sound is playing and the sound is instantiated 
             * again (for example, by the user clicking a button again, or 
             * the playhead passing the starting keyframe of the sound), the 
             * first instance of the sound continues to play and another 
             * instance of the same sound begins to play simultaneously. 
             */
            SOUND_SYNC_EVENT,


            /**
             * Same as SOUND_SYNC_EVENT, except that if the sound is already 
             * being played, no new instance of the sound is played.
             */
            SOUND_SYNC_START,


            /** Silences the specified sound. */
            SOUND_SYNC_STOP,
            

            /**
             * Stream sounds begin playing as soon as enough data for the 
             * first few frames has been downloaded; stream sounds are 
             * synchronized to the Timeline. 
             *
             * An example of a stream sound is the voice of a character in 
             * an animation that plays in multiple frames.
             */
            SOUND_SYNC_STREAM
        };
    }
}


/* -------------------------------------------------- Macros / Constants */

namespace DOM
{
    namespace FrameElement
    {
        /**
         * @brief Defines the universally-unique interface ID for 
         *        ISound.
         *
         * @note  Textual Representation: {EF9DFF11-1B84-41BA-AC7F-C2560F692F30}
         */
        FCM::ConstFCMIID IID_ISOUND_ELEMENT =
            {0xef9dff11, 0x1b84, 0x41ba, {0xac, 0x7f, 0xc2, 0x56, 0xf, 0x69, 0x2f, 0x30}};
    }
}


/* -------------------------------------------------- Structs / Unions */

namespace DOM
{
    namespace FrameElement
    {
        /**
         * @struct SOUND_LOOP_MODE
         *
         * @brief  Defines the loop mode for a sound.
         */
        struct SOUND_LOOP_MODE
        {
            /** 
             * Size of this structure. This must be set by the client/caller to 
             * sizeof(SOUND_LOOP_MODE).
             */
            size_t structSize;


            /** Repeat mode */
            SoundRepeatMode loopMode;


            /** 
             * This is valid only if loopMode is set to SOUND_REPEAT_FINITE. 
             * Defines the number of times the loop will be repeated.
             */
            FCM::U_Int16 repeatCount;
        };


        /**
         * @struct SOUND_LIMIT
         *
         * @brief  Defines the starting and ending points of the sound object. 
         */
        struct SOUND_LIMIT
        {
            /**
             * Size of this structure. This must be set by the client/caller to
             * sizeof(SOUND_LIMIT).
             */
            size_t structSize;
            

            /** Number of samples to skip at beginning of sound. */
            FCM::S_Int32 inPos44;


            /** Position in samples of last sample to play. */
            FCM::S_Int32 outPos44;
        };
    }
}

/* -------------------------------------------------- Class Decl */


namespace DOM 
{
    namespace FrameElement
    {
        /**
         * @class ISound
         *
         * @brief Defines the interface that represents a sound item attached to a frame.
         */
        BEGIN_DECLARE_INTERFACE(ISound, IID_ISOUND_ELEMENT)

            /**
             * @brief  This function gets the sound item in the library that is used to 
             *         instantiate the sound object.
             *
             * @param  pSoundItem (OUT)
             *         Sound Library Item
             *
             * @return On success, FCM_SUCCESS is returned, else an error code
             *         is returned.
             */
            virtual FCM::Result _FCMCALL GetSoundItem(DOM::LibraryItem::PIMediaItem& pSoundItem) = 0;


            /**
             * @brief  This function gets the list of sound effects.
             *
             * @param  pISoundEffects (OUT)
             *         List of sound effects. Each item in the list can be queried for ISoundEffect.
             *
             * @return On success, FCM_SUCCESS is returned, else an error code
             *         is returned.
             *
             * @see    DOM::Utils::ISoundEffect
             */
            virtual FCM::Result _FCMCALL GetSoundEffect(PIFCMList& pISoundEffects) = 0;


            /**
             * @brief  This function gets the loop mode of a sound.
             *
             * @param  loopMode (OUT)
             *         Loop mode
             *
             * @return On success, FCM_SUCCESS is returned, else an error code
             *         is returned.
             *
             * @note   Caller of this function must set loopMode.structSize to 
             *         sizeof(SOUND_LOOP_MODE).
             */
            virtual FCM::Result _FCMCALL GetLoopMode(SOUND_LOOP_MODE& loopMode) = 0;


            /**
             * @brief  This function gets the sync mode of a sound.
             *
             * @param  syncMode (OUT)
             *         Sync mode
             *
             * @return On success, FCM_SUCCESS is returned, else an error code
             *         is returned.
             */
            virtual FCM::Result _FCMCALL GetSyncMode(SoundSyncMode& syncMode) = 0;


            /**
             * @brief  This function gets the limits of the sound object.
             *
             * @param  soundLimit (OUT)
             *         Sound limit
             *
             * @return On success, FCM_SUCCESS is returned, else an error code
             *         is returned.
             *
             * @note   Caller of this function must set soundLimit.structSize to 
             *         sizeof(SOUND_LIMIT).
             */            
            virtual FCM::Result _FCMCALL GetSoundLimit(SOUND_LIMIT& soundLimit) = 0;

        END_DECLARE_INTERFACE 
    }
}


/* -------------------------------------------------- Inline / Functions */


#include "FCMPostConfig.h"

#endif // ISOUND_ELEMENT_H_

