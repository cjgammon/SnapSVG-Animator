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
 * @file  IFrame.h
 *
 * @brief This file contains the interface for IFrame. The IFrame
 *        interface represents a frame in the layer of a timeline. 
 */

#ifndef IFRAME_H_
#define IFRAME_H_

#include "FCMPreConfig.h"
#include "FCMPluginInterface.h"


/* -------------------------------------------------- Forward Decl */

namespace FCM
{
    FORWARD_DECLARE_INTERFACE(IFCMList);
};

namespace DOM
{
    FORWARD_DECLARE_INTERFACE(ITween);
}


/* -------------------------------------------------- Enums */

namespace DOM
{
    /**
     * @enum  KeyFrameLabelType
     *
     * @brief Defines the types of frames.
     */
    enum KeyFrameLabelType
    {
        /** No key frame label */
        KEY_FRAME_LABEL_NONE,

        /** Name of the frame */
        KEY_FRAME_LABEL_NAME,

        /** Comment for a frame */
        KEY_FRAME_LABEL_COMMENT,

        /** Anchor for a frame */
        KEY_FRAME_LABEL_ANCHOR
    };
}


/* -------------------------------------------------- Macros / Constants */

namespace DOM
{
    /**
     * @brief Defines the universally-unique interface ID for 
     *        IFrame.
     *
     * @note  Textual Representation: {386CB02E-74D0-4114-9E10-CBE8E8D164A7}
     */
    FCM::ConstFCMIID IID_IFRAME =
        {0x386cb02e, 0x74d0, 0x4114, {0x9e, 0x10, 0xcb, 0xe8, 0xe8, 0xd1, 0x64, 0xa7}};
}


/* -------------------------------------------------- Class Decl */

namespace DOM 
{

    /**
     * @class IFrame
     *
     * @brief Defines the interface that represents a frame in a layer in the 
     *        timeline.
     */
    BEGIN_DECLARE_INTERFACE(IFrame, IID_IFRAME)

        /**
         * @brief  This function gets the number of frames in a frame sequence.
         *
         * @param  duration (OUT)
         *         Number of frames in a frame sequence.
         *
         * @return On success, FCM_SUCCESS is returned, else an error code is returned.
         */    
        virtual FCM::Result _FCMCALL GetDuration(FCM::U_Int32& duration) = 0;

    
        /**
         * @brief  This function returns an array of FrameElements. This will contain the Sound
         *         and the FrameDisplayElements, which are items on stage. The order of 
         *         FrameDisplayElements is the order from the topChild to bottomChild
         *         in the frame. If there are multiple shapes on the Stage, and each is 
         *         ungrouped, they are treated as one FrameDisplayElement. If each shape is 
         *         grouped, so there are multiple groups on the Stage, they are 
         *         treated as separate FrameDisplayElements. In other words, raw, ungrouped 
         *         shapes are treated as a single FrameDisplayElement, regardless of how many 
         *         separate shapes are on the Stage. For example, if a frame contains three 
         *         raw, ungrouped shapes, then the number of FrameDisplayElements in 
         *         the list will be 1. 
         *
         * @param  pFrameElementsList (OUT)
         *         List of Frame elements. Each object in the list can be queried depending 
         *         on its type: 
         *            Sound - ISound
         *            Shape - IFrameDisplayElement, IShape
         *            Group - IFrameDisplayElement, IGroup
         *            Text  - IFrameDisplayElement, IClassicText
         *            Bitmap - IFrameDisplayElement, IBitmapInstance
         *            Graphic - IFrameDisplayElement, IInstance, ISymbolInstance, IGraphic
         *            Movie Clip - IFrameDisplayElement, IInstance, ISymbolInstance, IMovieClip
         *
         * @return On success, FCM_SUCCESS is returned, else an error code is returned.
         *
         * @see    DOM::FrameElement::IFrameDisplayElement
         *
         * @see    DOM::FrameElement::ISound
         *
         * @see    DOM::FrameElement::IShape
         *
         * @see    DOM::FrameElement::IGroup
         *
         * @see    DOM::FrameElement::IClassicText
         *
         * @see    DOM::FrameElement::IBitmapInstance
         *
         * @see    DOM::FrameElement::IInstance
         *
         * @see    DOM::FrameElement::ISymbolInstance
         *
         * @see    DOM::FrameElement::IGraphic
         *
         * @see    DOM::FrameElement::IMovieClip
         */    
        virtual FCM::Result _FCMCALL GetFrameElements(PIFCMList& pFrameElementsList) = 0;


