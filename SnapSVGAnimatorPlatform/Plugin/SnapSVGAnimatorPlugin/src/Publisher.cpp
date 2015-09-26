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

#include "Publisher.h"
#include "Utils.h"
#include "HTTPServer.h"
#include "FlashFCMPublicIDs.h"

#include "FrameElement/IShape.h"

#include "StrokeStyle/IDashedStrokeStyle.h"
#include "StrokeStyle/IDottedStrokeStyle.h"
#include "StrokeStyle/IHatchedStrokeStyle.h"
#include "StrokeStyle/IRaggedStrokeStyle.h"
#include "StrokeStyle/ISolidStrokeStyle.h"
#include "StrokeStyle/IStippleStrokeStyle.h"
#include "StrokeStyle/IStrokeWidth.h"

#include "FillStyle/ISolidFillStyle.h"
#include "FillStyle/IGradientFillStyle.h"
#include "FillStyle/IBitmapFillStyle.h"


#include "MediaInfo/IBitmapInfo.h"
#include "FrameElement/IBitmapInstance.h"
#include "FrameElement/ISound.h"
#include "MediaInfo/ISoundInfo.h"
#include "LibraryItem/IMediaItem.h"
#include "LibraryItem/IFolderItem.h"
#include "LibraryItem/IFontItem.h"
#include "LibraryItem/ISymbolItem.h"
#include "ILibraryItem.h"

#include "FrameElement/IButton.h"
#include "FrameElement/IClassicText.h"
#include "FrameElement/ITextStyle.h"
#include "FrameElement/IParagraph.h"
#include "FrameElement/ITextRun.h"
#include "FrameElement/ITextBehaviour.h"

#include "Service/Shape/IRegionGeneratorService.h"
#include "Service/Shape/IFilledRegion.h"
#include "Service/Shape/IStrokeGroup.h"
#include "Service/Shape/IPath.h"
#include "Service/Shape/IEdge.h"
#include "Service/Shape/IShapeService.h"
#include "Service/Image/IBitmapExportService.h"


#include "Utils/DOMTypes.h"
#include "Utils/ILinearColorGradient.h"
#include "Utils/IRadialColorGradient.h"

#include "OutputWriter.h"

#include "Exporter/Service/IResourcePalette.h"
#include "Exporter/Service/ITimelineBuilder2.h"
#include "Exporter/Service/ITimelineBuilderFactory.h"

#include "Exporter/Service/ISWFExportService.h"
#include <algorithm>
#include "PluginConfiguration.h"

namespace SnapSVGAnimator
{

    /* ----------------------------------------------------- CPublisher */
    
    CPublisher::CPublisher()
    {

    }

    CPublisher::~CPublisher()
    {

    }


    FCM::Result CPublisher::Publish(
        DOM::PIFLADocument pFlaDocument, 
        const PIFCMDictionary pDictPublishSettings, 
        const PIFCMDictionary pDictConfig)
    {
        return Export(pFlaDocument, NULL, NULL, pDictPublishSettings, pDictConfig);
    }

    // This function will be currently called in "Test-Scene" workflow. 
    // In future, it might be called in other workflows as well. 
    FCM::Result CPublisher::Publish(
        DOM::PIFLADocument pFlaDocument, 
        DOM::PITimeline pTimeline, 
        const Exporter::Service::RANGE &frameRange, 
        const PIFCMDictionary pDictPublishSettings, 
        const PIFCMDictionary pDictConfig)
    {
        return Export(pFlaDocument, pTimeline, &frameRange, pDictPublishSettings, pDictConfig);
    }


    FCM::Result CPublisher::Export(
        DOM::PIFLADocument pFlaDocument, 
        DOM::PITimeline pTimeline, 
        const Exporter::Service::RANGE* pFrameRange, 
        const PIFCMDictionary pDictPublishSettings, 
        const PIFCMDictionary pDictConfig)
    {
        std::string outFile;
        FCM::Result res;
        FCM::FCMGUID guid;
        FCM::AutoPtr<FCM::IFCMUnknown> pUnk;
        FCM::AutoPtr<FCM::IFCMCalloc> pCalloc;

        Init();

        pCalloc = SnapSVGAnimator::Utils::GetCallocService(GetCallback());
        ASSERT(pCalloc.m_Ptr != NULL);

        res = pFlaDocument->GetTypeId(guid);
        ASSERT(FCM_SUCCESS_CODE(res));

        std::string pub_guid = Utils::ToString(guid);
        Utils::Trace(GetCallback(), "Publishing begins for document with GUID: %s\n", 
            pub_guid.c_str());

        res = GetOutputFileName(pFlaDocument, pTimeline, pDictPublishSettings, outFile);
        if (FCM_FAILURE_CODE(res))
        {
            // FLA is untitled. Ideally, we should use a temporary location for output generation.
            // However, for now, we report an error.
            Utils::Trace(GetCallback(), "Failed to publish. Either save the FLA or provide output path in publish settings.\n");
            return res;
        }

        Utils::Trace(GetCallback(), "Creating output file : %s\n", outFile.c_str());

#ifdef USE_SWF_EXPORTER_SERVICE

        // Use the SWF Exporter Service to export to a SWF
        
        res = GetCallback()->GetService(Exporter::Service::EXPORTER_SWF_SERVICE, pUnk.m_Ptr);
        if (FCM_FAILURE_CODE(res))
        {
            return res;
        }
        
        FCM::StringRep16 outputFilePath = Utils::ToString16(outFile, GetCallback());

        FCM::AutoPtr<Exporter::Service::ISWFExportService> pSWfExportService = pUnk;

        if (!pTimeline)
        {
            // Export complete document
            res = pSWfExportService->ExportToFile(pFlaDocument, outputFilePath);
        }
        else
        {
            // Export only specified timeline
            res = pSWfExportService->ExportToFile(pTimeline, outputFilePath);
        }
        pCalloc->Free(outputFilePath);
        if (FCM_SUCCESS_CODE(res))
        {
            return res;
        }
        
        // Post-process the SWF

#else
        DOM::Utils::COLOR color;
        FCM::U_Int32 stageHeight;
        FCM::U_Int32 stageWidth;
        FCM::Double fps;
        FCM::U_Int32 framesPerSec;
        FCM::Boolean overwriteHTML = true;
        AutoPtr<ITimelineBuilderFactory> pTimelineBuilderFactory;
        FCM::FCMListPtr pTimelineList;
        FCM::U_Int32 timelineCount;

        
        // Read the overwriteHTML option from the publish settings
        {
            std::string str;
            Utils::ReadString(pDictPublishSettings, (FCM::StringRep8)DICT_OVERWRITE_HTML_FILE_KEY, str);
            if (!str.empty())
            {
                overwriteHTML = Utils::ToBool(str);
            }
        }

        // Read the minify option from the publish settings
        {
            std::string str;
            m_minify = true;
            Utils::ReadString(pDictPublishSettings, (FCM::StringRep8)DICT_MINIFY_KEY, str);
            if (!str.empty())
            {
                m_minify = Utils::ToBool(str);
            }
        }

        // Read the precision option from the publish settings
        DataPrecision precision = PRECISION_3;
        {
            std::string str;

            Utils::ReadString(pDictPublishSettings, (FCM::StringRep8)DICT_COMPACT_DATA_KEY, str);
            if (!str.empty())
            {
                bool isCompactData = Utils::ToBool(str);
                if (isCompactData)
                {
                    Utils::ReadString(pDictPublishSettings, (FCM::StringRep8)DICT_COMPACT_DATA_OPT_KEY, str);
                    precision = Utils::ToPrecision(str);
                }
                else
                {
                    // User does not want to compact data. Use highest precision.
                    precision = PRECISION_6;
                }
            }
        }

        // Create a output writer
        std::auto_ptr<IOutputWriter> pOutputWriter(new JSONOutputWriter(GetCallback(), m_minify, precision, overwriteHTML));
        if (pOutputWriter.get() == NULL)
        {
            return FCM_MEM_NOT_AVAILABLE;
        }
        
        // Start output
        pOutputWriter->StartOutput(outFile);

        // Create a Timeline Builder Factory for the root timeline of the document
        res = GetCallback()->CreateInstance(
            NULL, 
            CLSID_TimelineBuilderFactory, 
            IID_ITimelineBuilderFactory, 
            (void**)&pTimelineBuilderFactory);
        if (FCM_FAILURE_CODE(res))
        {
            return res;
        }

        (static_cast<TimelineBuilderFactory*>(pTimelineBuilderFactory.m_Ptr))->Init(
            pOutputWriter.get(), precision);

        ResourcePalette* pResPalette = static_cast<ResourcePalette*>(m_pResourcePalette.m_Ptr);
        pResPalette->Clear();
        pResPalette->Init(pOutputWriter.get());

        res = pFlaDocument->GetBackgroundColor(color);
        ASSERT(FCM_SUCCESS_CODE(res));

        res = pFlaDocument->GetStageHeight(stageHeight);
        ASSERT(FCM_SUCCESS_CODE(res));

        res = pFlaDocument->GetStageWidth(stageWidth);
        ASSERT(FCM_SUCCESS_CODE(res));

        res = pFlaDocument->GetFrameRate(fps);
        ASSERT(FCM_SUCCESS_CODE(res));

        framesPerSec = (FCM::U_Int32)fps;

        res = pOutputWriter->StartDocument(color, stageHeight, stageWidth, framesPerSec);
        ASSERT(FCM_SUCCESS_CODE(res));

        // Export complete document ?
        if (!pTimeline)
        {
            // Get all the timelines for the document
            res = pFlaDocument->GetTimelines(pTimelineList.m_Ptr);
            if (FCM_FAILURE_CODE(res))
            {
                return res;
            }

            res = pTimelineList->Count(timelineCount);
            if (FCM_FAILURE_CODE(res))
            {
                return res;
            }

            /*
             * IMPORTANT NOTE: 
             *  
             * For the current sample plugin, multiple scene export is not supported.
             * Supporting export of multiple scene is little tricky. This is due to
             * the fact that IFrameCommandGenerator::GenerateFrameCommands() expects
             * that a new empty resource palette is passed for each timeline.
             *
             * In other words, for each timeline (scene), a resource palette is exported.
             * So, if a resource is present in 2 scenes, its definition will be replicated 
             * in both the resource palettes. Plugin can choose to optimize by comparing the
             * resource names to find the common resources and put it in a global resource 
             * palette and also modifying the timeline builder commands to use the new 
             * resource ids.
             *
             * For our current implementation, we chosen to keep it simple and not support
             * multiple scenes. For this plugin to work, the feature "Scene" must be disabled 
             * by the corresponding DocType.
             */
            ASSERT(timelineCount == 1);

            // Generate frame commands for each timeline
            for (FCM::U_Int32 i = 0; i < timelineCount; i++)
            {
                Exporter::Service::RANGE range;
                AutoPtr<ITimelineBuilder> pTimelineBuilder;
                ITimelineWriter* pTimelineWriter;

                AutoPtr<DOM::ITimeline> timeline = pTimelineList[i];

                range.min = 0;
                res = timeline->GetMaxFrameCount(range.max);
                if (FCM_FAILURE_CODE(res))
                {
                    return res;
                }

                range.max--;

                // Generate frame commands
                res = m_frameCmdGeneratorService->GenerateFrameCommands(
                    timeline, 
                    range, 
                    pDictPublishSettings,
                    m_pResourcePalette, 
                    pTimelineBuilderFactory, 
                    pTimelineBuilder.m_Ptr);

                if (FCM_FAILURE_CODE(res))
                {
                    return res;
                }

                ((TimelineBuilder*)pTimelineBuilder.m_Ptr)->Build(0, NULL, &pTimelineWriter);
            }

            res = pOutputWriter->EndDocument();
            ASSERT(FCM_SUCCESS_CODE(res));

            res = pOutputWriter->EndOutput();
            ASSERT(FCM_SUCCESS_CODE(res));

            // Export the library items with linkages
            FCM::FCMListPtr pLibraryItemList;
            res = pFlaDocument->GetLibraryItems(pLibraryItemList.m_Ptr);
            if (FCM_FAILURE_CODE(res))
            {
                return res;
            }

            ExportLibraryItems(pLibraryItemList);
        }
        else
        {
            // Export a timeline
            AutoPtr<ITimelineBuilder> pTimelineBuilder;
            ITimelineWriter* pTimelineWriter;

            // Generate frame commands
            res = m_frameCmdGeneratorService->GenerateFrameCommands(
                pTimeline, 
                *pFrameRange, 
                pDictPublishSettings,
                m_pResourcePalette, 
                pTimelineBuilderFactory, 
                pTimelineBuilder.m_Ptr);

            if (FCM_FAILURE_CODE(res))
            {
                return res;
            }

            ((TimelineBuilder*)pTimelineBuilder.m_Ptr)->Build(0, NULL, &pTimelineWriter);

            res = pOutputWriter->EndDocument();
            ASSERT(FCM_SUCCESS_CODE(res));

            res = pOutputWriter->EndOutput();
            ASSERT(FCM_SUCCESS_CODE(res));
        }
        
        // Stop preview
        StopPreview();

#ifdef USE_RUNTIME

        // We are now going to copy the runtime from the zxp package to the output folder.
        std::string outFolder;
        
        Utils::GetParent(outFile, outFolder);

        CopyRuntime(outFolder);

#endif
        if (IsPreviewNeeded(pDictConfig))
        {
            StartPreview(outFile);
        }

#endif
        return FCM_SUCCESS;
    }


