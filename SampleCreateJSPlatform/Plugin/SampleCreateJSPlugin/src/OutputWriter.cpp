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

#include "OutputWriter.h"

#include <cstring>
#include <fstream>
#include "FlashFCMPublicIDs.h"
#include "FCMPluginInterface.h"
#include "libjson.h"
#include "Utils.h"
#include "FrameElement/ISound.h"
#include "Service/Image/IBitmapExportService.h"
#include "Service/TextLayout/ITextLinesGeneratorService.h"
#include "Service/TextLayout/ITextLine.h"
#include "Service/Sound/ISoundExportService.h"
#include "GraphicFilter/IDropShadowFilter.h"
#include "GraphicFilter/IAdjustColorFilter.h"
#include "GraphicFilter/IBevelFilter.h"
#include "GraphicFilter/IBlurFilter.h"
#include "GraphicFilter/IGlowFilter.h"
#include "GraphicFilter/IGradientBevelFilter.h"
#include "GraphicFilter/IGradientGlowFilter.h"
#include "Utils/ILinearColorGradient.h"
#include <math.h>

#ifdef _WINDOWS
#include "Windows.h"
#endif

namespace CreateJS
{
    static const std::string moveTo = "M";
    static const std::string lineTo = "L";
    static const std::string bezierCurveTo = "Q";
    static const std::string space = " ";
    static const std::string comma = ",";
    static const std::string semiColon = ";";

    static const FCM::Float GRADIENT_VECTOR_CONSTANT = 16384.0;

    static const char* htmlOutput =
        "<!DOCTYPE html>\r\n\
        <html>\r\n\
        <head>\r\n\
        </head> \r\n\
        \r\n\
        <body> \r\n\
            <script type=\"text/javascript\"> \r\n\
                var jsonfile = \"%s\",\r\n\
                    fps = %d,\r\n\
                    width = %d,\r\n\
                    height = %d;\r\n\
                \r\n\
                function ready(c) {\r\n\
                    c.timeline().repeat(-1);\r\n\
                    c.play();\r\n\
                }\r\n\
            </script>\r\n\
            <script src=\"./js/vendor/requirejs/require.js\" data-main=\"js/main\"></script> \r\n\
        </body>\r\n\
        </html>";

    /* -------------------------------------------------- JSONOutputWriter */

    FCM::Result JSONOutputWriter::StartOutput(std::string& outputFileName)
    {
        std::string parent;
        std::string jsonFile;

        Utils::GetParent(outputFileName, parent);
        Utils::GetFileNameWithoutExtension(outputFileName, jsonFile);
        m_outputHTMLFile = outputFileName;
        m_outputJSONFileName = jsonFile + ".json";
        m_outputJSONFilePath = parent + jsonFile + ".json";
        m_outputImageFolder = parent + IMAGE_FOLDER;
        m_outputSoundFolder = parent + SOUND_FOLDER;

        return FCM_SUCCESS;
    }


    FCM::Result JSONOutputWriter::EndOutput()
    {

        return FCM_SUCCESS;
    }


    FCM::Result JSONOutputWriter::StartDocument(
        const DOM::Utils::COLOR& background,
        FCM::U_Int32 stageHeight, 
        FCM::U_Int32 stageWidth,
        FCM::U_Int32 fps)
    {
        FCM::U_Int32 backColor;

        m_HTMLOutput = new char[strlen(htmlOutput) + FILENAME_MAX + 50];
        if (m_HTMLOutput == NULL)
        {
            return FCM_MEM_NOT_AVAILABLE;
        }

        backColor = (background.red << 16) | (background.green << 8) | (background.blue);
        sprintf(m_HTMLOutput, htmlOutput, m_outputJSONFileName.c_str(), fps, stageWidth, stageHeight, backColor);

        return FCM_SUCCESS;
    }


    FCM::Result JSONOutputWriter::EndDocument()
    {
        std::fstream file;
        m_pRootNode->push_back(*m_pShapeArray);
        m_pRootNode->push_back(*m_pBitmapArray);
        m_pRootNode->push_back(*m_pSoundArray);
        m_pRootNode->push_back(*m_pTextArray);
        m_pRootNode->push_back(*m_pTimelineArray);        

        // Write the JSON file (overwrite file if it already exists)
        Utils::OpenFStream(m_outputJSONFilePath, file, std::ios_base::trunc|std::ios_base::out, m_pCallback);

        JSONNode firstNode(JSON_NODE);
        firstNode.push_back(*m_pRootNode);

        file << firstNode.write_formatted();
        file.close();

        // Write the HTML file (overwrite file if it already exists)
        Utils::OpenFStream(m_outputHTMLFile, file, std::ios_base::trunc|std::ios_base::out, m_pCallback);

        file << m_HTMLOutput;
        file.close();

        delete [] m_HTMLOutput;

        return FCM_SUCCESS;
    }


    FCM::Result JSONOutputWriter::StartDefineTimeline()
    {
        return FCM_SUCCESS;
    }


    FCM::Result JSONOutputWriter::EndDefineTimeline(
        FCM::U_Int32 resId, 
        FCM::StringRep16 pName,
        ITimelineWriter* pTimelineWriter)
    {
        JSONTimelineWriter* pWriter = static_cast<JSONTimelineWriter*> (pTimelineWriter);

        pWriter->Finish(resId, pName);

        m_pTimelineArray->push_back(*(pWriter->GetRoot()));

        return FCM_SUCCESS;
    }


    FCM::Result JSONOutputWriter::StartDefineShape()
    {
        m_shapeElem = new JSONNode(JSON_NODE);
        ASSERT(m_shapeElem);

        m_pathArray = new JSONNode(JSON_ARRAY);
        ASSERT(m_pathArray);
        m_pathArray->set_name("path");

        return FCM_SUCCESS;
    }


    // Marks the end of a shape
    FCM::Result JSONOutputWriter::EndDefineShape(FCM::U_Int32 resId)
    {
        m_shapeElem->push_back(JSONNode(("charid"), CreateJS::Utils::ToString(resId)));
        m_shapeElem->push_back(*m_pathArray);

        m_pShapeArray->push_back(*m_shapeElem);

        delete m_pathArray;
        delete m_shapeElem;

        return FCM_SUCCESS;
    }


    // Start of fill region definition
    FCM::Result JSONOutputWriter::StartDefineFill()
    {
        m_pathElem = new JSONNode(JSON_NODE);
        ASSERT(m_pathElem);

        m_pathCmdStr.clear();

        return FCM_SUCCESS;
    }


    // Solid fill style definition
    FCM::Result JSONOutputWriter::DefineSolidFillStyle(const DOM::Utils::COLOR& color)
    {
        std::string colorStr = Utils::ToString(color);
        std::string colorOpacityStr = CreateJS::Utils::ToString((double)(color.alpha / 255.0));

        m_pathElem->push_back(JSONNode("color", colorStr.c_str()));
        m_pathElem->push_back(JSONNode("colorOpacity", colorOpacityStr.c_str()));

        return FCM_SUCCESS;
    }