        /**
         * @brief  This function returns an array of FrameElements of type IID_ISound, and
         *         IID_IFrameDisplayElements.
         *
         * @param  iid (IN)
         *         Interface ID, will work with IID_ISound, and IID_IFrameDisplayElements.
         *
         * @param  pFrameElementsList (OUT)
         *         List of Frame elements. Each object in the list can be queried depending 
         *         on its type: 
         *            Sound - ISound
         *            Shape - IFrameDisplayElement, IShape
         *            Group - IFrameDisplayElement, IGroup
         *            Text  - IFrameDisplayElement, IClassicText
         *            Bitmap - IFrameDisplayElement, IBitmapInstance
         *            Graphic - IFrameDisplayElement, IInstance, ISymbolInstance, IGraphic
         *            Movie Clip - IFrameDisplayElement, IInstance, ISymbolInstance, IMovieClip
         *
         * @return On success, FCM_SUCCESS is returned, else an error code is returned.
         */
        virtual FCM::Result _FCMCALL GetFrameElementsByType(
            ConstFCMIID& iid, 
            PIFCMList& pFrameElementsList) = 0; 


        /**
         * @brief  This function gets the start frame index of the frame span.
         *
         * @param  frameIndex (OUT)
         *         Start frame index of the frame span.
         *
         * @return On success, FCM_SUCCESS is returned, else an error code is returned.
         *
         * @note   For example, let a frame, span from frame number 6 to 10 and
         *         this function will return 6 for any frame within this span.
         */    
        virtual FCM::Result _FCMCALL GetStartFrameIndex(FCM::U_Int32& frameIndex) = 0;


        /**
         * @brief  This function returns the label for the frame.
         *
         * @param  ppLabel (OUT)
         *         Label of the frame.
         *
         * @return On success, FCM_SUCCESS is returned, else an error code is returned.
         *
         * @note   The memory allocated for 'ppLabel' in this function must be freed 
         *         by the caller using IFCMCalloc::Free().
         */
        virtual FCM::Result _FCMCALL GetLabel(FCM::StringRep16* ppLabel) = 0;


        /**
         * @brief  This function gets the type of label on the frame.
         *
         * @param  labelType (OUT)
         *         Type of label.
         *
         * @return On success, FCM_SUCCESS is returned, else an error code is returned.
         */    
        virtual FCM::Result _FCMCALL GetLabelType(KeyFrameLabelType& labelType) = 0;
        

        /**
         * @brief  This function gets a string that represents the frame script 
         *         associated with this frame.
         *
         * @param  ppScript (OUT)
         *         Frame script
         *
         * @return On success, FCM_SUCCESS is returned, else an error code is returned.
         *
         * @note   The memory allocated for 'ppScript' in this function must be freed 
         *         by the caller using IFCMCalloc::Free().
         */    
        virtual FCM::Result _FCMCALL GetScript(FCM::StringRep16* ppScript) = 0;
    
    
        /**
         * @brief  The function returns a tween object associated with this frame span. 
         *
         * @param  pTween (OUT)
         *         ITween object
         *
         * @return On success, FCM_SUCCESS is returned, else an error code is returned.
         */
        virtual FCM::Result _FCMCALL GetTween(PITween& pTween) = 0;

    END_DECLARE_INTERFACE 
};


/* -------------------------------------------------- Inline / Functions */


#include "FCMPostConfig.h"

#endif // IFRAME_H_

