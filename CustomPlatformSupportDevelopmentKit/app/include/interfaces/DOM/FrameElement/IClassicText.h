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
 * @file  IClassicText.h
 *
 * @brief This file contains the interface for IClassicText. IClassicText interface
 *        represents the text element on the stage.
 */

#ifndef ICLASSIC_TEXT_H_
#define ICLASSIC_TEXT_H_

#include "FCMPreConfig.h"
#include "FCMPluginInterface.h"
#include "FrameElement/IFrameDisplayElement.h"


/* -------------------------------------------------- Forward Decl */

namespace DOM
{
    namespace FrameElement
    {
        FORWARD_DECLARE_INTERFACE(ITextBehaviour);
    }
}


/* -------------------------------------------------- Enums */

namespace DOM
{
    namespace FrameElement
    {
        /**
         * @enum AAMode
         *
         * @brief This defines anti-alias mode.
         */
        enum AAMode
        {
            /** Device anti-alias mode */
            ANTI_ALIAS_MODE_DEVICE,
            

            /** Bitmap anti-alias mode */
            ANTI_ALIAS_MODE_BITMAP,
           

            /** Standard anti-alias mode */
            ANTI_ALIAS_MODE_STANDARD,
           

            /** Advanced anti-alias mode */
            ANTI_ALIAS_MODE_ADVANCED,
           

            /** Custom anti-alias mode */
            ANTI_ALIAS_MODE_CUSTOM
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
         *        IClassicText
         *
         * @note  Textual Representation: {5C41008C-3FBD-496D-A3D1-EE752D091DDA}
         */
        FCM::ConstFCMIID IID_ICLASSIC_TEXT_ELEMENT =
            {0x5c41008c, 0x3fbd, 0x496d, {0xa3, 0xd1, 0xee, 0x75, 0x2d, 0x9, 0x1d, 0xda}};
    }
}


/* -------------------------------------------------- Structs / Unions */

namespace DOM
{
    namespace FrameElement
    {
        /**
         * @struct CUSTOM_AA_MODE_PROP
         *
         * @brief  This structure defines a Custom Anti-Alias Mode properties. 
         */
        struct CUSTOM_AA_MODE_PROP
        {            
            /** Anti-Alias thickness */
            FCM::Float aaThickness;
            
            /** Anti-Alias Shapness */
            FCM::Float aaSharpness;
            
        };
        

        /**
         * @struct AA_MODE_PROP
         *
         * @brief  Structure that consists of Anti alias mode and custom mode.
         */    
        struct AA_MODE_PROP
        {
            /** 
             * Size of this structure. This must be set by the client/caller to 
             * sizeof(AA_MODE_PROP).
             */
            FCM::U_Int32 structSize;

            /** Anti Alias mode*/
            AAMode aaMode;

            /** Custom Anti Alias Mode  */
            CUSTOM_AA_MODE_PROP customAAModeProp;
        };
    }    
}    


/* -------------------------------------------------- Class Decl */

namespace DOM 
{
    namespace FrameElement
    {
        /**
         * @class IClassicText
         *
         * @brief Defines an interface that represents the text element present on the stage.
         */
        BEGIN_DECLARE_INTERFACE(IClassicText, IID_ICLASSIC_TEXT_ELEMENT)

            /**
             * @brief  This function gets anti-alias mode properties.
             *
             * @param  aaModeProp (OUT)
             *         Anti alias mode.
             *
             * @return On success, FCM_SUCCESS is returned; otherwise an error code is returned.
             *
             * @note   Caller of this function must set aaModeProp.structSize to 
             *         sizeof(AA_MODE_PROP).
             */
            virtual FCM::Result _FCMCALL GetAntiAliasModeProp(AA_MODE_PROP& aaModeProp) = 0;


            /**
             * @brief  This function returns the string with all the characters in the textrun.
             *
             * @param  ppText (OUT)
             *         The the string with all characters in the textrun is returned.
             *
             * @return On success, FCM_SUCCESS is returned; else an error code is returned.
             *
             * @note   The memory allocated for 'ppText' in this function must be freed 
             *         by the caller using IFCMCalloc::Free().
             */
            virtual FCM::Result _FCMCALL GetText(FCM::StringRep16* ppText) = 0;            
            

            /**
             * @brief  This function returns the list of paragraphs in the text element.
             *
             * @param  pParagraphList (OUT)
             *         List of paragraphs in the text element. Each item in the list can
             *         be queried for IParagraph.
             *
             * @return On success, FCM_SUCCESS is returned, else an error code is returned.
             *
             * @see    IParagraph
             *
             * @note   For empty dynamic and input text, this function will return a 
             *         list with one paragraph in it which in turn will contain only 
             *         one text run.
             */
            virtual FCM::Result _FCMCALL GetParagraphs(PIFCMList& pParagraphList) = 0;
            

            /**
             * @brief  The function returns the behaviour of a text element. 
             *
             * @param  pTextBehaviour (OUT)
             *         The current behaviour of the text element is returned.
             *
             * @return On success, FCM_SUCCESS is returned, else an error code is returned.
             */
            virtual FCM::Result _FCMCALL GetTextBehaviour(PITextBehaviour& pTextBehaviour) = 0;
            
        END_DECLARE_INTERFACE
    }
}


/* -------------------------------------------------- Inline / Functions */


#include "FCMPostConfig.h"

#endif // ICLASSIC_TEXT_H_