    // Bitmap fill style definition
    FCM::Result JSONOutputWriter::DefineBitmapFillStyle(
        FCM::Boolean clipped,
        const DOM::Utils::MATRIX2D& matrix,
        FCM::S_Int32 height, 
        FCM::S_Int32 width,
        std::string& name,
        DOM::LibraryItem::PIMediaItem pMediaItem)
    {
        FCM::Result res;
        JSONNode bitmapElem(JSON_NODE);
        std::string bitmapPath;
        std::string bitmapName;

        bitmapElem.set_name("image");
        
        bitmapElem.push_back(JSONNode(("height"), CreateJS::Utils::ToString(height)));
        bitmapElem.push_back(JSONNode(("width"), CreateJS::Utils::ToString(width)));

        FCM::AutoPtr<FCM::IFCMUnknown> pUnk;
        std::string bitmapRelPath;
        std::string bitmapExportPath = m_outputImageFolder + "/";
            
        bitmapExportPath += name;
            
        bitmapRelPath = "./";
        bitmapRelPath += IMAGE_FOLDER;
        bitmapRelPath += "/";
        bitmapRelPath += name;

        res = m_pCallback->GetService(DOM::FLA_BITMAP_SERVICE, pUnk.m_Ptr);
        ASSERT(FCM_SUCCESS_CODE(res));


        res = m_pCallback->GetService(DOM::FLA_BITMAP_SERVICE, pUnk.m_Ptr);
        ASSERT(FCM_SUCCESS_CODE(res));

        FCM::AutoPtr<DOM::Service::Image::IBitmapExportService> bitmapExportService = pUnk;
        if (bitmapExportService)
        {
            FCM::AutoPtr<FCM::IFCMCalloc> pCalloc;
            FCM::StringRep16 pFilePath = Utils::ToString16(bitmapExportPath, m_pCallback);
            res = bitmapExportService->ExportToFile(pMediaItem, pFilePath, 100);
            ASSERT(FCM_SUCCESS_CODE(res));

            pCalloc = CreateJS::Utils::GetCallocService(m_pCallback);
            ASSERT(pCalloc.m_Ptr != NULL);

            pCalloc->Free(pFilePath);
        }

        bitmapElem.push_back(JSONNode(("bitmapPath"), bitmapRelPath)); 

        DOM::Utils::MATRIX2D matrix1 = matrix;
        matrix1.a /= 20.0;
        matrix1.b /= 20.0;
        matrix1.c /= 20.0;
        matrix1.d /= 20.0;

        bitmapElem.push_back(JSONNode(("patternUnits"), "userSpaceOnUse"));
        bitmapElem.push_back(JSONNode(("patternTransform"), Utils::ToString(matrix1).c_str()));

        m_pathElem->push_back(bitmapElem);

        return FCM_SUCCESS;
    }


    // Start Linear Gradient fill style definition
    FCM::Result JSONOutputWriter::StartDefineLinearGradientFillStyle(
        DOM::FillStyle::GradientSpread spread,
        const DOM::Utils::MATRIX2D& matrix)
    {
        DOM::Utils::POINT2D point;

        m_gradientColor = new JSONNode(JSON_NODE);
        ASSERT(m_gradientColor);
        m_gradientColor->set_name("linearGradient");

        point.x = -GRADIENT_VECTOR_CONSTANT / 20;
        point.y = 0;
        Utils::TransformPoint(matrix, point, point);

        m_gradientColor->push_back(JSONNode("x1", Utils::ToString((double) (point.x))));
        m_gradientColor->push_back(JSONNode("y1", Utils::ToString((double) (point.y))));

        point.x = GRADIENT_VECTOR_CONSTANT / 20;
        point.y = 0;
        Utils::TransformPoint(matrix, point, point);

        m_gradientColor->push_back(JSONNode("x2", Utils::ToString((double) (point.x))));
        m_gradientColor->push_back(JSONNode("y2", Utils::ToString((double) (point.y))));

        m_gradientColor->push_back(JSONNode("spreadMethod", Utils::ToString(spread)));

        m_stopPointArray = new JSONNode(JSON_ARRAY);
        ASSERT(m_stopPointArray);
        m_stopPointArray->set_name("stop");

        return FCM_SUCCESS;
    }


    // Sets a specific key point in a color ramp (for both radial and linear gradient)
    FCM::Result JSONOutputWriter::SetKeyColorPoint(
        const DOM::Utils::GRADIENT_COLOR_POINT& colorPoint)
    {
        JSONNode stopEntry(JSON_NODE);
        FCM::Float offset;
        
        offset = (float)((colorPoint.pos * 100) / 255.0);

        stopEntry.push_back(JSONNode("offset", Utils::ToString((double) offset)));
        stopEntry.push_back(JSONNode("stopColor", Utils::ToString(colorPoint.color)));
        stopEntry.push_back(JSONNode("stopOpacity", Utils::ToString((double)(colorPoint.color.alpha / 255.0))));

        m_stopPointArray->push_back(stopEntry);

        return FCM_SUCCESS;
    }


    // End Linear Gradient fill style definition
    FCM::Result JSONOutputWriter::EndDefineLinearGradientFillStyle()
    {
        m_gradientColor->push_back(*m_stopPointArray);
        m_pathElem->push_back(*m_gradientColor);

        delete m_stopPointArray;
        delete m_gradientColor;

        return FCM_SUCCESS;
    }


    // Start Radial Gradient fill style definition
    FCM::Result JSONOutputWriter::StartDefineRadialGradientFillStyle(
        DOM::FillStyle::GradientSpread spread,
        const DOM::Utils::MATRIX2D& matrix,
        FCM::S_Int32 focalPoint)
    {
        DOM::Utils::POINT2D point;
        DOM::Utils::POINT2D point1;
        DOM::Utils::POINT2D point2;

        m_gradientColor = new JSONNode(JSON_NODE);
        ASSERT(m_gradientColor);
        m_gradientColor->set_name("radialGradient");

        point.x = 0;
        point.y = 0;
        Utils::TransformPoint(matrix, point, point1);

        point.x = GRADIENT_VECTOR_CONSTANT / 20;
        point.y = 0;
        Utils::TransformPoint(matrix, point, point2);

        FCM::Float xd = point1.x - point2.x;
        FCM::Float yd = point1.y - point2.y;
        FCM::Float r = sqrt(xd * xd + yd * yd);

        FCM::Float angle = atan2(yd, xd);
        double focusPointRatio = focalPoint / 255.0;
        double fx = -r * focusPointRatio * cos(angle);
        double fy = -r * focusPointRatio * sin(angle);

        m_gradientColor->push_back(JSONNode("cx", "0"));
        m_gradientColor->push_back(JSONNode("cy", "0"));
        m_gradientColor->push_back(JSONNode("r", Utils::ToString((double) r)));
        m_gradientColor->push_back(JSONNode("fx", Utils::ToString((double) fx)));
        m_gradientColor->push_back(JSONNode("fy", Utils::ToString((double) fy)));

        FCM::Float scaleFactor = (GRADIENT_VECTOR_CONSTANT / 20) / r;
        DOM::Utils::MATRIX2D matrix1 = {};
        matrix1.a = matrix.a * scaleFactor;
        matrix1.b = matrix.b * scaleFactor;
        matrix1.c = matrix.c * scaleFactor;
        matrix1.d = matrix.d * scaleFactor;
        matrix1.tx = matrix.tx;
        matrix1.ty = matrix.ty;

        m_gradientColor->push_back(JSONNode("gradientTransform", Utils::ToString(matrix1)));
        m_gradientColor->push_back(JSONNode("spreadMethod", Utils::ToString(spread)));

        m_stopPointArray = new JSONNode(JSON_ARRAY);
        ASSERT(m_stopPointArray);
        m_stopPointArray->set_name("stop");

        return FCM_SUCCESS;
    }


    // End Radial Gradient fill style definition
    FCM::Result JSONOutputWriter::EndDefineRadialGradientFillStyle()
    {
        m_gradientColor->push_back(*m_stopPointArray);
        m_pathElem->push_back(*m_gradientColor);

        delete m_stopPointArray;
        delete m_gradientColor;

        return FCM_SUCCESS;
    }


    // Start of fill region boundary
    FCM::Result JSONOutputWriter::StartDefineBoundary()
    {
        return StartDefinePath();
    }


