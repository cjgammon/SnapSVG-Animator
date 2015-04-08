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
 * @file  ITextStyle.h
 *
 * @brief This file contains the interface for ITextStyle. ITextStyle interface 
 *        represents properties which describe characters.
 */

#ifndef ITEXT_STYLE_H_
#define ITEXT_STYLE_H_

#include "FCMPreConfig.h"
#include "FCMPluginInterface.h"
#include "IFCMUnknown.h"
#include "Utils/DOMTypes.h"


/* -------------------------------------------------- Forward Decl */


/* -------------------------------------------------- Enums */

namespace DOM 
{
    namespace FrameElement
    {
        /**
        * @enum BaseLineShiftStyle
        *
        * @brief This enumerates the base line shift styles.
        */
        enum BaseLineShiftStyle
        { 
            /** Normal base line shift style */
            BASE_LINE_SHIFT_STYLE_NORMAL, 
                

            /** Superscript base line shift style */
            BASE_LINE_SHIFT_STYLE_SUPERSCRIPT, 
                

            /** Subscript base line shift style */
            BASE_LINE_SHIFT_STYLE_SUBSCRIPT 
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
         *        ITextStyle
         *
         * @note  Textual Representation: {C8538DE2-7D59-4E0B-B1ED-EF4A5C55CC09}
         */
        FCM::ConstFCMIID IID_ITEXT_STYLE =
             {0xc8538de2, 0x7d59, 0x4e0b, {0xb1, 0xed, 0xef, 0x4a, 0x5c, 0x55, 0xcc, 0x9}};


        /**
         * @brief Defines the 'regular' text style.
         */
        FCM::CStringRep8 const REGULAR_STYLE_STR = "RegularStyle";


        /**
         * @brief Defines the 'italic' text style.
         */
        FCM::CStringRep8 const ITALIC_STYLE_STR = "ItalicStyle";


        /**
         * @brief Defines the 'bold' text style.
         */
        FCM::CStringRep8 const BOLD_STYLE_STR = "BoldStyle";
        
        
        /**
         * @brief Defines the 'bold-italic' text style.
         */
        FCM::CStringRep8 const BOLD_ITALIC_STYLE_STR = "BoldItalicStyle";
    }
}


/* -------------------------------------------------- Structs / Unions */

        
/* -------------------------------------------------- Class Decl */

namespace DOM 
{
    namespace FrameElement
    {        
        /**
         * @class ITextStyle
         *
         * @brief Defines an interface that represents the properties that describe characters.
         */    
        BEGIN_DECLARE_INTERFACE(ITextStyle, IID_ITEXT_STYLE)                
                                
            /**
             * @brief  This function returns the font style as string.
             *
             * @param  ppFontStyle (OUT)
             *         The font style is returned as string.
             *
             * @return On success, FCM_SUCCESS is returned, else an error code is returned.
             *
             * @note   The memory allocated for 'ppFontStyle' in this function must be freed 
             *         by the caller using IFCMCalloc::Free().
             */
            virtual FCM::Result _FCMCALL GetFontStyle(FCM::StringRep8* ppFontStyle) = 0;
                                

            /**
             * @brief  This function returns the space between characters. Value returned 
             *         is between -60 to 60.
             *
             * @param  letterSpacing (OUT)
             *         The the space between characters is returned.
             *
             * @return On success, FCM_SUCCESS is returned, else an error code is returned. 
             */
            virtual FCM::Result _FCMCALL GetLetterSpacing(FCM::S_Int16& letterSpacing) = 0;
            

            /**
             * @brief  This function returns size of the font.
             *
             * @param  fontSize (OUT)
             *         The size of the font is returned.
             *
             * @return On success, FCM_SUCCESS is returned, else an error code is returned. 
             */
            virtual FCM::Result _FCMCALL GetFontSize(FCM::U_Int16& fontSize) = 0;
            

            /**
             * @brief  This function returns the name of the font.
             *
             * @param  ppFontName (OUT)
             *         The name of the font is returned.
             *
             * @return On success, FCM_SUCCESS is returned, else an error code is returned.
             *
             * @note   The memory allocated for 'ppFontName' in this function must be freed 
             *         by the caller using IFCMCalloc::Free().
             */
            virtual FCM::Result _FCMCALL GetFontName(FCM::StringRep16* ppFontName) = 0;                


           /**
            * @brief  This function returns true if kerning pair information in the 
            *         font(s) to kern the text can be used. If it returns false,   
            *         kerning pair information in the font(s) is ignored. It only works with  
            *         static text and will be ignored if used with other text types.
            *
            * @param  autoKern (OUT)
            *         Returns true if kerning pair info is to be used; else false is returned.
            *
            * @return On success, FCM_SUCCESS is returned, else an error code is returned.
            */            
           virtual FCM::Result _FCMCALL IsAutoKernEnabled(FCM::Boolean& autoKern) = 0;
                

           /**
            * @brief  This function returns color of the font.
            *
            * @param  fontColor (OUT)
            *         The color of the font is returned.
            *
            * @return On success, FCM_SUCCESS is returned, else an error code is returned. 
            */
           virtual FCM::Result _FCMCALL GetFontColor(Utils::COLOR& fontColor) = 0;
                

           /**
             * @brief  This function returns baseline shift style. This will be returned for 
             *         static text and in other cases BASE_LINE_SHIFT_STYLE_NORMAL is returned.
             *
             * @param  baseLineShiftStyle (OUT)
             *         The baseline shift style is returned.
             *
             * @return On success, FCM_SUCCESS is returned, else an error code is returned.
             */    
           virtual FCM::Result _FCMCALL GetBaseLineShiftStyle(
               BaseLineShiftStyle& baseLineShiftStyle) = 0;
    

           /**
            * @brief  This function returns whether the characters of the text are rotated by 
            *         90 degrees. This will be returned for static text with a vertical 
            *         orientation, and in other cases false will be returned.
            *
            * @param  rotated (OUT)
            *         returns true if characters of the text are rotated 90 degrees.
            *
            * @return On success, FCM_SUCCESS is returned, else an error code is returned.
            */
           virtual FCM::Result _FCMCALL IsRotated(FCM::Boolean& rotated) = 0;
    

           /**
            * @brief  This function returns string for the target. This will be returned for 
            *         static text, and in other cases NULL will be returned.
            *
            * @param  ppLinkTarget (OUT)
            *         The string for the target is returned.
            *
            * @return On success, FCM_SUCCESS is returned, else an error code is returned.
            */
           virtual FCM::Result _FCMCALL GetLinkTarget(FCM::StringRep16* ppLinkTarget) = 0;
    

           /**
            * @brief  This function returns string of the link. This will be returned for 
            *         static text, and in other cases NULL will be returned.
            *
            * @param  ppLink (OUT)
            *         The string of the link is returned.
            *
            * @return On success, FCM_SUCCESS is returned, else an error code is returned.
            */
           virtual FCM::Result _FCMCALL GetLink(FCM::StringRep16* ppLink) = 0;
                
       END_DECLARE_INTERFACE 
        
    }
}


/* -------------------------------------------------- Inline / Functions */


#include "FCMPostConfig.h"

#endif // ITEXT_STYLE_H_