    FCM::Result CPublisher::ClearCache()
    {
        if (m_pResourcePalette)
        {
            ResourcePalette* pResPalette = static_cast<ResourcePalette*>(m_pResourcePalette.m_Ptr);

            pResPalette->Clear();
        }
        return FCM_SUCCESS;
    }


    FCM::Result CPublisher::GetOutputFileName(        
        DOM::PIFLADocument pFlaDocument, 
        DOM::PITimeline pTimeline, 
        const PIFCMDictionary pDictPublishSettings,
        std::string& outFile)
    {
        FCM::Result res = FCM_SUCCESS;
        FCM::AutoPtr<FCM::IFCMUnknown> pUnk;
        FCM::AutoPtr<FCM::IFCMCalloc> pCalloc;

        pCalloc = SnapSVGAnimator::Utils::GetCallocService(GetCallback());
        ASSERT(pCalloc.m_Ptr != NULL);

        // Read the output file name from the publish settings
        Utils::ReadString(pDictPublishSettings, (FCM::StringRep8)DICT_OUT_FILE_KEY, outFile);
        if (outFile.empty())
        {
            FCM::StringRep16 path;
            std::string filePath;
            std::string parent;
            std::string ext;

            res = pFlaDocument->GetPath(&path);
            ASSERT(FCM_SUCCESS_CODE(res));

            if (path)
            {
                filePath = Utils::ToString(path, GetCallback());
                Utils::GetFileNameWithoutExtension(filePath, outFile);
            }
            else
            {
                res = Utils::GetAppTempDir(GetCallback(), filePath);
                if (!FCM_SUCCESS_CODE(res))
                {
                    return FCM_INVALID_PARAM;
                }
                outFile = "Untitled";
                outFile += Utils::ToString(rand() % 65536);
                filePath += "Untitled.fla";
            }

            if (pTimeline)
            {
                FCM::StringRep16 pSceneName;
                std::string sceneName;

                res = pTimeline->GetName(&pSceneName);
                ASSERT(FCM_SUCCESS_CODE(res));

                sceneName = Utils::ToString(pSceneName, GetCallback());

                outFile += "_";
                outFile += sceneName;
            }

            outFile += ".";
            outFile += OUTPUT_FILE_EXTENSION;

            Utils::GetFileExtension(filePath, ext);
                
            // Convert the extension to lower case and then compare
            std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
            if (ext.compare("xfl") == 0)
            {
                std::string immParent;
                Utils::GetParent(filePath, immParent);
                Utils::GetParent(immParent, parent);
            }
            else
            {
                // FLA
                Utils::GetParent(filePath, parent);
            }

            // Extract the extension and append output file extension.
            outFile = parent + outFile;

            pCalloc->Free(path);

        }

        return res;
    }


    FCM::Boolean CPublisher::IsPreviewNeeded(const PIFCMDictionary pDictConfig)
    {
        FCM::Boolean found;
        std::string previewNeeded;
        FCM::Boolean res;

        found = Utils::ReadString(pDictConfig, (FCM::StringRep8)kPublishSettingsKey_PreviewNeeded, previewNeeded);

        res = true;
        if (found)
        {
            if (previewNeeded == "true")
            {
                res = true;
            }
            else
            {
                res = false;
            }
        }
        return res;
    }
    
    FCM::Result CPublisher::StopPreview()
    {
        FCM::Result res = FCM_SUCCESS;
        
#ifdef USE_HTTP_SERVER

        HTTPServer* server;
        
        server = HTTPServer::GetInstance();
        if (server)
        {
            // Stop the web server just in case it is running
            server->Stop();
        }
        
#endif // USE_HTTP_SERVER
        
        return res;
    }


    FCM::Result CPublisher::StartPreview(const std::string& outFile)
    {
        FCM::Result res = FCM_SUCCESS;

#ifdef USE_HTTP_SERVER

        // We are now about to start a web server
        std::string fileName;
        HTTPServer* server;
        ServerConfigParam config;

        Utils::GetFileName(outFile, fileName);

        server = HTTPServer::GetInstance();

        int numTries = 0;
        while (numTries < MAX_RETRY_ATTEMPT)
        {
            // Configure the web server
            config.port = Utils::GetUnusedLocalPort();
            Utils::GetParent(outFile, config.root);
            server->SetConfig(config);

            // Start the web server
            res = server->Start();
            if (FCM_SUCCESS_CODE(res))
            {
                // Launch the browser
                Utils::LaunchBrowser(fileName, config.port, GetCallback());
                break;
            }
            numTries++;
        }

        if (numTries == MAX_RETRY_ATTEMPT)
        {
            Utils::Trace(GetCallback(), "Failed to start web server\n");
            res = FCM_GENERAL_ERROR;
        }

#endif // USE_HTTP_SERVER

        return res;
    }


    FCM::Result CPublisher::Init()
    {
        FCM::Result res = FCM_SUCCESS;;
        FCM::AutoPtr<FCM::IFCMUnknown> pUnk;

        if (!m_frameCmdGeneratorService)
        {
            // Get the frame command generator service
            res = GetCallback()->GetService(Exporter::Service::EXPORTER_FRAME_CMD_GENERATOR_SERVICE, pUnk.m_Ptr);
            m_frameCmdGeneratorService = pUnk;
        }

        if (!m_pResourcePalette)
        {
            // Create a Resource Palette
            res = GetCallback()->CreateInstance(NULL, CLSID_ResourcePalette, IID_IResourcePalette, (void**)&m_pResourcePalette);
            ASSERT(FCM_SUCCESS_CODE(res));
        }

        return res;
    }


