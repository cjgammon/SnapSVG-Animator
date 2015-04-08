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
 * @file  IMovieClip.h
 *
 * @brief This file contains the interface for IMovieClip. 
 *        IMovieClip interface represents a "MovieClip" symbol on the 
 *        stage.
 */

#ifndef IMOVIE_CLIP_INSTANCE_H_
#define IMOVIE_CLIP_INSTANCE_H_

#include "FCMPreConfig.h"
#include "FCMPluginInterface.h"
#include "ISymbolInstance.h"


/* -------------------------------------------------- Forward Decl */

namespace DOM
{
    FORWARD_DECLARE_INTERFACE(IGraphicEffect);
};


/* -------------------------------------------------- Enums */

namespace DOM
{
    namespace FrameElement
    {
        /**
         * @enum BlendMode
         *
         * @brief Defines the different Blend modes available in Flash for the movieclip instance.
         */
        enum BlendMode
        {
            /** Normal */
            NORMAL_BLEND_MODE,

            /** Layer */
            LAYER_BLEND_MODE,

            /** Darken */
            DARKEN_BLEND_MODE,

            /** Multiply */
            MULTIPLY_BLEND_MODE,

            /** Lighten */
            LIGHTEN_BLEND_MODE,

            /** Screen */
            SCREEN_BLEND_MODE,

            /** Overlay */
            OVERLAY_BLEND_MODE,

            /** Hardlight */
            HARDLIGHT_BLEND_MODE,

            /** Add */
            ADD_BLEND_MODE,

            /** Substract */
            SUBTRACT_BLEND_MODE,

            /** Difference */
            DIFFERENCE_BLEND_MODE,

            /** Invert */
            INVERT_BLEND_MODE,

            /** Alpha */
            ALPHA_BLEND_MODE,

            /** Erase */
            ERASE_BLEND_MODE
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
         *        IMovieClip
         *
         * @note  Textual Representation: {80F09FF2-2894-4D72-BCE7-00E7EFEB683F}
         */
        FCM::ConstFCMIID IID_IMOVIE_CLIP_INSTANCE =
            {0x80f09ff2, 0x2894, 0x4d72, {0xbc, 0xe7, 0x0, 0xe7, 0xef, 0xeb, 0x68, 0x3f}};
    }
}


/* -------------------------------------------------- Structs / Unions */


/* -------------------------------------------------- Class Decl */

namespace DOM 
{
    namespace FrameElement
    {
        /**
         * @class IMovieClip
         *
         * @brief Defines an interface that represents an instance of a "movie-clip"
         *        symbol present on the stage.
         */
        BEGIN_DECLARE_INTERFACE(IMovieClip, IID_IMOVIE_CLIP_INSTANCE)

            /**
             * @brief  This function gets the blend mode that is applied to
             *         the movie clip instance.
             *
             * @param  blendMode (OUT)
             *         Blend mode that is applied to the movie clip instance.
             *
             * @return On success, FCM_SUCCESS is returned, else an error code is returned.
             */
            virtual FCM::Result _FCMCALL GetBlendMode(BlendMode& blendMode) = 0;


            /**
             * @brief  This function is invoked to check whether the visible property of the movie-clip
             *         is enabled or disabled.
             *
             * @param  isVisible (OUT)
             *         Boolean variable is set to "true" when the visible property of movie-clip
             *         is enabled, else set to false.
             *
             * @return On success, FCM_SUCCESS is returned, else an error code is returned.
             */
            virtual FCM::Result _FCMCALL IsVisible(FCM::Boolean& isVisible) = 0;


            /**
             * @brief  Returns the name of the Movie-clip instance.
             *
             * @param  ppName (OUT)
             *         Name pf the instance of movie-clip.
             *
             * @return On success, FCM_SUCCESS is returned, else an error code is returned.
             *
             * @note   The memory allocated for 'ppName' in this function must be freed 
             *         by the caller using IFCMCalloc::Free().
             */
            virtual FCM::Result _FCMCALL GetName(FCM::StringRep16* ppName) = 0;
            
        END_DECLARE_INTERFACE
    }
}


/* -------------------------------------------------- Inline / Functions */


#include "FCMPostConfig.h"

#endif // IMOVIE_CLIP_INSTANCE_H_