    // Sets a segment of a path (Used for boundary, holes)
    FCM::Result JSONOutputWriter::SetSegment(const DOM::Utils::SEGMENT& segment)
    {
        if (m_firstSegment)
        {
            if (segment.segmentType == DOM::Utils::LINE_SEGMENT)
            {
                m_pathCmdStr.append(CreateJS::Utils::ToString((double)(segment.line.endPoint1.x)));
                m_pathCmdStr.append(space);
                m_pathCmdStr.append(CreateJS::Utils::ToString((double)(segment.line.endPoint1.y)));
                m_pathCmdStr.append(space);
            }
            else
            {
                m_pathCmdStr.append(CreateJS::Utils::ToString((double)(segment.quadBezierCurve.anchor1.x)));
                m_pathCmdStr.append(space);
                m_pathCmdStr.append(CreateJS::Utils::ToString((double)(segment.quadBezierCurve.anchor1.y)));
                m_pathCmdStr.append(space);
            }
            m_firstSegment = false;
        }

        if (segment.segmentType == DOM::Utils::LINE_SEGMENT)
        {
            m_pathCmdStr.append(lineTo);
            m_pathCmdStr.append(space);
            m_pathCmdStr.append(CreateJS::Utils::ToString((double)(segment.line.endPoint2.x)));
            m_pathCmdStr.append(space);
            m_pathCmdStr.append(CreateJS::Utils::ToString((double)(segment.line.endPoint2.y)));
            m_pathCmdStr.append(space);
        }
        else
        {
            m_pathCmdStr.append(bezierCurveTo);
            m_pathCmdStr.append(space);
            m_pathCmdStr.append(CreateJS::Utils::ToString((double)(segment.quadBezierCurve.control.x)));
            m_pathCmdStr.append(space);
            m_pathCmdStr.append(CreateJS::Utils::ToString((double)(segment.quadBezierCurve.control.y)));
            m_pathCmdStr.append(space);
            m_pathCmdStr.append(CreateJS::Utils::ToString((double)(segment.quadBezierCurve.anchor2.x)));
            m_pathCmdStr.append(space);
            m_pathCmdStr.append(CreateJS::Utils::ToString((double)(segment.quadBezierCurve.anchor2.y)));
            m_pathCmdStr.append(space);
        }

        return FCM_SUCCESS;
    }


    // End of fill region boundary
    FCM::Result JSONOutputWriter::EndDefineBoundary()
    {
        return EndDefinePath();
    }


    // Start of fill region hole
    FCM::Result JSONOutputWriter::StartDefineHole()
    {
        return StartDefinePath();
    }


    // End of fill region hole
    FCM::Result JSONOutputWriter::EndDefineHole()
    {
        return EndDefinePath();
    }


    // Start of stroke group
    FCM::Result JSONOutputWriter::StartDefineStrokeGroup()
    {
        // No need to do anything
        return FCM_SUCCESS;
    }


    // Start solid stroke style definition
    FCM::Result JSONOutputWriter::StartDefineSolidStrokeStyle(
        FCM::Double thickness,
        const DOM::StrokeStyle::JOIN_STYLE& joinStyle,
        const DOM::StrokeStyle::CAP_STYLE& capStyle,
        DOM::Utils::ScaleType scaleType,
        FCM::Boolean strokeHinting)
    {
        m_strokeStyle.type = SOLID_STROKE_STYLE_TYPE;
        m_strokeStyle.solidStrokeStyle.capStyle = capStyle;
        m_strokeStyle.solidStrokeStyle.joinStyle = joinStyle;
        m_strokeStyle.solidStrokeStyle.thickness = thickness;
        m_strokeStyle.solidStrokeStyle.scaleType = scaleType;
        m_strokeStyle.solidStrokeStyle.strokeHinting = strokeHinting;

        return FCM_SUCCESS;
    }


    // End of solid stroke style
    FCM::Result JSONOutputWriter::EndDefineSolidStrokeStyle()
    {
        // No need to do anything
        return FCM_SUCCESS;
    }


    // Start of stroke 
    FCM::Result JSONOutputWriter::StartDefineStroke()
    {
        m_pathElem = new JSONNode(JSON_NODE);
        ASSERT(m_pathElem);

        m_pathCmdStr.clear();
        StartDefinePath();

        return FCM_SUCCESS;
    }


    // End of a stroke 
    FCM::Result JSONOutputWriter::EndDefineStroke()
    {
        m_pathElem->push_back(JSONNode("d", m_pathCmdStr));

        if (m_strokeStyle.type == SOLID_STROKE_STYLE_TYPE)
        {
            m_pathElem->push_back(JSONNode("strokeWidth", CreateJS::Utils::ToString((double)m_strokeStyle.solidStrokeStyle.thickness).c_str()));
            m_pathElem->push_back(JSONNode("fill", "none"));
            m_pathElem->push_back(JSONNode("strokeLinecap", Utils::ToString(m_strokeStyle.solidStrokeStyle.capStyle.type).c_str()));
            m_pathElem->push_back(JSONNode("strokeLinejoin", Utils::ToString(m_strokeStyle.solidStrokeStyle.joinStyle.type).c_str()));

            if (m_strokeStyle.solidStrokeStyle.joinStyle.type == DOM::Utils::MITER_JOIN)
            {
                m_pathElem->push_back(JSONNode(
                    "stroke-miterlimit", 
                    CreateJS::Utils::ToString((double)m_strokeStyle.solidStrokeStyle.joinStyle.miterJoinProp.miterLimit).c_str()));
            }
            m_pathElem->push_back(JSONNode("pathType", "Stroke"));
        }
        m_pathArray->push_back(*m_pathElem);

        delete m_pathElem;

        m_pathElem = NULL;

        return FCM_SUCCESS;
    }


    // End of stroke group
    FCM::Result JSONOutputWriter::EndDefineStrokeGroup()
    {
        // No need to do anything
        return FCM_SUCCESS;
    }


    // End of fill style definition
    FCM::Result JSONOutputWriter::EndDefineFill()
    {
        m_pathElem->push_back(JSONNode("d", m_pathCmdStr));
        m_pathElem->push_back(JSONNode("pathType", JSON_TEXT("Fill")));
        m_pathElem->push_back(JSONNode("stroke", JSON_TEXT("none")));

        m_pathArray->push_back(*m_pathElem);

        delete m_pathElem;

        m_pathElem = NULL;
        
        return FCM_SUCCESS;
    }


    // Define a bitmap
    FCM::Result JSONOutputWriter::DefineBitmap(
        FCM::U_Int32 resId,
        FCM::S_Int32 height, 
        FCM::S_Int32 width,
        const std::string& name,
        DOM::LibraryItem::PIMediaItem pMediaItem)
    {
        FCM::Result res;
        JSONNode bitmapElem(JSON_NODE);
        std::string bitmapPath;
        std::string bitmapName;

        bitmapElem.set_name("image");
        
        bitmapElem.push_back(JSONNode(("charid"), CreateJS::Utils::ToString(resId)));
        bitmapElem.push_back(JSONNode(("height"), CreateJS::Utils::ToString(height)));
        bitmapElem.push_back(JSONNode(("width"), CreateJS::Utils::ToString(width)));

        FCM::AutoPtr<FCM::IFCMUnknown> pUnk;
        std::string bitmapRelPath;
        std::string bitmapExportPath = m_outputImageFolder + "/";
            
        bitmapExportPath += name;
            
        bitmapRelPath = "./";
        bitmapRelPath += IMAGE_FOLDER;
        bitmapRelPath += "/";
        bitmapRelPath += name;

        res = m_pCallback->GetService(DOM::FLA_BITMAP_SERVICE, pUnk.m_Ptr);
        ASSERT(FCM_SUCCESS_CODE(res));

        FCM::AutoPtr<DOM::Service::Image::IBitmapExportService> bitmapExportService = pUnk;
        if (bitmapExportService)
        {
            FCM::AutoPtr<FCM::IFCMCalloc> pCalloc;
            FCM::StringRep16 pFilePath = Utils::ToString16(bitmapExportPath, m_pCallback);
            res = bitmapExportService->ExportToFile(pMediaItem, pFilePath, 100);
            ASSERT(FCM_SUCCESS_CODE(res));

            pCalloc = CreateJS::Utils::GetCallocService(m_pCallback);
            ASSERT(pCalloc.m_Ptr != NULL);

            pCalloc->Free(pFilePath);
        }

        bitmapElem.push_back(JSONNode(("bitmapPath"), bitmapRelPath)); 

        m_pBitmapArray->push_back(bitmapElem);

        return FCM_SUCCESS;
    }

