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
 * @file  Utils.h
 *
 * @brief This file contains few utility functions.
 */

#ifndef UTILS_H_
#define UTILS_H_

#include "FCMTypes.h"
#include "FCMPluginInterface.h"
#include "Utils/DOMTypes.h"
#include <string>
#include "FillStyle/IGradientFillStyle.h"
#include "FrameElement/IClassicText.h"
#include "FrameElement/IParagraph.h"
#include "FrameElement/ITextStyle.h"
#include "FrameElement/ITextBehaviour.h"
#include "IFCMStringUtils.h"
#include <iostream>
#include <fstream>

/* -------------------------------------------------- Forward Decl */

#ifdef USE_HTTP_SERVER
    struct sockaddr_in;
#endif // USE_HTTP_SERVER


/* -------------------------------------------------- Enums */


/* -------------------------------------------------- Macros / Constants */

#ifdef _DEBUG

    #define ASSERT(cond)                    \
    {                                       \
        if (!(cond))                        \
        {                                   \
            assert(0);                      \
        }                                   \
    }

#else
    
    #define ASSERT(cond)

#endif

#ifdef _DEBUG
#define LOG(x) Utils::Log x
#else
#define LOG(x) 
#endif

#ifdef USE_HTTP_SERVER
    #ifdef _WINDOWS
        #define CLOSE_SOCKET(sock) closesocket(sock)
    #else
        #ifdef _MAC
            #define CLOSE_SOCKET(sock) close(sock)
        #endif
    #endif
#endif

/* -------------------------------------------------- Structs / Unions */

#ifdef USE_HTTP_SERVER
    #ifdef _MAC
        typedef int SOCKET;
    #endif
#endif // USE_HTTP_SERVER


/* -------------------------------------------------- Class Decl */

namespace SnapSVGAnimator
{
    class Utils
    {
    public:

        static std::string ToString(const FCM::FCMGUID& in);

        static std::string ToString(const double& in);

        static std::string ToString(const float& in);

        static std::string ToString(const FCM::U_Int32& in);

        static std::string ToString(const FCM::S_Int32& in);

        static std::string ToString(const DOM::Utils::MATRIX2D& matrix);

        static std::string ToString(const DOM::Utils::COLOR_MATRIX& colorMatrix);

        static std::string ToString(const DOM::Utils::CapType& capType);

        static std::string ToString(const DOM::Utils::JoinType& joinType);

        static std::string ToString(const DOM::FrameElement::AAMode& aaMode);

        static std::string ToString(const DOM::FrameElement::OrientationMode& mode);

        static std::string ToString(const DOM::FrameElement::TextFlow& flow);

        static std::string ToString(const DOM::FrameElement::LineMode& mode);

        static std::string ToString(const DOM::FrameElement::AlignMode& mode);

        static std::string ToString(const DOM::FrameElement::BaseLineShiftStyle& lineShiftStyle);
        
        static std::string ToString(FCM::CStringRep16 pStr16, FCM::PIFCMCallback pCallback);

        static std::string ToString(FCM::CStringRep8 pStr8);

        static std::string ToString(const DOM::Utils::RECT& rect);
        
        static FCM::StringRep16 ToString16(const std::string& str, FCM::PIFCMCallback pCallback);

        static FCM::U_Int32 ToVersion(const std::string& versionStr);

        static std::string ToString(const DOM::FillStyle::GradientSpread& spread);

        static std::string ToString(const DOM::Utils::COLOR& color);

        static void TransformPoint(
            const DOM::Utils::MATRIX2D& matrix, 
            DOM::Utils::POINT2D& inPoint,
            DOM::Utils::POINT2D& outPoint);

        static void GetParent(const std::string& path, std::string& parent);

        static void GetFileName(const std::string& path, std::string& fileName);

        static void GetFileNameWithoutExtension(const std::string& path, std::string& fileName);

        static void GetFileExtension(const std::string& path, std::string& extension);

        static void GetModuleFilePath(std::string& path, FCM::PIFCMCallback pCallback);
        
        static FCM::Result CreateDir(const std::string& path, FCM::PIFCMCallback pCallback);

        static FCM::Result GetAppTempDir(FCM::PIFCMCallback pCallback, std::string& path);

        static FCM::AutoPtr<FCM::IFCMCalloc> GetCallocService(FCM::PIFCMCallback pCallback);
        
        static FCM::AutoPtr<FCM::IFCMStringUtils> GetStringUtilsService(FCM::PIFCMCallback pCallback);

        static void GetLanguageCode(FCM::PIFCMCallback pCallback, std::string& langCode);

        static void GetAppVersion(FCM::PIFCMCallback pCallback, FCM::U_Int32& version);
   
        static void Trace(FCM::PIFCMCallback pCallback, const char* str, ...);

        static void Log(const char* fmt, ...);

        static void OpenFStream(const std::string& outputFileName, std::fstream &file, std::ios_base::openmode mode, FCM::PIFCMCallback pCallback);

        static FCM::Result CopyDir(const std::string& srcFolder, const std::string& dstFolder, FCM::PIFCMCallback pCallback);

        static FCM::Result Remove(const std::string& folder, FCM::PIFCMCallback pCallback);

#ifdef USE_HTTP_SERVER

        static void LaunchBrowser(const std::string& outputFileName, int port, FCM::PIFCMCallback pCallback);
		
        static int GetUnusedLocalPort();

    private:

        static void InitSockAddr(sockaddr_in* sockAddr);

#endif
	
    };
};

#endif // UTILS_H_