    //
    // Note: This function is NOT completely implemented but provides guidelines 
    // on how this can be possibly done.      
    //
    FCM::Result CPublisher::ExportLibraryItems(FCM::FCMListPtr pLibraryItemList)
    {
        FCM::U_Int32 count = 0;
        FCM::Result res;


        ASSERT(pLibraryItemList);

        res = pLibraryItemList->Count(count);
        ASSERT(FCM_SUCCESS_CODE(res));

        FCM::AutoPtr<FCM::IFCMUnknown> pUnkCalloc;
        res = GetCallback()->GetService(SRVCID_Core_Memory, pUnkCalloc.m_Ptr);
        AutoPtr<FCM::IFCMCalloc> callocService  = pUnkCalloc;

        for (FCM::U_Int32 index = 0; index < count ; index++)
        {
            FCM::StringRep16 pLibItemName = NULL;
            std::string libItemName;
            AutoPtr<IFCMDictionary> pDict;
            AutoPtr<DOM::ILibraryItem> pLibItem = pLibraryItemList[index];

            res = pLibItem->GetName(&pLibItemName);
            ASSERT(FCM_SUCCESS_CODE(res));
            libItemName = Utils::ToString(pLibItemName, GetCallback());

            AutoPtr<DOM::LibraryItem::IFolderItem> pFolderItem = pLibItem;
            if (pFolderItem)
            {
                FCM::FCMListPtr pChildren;

                res = pFolderItem->GetChildren(pChildren.m_Ptr);
                ASSERT(FCM_SUCCESS_CODE(res));

                // Export all its children
                res = ExportLibraryItems(pChildren);
                ASSERT(FCM_SUCCESS_CODE(res));
            }
            else
            {
                FCM::FCMDictRecTypeID type;
                FCM::U_Int32 valLen;
                AutoPtr<DOM::LibraryItem::IFontItem> pFontItem = pLibItem;
                AutoPtr<DOM::LibraryItem::ISymbolItem> pSymbolItem = pLibItem;
                AutoPtr<DOM::LibraryItem::IMediaItem> pMediaItem = pLibItem;

                res = pLibItem->GetProperties(pDict.m_Ptr);
                ASSERT(FCM_SUCCESS_CODE(res));

                res = pDict->GetInfo(kLibProp_LinkageClass_DictKey, 
                    type, valLen);

                if (FCM_SUCCESS_CODE(res))
                {
                    FCM::Boolean hasResource;
                    ResourcePalette* pResPalette = static_cast<ResourcePalette*>(m_pResourcePalette.m_Ptr);

                    // Library Item has linkage identifer

                    if (pSymbolItem)
                    {
                        //
                        // Check if it has been exported already by comparing names of resources 
                        // already exported from the timelines.
                        //
                        res = pResPalette->HasResource(libItemName, hasResource);
                        if (!hasResource)
                        {
                            // Resource is not yet exported. Export it using 
                            // FrameCommandGenerator::GenerateFrameCommands
                        }
                    }
                    else if (pMediaItem)
                    {
                        //
                        // Check if it has been exported already by comparing names of resources 
                        // already exported from the timelines.
                        //
                        res = pResPalette->HasResource(libItemName, hasResource);
                        if (!hasResource)
                        {
                            // Resource is not yet exported. Export it.

                            // Depending on bitmap/sound, export it.
                        }
                    }
                    else if (pFontItem)
                    {
                        // Use the font name to check if already exported.
                        
                        // Use IFontTableGeneratorService::CreateFontTableForFontItem() to create 
                        // a font table and then export it.
                    }
                }
            }

            callocService->Free((FCM::PVoid)pLibItemName);
        }
        return FCM_SUCCESS;
    }


    FCM::Result CPublisher::CopyRuntime(const std::string& outputFolder)
    {
        FCM::Result res;
        std::string sourceFolder;

        // Get the source folder
        Utils::GetModuleFilePath(sourceFolder, GetCallback());
        Utils::GetParent(sourceFolder, sourceFolder);
        Utils::GetParent(sourceFolder, sourceFolder);
        Utils::GetParent(sourceFolder, sourceFolder);

        // First let us remove the existing runtime folder (if any)
        Utils::Remove(outputFolder + RUNTIME_ROOT_FOLDER_NAME, GetCallback());

        if (m_minify)
        {
            // Copy the minified runtime
            std::string srcFolder = sourceFolder + RUNTIME_ROOT_FOLDER_NAME + "/" + 
                RUNTIME_COMMON_SUBFOLDER_NAME + "/";

            std::string srcFilePath = srcFolder + RUNTIME_MINIFIED_FILE_NAME;
            std::string dstFileFolder = outputFolder + RUNTIME_ROOT_FOLDER_NAME + "/";

            Utils::CreateDir(dstFileFolder, GetCallback());

            dstFileFolder += RUNTIME_COMMON_SUBFOLDER_NAME;
            dstFileFolder += "/";
            Utils::CreateDir(dstFileFolder, GetCallback());

            std::string dstFilePath = dstFileFolder + RUNTIME_MINIFIED_FILE_NAME;

            res = Utils::CopyAFile(srcFilePath, dstFilePath, GetCallback());

            // Copy the snap.svg library
            srcFilePath = srcFolder + THIRD_PARTY_SUBFOLDER_NAME + "/" + SNAP_SVG_SUBFOLDER_NAME + "/" + 
                SNAP_SVG_MINIFIED_LIB_FILE_NAME;

            dstFileFolder = dstFileFolder + THIRD_PARTY_SUBFOLDER_NAME + "/";
            Utils::CreateDir(dstFileFolder, GetCallback());

            dstFileFolder = dstFileFolder + SNAP_SVG_SUBFOLDER_NAME + "/";
            Utils::CreateDir(dstFileFolder, GetCallback());

            dstFilePath = dstFileFolder + SNAP_SVG_MINIFIED_LIB_FILE_NAME;
            res = Utils::CopyAFile(srcFilePath, dstFilePath, GetCallback());
        }
        else
        {
            // Copy the runtime folder
            res = Utils::CopyDir(sourceFolder + RUNTIME_ROOT_FOLDER_NAME, outputFolder, GetCallback());
        }

        return res;
    }

    /* ----------------------------------------------------- Resource Palette */

    FCM::Result ResourcePalette::AddSymbol(
        FCM::U_Int32 resourceId, 
        FCM::StringRep16 pName, 
        Exporter::Service::PITimelineBuilder pTimelineBuilder)
    {
        FCM::Result res;
        ITimelineWriter* pTimelineWriter;

        LOG(("[EndSymbol] ResId: %d\n", resourceId));

        m_resourceList.push_back(resourceId);

        if (pName != NULL)
        {
            m_resourceNames.push_back(Utils::ToString(pName, GetCallback()));
        }

        TimelineBuilder* pTimeline = static_cast<TimelineBuilder*>(pTimelineBuilder);

        res = pTimeline->Build(resourceId, pName, &pTimelineWriter);

        return res;
    }


    FCM::Result ResourcePalette::AddShape(
        FCM::U_Int32 resourceId, 
        DOM::FrameElement::PIShape pShape)
    {
        FCM::Result res;
        FCM::Boolean hasFancy;
        FCM::AutoPtr<DOM::FrameElement::IShape> pNewShape;

        LOG(("[DefineShape] ResId: %d\n", resourceId));

        m_resourceList.push_back(resourceId);
        m_pOutputWriter->StartDefineShape();

        if (pShape)
        {
            ExportFill(pShape);

            res = HasFancyStrokes(pShape, hasFancy);
            if (hasFancy)
            {
                res = ConvertStrokeToFill(pShape, pNewShape.m_Ptr);
                ASSERT(FCM_SUCCESS_CODE(res));

                ExportFill(pNewShape);
            }
            else
            {
                
                ExportStroke(pShape);
            }
        }

        m_pOutputWriter->EndDefineShape(resourceId);

        return FCM_SUCCESS;
    }


    FCM::Result ResourcePalette::AddSound(FCM::U_Int32 resourceId, DOM::LibraryItem::PIMediaItem pMediaItem)
    {
        FCM::Result res;
        DOM::AutoPtr<DOM::ILibraryItem> pLibItem;
        FCM::AutoPtr<FCM::IFCMUnknown> pUnknown;
        FCM::StringRep16 pName;
        std::string libName;

        LOG(("[DefineSound] ResId: %d\n", resourceId));

        m_resourceList.push_back(resourceId);

        // Store the resource name
        pLibItem = pMediaItem;

        res = pLibItem->GetName(&pName);
        ASSERT(FCM_SUCCESS_CODE(res));
        libName = Utils::ToString(pName, GetCallback());
        m_resourceNames.push_back(libName);

        res = pMediaItem->GetMediaInfo(pUnknown.m_Ptr);
        ASSERT(FCM_SUCCESS_CODE(res));

        AutoPtr<DOM::MediaInfo::ISoundInfo> pSoundInfo = pUnknown;
        ASSERT(pSoundInfo);
        
        m_pOutputWriter->DefineSound(resourceId, libName, pMediaItem);

        // Free the name
        FCM::AutoPtr<FCM::IFCMUnknown> pUnkCalloc;
        res = GetCallback()->GetService(SRVCID_Core_Memory, pUnkCalloc.m_Ptr);
        AutoPtr<FCM::IFCMCalloc> callocService  = pUnkCalloc;

        callocService->Free((FCM::PVoid)pName);

        return FCM_SUCCESS;
    }


    FCM::Result ResourcePalette::AddBitmap(FCM::U_Int32 resourceId, DOM::LibraryItem::PIMediaItem pMediaItem)
    {
        DOM::AutoPtr<DOM::ILibraryItem> pLibItem;
        FCM::Result res;
        FCM::StringRep16 pName;

        LOG(("[DefineBitmap] ResId: %d\n", resourceId));

        m_resourceList.push_back(resourceId);

        pLibItem = pMediaItem;

        // Store the resource name
        res = pLibItem->GetName(&pName);
        ASSERT(FCM_SUCCESS_CODE(res));
        std::string libItemName = Utils::ToString(pName, GetCallback());
        m_resourceNames.push_back(libItemName);

        AutoPtr<FCM::IFCMUnknown> medInfo;
        pMediaItem->GetMediaInfo(medInfo.m_Ptr);

        AutoPtr<DOM::MediaInfo::IBitmapInfo> bitsInfo = medInfo;
        ASSERT(bitsInfo);

        // Get image height
        FCM::S_Int32 height;
        res = bitsInfo->GetHeight(height);
        ASSERT(FCM_SUCCESS_CODE(res));

        // Get image width
        FCM::S_Int32 width;
        res = bitsInfo->GetWidth(width);
        ASSERT(FCM_SUCCESS_CODE(res));

        // Dump the definition of a bitmap
        res = m_pOutputWriter->DefineBitmap(resourceId, height, width, libItemName, pMediaItem);

        // Free the name
        FCM::AutoPtr<FCM::IFCMUnknown> pUnkCalloc;
        res = GetCallback()->GetService(SRVCID_Core_Memory, pUnkCalloc.m_Ptr);
        AutoPtr<FCM::IFCMCalloc> callocService  = pUnkCalloc;

        callocService->Free((FCM::PVoid)pName);

        return res;
    }