    FCM::Result JSONOutputWriter::DefineText(
            FCM::U_Int32 resId, 
            const std::string& name, 
            const DOM::Utils::COLOR& color, 
            const std::string& displayText, 
            DOM::FrameElement::PIClassicText pTextItem)
    {
        std::string txt = displayText;
        std::string colorStr = Utils::ToString(color);
        std::string find = "\r";
        std::string replace = "\\r";
        std::string::size_type i =0;
        JSONNode textElem(JSON_NODE);

        while (true) {
            /* Locate the substring to replace. */
            i = txt.find(find, i);
           
            if (i == std::string::npos) break;
            /* Make the replacement. */
            txt.replace(i, find.length(), replace);

            /* Advance index forward so the next iteration doesn't pick it up as well. */
            i += replace.length();
        }

        
        textElem.push_back(JSONNode(("charid"), CreateJS::Utils::ToString(resId)));
        textElem.push_back(JSONNode(("displayText"),txt ));
        textElem.push_back(JSONNode(("font"),name));
        textElem.push_back(JSONNode("color", colorStr.c_str()));

        m_pTextArray->push_back(textElem);

        return FCM_SUCCESS;
    }

    FCM::Result JSONOutputWriter::DefineSound(
            FCM::U_Int32 resId, 
            const std::string& name, 
            DOM::LibraryItem::PIMediaItem pMediaItem)
    {
        FCM::Result res;
        JSONNode soundElem(JSON_NODE);
        std::string soundPath;
        std::string soundName;
        soundElem.set_name("sound");
        soundElem.push_back(JSONNode(("charid"), CreateJS::Utils::ToString(resId)));
        FCM::AutoPtr<FCM::IFCMUnknown> pUnk;
        std::string soundRelPath;
        std::string soundExportPath = m_outputSoundFolder + "/";
        soundExportPath += name;
        soundRelPath = "./";
        soundRelPath += SOUND_FOLDER;
        soundRelPath += "/";
        soundRelPath += name;
        res = m_pCallback->GetService(DOM::FLA_SOUND_SERVICE, pUnk.m_Ptr);
        ASSERT(FCM_SUCCESS_CODE(res));
        FCM::AutoPtr<DOM::Service::Sound::ISoundExportService> soundExportService = pUnk;
        if (soundExportService)
        {
            FCM::AutoPtr<FCM::IFCMCalloc> pCalloc;
            FCM::StringRep16 pFilePath = Utils::ToString16(soundExportPath, m_pCallback);
            res = soundExportService->ExportToFile(pMediaItem, pFilePath);
            ASSERT(FCM_SUCCESS_CODE(res));
            pCalloc = CreateJS::Utils::GetCallocService(m_pCallback);
            ASSERT(pCalloc.m_Ptr != NULL);
            pCalloc->Free(pFilePath);
        }
        soundElem.push_back(JSONNode(("soundPath"), soundRelPath)); 
        m_pSoundArray->push_back(soundElem);
        return FCM_SUCCESS;
    }

    JSONOutputWriter::JSONOutputWriter(FCM::PIFCMCallback pCallback)
        : m_pCallback(pCallback),
          m_shapeElem(NULL),
          m_pathArray(NULL),
          m_pathElem(NULL),
          m_firstSegment(false),
          m_HTMLOutput(NULL)
    {
        m_pRootNode = new JSONNode(JSON_NODE);
        ASSERT(m_pRootNode);
        m_pRootNode->set_name("DOMDocument");

        m_pShapeArray = new JSONNode(JSON_ARRAY);
        ASSERT(m_pShapeArray);
        m_pShapeArray->set_name("Shape");

        m_pTimelineArray = new JSONNode(JSON_ARRAY);
        ASSERT(m_pTimelineArray);
        m_pTimelineArray->set_name("Timeline");

        m_pBitmapArray = new JSONNode(JSON_ARRAY);
        ASSERT(m_pBitmapArray);
        m_pBitmapArray->set_name("Bitmaps");

        m_pTextArray = new JSONNode(JSON_ARRAY);
        ASSERT(m_pTextArray);
        m_pTextArray->set_name("Text");

        m_pSoundArray = new JSONNode(JSON_ARRAY);
        ASSERT(m_pSoundArray);
        m_pSoundArray->set_name("Sounds");
        m_strokeStyle.type = INVALID_STROKE_STYLE_TYPE;
    }


    JSONOutputWriter::~JSONOutputWriter()
    {
        delete m_pBitmapArray;
        delete m_pSoundArray;

        delete m_pTimelineArray;

        delete m_pShapeArray;

        delete m_pTextArray;

        delete m_pRootNode;
    }


    FCM::Result JSONOutputWriter::StartDefinePath()
    {
        m_pathCmdStr.append(moveTo);
        m_pathCmdStr.append(space);
        m_firstSegment = true;

        return FCM_SUCCESS;
    }

    FCM::Result JSONOutputWriter::EndDefinePath()
    {
        // No need to do anything
        return FCM_SUCCESS;
    }

    /* -------------------------------------------------- JSONTimelineWriter */

    FCM::Result JSONTimelineWriter::PlaceObject(
        FCM::U_Int32 resId,
        FCM::U_Int32 objectId,
        FCM::U_Int32 placeAfterObjectId,
        const DOM::Utils::MATRIX2D* pMatrix,
        FCM::PIFCMUnknown pUnknown /* = NULL*/)
    {
        JSONNode commandElement(JSON_NODE);

        commandElement.push_back(JSONNode("cmdType", "Place"));
        commandElement.push_back(JSONNode("charid", CreateJS::Utils::ToString(resId)));
        commandElement.push_back(JSONNode("objectId", CreateJS::Utils::ToString(objectId)));
        commandElement.push_back(JSONNode("placeAfter", CreateJS::Utils::ToString(placeAfterObjectId)));

        if (pMatrix)
        {
            commandElement.push_back(JSONNode("transformMatrix", Utils::ToString(*pMatrix).c_str()));
        }

        m_pCommandArray->push_back(commandElement);

        return FCM_SUCCESS;
    }


    FCM::Result JSONTimelineWriter::PlaceObject(
        FCM::U_Int32 resId,
        FCM::U_Int32 objectId,
        FCM::PIFCMUnknown pUnknown /* = NULL*/)
    {
        FCM::Result res;

        JSONNode commandElement(JSON_NODE);
        FCM::AutoPtr<DOM::FrameElement::ISound> pSound;

        commandElement.push_back(JSONNode("cmdType", "Place"));
        commandElement.push_back(JSONNode("charid", CreateJS::Utils::ToString(resId)));
        commandElement.push_back(JSONNode("objectId", CreateJS::Utils::ToString(objectId)));

        pSound = pUnknown;
        if (pSound)
        {
            DOM::FrameElement::SOUND_LOOP_MODE lMode;
            DOM::FrameElement::SOUND_LIMIT soundLimit;
            DOM::FrameElement::SoundSyncMode syncMode;

            soundLimit.structSize = sizeof(DOM::FrameElement::SOUND_LIMIT);
            lMode.structSize = sizeof(DOM::FrameElement::SOUND_LOOP_MODE);

            res = pSound->GetLoopMode(lMode);
            ASSERT(FCM_SUCCESS_CODE(res));

            commandElement.push_back(JSONNode("loopMode", 
                CreateJS::Utils::ToString(lMode.loopMode)));
            commandElement.push_back(JSONNode("repeatCount", 
                CreateJS::Utils::ToString(lMode.repeatCount)));

            res = pSound->GetSyncMode(syncMode);
            ASSERT(FCM_SUCCESS_CODE(res));

            commandElement.push_back(JSONNode("syncMode", 
                CreateJS::Utils::ToString(syncMode)));

            // We should not get SOUND_SYNC_STOP as for stop, "RemoveObject" command will
            // be generated by Exporter Service.
            ASSERT(syncMode != DOM::FrameElement::SOUND_SYNC_STOP); 

            res = pSound->GetSoundLimit(soundLimit);
            ASSERT(FCM_SUCCESS_CODE(res));

            commandElement.push_back(JSONNode("LimitInPos44", 
                CreateJS::Utils::ToString(soundLimit.inPos44)));
            commandElement.push_back(JSONNode("LimitOutPos44", 
                CreateJS::Utils::ToString(soundLimit.outPos44)));
        }

        m_pCommandArray->push_back(commandElement);

        return res;
    }
    FCM::Result JSONTimelineWriter::RemoveObject(
        FCM::U_Int32 objectId)
    {
        JSONNode commandElement(JSON_NODE);

        commandElement.push_back(JSONNode("cmdType", "Remove"));
        commandElement.push_back(JSONNode("objectId", CreateJS::Utils::ToString(objectId)));

        m_pCommandArray->push_back(commandElement);

        return FCM_SUCCESS;
    }


