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
 * @file  IParagraph.h
 *
 * @brief This file contains the interface for IParagraph. IParagraph interface represents a 
 *        paragraph of text.
 */

#ifndef IPARAGRAPH_H_
#define IPARAGRAPH_H_

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
         * @enum AlignMode
         *
         * @brief This defines alignment mode of text.
         */
         enum AlignMode
         {
             /** 
              * Left align if orientation is horizontal. Top align if orientation 
              * is vertical 
              */
             ALIGN_MODE_LEFT,

             /** 
              * Right align if orientation is horizontal. Bottom align if orientation 
              * is vertical. 
              */
             ALIGN_MODE_RIGHT,                

             /** Center Align */
             ALIGN_MODE_CENTER,

             /** Justify Align */
             ALIGN_MODE_JUSTIFY
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
         *        IParagraph
         *
         * @note  Textual Representation: {BB9B73EA-06F7-4AC8-B51B-2E5A47BFC3B1}
         */
         FCM::ConstFCMIID IID_IPARAGRAPH =
             {0xbb9b73ea, 0x6f7, 0x4ac8, {0xb5, 0x1b, 0x2e, 0x5a, 0x47, 0xbf, 0xc3, 0xb1}};
     }
}


/* -------------------------------------------------- Structs / Unions */

namespace DOM 
{
    namespace FrameElement
    {   
        /**
         * @struct PARAGRAPH_STYLE
         *
         * @brief  Describes the paragraph layout data of the text element.
         */
        struct PARAGRAPH_STYLE
        {
            /** 
             * Size of this structure. This must be set by the client/caller to 
             * sizeof(PARAGRAPH_STYLE).
             */
            FCM::U_Int32 structSize;
                
            /** Paragraph alignment */
            AlignMode alignment;
                
            /** Paragraph indentation. Legal value is between -720 to 720. */
            FCM::S_Int16 indent;
                
            /** Line spacing of paragraph (also known as leading). Legal value is between -360 to 720. */
            FCM::S_Int16 lineSpacing;

            /** Paragraph's left margin. Legal value is between 0 to 720. */
            FCM::S_Int16 leftMargin;
                
            /** Paragraph's right margin. Legal value is between 0 to 720. */
            FCM::S_Int16 rightMargin;
         };
     }
}


/* -------------------------------------------------- Class Decl */

namespace DOM 
{
    namespace FrameElement
    {        
        /**
         * @class IParagraph
         *
         * @brief IParagraph interface represents paragraph of text.
         */    
        BEGIN_DECLARE_INTERFACE(IParagraph, IID_IPARAGRAPH)
                
            /**
             * @brief  This function returns index of first character within Text element 
             *         where this text run starts.
             *
             * @param  startIndex (OUT)
             *         The index of first character within Text element where this textrun 
             *         starts is returned.
             *
             * @return On success, FCM_SUCCESS is returned, else an error code is returned.
             */
            virtual FCM::Result _FCMCALL GetStartIndex(FCM::U_Int32& startIndex) = 0;


            /**
             * @brief  This function returns the length of the paragraph text.
             *
             * @param  length (OUT)
             *         Length of the paragraph text
             *
             * @return On success, FCM_SUCCESS is returned, else an error code is returned.
             */
            virtual FCM::Result _FCMCALL GetLength(FCM::U_Int32& length) = 0;                    
                

            /**
             * @brief  Returns the list of text runs, which fit into the paragraph.
             *
             * @param  pTextRunList (OUT)
             *         List of text runs. Each item in the list can be queried for 
             *         ITextRun.
             *
             * @return On success, FCM_SUCCESS is returned, else an error code is returned.
             *
             * @see    DOM::FrameElement::ITextRun
             */
            virtual FCM::Result _FCMCALL GetTextRuns(PIFCMList& pTextRunList) = 0;
                

            /**
             * @brief  This function returns the paragraph style.
             *
             * @param  paragraphStyle (OUT)
             *         Paragraph Style
             *
             * @return On success, FCM_SUCCESS is returned, else an error code is returned.
             *
             * @note   Caller of this function must set paragraphStyle.structSize to 
             *         sizeof(PARAGRAPH_STYLE).
             */                
            virtual FCM::Result _FCMCALL GetParagraphStyle(PARAGRAPH_STYLE& paragraphStyle) = 0;
            
        END_DECLARE_INTERFACE 
        
    }
}


/* -------------------------------------------------- Inline / Functions */


#include "FCMPostConfig.h"

#endif // IPARAGRAPH_H_

