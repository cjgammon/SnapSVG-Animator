/*************************************************************************
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
**************************************************************************/

/**
 * @file  OutputWriter.h
 *
 * @brief This file contains declarations for a output writer.
 */

#ifndef IOUTPUT_WRITER_H_
#define IOUTPUT_WRITER_H_

#include <string>

#include "FCMTypes.h"
#include "Utils/DOMTypes.h"
#include "LIbraryItem/IMediaItem.h"
#include "FrameElement/IClassicText.h"
#include "FrameElement/IMovieClip.h"
#include "FrameElement/ITextBehaviour.h"
#include "FrameElement/ITextRun.h"
#include "FrameElement/IParagraph.h"
#include "FrameElement/ITextStyle.h"
#include "StrokeStyle/ISolidStrokeStyle.h"
#include "FillStyle/IGradientFillStyle.h"
#include "IFrame.h"

/* -------------------------------------------------- Forward Decl */

namespace SnapSVGAnimator
{
    class ITimelineWriter;
}

/* -------------------------------------------------- Enums */


/* -------------------------------------------------- Macros / Constants */


/* -------------------------------------------------- Structs / Unions */

namespace SnapSVGAnimator
{
    struct STATIC_TEXT_INFO
    {
        ::DOM::FrameElement::OrientationMode orientationMode;
        ::DOM::FrameElement::TextFlow flow;
    };

    struct DYNAMIC_TEXT_INFO
    {
        ::DOM::FrameElement::LineMode lineMode;
        ::FCM::Boolean scrollable;
        ::FCM::Boolean borderDrawn;
        ::FCM::Boolean renderAsHtml;
    };

    struct INPUT_TEXT_INFO : public DYNAMIC_TEXT_INFO
    {
        ::FCM::Boolean password;
    };

    struct TEXT_BEHAVIOUR
    {
        int type;
        ::FCM::Boolean selectable;
        ::std::string name;
        union         {
            STATIC_TEXT_INFO staticText;
            DYNAMIC_TEXT_INFO dynamicText;
            INPUT_TEXT_INFO inputText;
        } u;
    };

    struct TEXT_STYLE
    {
        std::string fontStyle;
        FCM::S_Int16 letterSpacing;
        FCM::U_Int16 fontSize;
        std::string fontName;
        FCM::Boolean autoKern;
        DOM::Utils::COLOR fontColor;
        DOM::FrameElement::BaseLineShiftStyle baseLineShiftStyle;
        FCM::Boolean rotated;
        std::string link;
        std::string linkTarget;
    };
}


/* -------------------------------------------------- Class Decl */

namespace SnapSVGAnimator
{
    class IOutputWriter
    {
    public:

        // Marks the begining of the output
        virtual FCM::Result StartOutput(std::string& outputFileName) = 0;

        // Marks the end of the output
        virtual FCM::Result EndOutput() = 0;

        // Marks the begining of the Document
        virtual FCM::Result StartDocument(
            const DOM::Utils::COLOR& background, 
            FCM::U_Int32 stageHeight, 
            FCM::U_Int32 stageWidth,
            FCM::U_Int32 fps) = 0;

        // Marks the end of the Document
        virtual FCM::Result EndDocument() = 0;

        // Marks the start of a timeline
        virtual FCM::Result StartDefineTimeline() = 0;

        // Marks the end of a timeline
        virtual FCM::Result EndDefineTimeline(
            FCM::U_Int32 resId, 
            FCM::StringRep16 pName,
            ITimelineWriter* pTimelineWriter) = 0;

        // Marks the start of a shape
        virtual FCM::Result StartDefineShape() = 0;

        // Start of fill region definition
        virtual FCM::Result StartDefineFill() = 0;

        // Solid fill style definition
        virtual FCM::Result DefineSolidFillStyle(const DOM::Utils::COLOR& color) = 0;

        // Bitmap fill style definition
        virtual FCM::Result DefineBitmapFillStyle(
            FCM::Boolean clipped,
            const DOM::Utils::MATRIX2D& matrix,
            FCM::S_Int32 height, 
            FCM::S_Int32 width,
            const std::string& libPathName,
            DOM::LibraryItem::PIMediaItem pMediaItem) = 0;

        // Start Linear Gradient fill style definition
        virtual FCM::Result StartDefineLinearGradientFillStyle(
            DOM::FillStyle::GradientSpread spread,
            const DOM::Utils::MATRIX2D& matrix) = 0;

        // Sets a specific key point in a color ramp (for both radial and linear gradient)
        virtual FCM::Result SetKeyColorPoint(
            const DOM::Utils::GRADIENT_COLOR_POINT& colorPoint) = 0;

        // End Linear Gradient fill style definition
        virtual FCM::Result EndDefineLinearGradientFillStyle() = 0;

        // Start Radial Gradient fill style definition
        virtual FCM::Result StartDefineRadialGradientFillStyle(
            DOM::FillStyle::GradientSpread spread,
            const DOM::Utils::MATRIX2D& matrix,
            FCM::S_Int32 focalPoint) = 0;

        // End Radial Gradient fill style definition
        virtual FCM::Result EndDefineRadialGradientFillStyle() = 0;

        // Start of fill region boundary
        virtual FCM::Result StartDefineBoundary() = 0;

        // Sets a segment of a path (Used for boundary, holes)
        virtual FCM::Result SetSegment(const DOM::Utils::SEGMENT& segment) = 0;

