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
#include "PluginConfiguration.h"

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

namespace SnapSVGAnimator
{
    static const std::string moveTo = "M";
    static const std::string lineTo = "l";
    static const std::string bezierCurveTo = "q";
    static const std::string space = " ";
    static const std::string comma = ",";
    static const std::string semiColon = ";";

    static const FCM::Float GRADIENT_VECTOR_CONSTANT = 16384.0;

    static const char* htmlOutput =
        "<!DOCTYPE html>\n\
        <html>\n\
        <head>\n\
        <style>\n\
        #%s{\n\
            background-color: #%02X%02X%02X;\n\
        }\n\
        </style>\n\
        </head> \n\
        \n\
        <body> \n\
    <script src=\"./SnapSVGAnimator/js/vendor/snap.svg.js\"></script>\n\
    <script src=\"./SnapSVGAnimator/js/SVGAnimator.js\"></script>\n\
\n\
    <script type=\"text/javascript\"> \n\
        var jsonfile = \"%s\",\n\
            fps = %d,\n\
            width = %d,\n\
            height = %d,\n\
            AJAX_req;\n\
\n\
    function handle_AJAX_Complete() {\n\
        if( AJAX_req.readyState == 4 && AJAX_req.status == 200 )\n\
        {\n\
            json = JSON.parse(AJAX_req.responseText);\n\
            comp = new SVGAnim(\n\
                           json,\n\
                           width,\n\
                           height\n\
                           );\n\
            \n\
            comp.play();\n\
        }\n\
    }\n\
\n\
    function AJAX_JSON_Req( url )\n\
    {\n\
        AJAX_req = new XMLHttpRequest();\n\
        AJAX_req.open(\"GET\", url, true);\n\
        AJAX_req.setRequestHeader(\"Content-type\", \"application/json\");\n\
        \n\
        AJAX_req.onreadystatechange = handle_AJAX_Complete;\n\
        AJAX_req.send();\n\
    }\n\
\n\
            </script>\n\
        </body>\n\
        </html>";

    // scriptSrcPath and dataMain are needed only for non-minified versions of the runtime
    static const char* scriptSrcPathUnMinified = "js/vendor/requirejs/require.js";
    static const char* dataMainUnMinified = "data-main=\"%s/js/main\"";

    static const char* scriptSrcPathMinified = "%s/%s";
    static const char* dataMainMinified = "";

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
        char scriptSrcPath[256];
        char dataMainPath[256];

        m_HTMLOutput = new char[strlen(htmlOutput) + 2 * FILENAME_MAX + 50];
        if (m_HTMLOutput == NULL)
        {
            return FCM_MEM_NOT_AVAILABLE;
        }

        std::string outputFileName;
        Utils::GetFileNameWithoutExtension(m_outputJSONFileName, outputFileName);

        if (!m_minify)
        {
            strcpy(scriptSrcPath, scriptSrcPathUnMinified);
            sprintf(dataMainPath, dataMainUnMinified, RUNTIME_FOLDER_NAME);
        }
        else
        {
            sprintf(scriptSrcPath, scriptSrcPathMinified, RUNTIME_MINIFIED_SUBFOLDER_NAME, RUNTIME_MINIFIED_FILE_NAME);
            strcpy(dataMainPath, dataMainMinified);
        }

        sprintf(m_HTMLOutput, 
            htmlOutput, 
            outputFileName.c_str(), 
            background.red,
            background.green,
            background.blue,
            m_outputJSONFileName.c_str(), 
            fps, 
            stageWidth, 
            stageHeight,
            RUNTIME_FOLDER_NAME,
            scriptSrcPath,
            dataMainPath);

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

