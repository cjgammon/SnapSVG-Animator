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
 * @file  ITextBehaviour.h
 *
 * @brief This file contains the interface for ITextBehaviour. ITextElement interface 
 *        represents the type of the text element on the stage. 
 */

#ifndef ITEXT_BEHAVIOUR_H_
#define ITEXT_BEHAVIOUR_H_

#include "FCMPreConfig.h"
#include "FCMPluginInterface.h"
#include "IFCMUnknown.h"


/* -------------------------------------------------- Forward Decl */


/* -------------------------------------------------- Enums */

namespace DOM
{
    namespace FrameElement
    {
        /**
         * @enum  OrientationMode
         *
         * @brief This defines orientation mode of the static text element.
         */
        enum OrientationMode
        {
            /** Horizontal orientation */
            ORIENTATION_MODE_HORIZONTAL,
                

            /** Vertical orientation */
            ORIENTATION_MODE_VERTICAL
        };

            
        /**
         * @enum  TextFlow
         *
         * @brief This defines static text flow.
         */
        enum TextFlow
        {
            /** Left to right */
            TEXT_FLOW_LEFT_TO_RIGHT,
                

            /** Right to left */
            TEXT_FLOW_RIGHT_TO_LEFT
        };
        

       /**
        * @enum  LineMode
        *
        * @brief This defines the paragraph behavior mode of the dynamic/input text element.
        */
        enum LineMode
        {
            /** single line */
            LINE_MODE_SINGLE,
                

            /** multi line */
            LINE_MODE_MULTI,
                

            /** multi-line with no wrap */
            LINE_MODE_MULTI_NO_WRAP
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
         *        ITextBehaviour.
         *
         * @note  Textual Representation: {CD4A280-95E3-4476-A064-41984A589433}
         */
        FCM::ConstFCMIID IID_ITEXT_BEHAVIOUR =
            {0xcd4a280, 0x95e3, 0x4476, {0xa0, 0x64, 0x41, 0x98, 0x4a, 0x58, 0x94, 0x33}};


        /**
         * @brief Defines the universally-unique interface ID for 
         *        IStaticTextBehaviour.
         *
         * @note  Textual Representation: {6327C2D6-B763-4AC9-8FBD-F4C29134D17C}
         */
        FCM::ConstFCMIID IID_ISTATIC_TEXT_BEHAVIOUR =
                {0x6327c2d6, 0xb763, 0x4ac9, {0x8f, 0xbd, 0xf4, 0xc2, 0x91, 0x34, 0xd1, 0x7c}};


        /**
         * @brief Defines the universally-unique interface ID for 
         *        ITextBehaviour.
         *
         * @note  Textual Representation: {6499D11F-6AF1-4403-BDB1-760214F39D95}
         */
        FCM::ConstFCMIID IID_IMODIFIABLE_TEXT_BEHAVIOUR =
            {0x6499d11f, 0x6af1, 0x4403, {0xbd, 0xb1, 0x76, 0x2, 0x14, 0xf3, 0x9d, 0x95}};


        /**
         * @brief Defines the universally-unique interface ID for 
         *        IDynamicTextBehaviour.
         *
         * @note  Textual Representation: {94655A95-990B-4540-915B-7052933231F8}
         */
        FCM::ConstFCMIID IID_IDYNAMIC_TEXT_BEHAVIOUR =
            {0x94655a95, 0x990b, 0x4540, {0x91, 0x5b, 0x70, 0x52, 0x93, 0x32, 0x31, 0xf8}};


        /**
         * @brief Defines the universally-unique interface ID for 
         *        IInputTextBehaviour.
         *
         * @note  Textual Representation: {9D1E6460-990F-4D81-BB24-8D9159CD96F9}
         */
        FCM::ConstFCMIID IID_IINPUT_TEXT_BEHAVIOUR =
                {0x9d1e6460, 0x990f, 0x4d81, {0xbb, 0x24, 0x8d, 0x91, 0x59, 0xcd, 0x96, 0xf9}};            
        
    }
}


/* -------------------------------------------------- Structs / Unions */


/* -------------------------------------------------- Class Decl */

namespace DOM 
{
    namespace FrameElement
    {
        /**
         * @class ITextBehaviour
         *
         * @brief Defines an interface that represents type of the text element 
         *        present on the stage.
         */
        BEGIN_DECLARE_INTERFACE(ITextBehaviour, IID_ITEXT_BEHAVIOUR)

             /**
              * @brief  This function indicates whether the text of text element is selectable.
              *
              * @param  selectable (OUT)
              *         returns true if the text of text element is selectable.
              *
              * @return On success, FCM_SUCCESS is returned, else an error code is returned. 
              */
             virtual FCM::Result _FCMCALL IsSelectable(FCM::Boolean& selectable) = 0;
            