        // End of fill region boundary
        virtual FCM::Result EndDefineBoundary() = 0;

        // Start of fill region hole
        virtual FCM::Result StartDefineHole() = 0;

        // End of fill region hole
        virtual FCM::Result EndDefineHole() = 0;

        // Start of stroke group
        virtual FCM::Result StartDefineStrokeGroup() = 0;

        // Start solid stroke style definition
        virtual FCM::Result StartDefineSolidStrokeStyle(
            FCM::Double thickness,
            const DOM::StrokeStyle::JOIN_STYLE& joinStyle,
            const DOM::StrokeStyle::CAP_STYLE& capStyle,
            DOM::Utils::ScaleType scaleType,
            FCM::Boolean strokeHinting) = 0;

        // End of solid stroke style
        virtual FCM::Result EndDefineSolidStrokeStyle() = 0;

        // Start of stroke 
        virtual FCM::Result StartDefineStroke() = 0;

        // End of a stroke 
        virtual FCM::Result EndDefineStroke() = 0;

        // End of stroke group
        virtual FCM::Result EndDefineStrokeGroup() = 0;

        // End of fill style definition
        virtual FCM::Result EndDefineFill() = 0;

        // Marks the end of a shape
        virtual FCM::Result EndDefineShape(FCM::U_Int32 resId) = 0;

        // Define a bitmap
        virtual FCM::Result DefineBitmap(
            FCM::U_Int32 resId,
            FCM::S_Int32 height, 
            FCM::S_Int32 width,
            const std::string& libPathName,
            DOM::LibraryItem::PIMediaItem pMediaItem) = 0;

        // Start of a classic text definition
        virtual FCM::Result StartDefineClassicText(
            FCM::U_Int32 resId, 
            const DOM::FrameElement::AA_MODE_PROP& aaModeProp,
            const std::string& displayText,
            const TEXT_BEHAVIOUR& textBehaviour) = 0;

        // Start paragraph
        virtual FCM::Result StartDefineParagraph(
            FCM::U_Int32 startIndex,
            FCM::U_Int32 length,
            const DOM::FrameElement::PARAGRAPH_STYLE& paragraphStyle) = 0;

        // Start text run
        virtual FCM::Result StartDefineTextRun(
            FCM::U_Int32 startIndex,
            FCM::U_Int32 length,
            const TEXT_STYLE& textStyle) = 0;

        // End of a text run
        virtual FCM::Result EndDefineTextRun() = 0;

        // End of a paragraph
        virtual FCM::Result EndDefineParagraph() = 0;

        // End of a classic text definition
        virtual FCM::Result EndDefineClassicText() = 0;

        // Define Sound
        virtual FCM::Result DefineSound(
            FCM::U_Int32 resId, 
            const std::string& libPathName,
            DOM::LibraryItem::PIMediaItem pMediaItem) = 0;
    };

    
    class ITimelineWriter
    {
    public:

        virtual FCM::Result PlaceObject(
            FCM::U_Int32 resId,
            FCM::U_Int32 objectId,
            FCM::U_Int32 placeAfterObjectId,
            const DOM::Utils::MATRIX2D* pMatrix) = 0;

        virtual FCM::Result PlaceObject(
            FCM::U_Int32 resId,
            FCM::U_Int32 objectId,
            FCM::U_Int32 placeAfterObjectId,
            const DOM::Utils::MATRIX2D* pMatrix,
            FCM::Boolean loop,
            FCM::PIFCMUnknown pUnknown) = 0;

        virtual FCM::Result PlaceObject(
            FCM::U_Int32 resId,
            FCM::U_Int32 objectId,
            FCM::PIFCMUnknown pUnknown = NULL) = 0;

        virtual FCM::Result RemoveObject(
            FCM::U_Int32 objectId) = 0;

        virtual FCM::Result UpdateZOrder(
            FCM::U_Int32 objectId,
            FCM::U_Int32 placeAfterObjectId) = 0;

        virtual FCM::Result UpdateMask(
            FCM::U_Int32 objectId,
            FCM::U_Int32 maskTillObjectId) = 0;
 
        virtual FCM::Result UpdateBlendMode(
            FCM::U_Int32 objectId,
            DOM::FrameElement::BlendMode blendMode) = 0;

        virtual FCM::Result UpdateVisibility(
            FCM::U_Int32 objectId,
            FCM::Boolean visible) = 0;

        virtual FCM::Result AddGraphicFilter(
            FCM::U_Int32 objectId,
            FCM::PIFCMUnknown pFilter) = 0;

        virtual FCM::Result UpdateDisplayTransform(
            FCM::U_Int32 objectId,
            const DOM::Utils::MATRIX2D& matrix) = 0;

        virtual FCM::Result UpdateColorTransform(
            FCM::U_Int32 objectId,
            const DOM::Utils::COLOR_MATRIX& colorMatrix) = 0;

        virtual FCM::Result ShowFrame(FCM::U_Int32 frameNum) = 0;

        virtual FCM::Result AddFrameScript(FCM::CStringRep16 pScript, FCM::U_Int32 layerNum) = 0;

        virtual FCM::Result RemoveFrameScript(FCM::U_Int32 layerNum) = 0;

        virtual FCM::Result SetFrameLabel(FCM::StringRep16 pLabel, DOM::KeyFrameLabelType labelType) = 0;
    };
};

#endif // IOUTPUT_WRITER_H_