    FCM::Result ResourcePalette::AddClassicText(FCM::U_Int32 resourceId, DOM::FrameElement::PIClassicText pClassicText)
    {
        FCM::Result res;
        DOM::AutoPtr<DOM::FrameElement::IClassicText> pTextItem;
        FCM::StringRep16 textDisplay;
        std::string displayText;
        TEXT_BEHAVIOUR textBehaviour;
        FCMListPtr pParagraphsList;
        DOM::AutoPtr<DOM::FrameElement::ITextBehaviour> pTextBehaviour;
        DOM::FrameElement::AA_MODE_PROP aaModeProp;
        FCM::U_Int32 paraCount;

        ASSERT(pClassicText);

        pTextItem = pClassicText;

        res = pTextItem->GetText(&textDisplay);
        ASSERT(FCM_SUCCESS_CODE(res));
        displayText = Utils::ToString(textDisplay, GetCallback());

        res = pTextItem->GetAntiAliasModeProp(aaModeProp);
        ASSERT(FCM_SUCCESS_CODE(res));

        res = pTextItem->GetTextBehaviour(pTextBehaviour.m_Ptr);
        ASSERT(FCM_SUCCESS_CODE(res));

        res = GetTextBehaviour(pTextBehaviour, textBehaviour);
        ASSERT(FCM_SUCCESS_CODE(res));

        // Start define text
        res = m_pOutputWriter->StartDefineClassicText(resourceId, aaModeProp, displayText, textBehaviour);
        ASSERT(FCM_SUCCESS_CODE(res));

        res = pTextItem->GetParagraphs(pParagraphsList.m_Ptr);
        ASSERT(FCM_SUCCESS_CODE(res));

        res = pParagraphsList->Count(paraCount);
        ASSERT(FCM_SUCCESS_CODE(res));

        for (FCM::U_Int32 para = 0; para < paraCount; para++)
        {
            AutoPtr<DOM::FrameElement::IParagraph> pParagraph = pParagraphsList[para];

            if (pParagraph)
            {
                FCMListPtr pTextRunList;
                FCM::U_Int32 runCount;
                FCM::U_Int32 paraStartIndex;
                FCM::U_Int32 paraLength;
                DOM::FrameElement::PARAGRAPH_STYLE paragraphStyle;

                // Start Paragraph - startIndex, length, paragraphStyle
                res = pParagraph->GetStartIndex(paraStartIndex);
                ASSERT(FCM_SUCCESS_CODE(res));

                res = pParagraph->GetLength(paraLength);
                ASSERT(FCM_SUCCESS_CODE(res));

                paragraphStyle.structSize = sizeof(DOM::FrameElement::PARAGRAPH_STYLE);
                res = pParagraph->GetParagraphStyle(paragraphStyle);
                ASSERT(FCM_SUCCESS_CODE(res));

                res = m_pOutputWriter->StartDefineParagraph(paraStartIndex, paraLength, paragraphStyle);
                ASSERT(FCM_SUCCESS_CODE(res));

                res = pParagraph->GetTextRuns(pTextRunList.m_Ptr);
                ASSERT(FCM_SUCCESS_CODE(res));

                res = pTextRunList->Count(runCount);
                ASSERT(FCM_SUCCESS_CODE(res));

                for (FCM::U_Int32 trIndex = 0; trIndex < runCount; trIndex++)
                {
                    FCM::U_Int32 runStartIndex;
                    FCM::U_Int32 runLength;
                    AutoPtr<DOM::FrameElement::ITextRun> pTextRun = pTextRunList[trIndex];
                    AutoPtr<DOM::FrameElement::ITextStyle> runStyle;
                    TEXT_STYLE textStyle;

                    // Start text run - startIndex, length, textStyle
                    res = pTextRun->GetStartIndex(runStartIndex);
                    ASSERT(FCM_SUCCESS_CODE(res));

                    res = pTextRun->GetLength(runLength);
                    ASSERT(FCM_SUCCESS_CODE(res));

                    res = pTextRun->GetTextStyle(runStyle.m_Ptr);
                    ASSERT(FCM_SUCCESS_CODE(res));

                    // Extract text style 
                    res = GetTextStyle(runStyle, textStyle);
                    ASSERT(FCM_SUCCESS_CODE(res));

                    res = m_pOutputWriter->StartDefineTextRun(runStartIndex, runLength, textStyle);
                    ASSERT(FCM_SUCCESS_CODE(res));

                    // End text run
                    res = m_pOutputWriter->EndDefineTextRun();
                    ASSERT(FCM_SUCCESS_CODE(res));
                }

                // End Paragraph
                res = m_pOutputWriter->EndDefineParagraph();
                ASSERT(FCM_SUCCESS_CODE(res));
            }
        }

        // End define text
        res = m_pOutputWriter->EndDefineClassicText();
        ASSERT(FCM_SUCCESS_CODE(res));

        return res;
    }



    FCM::Result ResourcePalette::HasResource(FCM::U_Int32 resourceId, FCM::Boolean& hasResource)
    {
        hasResource = false;

        for (std::vector<FCM::U_Int32>::iterator listIter = m_resourceList.begin();
                listIter != m_resourceList.end(); listIter++)
        {
            if (*listIter == resourceId)
            {
                hasResource = true;
                break;
            }
        }

        //LOG(("[HasResource] ResId: %d HasResource: %d\n", resourceId, hasResource));

        return FCM_SUCCESS;
    }


    ResourcePalette::ResourcePalette()
    {
        m_pOutputWriter = NULL;
    }


    ResourcePalette::~ResourcePalette()
    {
    }


    void ResourcePalette::Init(IOutputWriter* pOutputWriter)
    {
        m_pOutputWriter = pOutputWriter;
    }

    void ResourcePalette::Clear()
    {
        m_resourceList.clear();
    }

    FCM::Result ResourcePalette::HasResource(
            const std::string& name, 
            FCM::Boolean& hasResource)
    {
        hasResource = false;
        for (FCM::U_Int32 index = 0; index < m_resourceNames.size(); index++)
        {
            if (m_resourceNames[index] == name)
            {
                hasResource = true;
                break;
            }
        }

        return FCM_SUCCESS;
    }


    FCM::Result ResourcePalette::ExportFill(DOM::FrameElement::PIShape pIShape)
    {
        FCM::Result res;
        FCM::FCMListPtr pFilledRegionList;
        FCM::AutoPtr<FCM::IFCMUnknown> pUnkSRVReg;
        FCM::U_Int32 regionCount;

        GetCallback()->GetService(DOM::FLA_REGION_GENERATOR_SERVICE,pUnkSRVReg.m_Ptr);
        AutoPtr<DOM::Service::Shape::IRegionGeneratorService> pIRegionGeneratorService(pUnkSRVReg);
        ASSERT(pIRegionGeneratorService);

        res = pIRegionGeneratorService->GetFilledRegions(pIShape, pFilledRegionList.m_Ptr);
        ASSERT(FCM_SUCCESS_CODE(res));

        pFilledRegionList->Count(regionCount);

        for (FCM::U_Int32 j = 0; j < regionCount; j++)
        {
            FCM::AutoPtr<DOM::Service::Shape::IFilledRegion> pFilledRegion = pFilledRegionList[j];
            FCM::AutoPtr<DOM::Service::Shape::IPath> pPath;

            m_pOutputWriter->StartDefineFill();

            // Fill Style
            FCM::AutoPtr<DOM::IFCMUnknown> fillStyle;

            res = pFilledRegion->GetFillStyle(fillStyle.m_Ptr);
            ASSERT(FCM_SUCCESS_CODE(res));

            ExportFillStyle(fillStyle);

            // Boundary
            res = pFilledRegion->GetBoundary(pPath.m_Ptr);
            ASSERT(FCM_SUCCESS_CODE(res));

            res = ExportFillBoundary(pPath);
            ASSERT(FCM_SUCCESS_CODE(res));

            // Hole List
            FCMListPtr pHoleList;
            FCM::U_Int32 holeCount;

            res = pFilledRegion->GetHoles(pHoleList.m_Ptr);
            ASSERT(FCM_SUCCESS_CODE(res));

            res = pHoleList->Count(holeCount);
            ASSERT(FCM_SUCCESS_CODE(res));

            for (FCM::U_Int32 k = 0; k < holeCount; k++)
            {
                FCM::FCMListPtr pEdgeList;
                FCM::AutoPtr<DOM::Service::Shape::IPath> pPath = pHoleList[k];

                res = ExportHole(pPath);
            }

            m_pOutputWriter->EndDefineFill();
        }

        return res;
    }


    FCM::Result ResourcePalette::ExportFillBoundary(DOM::Service::Shape::PIPath pPath)
    {
        FCM::Result res;

        m_pOutputWriter->StartDefineBoundary();

        res = ExportPath(pPath);
        ASSERT(FCM_SUCCESS_CODE(res));

        m_pOutputWriter->EndDefineBoundary();

        return res;
    }


    FCM::Result ResourcePalette::ExportHole(DOM::Service::Shape::PIPath pPath)
    {
        FCM::Result res;

        m_pOutputWriter->StartDefineHole();

        res = ExportPath(pPath);
        ASSERT(FCM_SUCCESS_CODE(res));

        m_pOutputWriter->EndDefineHole();

        return res;
    }


    FCM::Result ResourcePalette::ExportPath(DOM::Service::Shape::PIPath pPath)
    {
        FCM::Result res;
        FCM::U_Int32 edgeCount;
        FCM::FCMListPtr pEdgeList;

        res = pPath->GetEdges(pEdgeList.m_Ptr);
        ASSERT(FCM_SUCCESS_CODE(res));

        res = pEdgeList->Count(edgeCount);
        ASSERT(FCM_SUCCESS_CODE(res));

        for (FCM::U_Int32 l = 0; l < edgeCount; l++)
        {
            DOM::Utils::SEGMENT segment;

            segment.structSize = sizeof(DOM::Utils::SEGMENT);

            FCM::AutoPtr<DOM::Service::Shape::IEdge> pEdge = pEdgeList[l];

            res = pEdge->GetSegment(segment);

            m_pOutputWriter->SetSegment(segment);
        }

        return res;
    }

