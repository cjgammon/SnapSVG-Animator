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
 * @file  ITimelineBuilder.h
 *
 * @brief This file contains interface for ITimelineBuilder. 
 *        ITimelineBuilder represents a builder to build a timeline.
 */

#ifndef ITIMELINE_BUILDER_H_
#define ITIMELINE_BUILDER_H_

#include "FCMPreConfig.h"
#include "FCMPluginInterface.h"
#include "Utils/DOMTypes.h"
#include "FrameElement/IMovieClip.h"
#include "IFrame.h"


/* -------------------------------------------------- Forward Decl */

namespace DOM
{    
    namespace FrameElement
    {
        FORWARD_DECLARE_INTERFACE(IMovieClip);
        FORWARD_DECLARE_INTERFACE(ISound);
    }

    FORWARD_DECLARE_INTERFACE(ITween);
}


/* -------------------------------------------------- Enums */


/* -------------------------------------------------- Macros / Constants */

namespace Exporter
{
    namespace Service
    {
        /**
         * @brief Defines the universally-unique interface ID for 
         *        ITimelineBuilder.
         *
         * @note  Textual Representation:  {E1FA4E28-3A46-4106-BCDB-0F465B1FD462}
         */
        FCM::ConstFCMIID IID_ITIMELINE_BUILDER =
            {0xe1fa4e28, 0x3a46, 0x4106, {0xbc, 0xdb, 0xf, 0x46, 0x5b, 0x1f, 0xd4, 0x62}};
    }
}


/* -------------------------------------------------- Structs / Unions */

namespace Exporter
{
    namespace Service
    {
        /**
         * @struct SOUND_INFO
         *
         * @brief  This structure contains information about a sound object in the timeline. 
         */
        struct SOUND_INFO
        {
            /** Size of this structure. This will be set to sizeof(SOUND_INFO) */
            size_t                  structSize;

            /** Resource Identifier */
            FCM::U_Int32            resourceId;
        };


        /**
         * @struct DISPLAY_OBJECT_INFO
         *
         * @brief  This structure contains placement information about a display object 
         *         on the stage.
         */
        struct DISPLAY_OBJECT_INFO
        {
            /** Size of this structure. This will be set to sizeof(DISPLAY_OBJECT_INFO) */
            size_t                  structSize;

            /** Resource Identifier */
            FCM::U_Int32            resourceId;

            /** 
             * Object Id of the resource after which the current resource in context
             * to be added for a frame in the timeline.(Helps in Z ordering of the object)
             */
            FCM::U_Int32            placeAfterObjectId;
            
            /** Geometric transformation matrix */
            DOM::Utils::MATRIX2D    matrix;
        };
        
        /**
         * @struct DISPLAY_OBJECT_INFO_2
         *
         * @brief  This structure contains placement information about a display object
         *         on the stage, that are not present in DISPLAY_OBJECT_INFO
         *         Given a DISPLAY_OBJECT_INFO pointer pDisplayObjectInfo, access the fileds
         *         in this structure only if the condition
         *         (pDisplayObjectInfo->structSize >= sizeof(DISPLAY_OBJECT_INFO_2)) is true.
         */
        struct DISPLAY_OBJECT_INFO_2 : public DISPLAY_OBJECT_INFO
        {
            /** The bounds of the display object in its parent's co-ordinate space */
            DOM::Utils::RECT bounds;
        };

        /**
         * @typedef SHAPE_INFO
         *
         * @brief   This structure contains placement information about a shape object 
         *          on the stage.
         */
        typedef DISPLAY_OBJECT_INFO SHAPE_INFO;
        

        /**
         * @typedef BITMAP_INFO
         *
         * @brief   This structure contains placement information about a bitmap object 
         *          on the stage.
         */
        typedef DISPLAY_OBJECT_INFO BITMAP_INFO;


        /**
         * @typedef CLASSIC_TEXT_INFO
         *
         * @brief   This structure contains placement information about a text object 
         *          on the stage.
         */
        typedef DISPLAY_OBJECT_INFO CLASSIC_TEXT_INFO;


        /**
         * @typedef MOVIE_CLIP_INFO
         *
         * @brief   This structure contains placement information about a movie-clip object 
         *          on the stage.
         */
        typedef DISPLAY_OBJECT_INFO MOVIE_CLIP_INFO;


        /**
         * @typedef GRAPHIC_INFO
         *
         * @brief   This structure contains placement information about a graphic object 
         *          on the stage.
         */
        typedef DISPLAY_OBJECT_INFO GRAPHIC_INFO;

        /**
         * @typedef CLASSIC_TEXT_INFO_2
         *
         * @brief   This structure contains a few more placement information about a 
         *          text object in addition to containing all the the nformation that 
         *          is present in the structure CLASSIC_TEXT_INFO.
         */
        typedef DISPLAY_OBJECT_INFO_2 CLASSIC_TEXT_INFO_2;
    }
}


