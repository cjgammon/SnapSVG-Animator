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
 * @file  IFontTable.h
 *
 * @brief This file contains interface for IFontTable. IFontTable represents 
 *        a font table containing information about a font family.
 */

#ifndef IFONT_TABLE_H_
#define IFONT_TABLE_H_

#include "FCMPreConfig.h"
#include "FCMPluginInterface.h"
#include "IFCMUnknown.h"
#include "FrameElement/ITextBehaviour.h"


/* -------------------------------------------------- Forward Decl */


/* -------------------------------------------------- Enums */


/* -------------------------------------------------- Macros / Constants */

namespace DOM 
{
    namespace Service
    {
        namespace FontTable
        {        
            /**
             * @brief Defines the universally-unique interface ID for 
             *        IFontTable.
             *
             * @note  Textual Representation: {88871CED-E055-4502-BBFD-28784A54FFD8}
             */
            FCM::ConstFCMIID IID_IFONT_TABLE =
                {0x88871ced, 0xe055, 0x4502, {0xbb, 0xfd, 0x28, 0x78, 0x4a, 0x54, 0xff, 0xd8}};
        }
    }
}


/* -------------------------------------------------- Structs / Unions */


/* -------------------------------------------------- Class Decl */

namespace DOM 
{
    namespace Service
    {
        namespace FontTable
        {        
            /**
            * @class IFontTable
            *
            * @brief Represents a font table containing information about a font family.
            */
            BEGIN_DECLARE_INTERFACE(IFontTable, IID_IFONT_TABLE)
                
                /**
                 * @brief  This function determines EM Square unit used while generating glyphs.
                 *
                 * @param  emSquare (OUT)
                 *         EM Square unit used while generating glyphs is returned.
                 *
                 * @return On success, FCM_SUCCESS is returned, else an error code is returned.
                 */                
                virtual FCM::Result _FCMCALL GetEMSquare(FCM::U_Int16& emSquare) = 0;
              

                /**
                 * @brief  This function determines copy right info for a font.
                 *
                 * @param  ppCopyrightInfo (OUT)
                 *         The copy right info for a font is returned.
                 *
                 * @return On success, FCM_SUCCESS is returned, else an error code is returned.
                 *
                 * @note   The memory allocated for ppCopyrightInfo in this function must be freed 
                 *         by the caller using IFCMCalloc::Free().
                 */                
                virtual FCM::Result _FCMCALL GetCopyrightInfo(
                    FCM::StringRep16* ppCopyrightInfo) = 0;
          

                /**
                 * @brief  This function returns list of glyphs inside the font table .
                 *         Each glyph contains the char code and glyph outline.
                 *          
                 * @param  orientationMode (IN)
                 *         Orientation mode of the font.
                 *
                 * @param  pGlyphList (OUT)
                 *         The list of glyphs inside the font table is returned.
                 *
                 * @return On success, FCM_SUCCESS is returned, else an error code is returned.
                 *
                 * @see    IGlyph             
                 */
                virtual FCM::Result _FCMCALL GetGlyphs(
                    FrameElement::OrientationMode orientationMode, 
                    PIFCMList& pGlyphList) = 0;


                /**
                 * @brief  This function returns the ascent of font.
                 *
                 * @param  orientationMode (IN)
                 *         Orientation mode of the font.
                 *
                 * @param  ascent (OUT)
                 *         The ascent is returned.
                 *
                 * @return On success, FCM_SUCCESS is returned, else an error code is returned. 
                 *
                 */
                virtual FCM::Result _FCMCALL GetAscent(
                    FrameElement::OrientationMode orientationMode, 
                    FCM::Double& ascent) = 0;
                

                /**
                 * @brief  This function returns the descent of font.
                 *
                 * @param  orientationMode (IN)
                 *         Orientation mode of the font.
                 *
                 * @param  descent (OUT)
                 *         The descent is returned.
                 *
                 * @return On success, FCM_SUCCESS is returned, else an error code is returned.
                 *
                 */
                virtual FCM::Result _FCMCALL GetDescent(
                    FrameElement::OrientationMode orientationMode, 
                    FCM::Double& descent) = 0;


                /**
                 * @brief  This function returns list of KernPairs inside the font table.
                 *         Each KernPair contains left and right char and the kerning amount.
                 *
                 * @param  orientationMode (IN)
                 *         The orientation for which kern pair list is needed.
                 *
                 * @param  pKerningPairList (OUT)
                 *         The list of KerningPairs inside the font table is returned.
                 *
                 * @return On success, FCM_SUCCESS is returned, else an error code is returned.
                 *
                 * @see    IGlyph             
                 */                
                virtual FCM::Result _FCMCALL GetKerningPairs(
                    FrameElement::OrientationMode orientationMode, 
                    PIFCMList& pKerningPairList) = 0;
            
            END_DECLARE_INTERFACE 
        }
    }
}


/* -------------------------------------------------- Inline / Functions */


#include "FCMPostConfig.h"

#endif // IFONT_TABLE_H_