    FCM::Result ResourcePalette::ExportFillStyle(FCM::PIFCMUnknown pFillStyle)
    {
        FCM::Result res = FCM_SUCCESS;

        AutoPtr<DOM::FillStyle::ISolidFillStyle> pSolidFillStyle;
        AutoPtr<DOM::FillStyle::IGradientFillStyle> pGradientFillStyle;
        AutoPtr<DOM::FillStyle::IBitmapFillStyle> pBitmapFillStyle;

        // Check for solid fill color
        pSolidFillStyle = pFillStyle;
        if (pSolidFillStyle)
        {
            res = ExportSolidFillStyle(pSolidFillStyle);
            ASSERT(FCM_SUCCESS_CODE(res));
        }

        // Check for Gradient Fill
        pGradientFillStyle = pFillStyle;
        AutoPtr<FCM::IFCMUnknown> pGrad;

        if (pGradientFillStyle)
        {
            pGradientFillStyle->GetColorGradient(pGrad.m_Ptr);

            if (AutoPtr<DOM::Utils::IRadialColorGradient>(pGrad))
            {
                res = ExportRadialGradientFillStyle(pGradientFillStyle);
                ASSERT(FCM_SUCCESS_CODE(res));
            }
            else if (AutoPtr<DOM::Utils::ILinearColorGradient>(pGrad))
            {
                res = ExportLinearGradientFillStyle(pGradientFillStyle);
                ASSERT(FCM_SUCCESS_CODE(res));
            }
        }

        pBitmapFillStyle = pFillStyle;
        if (pBitmapFillStyle)
        {
            res = ExportBitmapFillStyle(pBitmapFillStyle);
            ASSERT(FCM_SUCCESS_CODE(res));
        }

        return res;
    }


    FCM::Result ResourcePalette::ExportStroke(DOM::FrameElement::PIShape pIShape)
    {
        FCM::FCMListPtr pStrokeGroupList;
        FCM::U_Int32 strokeStyleCount;
        FCM::Result res;

        FCM::AutoPtr<FCM::IFCMUnknown> pUnkSRVReg;
        GetCallback()->GetService(DOM::FLA_REGION_GENERATOR_SERVICE,pUnkSRVReg.m_Ptr);
        AutoPtr<DOM::Service::Shape::IRegionGeneratorService> pIRegionGeneratorService(pUnkSRVReg);
        ASSERT(pIRegionGeneratorService);

        res = pIRegionGeneratorService->GetStrokeGroups(pIShape, pStrokeGroupList.m_Ptr);
        ASSERT(FCM_SUCCESS_CODE(res));

        res = pStrokeGroupList->Count(strokeStyleCount);
        ASSERT(FCM_SUCCESS_CODE(res));

        AutoPtr<DOM::FillStyle::ISolidFillStyle> pSolidFillStyle = NULL;
        AutoPtr<DOM::FillStyle::IGradientFillStyle> pGradientFillStyle = NULL;
        AutoPtr<DOM::FillStyle::IBitmapFillStyle> pBitmapFillStyle = NULL;

        AutoPtr<FCM::IFCMUnknown> pGrad;
        for (FCM::U_Int32 j = 0; j < strokeStyleCount; j++)
        {
            AutoPtr<DOM::Service::Shape::IStrokeGroup> pStrokeGroup = pStrokeGroupList[j];
            ASSERT(pStrokeGroup);

            res = m_pOutputWriter->StartDefineStrokeGroup();
            ASSERT(FCM_SUCCESS_CODE(res));

            AutoPtr<FCM::IFCMUnknown> pStrokeStyle;
            pStrokeGroup->GetStrokeStyle(pStrokeStyle.m_Ptr);

            DOM::Utils::COLOR color = {};

            FCMListPtr pPathList;
            FCM::U_Int32 pathCount;

            res = pStrokeGroup->GetPaths(pPathList.m_Ptr);
            ASSERT(FCM_SUCCESS_CODE(res));

            res = pPathList->Count(pathCount);
            ASSERT(FCM_SUCCESS_CODE(res));

            for (FCM::U_Int32 k = 0; k < pathCount; k++)
            {
                FCM::AutoPtr<DOM::Service::Shape::IPath> pPath;

                pPath = pPathList[k];
                ASSERT(pPath);

                res = m_pOutputWriter->StartDefineStroke();
                ASSERT(FCM_SUCCESS_CODE(res));

                res = ExportStrokeStyle(pStrokeStyle);
                ASSERT(FCM_SUCCESS_CODE(res));

                res = ExportPath(pPath);
                ASSERT(FCM_SUCCESS_CODE(res));

                res = m_pOutputWriter->EndDefineStroke();
                ASSERT(FCM_SUCCESS_CODE(res));
            }

            res = m_pOutputWriter->EndDefineStrokeGroup();
            ASSERT(FCM_SUCCESS_CODE(res));
        }

        return res;
    }


    FCM::Result ResourcePalette::HasFancyStrokes(DOM::FrameElement::PIShape pShape, FCM::Boolean& hasFancy)
    {
        FCM::Result res;
        FCM::FCMListPtr pStrokeGroupList;
        FCM::U_Int32 strokeStyleCount;

        hasFancy = false;

        FCM::AutoPtr<FCM::IFCMUnknown> pUnkSRVReg;
        GetCallback()->GetService(DOM::FLA_REGION_GENERATOR_SERVICE,pUnkSRVReg.m_Ptr);
        AutoPtr<DOM::Service::Shape::IRegionGeneratorService> pIRegionGeneratorService(pUnkSRVReg);
        ASSERT(pIRegionGeneratorService);

        res = pIRegionGeneratorService->GetStrokeGroups(pShape, pStrokeGroupList.m_Ptr);
        ASSERT(FCM_SUCCESS_CODE(res));

        res = pStrokeGroupList->Count(strokeStyleCount);
        ASSERT(FCM_SUCCESS_CODE(res));

        for (FCM::U_Int32 j = 0; j < strokeStyleCount; j++)
        {
            
            AutoPtr<DOM::StrokeStyle::ISolidStrokeStyle> pSolidStrokeStyle;
            AutoPtr<DOM::Service::Shape::IStrokeGroup> pStrokeGroup = pStrokeGroupList[j];
            ASSERT(pStrokeGroup);

            AutoPtr<FCM::IFCMUnknown> pStrokeStyle;
            pStrokeGroup->GetStrokeStyle(pStrokeStyle.m_Ptr);

            pSolidStrokeStyle = pStrokeStyle;

            if (pSolidStrokeStyle)
            {
                FCM::AutoPtr<DOM::StrokeStyle::IStrokeWidth> pStrokeWidth;

                pSolidStrokeStyle->GetStrokeWidth(pStrokeWidth.m_Ptr);

                if (pStrokeWidth.m_Ptr)
                {
                    // Variable width stroke
                    hasFancy = true;
                    break;
                }            
            }
            else
            {
                // Not a solid stroke (may be dashed, dotted etc..)
                hasFancy = true;
                break;
            }
        }

        return FCM_SUCCESS;
    }


    // Convert strokes to fills
    FCM::Result ResourcePalette::ConvertStrokeToFill(
        DOM::FrameElement::PIShape pShape,
        DOM::FrameElement::PIShape& pNewShape)
    {
        FCM::Result res;
        FCM::AutoPtr<FCM::IFCMUnknown> pUnkSRVReg; 

        GetCallback()->GetService(DOM::FLA_SHAPE_SERVICE, pUnkSRVReg.m_Ptr);

        AutoPtr<DOM::Service::Shape::IShapeService> pIShapeService(pUnkSRVReg);
        ASSERT(pIShapeService);

        res = pIShapeService->ConvertStrokeToFill(pShape, pNewShape);
        ASSERT(FCM_SUCCESS_CODE(res));

        return FCM_SUCCESS;
    }

    FCM::Result ResourcePalette::ExportStrokeStyle(FCM::PIFCMUnknown pStrokeStyle)
    {
        FCM::Result res = FCM_SUCCESS;
        AutoPtr<DOM::StrokeStyle::ISolidStrokeStyle> pSolidStrokeStyle;

        pSolidStrokeStyle = pStrokeStyle;

        if (pSolidStrokeStyle)
        {
            res = ExportSolidStrokeStyle(pSolidStrokeStyle);
        }
        else
        {
            // Other stroke styles are not tested yet.
        }

        return res;
    }


    FCM::Result ResourcePalette::ExportSolidStrokeStyle(DOM::StrokeStyle::ISolidStrokeStyle* pSolidStrokeStyle)
    {
        FCM::Result res;
        FCM::Double thickness;
        AutoPtr<DOM::IFCMUnknown> pFillStyle;
        DOM::StrokeStyle::CAP_STYLE capStyle;
        DOM::StrokeStyle::JOIN_STYLE joinStyle;
        DOM::Utils::ScaleType scaleType;
        FCM::Boolean strokeHinting;


        capStyle.structSize = sizeof(DOM::StrokeStyle::CAP_STYLE);
        res = pSolidStrokeStyle->GetCapStyle(capStyle);
        ASSERT(FCM_SUCCESS_CODE(res));

        joinStyle.structSize = sizeof(DOM::StrokeStyle::JOIN_STYLE);
        res = pSolidStrokeStyle->GetJoinStyle(joinStyle);
        ASSERT(FCM_SUCCESS_CODE(res));

        res = pSolidStrokeStyle->GetThickness(thickness);
        ASSERT(FCM_SUCCESS_CODE(res));

        if (thickness < 0.1)
        {
            thickness = 0.1;
        }

        res = pSolidStrokeStyle->GetScaleType(scaleType);
        ASSERT(FCM_SUCCESS_CODE(res));

        res = pSolidStrokeStyle->GetStrokeHinting(strokeHinting);
        ASSERT(FCM_SUCCESS_CODE(res));

        res = m_pOutputWriter->StartDefineSolidStrokeStyle(
            thickness, 
            joinStyle, 
            capStyle, 
            scaleType, 
            strokeHinting);
        ASSERT(FCM_SUCCESS_CODE(res));

        // Stroke fill styles
        res = pSolidStrokeStyle->GetFillStyle(pFillStyle.m_Ptr);
        ASSERT(FCM_SUCCESS_CODE(res));

        res = ExportFillStyle(pFillStyle);
        ASSERT(FCM_SUCCESS_CODE(res));

        res = m_pOutputWriter->EndDefineSolidStrokeStyle();
        ASSERT(FCM_SUCCESS_CODE(res));

        return res;
    }