/* -------------------------------------------------- Class Decl */

namespace Exporter
{
    namespace Service
    {
        /**
         * @class ITimelineBuilder
         *
         * @brief This interface represents a builder for a timeline. Timeline builder
         *        contains methods that are invoked by FrameCommandGenerator service (implemented 
         *        in FlashPro) to add frame commands for a timeline.
         */
        BEGIN_DECLARE_INTERFACE(ITimelineBuilder, IID_ITIMELINE_BUILDER)

            /**  
             * @brief  This function is invoked to add a shape to the timeline.
             *
             * @param  objectId (IN)
             *         Object Identifier of the shape.
             *
             * @param  pShapeInfo (IN)
             *         Shape placement information.
             *
             * @return On success, FCM_SUCCESS is returned, else an error code is returned.
             */
            virtual FCM::Result _FCMCALL AddShape(
                FCM::U_Int32 objectId, 
                SHAPE_INFO* pShapeInfo) = 0;

            
            /**  
             * @brief  This function is invoked to add a text element to the timeline.
             *
             * @param  objectId (IN)
             *         Object Identifier of the text instance
             *
             * @param  pClassicTextInfo (IN)
             *         Text placement information
             *
             * @return On success, FCM_SUCCESS is returned, else an error code is returned.
             */
            virtual FCM::Result _FCMCALL AddClassicText(
                FCM::U_Int32 objectId, 
                CLASSIC_TEXT_INFO* pClassicTextInfo) = 0;
            
            
            /**  
             * @brief  This function is invoked to add a bitmap element to the timeline.
             *
             * @param  objectId (IN)
             *         Object Identifier of the bitmap
             *
             * @param  pBitmapInfo (IN)
             *         Bitmap placement information
             *
             * @return On success, FCM_SUCCESS is returned, else an error code is returned.
             */
            virtual FCM::Result _FCMCALL AddBitmap(
                FCM::U_Int32 objectId, 
                BITMAP_INFO* pBitmapInfo) = 0;

            
            /**  
             * @brief  This function is invoked to add a movie-clip element to the timeline.
             *
             * @param  objectId (IN)
             *         Object Identifier of the movie-clip.
             *
             * @param  pMovieClipInfo (IN)
             *         Movie-clip placement information.
             *
             * @param  pMovieClip (IN)
             *         Movie-clip object
             *
             * @return On success, FCM_SUCCESS is returned, else an error code is returned.
             */
            virtual FCM::Result _FCMCALL AddMovieClip(
                FCM::U_Int32 objectId, 
                MOVIE_CLIP_INFO* pMovieClipInfo, 
                DOM::FrameElement::PIMovieClip pMovieClip) = 0;
            
            
            /**  
             * @brief  This function is invoked to add a graphic element to the timeline.
             *
             * @param  objectId (IN)
             *         Object Identifier of the graphic.
             *
             * @param  pGraphicInfo (IN)
             *         Graphic placement information.
             *
             * @return On success, FCM_SUCCESS is returned, else an error code is returned.
             */
            virtual FCM::Result _FCMCALL AddGraphic(
                FCM::U_Int32 objectId, 
                GRAPHIC_INFO* pGraphicInfo) = 0;


            /**  
             * @brief  This function is invoked to add a sound element to the timeline.
             *
             * @param  objectId (IN)
             *         Object Identifier of the sound.
             *
             * @param  pSoundInfo (IN)
             *         Information about the sound item getting added to timeline.
             *
             * @param  pSound (IN)
             *         Sound object
             *
             * @return On success, FCM_SUCCESS is returned, else an error code is returned.
             */
            virtual FCM::Result _FCMCALL AddSound(
                FCM::U_Int32 objectId, 
                SOUND_INFO* pSoundInfo, 
                DOM::FrameElement::PISound pSound) = 0;

             
            /**  
             * @brief  This function is invoked when depth of object changes in any frame 
             *         as we traverse the frames in the timeline.
             *
             * @param  objectId (IN)
             *         Object Identifier
             *
             * @param  placeAfterObjectId (IN)
             *         Object Id of the resource after which the current resource in context
             *         to be added for a frame in the timeline. Helps in Z ordering of the object.
             *
             * @return On success, FCM_SUCCESS is returned, else an error code is returned.
             */
            virtual FCM::Result _FCMCALL UpdateZOrder(
                FCM::U_Int32 objectId, 
                FCM::U_Int32 placeAfterObjectId) = 0;


            /**  
             * @brief This function is invoked when the object is removed from a frame.
             *
             * @param objectId (IN)
             *        Object Identifier      
             *
             * @return On success, FCM_SUCCESS is returned, else an error code is returned.
             */
            virtual FCM::Result _FCMCALL Remove(FCM::U_Int32 objectId) = 0;


