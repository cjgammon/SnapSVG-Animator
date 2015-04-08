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
 * @file  ITextLinesGeneratorService.h
 *
 * @brief This file contains the interface for ITextLinesGeneratorService. 
 *        ITextLinesGeneratorService contains methods which fetches text lines from the 
 *        text element.
 */

#ifndef ITEXT_LINES_GENERATOR_SERVICE_H_
#define ITEXT_LINES_GENERATOR_SERVICE_H_

#include "FCMPreConfig.h"
#include "FCMPluginInterface.h"
#include "IFCMUnknown.h"
#include "Utils/DOMTypes.h"


/* -------------------------------------------------- Forward Decl */

namespace DOM
{
    namespace FrameElement
    {
        FORWARD_DECLARE_INTERFACE(IClassicText);
    }
}


/* -------------------------------------------------- Enums */


/* -------------------------------------------------- Macros / Constants */

namespace DOM 
{
    namespace Service
    {
        namespace TextLayout
        {        
            /**
             * @brief Defines the Interface ID (which is universally unique) for 
             *        ITextLinesGeneratorService.
             *
             * @note  Textual Representation: {30D8005F-7046-4E66-A437-4FCD6A78DED5}
             */
            FCM::ConstFCMIID IID_ITEXT_LINES_GENERATOR_SERVICE =
                {0x30d8005f, 0x7046, 0x4e66, {0xa4, 0x37, 0x4f, 0xcd, 0x6a, 0x78, 0xde, 0xd5}};
        }
    }
}


/* -------------------------------------------------- Structs / Unions */


/* -------------------------------------------------- Class Decl */

namespace DOM 
{
    namespace Service
    {
        namespace TextLayout
        {        
            /**
             * @class ITextLinesGeneratorService
             *
             * @brief Defines the interface to generate text lines for a text element.
             */    
            BEGIN_DECLARE_INTERFACE(ITextLinesGeneratorService, IID_ITEXT_LINES_GENERATOR_SERVICE)
                
                /**
                 * @brief  This function returns the list of text lines present in the text 
                 *         element.
                 *
                 * @param  pClassicText (IN)
                 *         The text element from which text lines are to be fetched.         
                 *
                 * @param  pTextLineList (OUT)
                 *         The list of text lines (ITextLine) present in the text element.
                 *
                 * @return On success, FCM_SUCCESS is returned, else an error code is returned. 
                 *
                 * @see    DOM::Service::TextLayout::ITextLine             
                 */
                virtual FCM::Result _FCMCALL GetTextLines(
                    const FrameElement::PIClassicText& pClassicText, 
                    PIFCMList& pTextLineList) = 0;
            
            END_DECLARE_INTERFACE 
        }
    }
}


/* -------------------------------------------------- Inline / Functions */


#include "FCMPostConfig.h"

#endif // ITEXT_LINES_GENERATOR_SERVICE_H_