    FCM::Result ResourcePalette::ExportSolidFillStyle(DOM::FillStyle::ISolidFillStyle* pSolidFillStyle)
    {
        FCM::Result res;
        DOM::Utils::COLOR color;

        AutoPtr<DOM::FillStyle::ISolidFillStyle> solidFill = pSolidFillStyle;
        ASSERT(solidFill);

        res = solidFill->GetColor(color);
        ASSERT(FCM_SUCCESS_CODE(res));

        m_pOutputWriter->DefineSolidFillStyle(color);

        return res;
    }


    FCM::Result ResourcePalette::ExportRadialGradientFillStyle(DOM::FillStyle::IGradientFillStyle* pGradientFillStyle)
    {
        DOM::FillStyle::GradientSpread spread;

        AutoPtr<FCM::IFCMUnknown> pGrad;

        AutoPtr<DOM::FillStyle::IGradientFillStyle> gradientFill = pGradientFillStyle;
        FCM::Result res = gradientFill->GetSpread(spread);
        ASSERT(FCM_SUCCESS_CODE(res));

        res = gradientFill->GetColorGradient(pGrad.m_Ptr);
        ASSERT(FCM_SUCCESS_CODE(res));

        AutoPtr<DOM::Utils::IRadialColorGradient> radialColorGradient = pGrad;
        ASSERT(radialColorGradient);

        DOM::Utils::MATRIX2D matrix;
        res = gradientFill->GetMatrix(matrix);
        ASSERT(FCM_SUCCESS_CODE(res));

        FCM::S_Int32 focalPoint = 0;
        res = radialColorGradient->GetFocalPoint(focalPoint);
        ASSERT(FCM_SUCCESS_CODE(res));

        res = m_pOutputWriter->StartDefineRadialGradientFillStyle(spread, matrix, focalPoint);
        ASSERT(FCM_SUCCESS_CODE(res));

        FCM::U_Int8 nColors;
        res = radialColorGradient->GetKeyColorCount(nColors);
        ASSERT(FCM_SUCCESS_CODE(res));

        for (FCM::U_Int8 i = 0; i < nColors; i++)
        {
            DOM::Utils::GRADIENT_COLOR_POINT point;

            res = radialColorGradient->GetKeyColorAtIndex(i, point);
            ASSERT(FCM_SUCCESS_CODE(res));

            res = m_pOutputWriter->SetKeyColorPoint(point);
            ASSERT(FCM_SUCCESS_CODE(res));
        }

        res = m_pOutputWriter->EndDefineRadialGradientFillStyle();
        ASSERT(FCM_SUCCESS_CODE(res));

        return res;
    }


    FCM::Result ResourcePalette::ExportLinearGradientFillStyle(DOM::FillStyle::IGradientFillStyle* pGradientFillStyle)
    {
        DOM::FillStyle::GradientSpread spread;
        AutoPtr<FCM::IFCMUnknown> pGrad;

        AutoPtr<DOM::FillStyle::IGradientFillStyle> gradientFill = pGradientFillStyle;
        FCM::Result res = gradientFill->GetSpread(spread);
        ASSERT(FCM_SUCCESS_CODE(res));

        res = gradientFill->GetColorGradient(pGrad.m_Ptr);
        ASSERT(FCM_SUCCESS_CODE(res));

        AutoPtr<DOM::Utils::ILinearColorGradient> linearColorGradient = pGrad;
        ASSERT(linearColorGradient);

        DOM::Utils::MATRIX2D matrix;
        res = gradientFill->GetMatrix(matrix);
        ASSERT(FCM_SUCCESS_CODE(res));

        res = m_pOutputWriter->StartDefineLinearGradientFillStyle(spread, matrix);
        ASSERT(FCM_SUCCESS_CODE(res));

        FCM::U_Int8 nColors;
        res = linearColorGradient->GetKeyColorCount(nColors);
        ASSERT(FCM_SUCCESS_CODE(res));

        for (FCM::U_Int8 i = 0; i < nColors; i++)
        {
            DOM::Utils::GRADIENT_COLOR_POINT point;

            res = linearColorGradient->GetKeyColorAtIndex(i, point);
            ASSERT(FCM_SUCCESS_CODE(res));

            res = m_pOutputWriter->SetKeyColorPoint(point);
            ASSERT(FCM_SUCCESS_CODE(res));
        }

        res = m_pOutputWriter->EndDefineLinearGradientFillStyle();
        ASSERT(FCM_SUCCESS_CODE(res));

        return res;
    }


    FCM::Result ResourcePalette::ExportBitmapFillStyle(DOM::FillStyle::IBitmapFillStyle* pBitmapFillStyle)
    {
        DOM::AutoPtr<DOM::ILibraryItem> pLibItem;
        DOM::AutoPtr<DOM::LibraryItem::IMediaItem> pMediaItem;
        FCM::Result res;
        FCM::Boolean isClipped;
        DOM::Utils::MATRIX2D matrix;
        std::string name;
        FCM::StringRep16 pName;

        // IsClipped ?
        res = pBitmapFillStyle->IsClipped(isClipped);
        ASSERT(FCM_SUCCESS_CODE(res));

        // Matrix
        res = pBitmapFillStyle->GetMatrix(matrix);
        ASSERT(FCM_SUCCESS_CODE(res));

        // Get name
        res = pBitmapFillStyle->GetBitmap(pMediaItem.m_Ptr);
        ASSERT(FCM_SUCCESS_CODE(res));

        pLibItem = pMediaItem;

        AutoPtr<FCM::IFCMUnknown> medInfo;
        pMediaItem->GetMediaInfo(medInfo.m_Ptr);

        AutoPtr<DOM::MediaInfo::IBitmapInfo> bitsInfo = medInfo;
        ASSERT(bitsInfo);

        // Get image height
        FCM::S_Int32 height;
        res = bitsInfo->GetHeight(height);
        ASSERT(FCM_SUCCESS_CODE(res));

        // Store the resource name
        res = pLibItem->GetName(&pName);
        ASSERT(FCM_SUCCESS_CODE(res));
        std::string libItemName = Utils::ToString(pName, GetCallback());
        m_resourceNames.push_back(libItemName);

        // Get image width
        FCM::S_Int32 width;
        res = bitsInfo->GetWidth(width);
        ASSERT(FCM_SUCCESS_CODE(res));

        // Dump the definition of a bitmap fill style
        res = m_pOutputWriter->DefineBitmapFillStyle(
            isClipped, 
            matrix, 
            height, 
            width, 
            libItemName, 
            pMediaItem);
        ASSERT(FCM_SUCCESS_CODE(res));

        // Free the name
        FCM::AutoPtr<FCM::IFCMUnknown> pUnkCalloc;
        res = GetCallback()->GetService(SRVCID_Core_Memory, pUnkCalloc.m_Ptr);
        AutoPtr<FCM::IFCMCalloc> callocService = pUnkCalloc;

        callocService->Free((FCM::PVoid)pName);

        return res;
    }


    FCM::Result ResourcePalette::GetTextStyle(DOM::FrameElement::ITextStyle* pTextStyleItem, TEXT_STYLE& textStyle)
    {
        FCM::StringRep16 pFontName;
        FCM::StringRep16 pLink;
        FCM::StringRep16 pLinkTarget;
        FCM::StringRep8 pFontStyle;
        FCM::Result res;

        res = pTextStyleItem->GetBaseLineShiftStyle(textStyle.baseLineShiftStyle);
        ASSERT(FCM_SUCCESS_CODE(res));

        res = pTextStyleItem->GetFontColor(textStyle.fontColor);
        ASSERT(FCM_SUCCESS_CODE(res));

        res = pTextStyleItem->GetFontName(&pFontName);
        ASSERT(FCM_SUCCESS_CODE(res));
        textStyle.fontName = Utils::ToString(pFontName, GetCallback());

        res = pTextStyleItem->GetFontSize(textStyle.fontSize);
        ASSERT(FCM_SUCCESS_CODE(res));

        res = pTextStyleItem->GetFontStyle(&pFontStyle);
        ASSERT(FCM_SUCCESS_CODE(res));
        textStyle.fontStyle = Utils::ToString(pFontStyle);

        res = pTextStyleItem->GetLetterSpacing(textStyle.letterSpacing);
        ASSERT(FCM_SUCCESS_CODE(res));

        res = pTextStyleItem->GetLink(&pLink);
        ASSERT(FCM_SUCCESS_CODE(res));
        textStyle.link = Utils::ToString(pLink, GetCallback());

        res = pTextStyleItem->GetLinkTarget(&pLinkTarget);
        ASSERT(FCM_SUCCESS_CODE(res));
        textStyle.linkTarget = Utils::ToString(pLinkTarget, GetCallback());

        res = pTextStyleItem->IsAutoKernEnabled(textStyle.autoKern);
        ASSERT(FCM_SUCCESS_CODE(res));

        res = pTextStyleItem->IsRotated(textStyle.rotated);
        ASSERT(FCM_SUCCESS_CODE(res));

        // Cleanup
        FCM::AutoPtr<FCM::IFCMUnknown> pUnkCalloc;
        res = GetCallback()->GetService(SRVCID_Core_Memory, pUnkCalloc.m_Ptr);
        AutoPtr<FCM::IFCMCalloc> callocService  = pUnkCalloc;

        callocService->Free((FCM::PVoid)pFontName);
        callocService->Free((FCM::PVoid)pFontStyle);
        callocService->Free((FCM::PVoid)pLink);
        callocService->Free((FCM::PVoid)pLinkTarget);
        
        return res;
    }