        END_DECLARE_INTERFACE 
            

        /**
         * @class IStaticTextBehaviour
         *
         * @brief Defines an interface that represents the static text type of the text element 
         *        present on the stage.
         */    
        BEGIN_DECLARE_INTERFACE(IStaticTextBehaviour, IID_ISTATIC_TEXT_BEHAVIOUR) 
                
            /**
             * @brief  This function returns the orientation of the text element.
             *
             * @param  orientationMode (OUT)
             *         The orientation of the text element is returned.
             *
             * @return On success, FCM_SUCCESS is returned, else an error code is returned. 
             */
            virtual FCM::Result _FCMCALL GetOrientationMode(OrientationMode& orientationMode) = 0;
        

            /**
             * @brief  This function returns the flow of the text element.
             *
             * @param  flow (OUT)
             *         Text flow is returned.
             *
             * @return On success, FCM_SUCCESS is returned; otherwise an error code is returned. 
             */
            virtual FCM::Result _FCMCALL GetFlow(TextFlow& flow) = 0;

        END_DECLARE_INTERFACE 


        /**
         * @class IModifiableTextBehaviour
         *
         * @brief Defines an interface that represents the base class for the dynamic/input 
         *        text type of the text element present on the stage.
         */    
        BEGIN_DECLARE_INTERFACE(IModifiableTextBehaviour, IID_IMODIFIABLE_TEXT_BEHAVIOUR)
            
            /**
             * @brief  This function returns the instance name of the text element on the stage.
             *
             * @param  ppName (OUT)
             *         The instance name of text element on stage is returned.
             *
             * @return On success, FCM_SUCCESS is returned, else an error code is returned. 
             *
             * @note   The memory allocated for 'ppName' in this function must be freed 
             *         by the caller using IFCMCalloc::Free().
             */
            virtual FCM::Result _FCMCALL GetInstanceName(FCM::StringRep16* ppName) = 0;
            

            /**
             * @brief  This function returns the paragraph behavior mode of the text element.
             *
             * @param  lineMode (OUT)
             *         The paragraph behavior of the text element is returned.
             *
             * @return On success, FCM_SUCCESS is returned, else an error code is returned.            
             */
             virtual FCM::Result _FCMCALL GetLineMode(LineMode& lineMode) = 0;
        

            /**
             * @brief  This function returns if the text of text element is scrollable.
             *
             * @param  scrollable (OUT)
             *         Returns true if the text of the text element is scrollable.
             *
             * @return On success, FCM_SUCCESS is returned, else an error code is returned. 
             */
            virtual FCM::Result _FCMCALL IsScrollable(FCM::Boolean& scrollable) = 0;
            

            /**
             * @brief  This function returns whether a border around the text element is to be drawn.
             *
             * @param  borderDrawn (OUT)
             *         returns true if a border around the text element is to be drawn.
             *
             * @return On success, FCM_SUCCESS is returned, else an error code is returned. 
             */																																														
            virtual FCM::Result _FCMCALL IsBorderDrawn(FCM::Boolean& borderDrawn) = 0;
            

            /**
             * @brief  This function returns if contents of the text element is to be rendered as html.
             *
             * @param  renderAsHtml (OUT)
             *         returns true if a text of the text element is to be rendered as HTML.
             *
             * @return On success, FCM_SUCCESS is returned, else an error code is returned. 
             */
            virtual FCM::Result _FCMCALL GetRenderAsHtml(FCM::Boolean& renderAsHtml) = 0;            

        END_DECLARE_INTERFACE 
            

        /**
         * @class IDynamicTextBehaviour
         *
         * @brief Defines an interface that represents the dynamic text type of the text element 
         *        present on the stage.
         */
        BEGIN_DECLARE_INTERFACE(IDynamicTextBehaviour, IID_IDYNAMIC_TEXT_BEHAVIOUR)               
                
        END_DECLARE_INTERFACE 
            

        /**
         * @class IInputTextBehaviour
         *
         * @brief Defines an interface that represents the Input text type of the text element 
         *        present on the stage.
         */
        BEGIN_DECLARE_INTERFACE(IInputTextBehaviour, IID_IINPUT_TEXT_BEHAVIOUR)
                
            /**
             * @brief  This function checks if the Line mode password is set.
             *
             * @param  password (OUT)
             *         This variable is set to true if LinemodePassword is enabled.
             *
             * @return On success, FCM_SUCCESS is returned, else an error code is returned.  
             */
            virtual FCM::Result _FCMCALL IsLineModePassword(FCM::Boolean& password) = 0;

        END_DECLARE_INTERFACE 
        
    }
}


/* -------------------------------------------------- Inline / Functions */


#include "FCMPostConfig.h"

#endif // ITEXT_BEHAVIOUR_H_