    FCM::Result JSONTimelineWriter::UpdateZOrder(
        FCM::U_Int32 objectId,
        FCM::U_Int32 placeAfterObjectId)
    {
        JSONNode commandElement(JSON_NODE);

        commandElement.push_back(JSONNode("cmdType", "UpdateZOrder"));
        commandElement.push_back(JSONNode("objectId", CreateJS::Utils::ToString(objectId)));
        commandElement.push_back(JSONNode("placeAfter", CreateJS::Utils::ToString(placeAfterObjectId)));

        m_pCommandArray->push_back(commandElement);

        return FCM_SUCCESS;
    }


    FCM::Result JSONTimelineWriter::UpdateMask(
        FCM::U_Int32 objectId,
        FCM::U_Int32 maskTillObjectId)
    {
        JSONNode commandElement(JSON_NODE);

        commandElement.push_back(JSONNode("cmdType", "UpdateMask"));
        commandElement.push_back(JSONNode("objectId", CreateJS::Utils::ToString(objectId)));
        commandElement.push_back(JSONNode("maskTill", CreateJS::Utils::ToString(maskTillObjectId)));

        m_pCommandArray->push_back(commandElement);
        
        return FCM_SUCCESS;
    }

    FCM::Result JSONTimelineWriter::UpdateBlendMode(
        FCM::U_Int32 objectId,
        DOM::FrameElement::BlendMode blendMode)
    {
        JSONNode commandElement(JSON_NODE);

        commandElement.push_back(JSONNode("cmdType", "UpdateBlendMode"));
        commandElement.push_back(JSONNode("objectId", CreateJS::Utils::ToString(objectId)));
        if(blendMode == 0)
            commandElement.push_back(JSONNode("blendMode","Normal"));
        else if(blendMode == 1)
            commandElement.push_back(JSONNode("blendMode","Layer"));
        else if(blendMode == 2)
            commandElement.push_back(JSONNode("blendMode","Darken"));
        else if(blendMode == 3)
            commandElement.push_back(JSONNode("blendMode","Multiply"));
        else if(blendMode == 4)
            commandElement.push_back(JSONNode("blendMode","Lighten"));
        else if(blendMode == 5)
            commandElement.push_back(JSONNode("blendMode","Screen"));
        else if(blendMode == 6)
            commandElement.push_back(JSONNode("blendMode","Overlay"));
        else if(blendMode == 7)
            commandElement.push_back(JSONNode("blendMode","Hardlight"));
        else if(blendMode == 8)
            commandElement.push_back(JSONNode("blendMode","Add"));
        else if(blendMode == 9)
            commandElement.push_back(JSONNode("blendMode","Substract"));
        else if(blendMode == 10)
            commandElement.push_back(JSONNode("blendMode","Difference"));
        else if(blendMode == 11)
            commandElement.push_back(JSONNode("blendMode","Invert"));
        else if(blendMode == 12)
            commandElement.push_back(JSONNode("blendMode","Alpha"));
        else if(blendMode == 13)
            commandElement.push_back(JSONNode("blendMode","Erase"));

         m_pCommandArray->push_back(commandElement);
        return FCM_SUCCESS;
    }


    FCM::Result JSONTimelineWriter::UpdateVisibility(
        FCM::U_Int32 objectId,
        FCM::Boolean visible)
    {
        JSONNode commandElement(JSON_NODE);

        commandElement.push_back(JSONNode("cmdType", "UpdateVisibility"));
        commandElement.push_back(JSONNode("objectId", CreateJS::Utils::ToString(objectId)));

        if (visible)
        {
            commandElement.push_back(JSONNode("visibility", "true"));
        }
        else
        {
            commandElement.push_back(JSONNode("visibility", "false"));
        }

        m_pCommandArray->push_back(commandElement);

        return FCM_SUCCESS;
    }