    FCM::Result ResourcePalette::GetTextBehaviour(DOM::FrameElement::ITextBehaviour* pTextBehaviour, TEXT_BEHAVIOUR& textBehaviour)
    {
        FCM::Result res;
        FCM::AutoPtr<DOM::FrameElement::IStaticTextBehaviour> staticTextBehaviour;
        FCM::AutoPtr<DOM::FrameElement::IModifiableTextBehaviour> modifiableTextBehaviour;
        FCM::AutoPtr<DOM::FrameElement::IDynamicTextBehaviour> dynamicTextBehaviour;
        FCM::AutoPtr<DOM::FrameElement::IInputTextBehaviour> inputTextBehaviour;

        ASSERT(pTextBehaviour);

        res = pTextBehaviour->IsSelectable(textBehaviour.selectable);
        ASSERT(FCM_SUCCESS_CODE(res));

        staticTextBehaviour = pTextBehaviour;
        if (staticTextBehaviour)
        {
            // Static text
            textBehaviour.type = 0;

            res = staticTextBehaviour->GetFlow(textBehaviour.u.staticText.flow);
            ASSERT(FCM_SUCCESS_CODE(res));

            res = staticTextBehaviour->GetOrientationMode(textBehaviour.u.staticText.orientationMode);
            ASSERT(FCM_SUCCESS_CODE(res));

            textBehaviour.name = "";
        }
        else
        {
            modifiableTextBehaviour = pTextBehaviour;
            if (modifiableTextBehaviour)
            {
                FCM::StringRep16 pInstanceName;
                DOM::FrameElement::LineMode lineMode;
                FCM::Boolean renderAsHTML;
                FCM::Boolean borderDrawn;
                FCM::Boolean scrollable;

                res = modifiableTextBehaviour->GetInstanceName(&pInstanceName);
                ASSERT(FCM_SUCCESS_CODE(res));
                textBehaviour.name = Utils::ToString(pInstanceName, GetCallback());

                res = modifiableTextBehaviour->GetLineMode(lineMode);
                ASSERT(FCM_SUCCESS_CODE(res));

                res = modifiableTextBehaviour->GetRenderAsHtml(renderAsHTML);
                ASSERT(FCM_SUCCESS_CODE(res));

                res = modifiableTextBehaviour->IsBorderDrawn(borderDrawn);
                ASSERT(FCM_SUCCESS_CODE(res));

                res = modifiableTextBehaviour->IsScrollable(scrollable);
                ASSERT(FCM_SUCCESS_CODE(res));

                dynamicTextBehaviour = pTextBehaviour;
                if (dynamicTextBehaviour)
                {
                    textBehaviour.type = 1;

                    textBehaviour.u.dynamicText.borderDrawn = borderDrawn;
                    textBehaviour.u.dynamicText.lineMode = lineMode;
                    textBehaviour.u.dynamicText.renderAsHtml = renderAsHTML;
                    textBehaviour.u.dynamicText.scrollable = scrollable;
                }
                else
                {
                    inputTextBehaviour = pTextBehaviour;
                    if (inputTextBehaviour)
                    {
                        textBehaviour.type = 2;
                    
                        textBehaviour.u.inputText.borderDrawn = borderDrawn;
                        textBehaviour.u.inputText.lineMode = lineMode;
                        textBehaviour.u.inputText.renderAsHtml = renderAsHTML;
                        textBehaviour.u.inputText.scrollable = scrollable;
                        
                        res = inputTextBehaviour->IsLineModePassword(textBehaviour.u.inputText.password);
                        ASSERT(FCM_SUCCESS_CODE(res));
                    }
                }

                // Cleanup
                FCM::AutoPtr<FCM::IFCMUnknown> pUnkCalloc;
                res = GetCallback()->GetService(SRVCID_Core_Memory, pUnkCalloc.m_Ptr);
                AutoPtr<FCM::IFCMCalloc> callocService  = pUnkCalloc;

                callocService->Free((FCM::PVoid)pInstanceName);
            }
        }

        return FCM_SUCCESS;
    }


    /* ----------------------------------------------------- TimelineBuilder */

    FCM::Result TimelineBuilder::AddShape(FCM::U_Int32 objectId, SHAPE_INFO* pShapeInfo)
    {
        FCM::Result res;

        ASSERT(pShapeInfo);
        ASSERT(pShapeInfo->structSize >= sizeof(SHAPE_INFO));

        LOG(("[AddShape] ObjId: %d ResId: %d PlaceAfter: %d\n", 
            objectId, pShapeInfo->resourceId, pShapeInfo->placeAfterObjectId));

        res = m_pTimelineWriter->PlaceObject(
            pShapeInfo->resourceId, 
            objectId, 
            pShapeInfo->placeAfterObjectId, 
            &pShapeInfo->matrix);

        return res;
    }

    FCM::Result TimelineBuilder::AddClassicText(FCM::U_Int32 objectId, CLASSIC_TEXT_INFO* pClassicTextInfo)
    {
        FCM::Result res;

        ASSERT(pClassicTextInfo);
        ASSERT(pClassicTextInfo->structSize >= sizeof(CLASSIC_TEXT_INFO));

        LOG(("[AddClassicText] ObjId: %d ResId: %d PlaceAfter: %d\n", 
            objectId, pClassicTextInfo->resourceId, pClassicTextInfo->placeAfterObjectId));
        
        ASSERT(pClassicTextInfo->structSize >= sizeof(DISPLAY_OBJECT_INFO_2))

        DOM::Utils::RECT* pRect = NULL;
        DISPLAY_OBJECT_INFO_2 *ptr = static_cast<DISPLAY_OBJECT_INFO_2*>(pClassicTextInfo);
        if(ptr)
        {
            pRect = &ptr->bounds;
            // This rect object gives the bound of the text field.
            // This will have to be transformed using the pClassicTextInfo->matrix
            // to map it to its parent's co-orinate space to render it.
        }
        
        res = m_pTimelineWriter->PlaceObject(
            pClassicTextInfo->resourceId, 
            objectId, 
            pClassicTextInfo->placeAfterObjectId, 
            &pClassicTextInfo->matrix,
            pRect);

        return res;
    }

    FCM::Result TimelineBuilder::AddBitmap(FCM::U_Int32 objectId, BITMAP_INFO* pBitmapInfo)
    {
        FCM::Result res;

        ASSERT(pBitmapInfo);
        ASSERT(pBitmapInfo->structSize >= sizeof(BITMAP_INFO));

        LOG(("[AddBitmap] ObjId: %d ResId: %d PlaceAfter: %d\n", 
            objectId, pBitmapInfo->resourceId, pBitmapInfo->placeAfterObjectId));

        res = m_pTimelineWriter->PlaceObject(
            pBitmapInfo->resourceId, 
            objectId, 
            pBitmapInfo->placeAfterObjectId, 
            &pBitmapInfo->matrix);

        return res;
    }

    FCM::Result TimelineBuilder::AddMovieClip(FCM::U_Int32 objectId, MOVIE_CLIP_INFO* pMovieClipInfo, DOM::FrameElement::PIMovieClip pMovieClip)
    {
        FCM::Result res;
        FCM::AutoPtr<FCM::IFCMUnknown> pUnknown = pMovieClip;

        ASSERT(pMovieClipInfo);
        ASSERT(pMovieClipInfo->structSize >= sizeof(MOVIE_CLIP_INFO));
      
        LOG(("[AddMovieClip] ObjId: %d ResId: %d PlaceAfter: %d\n", 
            objectId, pMovieClipInfo->resourceId, pMovieClipInfo->placeAfterObjectId));

        res = m_pTimelineWriter->PlaceObject(
            pMovieClipInfo->resourceId, 
            objectId, 
            pMovieClipInfo->placeAfterObjectId, 
            &pMovieClipInfo->matrix,
            true,
            pUnknown);

        return res;
    }

    FCM::Result TimelineBuilder::AddGraphic(FCM::U_Int32 objectId, GRAPHIC_INFO* pGraphicInfo)
    {
        FCM::Result res;

        ASSERT(pGraphicInfo);
        ASSERT(pGraphicInfo->structSize >= sizeof(GRAPHIC_INFO));

        LOG(("[AddGraphic] ObjId: %d ResId: %d PlaceAfter: %d\n", 
            objectId, pGraphicInfo->resourceId, pGraphicInfo->placeAfterObjectId));

        res = m_pTimelineWriter->PlaceObject(
            pGraphicInfo->resourceId, 
            objectId, 
            pGraphicInfo->placeAfterObjectId, 
            &pGraphicInfo->matrix,
            false,
            NULL);

        return res;
    }

    FCM::Result TimelineBuilder::AddSound(
        FCM::U_Int32 objectId, 
        SOUND_INFO* pSoundInfo, 
        DOM::FrameElement::PISound pSound)
    {
        FCM::AutoPtr<FCM::IFCMUnknown> pUnknown = pSound;
        FCM::Result res;

        ASSERT(pSoundInfo);
        ASSERT(pSoundInfo->structSize == sizeof(SOUND_INFO));

        LOG(("[AddSound] ObjId: %d ResId: %d\n", 
            objectId, pSoundInfo->resourceId));

        res = m_pTimelineWriter->PlaceObject(
            pSoundInfo->resourceId, 
            objectId, 
            pUnknown);

        return res;
    }

    FCM::Result TimelineBuilder::UpdateZOrder(FCM::U_Int32 objectId, FCM::U_Int32 placeAfterObjectId)
    {
        FCM::Result res = FCM_SUCCESS;

        LOG(("[UpdateZOrder] ObjId: %d PlaceAfter: %d\n", 
            objectId, placeAfterObjectId));

        res = m_pTimelineWriter->UpdateZOrder(objectId, placeAfterObjectId);

        return res;
    }

     FCM::Result TimelineBuilder::UpdateMask(FCM::U_Int32 objectId, FCM::U_Int32 maskTillObjectId)
    {
        FCM::Result res = FCM_SUCCESS;

        LOG(("[UpdateMask] ObjId: %d MaskTill: %d\n", 
            objectId, maskTillObjectId));

        res = m_pTimelineWriter->UpdateMask(objectId, maskTillObjectId);

        return res;
    }
     