        if (m_minify)
        {
            // Minify JSON
            file << firstNode.write();
        }
        else
        {
            // Pretty printing of JSON
            file << firstNode.write_formatted();
        }
        
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
        m_shapeElem->push_back(JSONNode(("charid"), SnapSVGAnimator::Utils::ToString(resId)));
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
        std::string colorOpacityStr = SnapSVGAnimator::Utils::ToString((float)(color.alpha / 255.0), m_dataPrecision);

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
        const std::string& libPathName,
        DOM::LibraryItem::PIMediaItem pMediaItem)
    {
        FCM::Result res;
        std::string name;
        JSONNode bitmapElem(JSON_NODE);
        std::string bitmapPath;
        std::string bitmapName;

        bitmapElem.set_name("image");
        
        bitmapElem.push_back(JSONNode(("height"), SnapSVGAnimator::Utils::ToString(height)));
        bitmapElem.push_back(JSONNode(("width"), SnapSVGAnimator::Utils::ToString(width)));

        FCM::AutoPtr<FCM::IFCMUnknown> pUnk;
        std::string bitmapRelPath;
        std::string bitmapExportPath = m_outputImageFolder + "/";
            
        FCM::Boolean alreadyExported = GetImageExportFileName(libPathName, name);
        if (!alreadyExported)
        {
            if (!m_imageFolderCreated)
            {
                res = Utils::CreateDir(m_outputImageFolder, m_pCallback);
                if (!(FCM_SUCCESS_CODE(res)))
                {
                    Utils::Trace(m_pCallback, "Output image folder (%s) could not be created\n", m_outputImageFolder.c_str());
                    return res;
                }
                m_imageFolderCreated = true;
            }
            CreateImageFileName(libPathName, name);
            SetImageExportFileName(libPathName, name);
        }

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

            pCalloc = SnapSVGAnimator::Utils::GetCallocService(m_pCallback);
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
        bitmapElem.push_back(JSONNode(("patternTransform"), Utils::ToString(matrix1, m_dataPrecision).c_str()));

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

        m_gradientColor->push_back(JSONNode("x1", Utils::ToString(point.x, m_dataPrecision)));
        m_gradientColor->push_back(JSONNode("y1", Utils::ToString(point.y, m_dataPrecision)));

        point.x = GRADIENT_VECTOR_CONSTANT / 20;
        point.y = 0;
        Utils::TransformPoint(matrix, point, point);

        m_gradientColor->push_back(JSONNode("x2", Utils::ToString(point.x, m_dataPrecision)));
        m_gradientColor->push_back(JSONNode("y2", Utils::ToString(point.y, m_dataPrecision)));

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

        stopEntry.push_back(JSONNode("offset", Utils::ToString(offset, m_dataPrecision)));
        stopEntry.push_back(JSONNode("stopColor", Utils::ToString(colorPoint.color)));
        stopEntry.push_back(JSONNode("stopOpacity", Utils::ToString((colorPoint.color.alpha / 255.0), m_dataPrecision)));

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
        float focusPointRatio = focalPoint / (float)255.0;
        float fx = -r * focusPointRatio * cos(angle);
        float fy = -r * focusPointRatio * sin(angle);

        m_gradientColor->push_back(JSONNode("cx", "0"));
        m_gradientColor->push_back(JSONNode("cy", "0"));
        m_gradientColor->push_back(JSONNode("r", Utils::ToString((float) r, m_dataPrecision)));
        m_gradientColor->push_back(JSONNode("fx", Utils::ToString((float) fx, m_dataPrecision)));
        m_gradientColor->push_back(JSONNode("fy", Utils::ToString((float) fy, m_dataPrecision)));

        FCM::Float scaleFactor = (GRADIENT_VECTOR_CONSTANT / 20) / r;
        DOM::Utils::MATRIX2D matrix1 = {};
        matrix1.a = matrix.a * scaleFactor;
        matrix1.b = matrix.b * scaleFactor;
        matrix1.c = matrix.c * scaleFactor;
        matrix1.d = matrix.d * scaleFactor;
        matrix1.tx = matrix.tx;
        matrix1.ty = matrix.ty;

        m_gradientColor->push_back(JSONNode("gradientTransform", Utils::ToString(matrix1, m_dataPrecision)));
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
            if (m_minify)
            {
                m_pathCmdStr.append(space);
            }

            if (segment.segmentType == DOM::Utils::LINE_SEGMENT)
            {
                m_pathCmdStr.append(SnapSVGAnimator::Utils::ToString(segment.line.endPoint1.x, m_dataPrecision));
                m_pathCmdStr.append(space);
                m_pathCmdStr.append(SnapSVGAnimator::Utils::ToString(segment.line.endPoint1.y, m_dataPrecision));
            }
            else
            {
                m_pathCmdStr.append(SnapSVGAnimator::Utils::ToString(segment.quadBezierCurve.anchor1.x, m_dataPrecision));
                m_pathCmdStr.append(space);
                m_pathCmdStr.append(SnapSVGAnimator::Utils::ToString(segment.quadBezierCurve.anchor1.y, m_dataPrecision));
            }

            if (!m_minify)
            {
                m_pathCmdStr.append(space);
            }

            m_firstSegment = false;
        }

        if (segment.segmentType == DOM::Utils::LINE_SEGMENT)
        {
            m_pathCmdStr.append(lineTo);
            if (!m_minify)
            {
                m_pathCmdStr.append(space);
            }

            float relPt = segment.line.endPoint2.x - segment.line.endPoint1.x;
            m_pathCmdStr.append(SnapSVGAnimator::Utils::ToString(relPt, m_dataPrecision));
            m_pathCmdStr.append(space);
            relPt = segment.line.endPoint2.y - segment.line.endPoint1.y;
            m_pathCmdStr.append(SnapSVGAnimator::Utils::ToString(relPt, m_dataPrecision));

            if (!m_minify)
            {
                m_pathCmdStr.append(space);
            }
        }
        else
        {
            m_pathCmdStr.append(bezierCurveTo);
            if (!m_minify)
            {
                m_pathCmdStr.append(space);
            }
            float relPt = segment.quadBezierCurve.control.x - segment.quadBezierCurve.anchor1.x;
            m_pathCmdStr.append(SnapSVGAnimator::Utils::ToString(relPt, m_dataPrecision));
            m_pathCmdStr.append(space);
            relPt = segment.quadBezierCurve.control.y - segment.quadBezierCurve.anchor1.y;
            m_pathCmdStr.append(SnapSVGAnimator::Utils::ToString(relPt, m_dataPrecision));
            m_pathCmdStr.append(space);
            relPt = segment.quadBezierCurve.anchor2.x - segment.quadBezierCurve.anchor1.x;
            m_pathCmdStr.append(SnapSVGAnimator::Utils::ToString(relPt, m_dataPrecision));
            m_pathCmdStr.append(space);
            relPt = segment.quadBezierCurve.anchor2.y - segment.quadBezierCurve.anchor1.y;
            m_pathCmdStr.append(SnapSVGAnimator::Utils::ToString(relPt, m_dataPrecision));
            if (!m_minify)
            {
                m_pathCmdStr.append(space);
            }
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
            m_pathElem->push_back(JSONNode("strokeWidth", 
                SnapSVGAnimator::Utils::ToString((double)m_strokeStyle.solidStrokeStyle.thickness, m_dataPrecision).c_str()));
            m_pathElem->push_back(JSONNode("fill", "none"));
            m_pathElem->push_back(JSONNode("strokeLinecap", Utils::ToString(m_strokeStyle.solidStrokeStyle.capStyle.type).c_str()));
            m_pathElem->push_back(JSONNode("strokeLinejoin", Utils::ToString(m_strokeStyle.solidStrokeStyle.joinStyle.type).c_str()));

            if (m_strokeStyle.solidStrokeStyle.joinStyle.type == DOM::Utils::MITER_JOIN)
            {
                m_pathElem->push_back(JSONNode(
                    "stroke-miterlimit", 
                    SnapSVGAnimator::Utils::ToString((double)m_strokeStyle.solidStrokeStyle.joinStyle.miterJoinProp.miterLimit,
                        m_dataPrecision).c_str()));
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
        const std::string& libPathName,
        DOM::LibraryItem::PIMediaItem pMediaItem)
    {
        FCM::Result res;
        JSONNode bitmapElem(JSON_NODE);
        std::string bitmapPath;
        std::string bitmapName;
        std::string name;

        bitmapElem.set_name("image");
        
        bitmapElem.push_back(JSONNode(("charid"), SnapSVGAnimator::Utils::ToString(resId)));
        bitmapElem.push_back(JSONNode(("height"), SnapSVGAnimator::Utils::ToString(height)));
        bitmapElem.push_back(JSONNode(("width"), SnapSVGAnimator::Utils::ToString(width)));

        FCM::AutoPtr<FCM::IFCMUnknown> pUnk;
        std::string bitmapRelPath;
        std::string bitmapExportPath = m_outputImageFolder + "/";
            
        FCM::Boolean alreadyExported = GetImageExportFileName(libPathName, name);
        if (!alreadyExported)
        {
            if (!m_imageFolderCreated)
            {
                res = Utils::CreateDir(m_outputImageFolder, m_pCallback);
                if (!(FCM_SUCCESS_CODE(res)))
                {
                    Utils::Trace(m_pCallback, "Output image folder (%s) could not be created\n", m_outputImageFolder.c_str());
                    return res;
                }
                m_imageFolderCreated = true;
            }
            CreateImageFileName(libPathName, name);
            SetImageExportFileName(libPathName, name);
        }
        
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

            pCalloc = SnapSVGAnimator::Utils::GetCallocService(m_pCallback);
            ASSERT(pCalloc.m_Ptr != NULL);

            pCalloc->Free(pFilePath);
        }

        bitmapElem.push_back(JSONNode(("bitmapPath"), bitmapRelPath)); 

        m_pBitmapArray->push_back(bitmapElem);

        return FCM_SUCCESS;
    }


    FCM::Result JSONOutputWriter::StartDefineClassicText(
        FCM::U_Int32 resId, 
        const DOM::FrameElement::AA_MODE_PROP& aaModeProp,
        const std::string& displayText,
        const TEXT_BEHAVIOUR& textBehaviour)
    {
        JSONNode aaMode(JSON_NODE);
        JSONNode behaviour(JSON_NODE);

        m_pTextElem = new JSONNode(JSON_NODE);
        ASSERT(m_pTextElem != NULL);

        m_pTextElem->set_name("text");
        m_pTextElem->push_back(JSONNode(("charid"), SnapSVGAnimator::Utils::ToString(resId)));

        aaMode.set_name("aaMode");
        aaMode.push_back(JSONNode(("mode"), SnapSVGAnimator::Utils::ToString(aaModeProp.aaMode)));
        if (aaModeProp.aaMode == DOM::FrameElement::ANTI_ALIAS_MODE_CUSTOM)
        {
            aaMode.push_back(JSONNode(("thickness"), 
                SnapSVGAnimator::Utils::ToString(aaModeProp.customAAModeProp.aaThickness, m_dataPrecision)));
            aaMode.push_back(JSONNode(("sharpness"), 
                SnapSVGAnimator::Utils::ToString(aaModeProp.customAAModeProp.aaSharpness, m_dataPrecision)));
        }
        m_pTextElem->push_back(aaMode);
        
        m_pTextElem->push_back(JSONNode(("txt"), displayText));

        behaviour.set_name("behaviour");

        if (textBehaviour.type == 0)
        {
            // Static Text
            behaviour.push_back(JSONNode(("type"), "Static"));
            behaviour.push_back(JSONNode(("flow"), SnapSVGAnimator::Utils::ToString(textBehaviour.u.staticText.flow)));
            behaviour.push_back(JSONNode(("orientation"), SnapSVGAnimator::Utils::ToString(textBehaviour.u.staticText.orientationMode)));
        }
        else if (textBehaviour.type == 1)
        {
            // Dynamic text
            behaviour.push_back(JSONNode(("type"), "Dynamic"));
            behaviour.push_back(JSONNode(("name"), textBehaviour.name));
            behaviour.push_back(JSONNode(("isBorderDrawn"), textBehaviour.u.dynamicText.borderDrawn ? "true" : "false"));
            behaviour.push_back(JSONNode(("lineMode"), SnapSVGAnimator::Utils::ToString(textBehaviour.u.dynamicText.lineMode)));
            behaviour.push_back(JSONNode(("isRenderAsHTML"), textBehaviour.u.dynamicText.renderAsHtml ? "true" : "false"));
            behaviour.push_back(JSONNode(("isScrollable"), textBehaviour.u.dynamicText.scrollable ? "true" : "false"));
        }
        else
        {
            // Input text
            behaviour.push_back(JSONNode(("type"), "Input"));
            behaviour.push_back(JSONNode(("name"), textBehaviour.name));
            behaviour.push_back(JSONNode(("isBorderDrawn"), textBehaviour.u.inputText.borderDrawn ? "true" : "false"));
            behaviour.push_back(JSONNode(("lineMode"), SnapSVGAnimator::Utils::ToString(textBehaviour.u.inputText.lineMode)));
            behaviour.push_back(JSONNode(("isRenderAsHTML"), textBehaviour.u.inputText.renderAsHtml ? "true" : "false"));
            behaviour.push_back(JSONNode(("isScrollable"), textBehaviour.u.inputText.scrollable ? "true" : "false"));
            behaviour.push_back(JSONNode(("isPassword"), textBehaviour.u.inputText.password ? "true" : "false"));
        }

        behaviour.push_back(JSONNode(("isSelectable"), textBehaviour.selectable  ? "true" : "false"));

        m_pTextElem->push_back(behaviour);

        // Start a paragraph array
        m_pTextParaArray = new JSONNode(JSON_ARRAY);
        ASSERT(m_pTextParaArray != NULL);

        m_pTextParaArray->set_name("paras");

        return FCM_SUCCESS;
    }


    FCM::Result JSONOutputWriter::StartDefineParagraph(
        FCM::U_Int32 startIndex,
        FCM::U_Int32 length,
        const DOM::FrameElement::PARAGRAPH_STYLE& paragraphStyle)
    {
        m_pTextPara = new JSONNode(JSON_NODE);
        ASSERT(m_pTextPara != NULL);

        m_pTextPara->push_back(JSONNode(("startIndex"), SnapSVGAnimator::Utils::ToString(startIndex)));
        m_pTextPara->push_back(JSONNode(("length"), SnapSVGAnimator::Utils::ToString(length)));
        m_pTextPara->push_back(JSONNode(("indent"), SnapSVGAnimator::Utils::ToString(paragraphStyle.indent)));
        m_pTextPara->push_back(JSONNode(("leftMargin"), SnapSVGAnimator::Utils::ToString(paragraphStyle.leftMargin)));
        m_pTextPara->push_back(JSONNode(("rightMargin"), SnapSVGAnimator::Utils::ToString(paragraphStyle.rightMargin)));
        m_pTextPara->push_back(JSONNode(("linespacing"), SnapSVGAnimator::Utils::ToString(paragraphStyle.lineSpacing)));
        m_pTextPara->push_back(JSONNode(("alignment"), SnapSVGAnimator::Utils::ToString(paragraphStyle.alignment)));

        m_pTextRunArray = new JSONNode(JSON_ARRAY);
        ASSERT(m_pTextRunArray != NULL);

        m_pTextRunArray->set_name("textRun");

        return FCM_SUCCESS;
    }


    FCM::Result JSONOutputWriter::StartDefineTextRun(
        FCM::U_Int32 startIndex,
        FCM::U_Int32 length,
        const TEXT_STYLE& textStyle)
    {
        JSONNode textRun(JSON_NODE);
        JSONNode style(JSON_NODE);

        textRun.push_back(JSONNode(("startIndex"), SnapSVGAnimator::Utils::ToString(startIndex)));
        textRun.push_back(JSONNode(("length"), SnapSVGAnimator::Utils::ToString(length)));

        style.set_name("style");
        style.push_back(JSONNode("fontName", textStyle.fontName));
        style.push_back(JSONNode("fontSize", SnapSVGAnimator::Utils::ToString(textStyle.fontSize)));
        style.push_back(JSONNode("fontColor", SnapSVGAnimator::Utils::ToString(textStyle.fontColor)));
        style.push_back(JSONNode("fontStyle", textStyle.fontStyle));
        style.push_back(JSONNode("letterSpacing", SnapSVGAnimator::Utils::ToString(textStyle.letterSpacing)));
        style.push_back(JSONNode("isRotated", textStyle.rotated ? "true" : "false"));
        style.push_back(JSONNode("isAutoKern", textStyle.autoKern ? "true" : "false"));
        style.push_back(JSONNode("baseLineShiftStyle", SnapSVGAnimator::Utils::ToString(textStyle.baseLineShiftStyle)));
        style.push_back(JSONNode("link", textStyle.link));
        style.push_back(JSONNode("linkTarget", textStyle.linkTarget));

        textRun.push_back(style);
        m_pTextRunArray->push_back(textRun);

        return FCM_SUCCESS;
    }


    FCM::Result JSONOutputWriter::EndDefineTextRun()
    {
        return FCM_SUCCESS;
    }


    FCM::Result JSONOutputWriter::EndDefineParagraph()
    {
        m_pTextPara->push_back(*m_pTextRunArray);
        delete m_pTextRunArray;
        m_pTextRunArray = NULL;

        m_pTextParaArray->push_back(*m_pTextPara);
        delete m_pTextPara;
        m_pTextPara = NULL;

        return FCM_SUCCESS;
    }


    FCM::Result JSONOutputWriter::EndDefineClassicText()
    {
        m_pTextElem->push_back(*m_pTextParaArray);

        delete m_pTextParaArray;
        m_pTextParaArray = NULL;

        m_pTextArray->push_back(*m_pTextElem);

        delete m_pTextElem;
        m_pTextElem = NULL;

        return FCM_SUCCESS;
    }


    FCM::Result JSONOutputWriter::DefineSound(
            FCM::U_Int32 resId, 
            const std::string& libPathName,
            DOM::LibraryItem::PIMediaItem pMediaItem)
    {
        FCM::Result res;
        JSONNode soundElem(JSON_NODE);
        std::string soundPath;
        std::string soundName;
        std::string name;

        soundElem.set_name("sound");
        soundElem.push_back(JSONNode(("charid"), SnapSVGAnimator::Utils::ToString(resId)));
        
        FCM::AutoPtr<FCM::IFCMUnknown> pUnk;
        std::string soundRelPath;
        std::string soundExportPath = m_outputSoundFolder + "/";

        if (!m_soundFolderCreated)
        {
            res = Utils::CreateDir(m_outputSoundFolder, m_pCallback);
            if (!(FCM_SUCCESS_CODE(res)))
            {
                Utils::Trace(m_pCallback, "Output sound folder (%s) could not be created\n", m_outputSoundFolder.c_str());
                return res;
            }
            m_soundFolderCreated = true;
        }
        
        CreateSoundFileName(libPathName, name);
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
            pCalloc = SnapSVGAnimator::Utils::GetCallocService(m_pCallback);
            ASSERT(pCalloc.m_Ptr != NULL);
            pCalloc->Free(pFilePath);
        }
        
        soundElem.push_back(JSONNode(("soundPath"), soundRelPath)); 
        m_pSoundArray->push_back(soundElem);
        
        return FCM_SUCCESS;
    }

    JSONOutputWriter::JSONOutputWriter(
        FCM::PIFCMCallback pCallback, 
        bool minify, 
        DataPrecision dataPrecision)
        : m_pCallback(pCallback),
          m_shapeElem(NULL),
          m_pathArray(NULL),
          m_pathElem(NULL),
          m_firstSegment(false),
          m_HTMLOutput(NULL),
          m_imageFileNameLabel(0),
          m_soundFileNameLabel(0),
          m_imageFolderCreated(false),
          m_soundFolderCreated(false),
          m_minify(minify),
          m_dataPrecision(dataPrecision)
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

        if (!m_minify)
        {
            m_pathCmdStr.append(space);
        }

        m_firstSegment = true;

        return FCM_SUCCESS;
    }

    FCM::Result JSONOutputWriter::EndDefinePath()
    {
        // No need to do anything
        return FCM_SUCCESS;
    }

    FCM::Result JSONOutputWriter::CreateImageFileName(const std::string& libPathName, std::string& name)
    {
        std::string str;
        size_t pos;
        std::string fileLabel;

        fileLabel = Utils::ToString(m_imageFileNameLabel);
        name = "Image" + fileLabel;
        m_imageFileNameLabel++;

        str = libPathName;

        // DOM APIs do not provide a way to get the compression of the image.
        // For time being, we will use the extension of the library item name.
        pos = str.rfind(".");
        if (pos != std::string::npos)
        {
            if (str.substr(pos + 1) == "jpg")
            {
                name += ".jpg";
            }
            else if (str.substr(pos + 1) == "png")
            {
                name += ".png";
            }
            else
            {
                name += ".png";
            }
        }
        else
        {
            name += ".png";
        }

        return FCM_SUCCESS;
    }


    FCM::Result JSONOutputWriter::CreateSoundFileName(const std::string& libPathName, std::string& name)
    {
        std::string str;
        size_t pos;
        std::string fileLabel;

        fileLabel = Utils::ToString(m_soundFileNameLabel);
        name = "Sound" + fileLabel;
        m_soundFileNameLabel++;

        str = libPathName;

        // DOM APIs do not provide a way to get the compression of the sound.
        // For time being, we will use the extension of the library item name.
        pos = str.rfind(".");
        if (pos != std::string::npos)
        {
            if (str.substr(pos + 1) == "wav")
            {
                name += ".WAV";
            }
            else if (str.substr(pos + 1) == "mp3")
            {
                name += ".MP3";
            }
            else
            {
                name += ".MP3";
            }
        }
        else
        {
            name += ".MP3";
        }

        return FCM_SUCCESS;
    }


    FCM::Boolean JSONOutputWriter::GetImageExportFileName(const std::string& libPathName, std::string& name)
    {
        std::map<std::string, std::string>::iterator it = m_imageMap.find(libPathName);

        name = "";

        if (it != m_imageMap.end())
        {
            // Image already exported
            name = it->second;
            return true;
        }

        return false;
    }


    void JSONOutputWriter::SetImageExportFileName(const std::string& libPathName, const std::string& name)
    {
        // Assumption: Name is not already present in the map
        ASSERT(m_imageMap.find(libPathName) == m_imageMap.end());

        m_imageMap.insert(std::pair<std::string, std::string>(libPathName, name));
    }

    /* -------------------------------------------------- JSONTimelineWriter */

    FCM::Result JSONTimelineWriter::PlaceObject(
        FCM::U_Int32 resId,
        FCM::U_Int32 objectId,
        FCM::U_Int32 placeAfterObjectId,
        const DOM::Utils::MATRIX2D* pMatrix,
        const DOM::Utils::RECT* pRect /* = NULL */)
    {
        JSONNode commandElement(JSON_NODE);

        commandElement.push_back(JSONNode("cmdType", "Place"));
        commandElement.push_back(JSONNode("charid", SnapSVGAnimator::Utils::ToString(resId)));
        commandElement.push_back(JSONNode("objectId", SnapSVGAnimator::Utils::ToString(objectId)));
        commandElement.push_back(JSONNode("placeAfter", SnapSVGAnimator::Utils::ToString(placeAfterObjectId)));

        if (pMatrix)
        {
            commandElement.push_back(JSONNode("transformMatrix", Utils::ToString(*pMatrix, m_dataPrecision).c_str()));
        }

        if (pRect)
        {
            commandElement.push_back(JSONNode("bounds", Utils::ToString(*pRect, m_dataPrecision).c_str()));
        }

        m_pCommandArray->push_back(commandElement);

        return FCM_SUCCESS;
    }


    FCM::Result JSONTimelineWriter::PlaceObject(
        FCM::U_Int32 resId,
        FCM::U_Int32 objectId,
        FCM::U_Int32 placeAfterObjectId,
        const DOM::Utils::MATRIX2D* pMatrix,
        FCM::Boolean loop,
        FCM::PIFCMUnknown pUnknown)
    {
        JSONNode commandElement(JSON_NODE);

        commandElement.push_back(JSONNode("cmdType", "Place"));
        commandElement.push_back(JSONNode("charid", SnapSVGAnimator::Utils::ToString(resId)));
        commandElement.push_back(JSONNode("objectId", SnapSVGAnimator::Utils::ToString(objectId)));
        commandElement.push_back(JSONNode("placeAfter", SnapSVGAnimator::Utils::ToString(placeAfterObjectId)));

        if (pMatrix)
        {
            commandElement.push_back(JSONNode("transformMatrix", Utils::ToString(*pMatrix, m_dataPrecision).c_str()));
        }

        commandElement.push_back(JSONNode("loop", loop ? "true" : "false"));
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
        commandElement.push_back(JSONNode("charid", SnapSVGAnimator::Utils::ToString(resId)));
        commandElement.push_back(JSONNode("objectId", SnapSVGAnimator::Utils::ToString(objectId)));

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
                SnapSVGAnimator::Utils::ToString(lMode.loopMode)));
            commandElement.push_back(JSONNode("repeatCount", 
                SnapSVGAnimator::Utils::ToString(lMode.repeatCount)));

            res = pSound->GetSyncMode(syncMode);
            ASSERT(FCM_SUCCESS_CODE(res));

            commandElement.push_back(JSONNode("syncMode", 
                SnapSVGAnimator::Utils::ToString(syncMode)));

            // We should not get SOUND_SYNC_STOP as for stop, "RemoveObject" command will
            // be generated by Exporter Service.
            ASSERT(syncMode != DOM::FrameElement::SOUND_SYNC_STOP); 

            res = pSound->GetSoundLimit(soundLimit);
            ASSERT(FCM_SUCCESS_CODE(res));

            commandElement.push_back(JSONNode("LimitInPos44", 
                SnapSVGAnimator::Utils::ToString(soundLimit.inPos44)));
            commandElement.push_back(JSONNode("LimitOutPos44", 
                SnapSVGAnimator::Utils::ToString(soundLimit.outPos44)));
        }

        m_pCommandArray->push_back(commandElement);

        return res;
    }


    FCM::Result JSONTimelineWriter::RemoveObject(
        FCM::U_Int32 objectId)
    {
        JSONNode commandElement(JSON_NODE);

        commandElement.push_back(JSONNode("cmdType", "Remove"));
        commandElement.push_back(JSONNode("objectId", SnapSVGAnimator::Utils::ToString(objectId)));

        m_pCommandArray->push_back(commandElement);

        return FCM_SUCCESS;
    }


    FCM::Result JSONTimelineWriter::UpdateZOrder(
        FCM::U_Int32 objectId,
        FCM::U_Int32 placeAfterObjectId)
    {
        JSONNode commandElement(JSON_NODE);

        commandElement.push_back(JSONNode("cmdType", "UpdateZOrder"));
        commandElement.push_back(JSONNode("objectId", SnapSVGAnimator::Utils::ToString(objectId)));
        commandElement.push_back(JSONNode("placeAfter", SnapSVGAnimator::Utils::ToString(placeAfterObjectId)));

        m_pCommandArray->push_back(commandElement);

        return FCM_SUCCESS;
    }


    FCM::Result JSONTimelineWriter::UpdateMask(
        FCM::U_Int32 objectId,
        FCM::U_Int32 maskTillObjectId)
    {
        MaskInfo info;

        info.maskTillObjectId = maskTillObjectId;
        info.objectId = objectId;

        maskInfoList.push_back(info);

        return FCM_SUCCESS;
    }

    FCM::Result JSONTimelineWriter::DeferUpdateMask(
        FCM::U_Int32 objectId,
        FCM::U_Int32 maskTillObjectId)
    {
        JSONNode commandElement(JSON_NODE);

        commandElement.push_back(JSONNode("cmdType", "UpdateMask"));
        commandElement.push_back(JSONNode("objectId", SnapSVGAnimator::Utils::ToString(objectId)));
        commandElement.push_back(JSONNode("maskTill", SnapSVGAnimator::Utils::ToString(maskTillObjectId)));

        m_pCommandArray->push_back(commandElement);
        
        return FCM_SUCCESS;
    }

    FCM::Result JSONTimelineWriter::DeferUpdateMasks()
    {
        JSONNode commandElement(JSON_NODE);

        for (FCM::U_Int32 i = 0; i < maskInfoList.size(); i++)
        {
            MaskInfo& info = maskInfoList.at(i);
            DeferUpdateMask(info.objectId, info.maskTillObjectId);
        }
        
        maskInfoList.clear();

        return FCM_SUCCESS;
    }

    FCM::Result JSONTimelineWriter::UpdateBlendMode(
        FCM::U_Int32 objectId,
        DOM::FrameElement::BlendMode blendMode)
    {
        JSONNode commandElement(JSON_NODE);

        commandElement.push_back(JSONNode("cmdType", "UpdateBlendMode"));
        commandElement.push_back(JSONNode("objectId", SnapSVGAnimator::Utils::ToString(objectId)));
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
        commandElement.push_back(JSONNode("objectId", SnapSVGAnimator::Utils::ToString(objectId)));

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
        commandElement.push_back(JSONNode("objectId", SnapSVGAnimator::Utils::ToString(objectId)));
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
            commandElement.push_back(JSONNode("angle", 
                SnapSVGAnimator::Utils::ToString((double)angle, m_dataPrecision)));

            res = pDropShadowFilter->GetBlurX(blurX);
            ASSERT(FCM_SUCCESS_CODE(res));
            commandElement.push_back(JSONNode("blurX", 
                SnapSVGAnimator::Utils::ToString((double)blurX, m_dataPrecision)));

            res = pDropShadowFilter->GetBlurY(blurY);
            ASSERT(FCM_SUCCESS_CODE(res));
            commandElement.push_back(JSONNode("blurY", 
                SnapSVGAnimator::Utils::ToString((double)blurY, m_dataPrecision)));

            res = pDropShadowFilter->GetDistance(distance);
            ASSERT(FCM_SUCCESS_CODE(res));
            commandElement.push_back(JSONNode("distance", 
                SnapSVGAnimator::Utils::ToString((double)distance, m_dataPrecision)));

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
            commandElement.push_back(JSONNode("strength", SnapSVGAnimator::Utils::ToString(strength)));

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
            commandElement.push_back(JSONNode("blurX", 
                SnapSVGAnimator::Utils::ToString((double)blurX, m_dataPrecision)));

            res = pBlurFilter->GetBlurY(blurY);
            ASSERT(FCM_SUCCESS_CODE(res));
            commandElement.push_back(JSONNode("blurY", 
                SnapSVGAnimator::Utils::ToString((double)blurY, m_dataPrecision)));

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
            commandElement.push_back(JSONNode("blurX", 
                SnapSVGAnimator::Utils::ToString((double)blurX, m_dataPrecision)));

            res = pGlowFilter->GetBlurY(blurY);
            ASSERT(FCM_SUCCESS_CODE(res));
            commandElement.push_back(JSONNode("blurY", 
                SnapSVGAnimator::Utils::ToString((double)blurY, m_dataPrecision)));

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
            commandElement.push_back(JSONNode("strength", SnapSVGAnimator::Utils::ToString(strength)));

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
            commandElement.push_back(JSONNode("angle", 
                SnapSVGAnimator::Utils::ToString((double)angle, m_dataPrecision)));

            res = pBevelFilter->GetBlurX(blurX);
            ASSERT(FCM_SUCCESS_CODE(res));
            commandElement.push_back(JSONNode("blurX", 
                SnapSVGAnimator::Utils::ToString((double)blurX, m_dataPrecision)));

            res = pBevelFilter->GetBlurY(blurY);
            ASSERT(FCM_SUCCESS_CODE(res));
            commandElement.push_back(JSONNode("blurY", 
                SnapSVGAnimator::Utils::ToString((double)blurY, m_dataPrecision)));

            res = pBevelFilter->GetDistance(distance);
            ASSERT(FCM_SUCCESS_CODE(res));
            commandElement.push_back(JSONNode("distance", 
                SnapSVGAnimator::Utils::ToString((double)distance, m_dataPrecision)));

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
            commandElement.push_back(JSONNode("strength", SnapSVGAnimator::Utils::ToString(strength)));

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
            commandElement.push_back(JSONNode("angle", 
                SnapSVGAnimator::Utils::ToString((double)angle, m_dataPrecision)));

            res = pGradientGlowFilter->GetBlurX(blurX);
            ASSERT(FCM_SUCCESS_CODE(res));
            commandElement.push_back(JSONNode("blurX", 
                SnapSVGAnimator::Utils::ToString((double)blurX, m_dataPrecision)));

            res = pGradientGlowFilter->GetBlurY(blurY);
            ASSERT(FCM_SUCCESS_CODE(res));
            commandElement.push_back(JSONNode("blurY", 
                SnapSVGAnimator::Utils::ToString((double)blurY, m_dataPrecision)));

            res = pGradientGlowFilter->GetDistance(distance);
            ASSERT(FCM_SUCCESS_CODE(res));
            commandElement.push_back(JSONNode("distance", 
                SnapSVGAnimator::Utils::ToString((double)distance, m_dataPrecision)));

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
            commandElement.push_back(JSONNode("strength", SnapSVGAnimator::Utils::ToString(strength)));

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

                    stopEntry.push_back(JSONNode("offset", Utils::ToString((float) offset, m_dataPrecision)));
                    stopEntry.push_back(JSONNode("stopColor", Utils::ToString(colorPoint.color)));
                    stopEntry.push_back(JSONNode("stopOpacity", 
                        Utils::ToString((float)(colorPoint.color.alpha / 255.0), m_dataPrecision)));
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
            commandElement.push_back(JSONNode("angle", 
                SnapSVGAnimator::Utils::ToString((double)angle, m_dataPrecision)));

            res = pGradientBevelFilter->GetBlurX(blurX);
            ASSERT(FCM_SUCCESS_CODE(res));
            commandElement.push_back(JSONNode("blurX", 
                SnapSVGAnimator::Utils::ToString((double)blurX, m_dataPrecision)));

            res = pGradientBevelFilter->GetBlurY(blurY);
            ASSERT(FCM_SUCCESS_CODE(res));
            commandElement.push_back(JSONNode("blurY", 
                SnapSVGAnimator::Utils::ToString((double)blurY, m_dataPrecision)));

            res = pGradientBevelFilter->GetDistance(distance);
            ASSERT(FCM_SUCCESS_CODE(res));
            commandElement.push_back(JSONNode("distance", 
                SnapSVGAnimator::Utils::ToString((double)distance, m_dataPrecision)));

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
            commandElement.push_back(JSONNode("strength", SnapSVGAnimator::Utils::ToString(strength)));

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

                    stopEntry.push_back(JSONNode("offset", 
                        Utils::ToString((float) offset, m_dataPrecision)));
                    stopEntry.push_back(JSONNode("stopColor", Utils::ToString(colorPoint.color)));
                    stopEntry.push_back(JSONNode("stopOpacity", 
                        Utils::ToString((float)(colorPoint.color.alpha / 255.0), m_dataPrecision)));
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
            commandElement.push_back(JSONNode("brightness", 
                SnapSVGAnimator::Utils::ToString((double)brightness, m_dataPrecision)));

            res = pAdjustColorFilter->GetContrast(contrast);
            ASSERT(FCM_SUCCESS_CODE(res));
            commandElement.push_back(JSONNode("contrast", 
                SnapSVGAnimator::Utils::ToString((double)contrast, m_dataPrecision)));

            res = pAdjustColorFilter->GetSaturation(saturation);
            ASSERT(FCM_SUCCESS_CODE(res));
            commandElement.push_back(JSONNode("saturation", 
                SnapSVGAnimator::Utils::ToString((double)saturation, m_dataPrecision)));

            res = pAdjustColorFilter->GetHue(hue);
            ASSERT(FCM_SUCCESS_CODE(res));
            commandElement.push_back(JSONNode("hue", 
                SnapSVGAnimator::Utils::ToString((double)hue, m_dataPrecision)));
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
        commandElement.push_back(JSONNode("objectId", SnapSVGAnimator::Utils::ToString(objectId)));
        transformMat = SnapSVGAnimator::Utils::ToString(matrix, m_dataPrecision);
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
        commandElement.push_back(JSONNode("objectId", SnapSVGAnimator::Utils::ToString(objectId)));
        colorMat = SnapSVGAnimator::Utils::ToString(colorMatrix, m_dataPrecision);
        commandElement.push_back(JSONNode("colorMatrix", colorMat.c_str()));

        m_pCommandArray->push_back(commandElement);

        return FCM_SUCCESS;
    }


    FCM::Result JSONTimelineWriter::ShowFrame(FCM::U_Int32 frameNum)
    {
        DeferUpdateMasks();

        if (!m_pCommandArray->empty())
        {
            m_pFrameElement->push_back(JSONNode(("num"), SnapSVGAnimator::Utils::ToString(frameNum)));
            m_pFrameElement->push_back(*m_pCommandArray);
            m_pFrameArray->push_back(*m_pFrameElement);
        }

        m_FrameCount++;

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
        // As frame script is not supported, let us disable it.
#if 0
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
#endif

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


    JSONTimelineWriter::JSONTimelineWriter(
        FCM::PIFCMCallback pCallback,
        DataPrecision dataPrecision) :
        m_pCallback(pCallback),
        m_dataPrecision(dataPrecision)
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

        m_FrameCount = 0;
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
                SnapSVGAnimator::Utils::ToString(resId)));
        }

        m_pTimelineElement->push_back(
                JSONNode(("frameCount"), 
                SnapSVGAnimator::Utils::ToString(m_FrameCount)));

        m_pTimelineElement->push_back(*m_pFrameArray);
    }

};