    FCM::Result JSONTimelineWriter::AddGraphicFilter(
        FCM::U_Int32 objectId,
        FCM::PIFCMUnknown pFilter)
    {
        FCM::Result res;
        JSONNode commandElement(JSON_NODE);
        commandElement.push_back(JSONNode("cmdType", "UpdateFilter"));
        commandElement.push_back(JSONNode("objectId", CreateJS::Utils::ToString(objectId)));
        FCM::AutoPtr<DOM::GraphicFilter::IDropShadowFilter> pDropShadowFilter = pFilter;
        FCM::AutoPtr<DOM::GraphicFilter::IBlurFilter> pBlurFilter = pFilter;
        FCM::AutoPtr<DOM::GraphicFilter::IGlowFilter> pGlowFilter = pFilter;
        FCM::AutoPtr<DOM::GraphicFilter::IBevelFilter> pBevelFilter = pFilter;
        FCM::AutoPtr<DOM::GraphicFilter::IGradientGlowFilter> pGradientGlowFilter = pFilter;
        FCM::AutoPtr<DOM::GraphicFilter::IGradientBevelFilter> pGradientBevelFilter = pFilter;
        FCM::AutoPtr<DOM::GraphicFilter::IAdjustColorFilter> pAdjustColorFilter = pFilter;

        if (pDropShadowFilter)
        {
            FCM::Boolean enabled;
            FCM::Double  angle;
            FCM::Double  blurX;
            FCM::Double  blurY;
            FCM::Double  distance;
            FCM::Boolean hideObject;
            FCM::Boolean innerShadow;
            FCM::Boolean knockOut;
            DOM::Utils::FilterQualityType qualityType;
            DOM::Utils::COLOR color;
            FCM::S_Int32 strength;
            std::string colorStr;

            commandElement.push_back(JSONNode("filterType", "DropShadowFilter"));

            pDropShadowFilter->IsEnabled(enabled);
            if(enabled)
            {
                commandElement.push_back(JSONNode("enabled", "true"));
            }
            else
            {
                commandElement.push_back(JSONNode("enabled", "false"));
            }

            res = pDropShadowFilter->GetAngle(angle);
            ASSERT(FCM_SUCCESS_CODE(res));
            commandElement.push_back(JSONNode("angle", CreateJS::Utils::ToString((double)angle)));

            res = pDropShadowFilter->GetBlurX(blurX);
            ASSERT(FCM_SUCCESS_CODE(res));
            commandElement.push_back(JSONNode("blurX", CreateJS::Utils::ToString((double)blurX)));

            res = pDropShadowFilter->GetBlurY(blurY);
            ASSERT(FCM_SUCCESS_CODE(res));
            commandElement.push_back(JSONNode("blurY", CreateJS::Utils::ToString((double)blurY)));

            res = pDropShadowFilter->GetDistance(distance);
            ASSERT(FCM_SUCCESS_CODE(res));
            commandElement.push_back(JSONNode("distance", CreateJS::Utils::ToString((double)distance)));

            res = pDropShadowFilter->GetHideObject(hideObject);
            ASSERT(FCM_SUCCESS_CODE(res));
            if(hideObject)
            {
                commandElement.push_back(JSONNode("hideObject", "true"));
            }
            else
            {
                commandElement.push_back(JSONNode("hideObject", "false"));
            }

            res = pDropShadowFilter->GetInnerShadow(innerShadow);
            ASSERT(FCM_SUCCESS_CODE(res));
            if(innerShadow)
            {
                commandElement.push_back(JSONNode("innerShadow", "true"));
            }
            else
            {
                commandElement.push_back(JSONNode("innerShadow", "false"));
            }

            res = pDropShadowFilter->GetKnockout(knockOut);
            ASSERT(FCM_SUCCESS_CODE(res));
            if(knockOut)
            {
                commandElement.push_back(JSONNode("knockOut", "true"));
            }
            else
            {
                commandElement.push_back(JSONNode("knockOut", "false"));
            }

            res = pDropShadowFilter->GetQuality(qualityType);
            ASSERT(FCM_SUCCESS_CODE(res));
            if (qualityType == 0)
                commandElement.push_back(JSONNode("qualityType", "low"));
            else if (qualityType == 1)
                commandElement.push_back(JSONNode("qualityType", "medium"));
            else if (qualityType == 2)
                commandElement.push_back(JSONNode("qualityType", "high"));

            res = pDropShadowFilter->GetStrength(strength);
            ASSERT(FCM_SUCCESS_CODE(res));
            commandElement.push_back(JSONNode("strength", CreateJS::Utils::ToString(strength)));

            res = pDropShadowFilter->GetShadowColor(color);
            ASSERT(FCM_SUCCESS_CODE(res));
            colorStr = Utils::ToString(color);
            commandElement.push_back(JSONNode("shadowColor", colorStr.c_str()));

        }
        if(pBlurFilter)
        {
            FCM::Boolean enabled;
            FCM::Double  blurX;
            FCM::Double  blurY;
            DOM::Utils::FilterQualityType qualityType;


            commandElement.push_back(JSONNode("filterType", "BlurFilter"));

            res = pBlurFilter->IsEnabled(enabled);
            if(enabled)
            {
                commandElement.push_back(JSONNode("enabled", "true"));
            }
            else
            {
                commandElement.push_back(JSONNode("enabled", "false"));
            }

            res = pBlurFilter->GetBlurX(blurX);
            ASSERT(FCM_SUCCESS_CODE(res));
            commandElement.push_back(JSONNode("blurX", CreateJS::Utils::ToString((double)blurX)));

            res = pBlurFilter->GetBlurY(blurY);
            ASSERT(FCM_SUCCESS_CODE(res));
            commandElement.push_back(JSONNode("blurY", CreateJS::Utils::ToString((double)blurY)));

            res = pBlurFilter->GetQuality(qualityType);
            ASSERT(FCM_SUCCESS_CODE(res));
            if (qualityType == 0)
                commandElement.push_back(JSONNode("qualityType", "low"));
            else if (qualityType == 1)
                commandElement.push_back(JSONNode("qualityType", "medium"));
            else if (qualityType == 2)
                commandElement.push_back(JSONNode("qualityType", "high"));
        }

        if(pGlowFilter)
        {
            FCM::Boolean enabled;
            FCM::Double  blurX;
            FCM::Double  blurY;
            FCM::Boolean innerShadow;
            FCM::Boolean knockOut;
            DOM::Utils::FilterQualityType qualityType;
            DOM::Utils::COLOR color;
            FCM::S_Int32 strength;
            std::string colorStr;

            commandElement.push_back(JSONNode("filterType", "GlowFilter"));

            res = pGlowFilter->IsEnabled(enabled);
            if(enabled)
            {
                commandElement.push_back(JSONNode("enabled", "true"));
            }
            else
            {
                commandElement.push_back(JSONNode("enabled", "false"));
            }

            res = pGlowFilter->GetBlurX(blurX);
            ASSERT(FCM_SUCCESS_CODE(res));
            commandElement.push_back(JSONNode("blurX", CreateJS::Utils::ToString((double)blurX)));

            res = pGlowFilter->GetBlurY(blurY);
            ASSERT(FCM_SUCCESS_CODE(res));
            commandElement.push_back(JSONNode("blurY", CreateJS::Utils::ToString((double)blurY)));

            res = pGlowFilter->GetInnerShadow(innerShadow);
            ASSERT(FCM_SUCCESS_CODE(res));
            if(innerShadow)
            {
                commandElement.push_back(JSONNode("innerShadow", "true"));
            }
            else
            {
                commandElement.push_back(JSONNode("innerShadow", "false"));
            }

            res = pGlowFilter->GetKnockout(knockOut);
            ASSERT(FCM_SUCCESS_CODE(res));
            if(knockOut)
            {
                commandElement.push_back(JSONNode("knockOut", "true"));
            }
            else
            {
                commandElement.push_back(JSONNode("knockOut", "false"));
            }

            res = pGlowFilter->GetQuality(qualityType);
            ASSERT(FCM_SUCCESS_CODE(res));
            if (qualityType == 0)
                commandElement.push_back(JSONNode("qualityType", "low"));
            else if (qualityType == 1)
                commandElement.push_back(JSONNode("qualityType", "medium"));
            else if (qualityType == 2)
                commandElement.push_back(JSONNode("qualityType", "high"));

            res = pGlowFilter->GetStrength(strength);
            ASSERT(FCM_SUCCESS_CODE(res));
            commandElement.push_back(JSONNode("strength", CreateJS::Utils::ToString(strength)));

            res = pGlowFilter->GetShadowColor(color);
            ASSERT(FCM_SUCCESS_CODE(res));
            colorStr = Utils::ToString(color);
            commandElement.push_back(JSONNode("shadowColor", colorStr.c_str()));
        }

        if(pBevelFilter)
        {
            FCM::Boolean enabled;
            FCM::Double  angle;
            FCM::Double  blurX;
            FCM::Double  blurY;
            FCM::Double  distance;
            DOM::Utils::COLOR highlightColor;
            FCM::Boolean knockOut;
            DOM::Utils::FilterQualityType qualityType;
            DOM::Utils::COLOR color;
            FCM::S_Int32 strength;
            DOM::Utils::FilterType filterType;
            std::string colorStr;
            std::string colorString;

            commandElement.push_back(JSONNode("filterType", "BevelFilter"));

            res = pBevelFilter->IsEnabled(enabled);
            if(enabled)
            {
                commandElement.push_back(JSONNode("enabled", "true"));
            }
            else
            {
                commandElement.push_back(JSONNode("enabled", "false"));
            }

            res = pBevelFilter->GetAngle(angle);
            ASSERT(FCM_SUCCESS_CODE(res));
            commandElement.push_back(JSONNode("angle", CreateJS::Utils::ToString((double)angle)));

            res = pBevelFilter->GetBlurX(blurX);
            ASSERT(FCM_SUCCESS_CODE(res));
            commandElement.push_back(JSONNode("blurX", CreateJS::Utils::ToString((double)blurX)));

            res = pBevelFilter->GetBlurY(blurY);
            ASSERT(FCM_SUCCESS_CODE(res));
            commandElement.push_back(JSONNode("blurY", CreateJS::Utils::ToString((double)blurY)));

            res = pBevelFilter->GetDistance(distance);
            ASSERT(FCM_SUCCESS_CODE(res));
            commandElement.push_back(JSONNode("distance", CreateJS::Utils::ToString((double)distance)));

            res = pBevelFilter->GetHighlightColor(highlightColor);
            ASSERT(FCM_SUCCESS_CODE(res));
            colorString = Utils::ToString(highlightColor);
            commandElement.push_back(JSONNode("highlightColor",colorString.c_str()));

            res = pBevelFilter->GetKnockout(knockOut);
            ASSERT(FCM_SUCCESS_CODE(res));
            if(knockOut)
            {
                commandElement.push_back(JSONNode("knockOut", "true"));
            }
            else
            {
                commandElement.push_back(JSONNode("knockOut", "false"));
            }

            res = pBevelFilter->GetQuality(qualityType);
            ASSERT(FCM_SUCCESS_CODE(res));
            if (qualityType == 0)
                commandElement.push_back(JSONNode("qualityType", "low"));
            else if (qualityType == 1)
                commandElement.push_back(JSONNode("qualityType", "medium"));
            else if (qualityType == 2)
                commandElement.push_back(JSONNode("qualityType", "high"));

            res = pBevelFilter->GetStrength(strength);
            ASSERT(FCM_SUCCESS_CODE(res));
            commandElement.push_back(JSONNode("strength", CreateJS::Utils::ToString(strength)));

            res = pBevelFilter->GetShadowColor(color);
            ASSERT(FCM_SUCCESS_CODE(res));
            colorStr = Utils::ToString(color);
            commandElement.push_back(JSONNode("shadowColor", colorStr.c_str()));

            res = pBevelFilter->GetFilterType(filterType);
            ASSERT(FCM_SUCCESS_CODE(res));
            if (filterType == 0)
                commandElement.push_back(JSONNode("filterType", "inner"));
            else if (filterType == 1)
                commandElement.push_back(JSONNode("filterType", "outer"));
            else if (filterType == 2)
                commandElement.push_back(JSONNode("filterType", "full"));

        }

        if(pGradientGlowFilter)
        {
            FCM::Boolean enabled;
            FCM::Double  angle;
            FCM::Double  blurX;
            FCM::Double  blurY;
            FCM::Double  distance;
            FCM::Boolean knockOut;
            DOM::Utils::FilterQualityType qualityType;
            FCM::S_Int32 strength;
            DOM::Utils::FilterType filterType;

            commandElement.push_back(JSONNode("filterType", "GradientGlowFilter"));

            pGradientGlowFilter->IsEnabled(enabled);
            if(enabled)
            {
                commandElement.push_back(JSONNode("enabled", "true"));
            }
            else
            {
                commandElement.push_back(JSONNode("enabled", "false"));
            }

            res = pGradientGlowFilter->GetAngle(angle);
            ASSERT(FCM_SUCCESS_CODE(res));
            commandElement.push_back(JSONNode("angle", CreateJS::Utils::ToString((double)angle)));

            res = pGradientGlowFilter->GetBlurX(blurX);
            ASSERT(FCM_SUCCESS_CODE(res));
            commandElement.push_back(JSONNode("blurX", CreateJS::Utils::ToString((double)blurX)));

            res = pGradientGlowFilter->GetBlurY(blurY);
            ASSERT(FCM_SUCCESS_CODE(res));
            commandElement.push_back(JSONNode("blurY", CreateJS::Utils::ToString((double)blurY)));

            res = pGradientGlowFilter->GetDistance(distance);
            ASSERT(FCM_SUCCESS_CODE(res));
            commandElement.push_back(JSONNode("distance", CreateJS::Utils::ToString((double)distance)));

            res = pGradientGlowFilter->GetKnockout(knockOut);
            ASSERT(FCM_SUCCESS_CODE(res));
            if(knockOut)
            {
                commandElement.push_back(JSONNode("knockOut", "true"));
            }
            else
            {
                commandElement.push_back(JSONNode("knockOut", "false"));
            }

            res = pGradientGlowFilter->GetQuality(qualityType);
            ASSERT(FCM_SUCCESS_CODE(res));
            if (qualityType == 0)
                commandElement.push_back(JSONNode("qualityType", "low"));
            else if (qualityType == 1)
                commandElement.push_back(JSONNode("qualityType", "medium"));
            else if (qualityType == 2)
                commandElement.push_back(JSONNode("qualityType", "high"));

            res = pGradientGlowFilter->GetStrength(strength);
            ASSERT(FCM_SUCCESS_CODE(res));
            commandElement.push_back(JSONNode("strength", CreateJS::Utils::ToString(strength)));

            res = pGradientGlowFilter->GetFilterType(filterType);
            ASSERT(FCM_SUCCESS_CODE(res));
            if (filterType == 0)
                commandElement.push_back(JSONNode("filterType", "inner"));
            else if (filterType == 1)
                commandElement.push_back(JSONNode("filterType", "outer"));
            else if (filterType == 2)
                commandElement.push_back(JSONNode("filterType", "full"));

            FCM::AutoPtr<FCM::IFCMUnknown> pColorGradient;
            res = pGradientGlowFilter->GetGradient(pColorGradient.m_Ptr);
            ASSERT(FCM_SUCCESS_CODE(res));

            FCM::AutoPtr<DOM::Utils::ILinearColorGradient> pLinearGradient = pColorGradient;
            if (pLinearGradient)
            {

                FCM::U_Int8 colorCount;
                //DOM::Utils::GRADIENT_COLOR_POINT colorPoint;

                res = pLinearGradient->GetKeyColorCount(colorCount);
                ASSERT(FCM_SUCCESS_CODE(res));

                std::string colorArray ;
                std::string posArray ;
                JSONNode*   stopPointArray = new JSONNode(JSON_ARRAY);

                for (FCM::U_Int32 l = 0; l < colorCount; l++)
                {
                    DOM::Utils::GRADIENT_COLOR_POINT colorPoint;
                    pLinearGradient->GetKeyColorAtIndex(l, colorPoint);
                    JSONNode stopEntry(JSON_NODE);
                    FCM::Float offset;

                    offset = (float)((colorPoint.pos * 100) / 255.0);

                    stopEntry.push_back(JSONNode("offset", Utils::ToString((double) offset)));
                    stopEntry.push_back(JSONNode("stopColor", Utils::ToString(colorPoint.color)));
                    stopEntry.push_back(JSONNode("stopOpacity", Utils::ToString((double)(colorPoint.color.alpha / 255.0))));
                    stopPointArray->set_name("GradientStops");
                    stopPointArray->push_back(stopEntry);
                }

                commandElement.push_back(*stopPointArray);

            }//lineargradient
        }

        if(pGradientBevelFilter)
        {
            FCM::Boolean enabled;
            FCM::Double  angle;
            FCM::Double  blurX;
            FCM::Double  blurY;
            FCM::Double  distance;
            FCM::Boolean knockOut;
            DOM::Utils::FilterQualityType qualityType;
            FCM::S_Int32 strength;
            DOM::Utils::FilterType filterType;

            commandElement.push_back(JSONNode("filterType", "GradientBevelFilter"));

            pGradientBevelFilter->IsEnabled(enabled);
            if(enabled)
            {
                commandElement.push_back(JSONNode("enabled", "true"));
            }
            else
            {
                commandElement.push_back(JSONNode("enabled", "false"));
            }

            res = pGradientBevelFilter->GetAngle(angle);
            ASSERT(FCM_SUCCESS_CODE(res));
            commandElement.push_back(JSONNode("angle", CreateJS::Utils::ToString((double)angle)));

            res = pGradientBevelFilter->GetBlurX(blurX);
            ASSERT(FCM_SUCCESS_CODE(res));
            commandElement.push_back(JSONNode("blurX", CreateJS::Utils::ToString((double)blurX)));

            res = pGradientBevelFilter->GetBlurY(blurY);
            ASSERT(FCM_SUCCESS_CODE(res));
            commandElement.push_back(JSONNode("blurY", CreateJS::Utils::ToString((double)blurY)));

            res = pGradientBevelFilter->GetDistance(distance);
            ASSERT(FCM_SUCCESS_CODE(res));
            commandElement.push_back(JSONNode("distance", CreateJS::Utils::ToString((double)distance)));

            res = pGradientBevelFilter->GetKnockout(knockOut);
            ASSERT(FCM_SUCCESS_CODE(res));
            if(knockOut)
            {
                commandElement.push_back(JSONNode("knockOut", "true"));
            }
            else
            {
                commandElement.push_back(JSONNode("knockOut", "false"));
            }

            res = pGradientBevelFilter->GetQuality(qualityType);
            ASSERT(FCM_SUCCESS_CODE(res));
            if (qualityType == 0)
                commandElement.push_back(JSONNode("qualityType", "low"));
            else if (qualityType == 1)
                commandElement.push_back(JSONNode("qualityType", "medium"));
            else if (qualityType == 2)
                commandElement.push_back(JSONNode("qualityType", "high"));

            res = pGradientBevelFilter->GetStrength(strength);
            ASSERT(FCM_SUCCESS_CODE(res));
            commandElement.push_back(JSONNode("strength", CreateJS::Utils::ToString(strength)));

            res = pGradientBevelFilter->GetFilterType(filterType);
            ASSERT(FCM_SUCCESS_CODE(res));
            if (filterType == 0)
                commandElement.push_back(JSONNode("filterType", "inner"));
            else if (filterType == 1)
                commandElement.push_back(JSONNode("filterType", "outer"));
            else if (filterType == 2)
                commandElement.push_back(JSONNode("filterType", "full"));

            FCM::AutoPtr<FCM::IFCMUnknown> pColorGradient;
            res = pGradientBevelFilter->GetGradient(pColorGradient.m_Ptr);
            ASSERT(FCM_SUCCESS_CODE(res));

            FCM::AutoPtr<DOM::Utils::ILinearColorGradient> pLinearGradient = pColorGradient;
            if (pLinearGradient)
            {

                FCM::U_Int8 colorCount;
                //DOM::Utils::GRADIENT_COLOR_POINT colorPoint;

                res = pLinearGradient->GetKeyColorCount(colorCount);
                ASSERT(FCM_SUCCESS_CODE(res));

                std::string colorArray ;
                std::string posArray ;
                JSONNode*   stopPointsArray = new JSONNode(JSON_ARRAY);

                for (FCM::U_Int32 l = 0; l < colorCount; l++)
                {
                    DOM::Utils::GRADIENT_COLOR_POINT colorPoint;
                    pLinearGradient->GetKeyColorAtIndex(l, colorPoint);
                    JSONNode stopEntry(JSON_NODE);
                    FCM::Float offset;

                    offset = (float)((colorPoint.pos * 100) / 255.0);

                    stopEntry.push_back(JSONNode("offset", Utils::ToString((double) offset)));
                    stopEntry.push_back(JSONNode("stopColor", Utils::ToString(colorPoint.color)));
                    stopEntry.push_back(JSONNode("stopOpacity", Utils::ToString((double)(colorPoint.color.alpha / 255.0))));
                    stopPointsArray->set_name("GradientStops");
                    stopPointsArray->push_back(stopEntry);
                }

                commandElement.push_back(*stopPointsArray);

            }//lineargradient
        }

        if(pAdjustColorFilter)
        {
            FCM::Double brightness;
            FCM::Double contrast;
            FCM::Double saturation;
            FCM::Double hue;
            FCM::Boolean enabled;

            commandElement.push_back(JSONNode("filterType", "AdjustColorFilter"));

            pAdjustColorFilter->IsEnabled(enabled);
            if(enabled)
            {
                commandElement.push_back(JSONNode("enabled", "true"));
            }
            else
            {
                commandElement.push_back(JSONNode("enabled", "false"));
            }

            res = pAdjustColorFilter->GetBrightness(brightness);
            ASSERT(FCM_SUCCESS_CODE(res));
            commandElement.push_back(JSONNode("brightness", CreateJS::Utils::ToString((double)brightness)));

            res = pAdjustColorFilter->GetContrast(contrast);
            ASSERT(FCM_SUCCESS_CODE(res));
            commandElement.push_back(JSONNode("contrast", CreateJS::Utils::ToString((double)contrast)));

            res = pAdjustColorFilter->GetSaturation(saturation);
            ASSERT(FCM_SUCCESS_CODE(res));
            commandElement.push_back(JSONNode("saturation", CreateJS::Utils::ToString((double)saturation)));

            res = pAdjustColorFilter->GetHue(hue);
            ASSERT(FCM_SUCCESS_CODE(res));
            commandElement.push_back(JSONNode("hue", CreateJS::Utils::ToString((double)hue)));
        }

        m_pCommandArray->push_back(commandElement);

        return FCM_SUCCESS;
    }