     FCM::Result TimelineBuilder::Remove(FCM::U_Int32 objectId)
    {
        FCM::Result res;

        LOG(("[Remove] ObjId: %d\n", objectId));

        res = m_pTimelineWriter->RemoveObject(objectId);

        return res;
    }

    FCM::Result TimelineBuilder::UpdateBlendMode(FCM::U_Int32 objectId, DOM::FrameElement::BlendMode blendMode)
    {
        FCM::Result res;

        LOG(("[UpdateBlendMode] ObjId: %d BlendMode: %d\n", objectId, blendMode));

        res = m_pTimelineWriter->UpdateBlendMode(objectId, blendMode);

        return res;
    }

    FCM::Result TimelineBuilder::UpdateVisibility(FCM::U_Int32 objectId, FCM::Boolean visible)
    {
        FCM::Result res;

        LOG(("[UpdateVisibility] ObjId: %d Visible: %d\n", objectId, visible));

        res = m_pTimelineWriter->UpdateVisibility(objectId, visible);

        return res;
    }


    FCM::Result TimelineBuilder::UpdateGraphicFilter(FCM::U_Int32 objectId, PIFCMList pFilterable)
    {
        FCM::U_Int32 count;
        FCM::Result res;
        FCM::FCMListPtr pFilterList;

        LOG(("[UpdateGraphicFilter] ObjId: %d\n", objectId));

        res = pFilterable->Count(count);
        ASSERT(FCM_SUCCESS_CODE(res));
        
        for (FCM::U_Int32 i = 0; i < count; i++)
        {
            FCM::AutoPtr<FCM::IFCMUnknown> pUnknown = (*pFilterable)[i];
            res = m_pTimelineWriter->AddGraphicFilter(objectId, pUnknown.m_Ptr);

            if (FCM_FAILURE_CODE(res))
            {
                return res;
            }
        }

        return FCM_SUCCESS;
    }


    FCM::Result TimelineBuilder::UpdateDisplayTransform(FCM::U_Int32 objectId, const DOM::Utils::MATRIX2D& matrix)
    {
        FCM::Result res;

        LOG(("[UpdateDisplayTransform] ObjId: %d\n", objectId));

        res = m_pTimelineWriter->UpdateDisplayTransform(objectId, matrix);

        return res;
    }

    FCM::Result TimelineBuilder::UpdateColorTransform(FCM::U_Int32 objectId, const DOM::Utils::COLOR_MATRIX& colorMatrix)
    {
        FCM::Result res;

        LOG(("[UpdateColorTransform] ObjId: %d\n", objectId));

        res = m_pTimelineWriter->UpdateColorTransform(objectId, colorMatrix);

        return res;
    }

    FCM::Result TimelineBuilder::ShowFrame()        
    {
        FCM::Result res;

        LOG(("[ShowFrame] Frame: %d\n", m_frameIndex));

        res = m_pTimelineWriter->ShowFrame(m_frameIndex);

        m_frameIndex++;

        return res;
    }

    FCM::Result TimelineBuilder::AddFrameScript(FCM::CStringRep16 pScript, FCM::U_Int32 layerNum)
    {
        FCM::Result res = FCM_SUCCESS;

        LOG(("[AddFrameScript] LayerNum: %d\n", layerNum));

        if (pScript != NULL)
        {
            res = m_pTimelineWriter->AddFrameScript(pScript, layerNum);
        }

        return res;
    }

    FCM::Result TimelineBuilder::RemoveFrameScript(FCM::U_Int32 layerNum)
    {
        FCM::Result res = FCM_SUCCESS;

        LOG(("[RemoveFrameScript] LayerNum: %d\n", layerNum));

        res = m_pTimelineWriter->RemoveFrameScript(layerNum);

        return res;
    }

    FCM::Result TimelineBuilder::SetFrameLabel(FCM::StringRep16 pLabel, DOM::KeyFrameLabelType labelType)
    {
        FCM::Result res = FCM_SUCCESS;

        LOG(("[SetFrameLabel]\n"));

        if (pLabel != NULL)
        {
            res = m_pTimelineWriter->SetFrameLabel(pLabel, labelType);
        }

        return res;
    }

    FCM::Result TimelineBuilder::Build(
        FCM::U_Int32 resourceId, 
        FCM::StringRep16 pName,
        ITimelineWriter** ppTimelineWriter)
    {
        FCM::Result res;

        res = m_pOutputWriter->EndDefineTimeline(resourceId, pName, m_pTimelineWriter);

        *ppTimelineWriter = m_pTimelineWriter;

        return res;
    }


    TimelineBuilder::TimelineBuilder() :
        m_pOutputWriter(NULL),
        m_frameIndex(0)
    {
        //LOG(("[CreateTimeline]\n"));
    }

    TimelineBuilder::~TimelineBuilder()
    {
    }

    void TimelineBuilder::Init(IOutputWriter* pOutputWriter, DataPrecision precision)
    {
        m_pOutputWriter = pOutputWriter;

        m_pOutputWriter->StartDefineTimeline();

        m_pTimelineWriter = new JSONTimelineWriter(GetCallback(), precision);
        ASSERT(m_pTimelineWriter);
    }

    /* ----------------------------------------------------- TimelineBuilderFactory */

    TimelineBuilderFactory::TimelineBuilderFactory()
    {
    }

    TimelineBuilderFactory::~TimelineBuilderFactory()
    {
    }

    FCM::Result TimelineBuilderFactory::CreateTimelineBuilder(PITimelineBuilder& pTimelineBuilder)
    {
        FCM::Result res = GetCallback()->CreateInstance(NULL, CLSID_TimelineBuilder, IID_ITIMELINE_BUILDER_2, (void**)&pTimelineBuilder);

        TimelineBuilder* pTimeline = static_cast<TimelineBuilder*>(pTimelineBuilder);
        
        pTimeline->Init(m_pOutputWriter, m_dataPrecision);

        return res;
    }

    void TimelineBuilderFactory::Init(IOutputWriter* pOutputWriter, DataPrecision dataPrecision)
    {
        m_pOutputWriter = pOutputWriter;
        m_dataPrecision = dataPrecision;
    }

    FCM::Result RegisterPublisher(PIFCMDictionary pPlugins, FCM::FCMCLSID docId)
    {
        FCM::Result res;

        /*
         * Dictionary structure for a Publisher plugin is as follows:
         *
         *  Level 0 :    
         *              --------------------------
         *             | Flash.Component |  ----- | -----------------------------
         *              --------------------------                               |
         *                                                                       |
         *  Level 1:                                   <-------------------------                          
         *              ------------------------------  
         *             | CLSID_Publisher_GUID | ----- | -------------------------
         *              ------------------------------                           |
         *                                                                       |
         *  Level 2:                                      <---------------------- 
         *              -----------------------------------
         *             | Flash.Component.Category.Publisher |  ----- |-----------
         *              -----------------------------------                      |
         *                                                                       |
         *  Level 3:                                                           <-
         *              -------------------------------------------------------------------
         *             | Flash.Component.Category.Name           | PUBLISHER_NAME          |
         *              -------------------------------------------------------------------|
         *             | Flash.Component.Category.UniversalName  | PUBLISHER_UNIVERSAL_NAME|
         *              -------------------------------------------------------------------|
         *             | Flash.Component.Publisher.UI            | PUBLISH_SETTINGS_UI_ID  |
         *              -------------------------------------------------------------------|
         *             | Flash.Component.Publisher.TargetDocs    |              -----------|--
         *              -------------------------------------------------------------------| |
         *                                                                                   |
         *  Level 4:                                                    <--------------------
         *              -----------------------------------------------
         *             | CLSID_DocType   |  Empty String               |
         *              -----------------------------------------------
         *
         *  Note that before calling this function the level 0 dictionary has already
         *  been added. Here, the 1st, 2nd and 3rd level dictionaries are being added.
         */ 

        {
            // Level 1 Dictionary
            AutoPtr<IFCMDictionary> pPlugin;
            res = pPlugins->AddLevel(
                (const FCM::StringRep8)Utils::ToString(CLSID_Publisher).c_str(), 
                pPlugin.m_Ptr);

            {
                // Level 2 Dictionary
                AutoPtr<IFCMDictionary> pCategory;
                res = pPlugin->AddLevel(
                    (const FCM::StringRep8)kFlashCategoryKey_Publisher, 
                    pCategory.m_Ptr);

                {
                    // Level 3 Dictionary

                    // Add short name
                    std::string str_name = PUBLISHER_NAME;
                    res = pCategory->Add(
                        (const FCM::StringRep8)kFlashCategoryKey_Name,
                        kFCMDictType_StringRep8, 
                        (FCM::PVoid)str_name.c_str(),
                        (FCM::U_Int32)str_name.length() + 1);

                    // Add universal name - Used to refer to it from JSFL. Also, used in 
                    // error/warning messages.
                    std::string str_uniname = PUBLISHER_UNIVERSAL_NAME;
                    res = pCategory->Add(
                        (const FCM::StringRep8)kFlashCategoryKey_UniversalName,
                        kFCMDictType_StringRep8,
                        (FCM::PVoid)str_uniname.c_str(),
                        (FCM::U_Int32)str_uniname.length() + 1);

                    std::string str_ui = PUBLISH_SETTINGS_UI_ID;
                    res = pCategory->Add(
                        (const FCM::StringRep8)kFlashPublisherKey_UI, 
                        kFCMDictType_StringRep8, 
                        (FCM::PVoid)str_ui.c_str(),
                        (FCM::U_Int32)str_ui.length() + 1);

                    AutoPtr<IFCMDictionary> pDocs;
                    res = pCategory->AddLevel((const FCM::StringRep8)kFlashPublisherKey_TargetDocs, pDocs.m_Ptr);

                    {
                        // Level 4 Dictionary
                        std::string empytString = "";   
                        res = pDocs->Add(
                            (const FCM::StringRep8)Utils::ToString(docId).c_str(), 
                            kFCMDictType_StringRep8, 
                            (FCM::PVoid)empytString.c_str(),
                            (FCM::U_Int32)empytString.length() + 1);
                    }
                }
            }
        }

        return res;
    }
};