            /**  
             * @brief This function is invoked when the blend mode of the object changes in
             *        any frame.
             *
             * @param objectId (IN)
             *        Object Identifier
             *
             * @param blendMode (IN)
             *        Blend mode which needs to be applied to the resource.
             *
             * @return On success, FCM_SUCCESS is returned, else an error code is returned.
             */
            virtual FCM::Result _FCMCALL UpdateBlendMode(
                FCM::U_Int32 objectId, 
                DOM::FrameElement::BlendMode blendMode) = 0;


            /**  
             * @brief This function is invoked when visibility of the object changes in any frame. 
             *
             * @param objectId (IN)
             *        Object Identifier
             *
             * @param visible (IN)
             *        Boolean variable which determines whether or not the object should be visible.
             *
             * @return On success, FCM_SUCCESS is returned, else an error code is returned.
             */
            virtual FCM::Result _FCMCALL UpdateVisibility(
                FCM::U_Int32 objectId, 
                FCM::Boolean visible) = 0;


            /**  
             * @brief This function is invoked when the filter is applied or already applied filter for 
             *        the object changes in any frame.
             *
             * @param objectId (IN)
             *        Object Identifier
             *
             * @param pFilterable (IN)
             *        List of filter/s which needs to be applied to the resource.
             *
             * @return On success, FCM_SUCCESS is returned, else an error code is returned.
             */
            virtual FCM::Result _FCMCALL UpdateGraphicFilter(
                FCM::U_Int32 objectId, 
                PIFCMList pFilterable) = 0;

            
            /**  
             * @brief This function is invoked when transform matrix of the object changes in  
             *        any frame.
             *
             * @param objectId (IN)
             *        Object Identifier
             *
             * @param matrix (IN)
             *        A 2D matrix which defines the transform to be applied to the object.
             *
             * @return On success, FCM_SUCCESS is returned, else an error code is returned.
             */
            virtual FCM::Result _FCMCALL UpdateDisplayTransform(
                FCM::U_Int32 objectId, 
                const DOM::Utils::MATRIX2D& matrix) = 0;


            /**  
             * @brief This function is invoked when color transform matrix of the object 
             *        changes in any frame.
             *
             * @param objectId (IN)
             *        Object Identifier
             *
             * @param colorMatrix (IN)
             *        A matrix which defines the color transform to be applied to the object.
             *
             * @return On success, FCM_SUCCESS is returned, else an error code is returned.
             */
            virtual FCM::Result _FCMCALL UpdateColorTransform(
                FCM::U_Int32 objectId, 
                const DOM::Utils::COLOR_MATRIX& colorMatrix) = 0;


             /**  
             * @brief This function is invoked for every frame when all the associated 
             *        frame commands for the frame are executed. This "ShowFrame" command indicates 
             *        that a frame is complete and can now be rendered during playback.
             *
             * @return On success, FCM_SUCCESS is returned; otherwise an error code is returned.
             */
            virtual FCM::Result _FCMCALL ShowFrame() = 0;  


            /**  
             * @brief  This function is invoked for a frame which has frame script associated with it.
             *         This function can be called multiple times between two consecutive calls to
             *         ShowFrame(). Number of times this function is called depends on the number of 
             *         layers that has frame scripts.
             *
             * @param  pScript (IN)
             *         Script associated with the frame in context.
             *
             * @param  layerNum (IN)
             *         Layer number in which the frame script resides. This will be passed back in RemoveFrameScript().
             *
             * @return On success, FCM_SUCCESS is returned, else an error code is returned.
             *
             * @note   Caller of this function owns "pScript" and is responsible for deleting it.
             */
            virtual FCM::Result _FCMCALL AddFrameScript(FCM::CStringRep16 pScript, FCM::U_Int32 layerNum) = 0;


            /**  
             * @brief This function is invoked to remove frame script associated with a frame.
             *
             * @param  layerNum (IN)
             *         Layer number in which the frame script resided and now needs to be removed.
             *
             * @return On success, FCM_SUCCESS is returned; otherwise an error code is returned.
             */
            virtual FCM::Result _FCMCALL RemoveFrameScript(FCM::U_Int32 layerNum) = 0;

            
            /**  
             * @brief  This function is invoked for a frame which has frame label associated with it.
             *         This function can be called multiple times between two consecutive calls to
             *         ShowFrame(). 
             *
             * @param  pLabel (IN)
             *         Name of the label
             *
             * @param  labelType (IN)
             *         Type of label. This can be either KEY_FRAME_LABEL_NAME or KEY_FRAME_LABEL_ANCHOR.
             *
             * @return On success, FCM_SUCCESS is returned; otherwise an error code is returned.
             *
             * @note   Caller of this function owns "pLabel" and is responsible for deleting it.
             */
            virtual FCM::Result _FCMCALL SetFrameLabel(FCM::StringRep16 pLabel, DOM::KeyFrameLabelType labelType) = 0;

        END_DECLARE_INTERFACE

    }
}


/* -------------------------------------------------- Inline / Functions */


#include "FCMPostConfig.h"

#endif // ITIMELINE_BUILDER_H_