    FCM::Result JSONTimelineWriter::UpdateDisplayTransform(
        FCM::U_Int32 objectId,
        const DOM::Utils::MATRIX2D& matrix)
    {
        JSONNode commandElement(JSON_NODE);
        std::string transformMat;

        commandElement.push_back(JSONNode("cmdType", "Move"));
        commandElement.push_back(JSONNode("objectId", CreateJS::Utils::ToString(objectId)));
        transformMat = CreateJS::Utils::ToString(matrix);
        commandElement.push_back(JSONNode("transformMatrix", transformMat.c_str()));

        m_pCommandArray->push_back(commandElement);

        return FCM_SUCCESS;
    }


    FCM::Result JSONTimelineWriter::UpdateColorTransform(
        FCM::U_Int32 objectId,
        const DOM::Utils::COLOR_MATRIX& colorMatrix)
    {
        JSONNode commandElement(JSON_NODE);
        std::string colorMat;

        commandElement.push_back(JSONNode("cmdType", "UpdateColorTransform"));
        commandElement.push_back(JSONNode("objectId", CreateJS::Utils::ToString(objectId)));
        colorMat = CreateJS::Utils::ToString(colorMatrix);
        commandElement.push_back(JSONNode("colorMatrix", colorMat.c_str()));

        m_pCommandArray->push_back(commandElement);

        return FCM_SUCCESS;
    }


    FCM::Result JSONTimelineWriter::ShowFrame(FCM::U_Int32 frameNum)
    {
        m_pFrameElement->push_back(JSONNode(("num"), CreateJS::Utils::ToString(frameNum)));
        m_pFrameElement->push_back(*m_pCommandArray);
        m_pFrameArray->push_back(*m_pFrameElement);

        delete m_pCommandArray;
        delete m_pFrameElement;

        m_pCommandArray = new JSONNode(JSON_ARRAY);
        m_pCommandArray->set_name("Command");

        m_pFrameElement = new JSONNode(JSON_NODE);
        ASSERT(m_pFrameElement);

        
        return FCM_SUCCESS;
    }


    FCM::Result JSONTimelineWriter::AddFrameScript(FCM::CStringRep16 pScript, FCM::U_Int32 layerNum)
    {
        std::string script = Utils::ToString(pScript, m_pCallback);

        std::string scriptWithLayerNumber = "script Layer" +  Utils::ToString(layerNum);

        std::string find = "\n";
        std::string replace = "\\n";
        std::string::size_type i =0;
        JSONNode textElem(JSON_NODE);

        while (true) {
            /* Locate the substring to replace. */
            i = script.find(find, i);
           
            if (i == std::string::npos) break;
            /* Make the replacement. */
            script.replace(i, find.length(), replace);

            /* Advance index forward so the next iteration doesn't pick it up as well. */
            i += replace.length();
        }

        
        Utils::Trace(m_pCallback, "[AddFrameScript] (Layer: %d): %s\n", layerNum, script.c_str());

        m_pFrameElement->push_back(JSONNode(scriptWithLayerNumber,script));

        return FCM_SUCCESS;
    }


    FCM::Result JSONTimelineWriter::RemoveFrameScript(FCM::U_Int32 layerNum)
    {
        Utils::Trace(m_pCallback, "[RemoveFrameScript] (Layer: %d)\n", layerNum);

        return FCM_SUCCESS;
    }

    FCM::Result JSONTimelineWriter::SetFrameLabel(FCM::StringRep16 pLabel, DOM::KeyFrameLabelType labelType)
    {
        std::string label = Utils::ToString(pLabel, m_pCallback);
        Utils::Trace(m_pCallback, "[SetFrameLabel] (Type: %d): %s\n", labelType, label.c_str());

        if(labelType == 1)
             m_pFrameElement->push_back(JSONNode("LabelType:Name",label));
        else if(labelType == 2)
             m_pFrameElement->push_back(JSONNode("labelType:Comment",label));
        else if(labelType == 3)
             m_pFrameElement->push_back(JSONNode("labelType:Ancor",label));
        else if(labelType == 0)
             m_pFrameElement->push_back(JSONNode("labelType","None"));

        return FCM_SUCCESS;
    }

	
    JSONTimelineWriter::JSONTimelineWriter(FCM::PIFCMCallback pCallback) :
        m_pCallback(pCallback)
    {
        m_pCommandArray = new JSONNode(JSON_ARRAY);
        ASSERT(m_pCommandArray);
        m_pCommandArray->set_name("Command");

        m_pFrameArray = new JSONNode(JSON_ARRAY);
        ASSERT(m_pFrameArray);
        m_pFrameArray->set_name("Frame");

        m_pTimelineElement = new JSONNode(JSON_NODE);
        ASSERT(m_pTimelineElement);
        m_pTimelineElement->set_name("Timeline");

        m_pFrameElement = new JSONNode(JSON_NODE);
        ASSERT(m_pFrameElement);
        //m_pCommandArray->set_name("Command");
    }


    JSONTimelineWriter::~JSONTimelineWriter()
    {
        delete m_pCommandArray;

        delete m_pFrameArray;

        delete m_pTimelineElement;
        
        delete m_pFrameElement;
    }


    const JSONNode* JSONTimelineWriter::GetRoot()
    {
        return m_pTimelineElement;
    }


    void JSONTimelineWriter::Finish(FCM::U_Int32 resId, FCM::StringRep16 pName)
    {
        if (resId != 0)
        {
            m_pTimelineElement->push_back(
                JSONNode(("charid"), 
                CreateJS::Utils::ToString(resId)));
        }

        m_pTimelineElement->push_back(*m_pFrameArray);
    }

};
