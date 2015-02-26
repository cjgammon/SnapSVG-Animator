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

#include "Utils.h"

#ifdef _WINDOWS
#include "Windows.h"
#endif

#ifdef __APPLE__
#include "CoreFoundation/CoreFoundation.h"
#include <dlfcn.h>
#endif

#include <iomanip>
#include <algorithm>
#include <sstream>

#include "IFCMStringUtils.h"

#include <string>
#include <cstring>
#include <stdlib.h>
#include "Application/Service/IOutputConsoleService.h"
#include "Application/Service/IFlashApplicationService.h"
#include "FlashFCMPublicIDs.h"

/* -------------------------------------------------- Constants */

#ifdef _WINDOWS
EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#endif

namespace CreateJS
{
    static std::string comma = ",";
}


/* -------------------------------------------------- Static Functions */


/* -------------------------------------------------- Utils */

namespace CreateJS
{
    FCM::AutoPtr<FCM::IFCMStringUtils> Utils::GetStringUtilsService(FCM::PIFCMCallback pCallback)
    {
        FCM::AutoPtr<FCM::IFCMUnknown> pIFCMStringUtilsUnknown;
        FCM::Result res = pCallback->GetService(FCM::SRVCID_Core_StringUtils, pIFCMStringUtilsUnknown.m_Ptr);
        if (FCM_FAILURE_CODE(res))
        {
            return NULL;
        }
        FCM::AutoPtr<FCM::IFCMStringUtils> pIFCMStringUtils = pIFCMStringUtilsUnknown;
        return pIFCMStringUtils;
    }
    

    FCM::AutoPtr<FCM::IFCMCalloc> Utils::GetCallocService(FCM::PIFCMCallback pCallback)
    {
        FCM::AutoPtr<FCM::IFCMUnknown> pIFCMCallocUnknown;
        FCM::Result res = pCallback->GetService(FCM::SRVCID_Core_Memory, pIFCMCallocUnknown.m_Ptr);
        if (FCM_FAILURE_CODE(res))
        {
            return NULL;
        }
        FCM::AutoPtr<FCM::IFCMCalloc> pIFCMCalloc = pIFCMCallocUnknown;
        return pIFCMCalloc;
    }
    

    void Utils::GetLanguageCode(FCM::PIFCMCallback pCallback, std::string& langCode)
    {
        FCM::StringRep8 pLanguageCode;
        FCM::AutoPtr<FCM::IFCMUnknown> pUnk;
        FCM::AutoPtr<Application::Service::IFlashApplicationService> pAppService;
        FCM::Result res;
        
        res = pCallback->GetService(Application::Service::FLASHAPP_SERVICE, pUnk.m_Ptr);
        pAppService = pUnk;

        if (pAppService)
        {
            res = pAppService->GetLanguageCode(&pLanguageCode);
            if (FCM_SUCCESS_CODE(res))
            {
                langCode = ToString(pLanguageCode);

                FCM::AutoPtr<FCM::IFCMCalloc> pCalloc = GetCallocService(pCallback);
                pCalloc->Free(pLanguageCode);
            }
        }
    }

    void Utils::GetAppVersion(FCM::PIFCMCallback pCallback, FCM::U_Int32& version)
    {
        FCM::AutoPtr<FCM::IFCMUnknown> pUnk;
        FCM::AutoPtr<Application::Service::IFlashApplicationService> pAppService;
        FCM::Result res;
        
        version = 0;

        res = pCallback->GetService(Application::Service::FLASHAPP_SERVICE, pUnk.m_Ptr);
        pAppService = pUnk;

        if (pAppService)
        {
            res = pAppService->GetVersion(version);
            ASSERT(FCM_SUCCESS_CODE(res))
        }
    }

    std::string Utils::ToString(const FCM::FCMGUID& in)
    {
        std::ostringstream result;
        unsigned i;

        result.fill('0');

        result << std::hex;
        result  << std::setw(8) << (in.Data1);
        result << "-";
        result  << std::setw(4) << (in.Data2);
        result << "-";
        result  << std::setw(4) << (in.Data3);
        result << "-";

        for (i = 0; i < 2 ; ++i)
        {
            result << std::setw(2) << (unsigned int) (in.Data4[i]);
        }
        result << "-";

        for (; i < 8 ; ++i)
        {
            result << std::setw(2) << (unsigned int) (in.Data4[i]);
        }

        std::string guid_str = result.str();

        std::transform(guid_str.begin(), guid_str.end(), guid_str.begin(), ::toupper);

        return guid_str;
    }

    
    std::string Utils::ToString(FCM::CStringRep16 pStr16, FCM::PIFCMCallback pCallback)
    {
        FCM::StringRep8 pStr8 = NULL;
        FCM::AutoPtr<FCM::IFCMStringUtils> pStrUtils = GetStringUtilsService(pCallback);
        pStrUtils->ConvertStringRep16to8(pStr16, pStr8);
        
        std::string string = (const char*)pStr8;
        
        FCM::AutoPtr<FCM::IFCMCalloc> pCalloc = GetCallocService(pCallback);
        pCalloc->Free(pStr8);
        
        return string;
    }

    std::string Utils::ToString(FCM::CStringRep8 pStr8)
    {
        std::string string = (const char*)pStr8;
        return string;
    }
    
    std::string Utils::ToString(const double& in)
    {
        char buffer[32];
        sprintf(buffer,"%.6f", in);
        std::string str(buffer);
        return str;
    }
    
    std::string Utils::ToString(const float& in)
    {
        char buffer[32];
        sprintf(buffer,"%.6f", in);
        std::string str(buffer);
        return str;
    }
    
    std::string Utils::ToString(const FCM::U_Int32& in)
    {
        char buffer[32];
        sprintf(buffer,"%u", in);
        std::string str(buffer);
        return str;
    }
    
    std::string Utils::ToString(const FCM::S_Int32& in)
    {
        char buffer[32];
        sprintf(buffer,"%d", in);
        std::string str(buffer);
        return str;
    }
    
    std::string Utils::ToString(const DOM::Utils::MATRIX2D& matrix)
    {
        std::string matrixString = "";

        matrixString.append(ToString(matrix.a));
        matrixString.append(comma);
        matrixString.append(ToString(matrix.b));
        matrixString.append(comma);
        matrixString.append(ToString(matrix.c));
        matrixString.append(comma);
        matrixString.append(ToString(matrix.d));
        matrixString.append(comma);
        matrixString.append(ToString(matrix.tx));
        matrixString.append(comma);
        matrixString.append(ToString(matrix.ty));

        return matrixString;
    }

    std::string Utils::ToString(const DOM::Utils::CapType& capType)
    {
        std::string str;

        switch (capType)
        {
            case DOM::Utils::NO_CAP:
                str = "butt";
                break;

            case DOM::Utils::ROUND_CAP:
                str = "round";
                break;

            case DOM::Utils::SQUARE_CAP:
                str = "square";
                break;
        }

        return str;
    }

    std::string Utils::ToString(const DOM::Utils::JoinType& joinType)
    {
        std::string str;

        switch (joinType)
        {
            case DOM::Utils::MITER_JOIN:
                str = "miter";
                break;

            case DOM::Utils::ROUND_JOIN:
                str = "round";
                break;

            case DOM::Utils::BEVEL_JOIN:
                str = "bevel";
                break;
        }

        return str;
    }

    
    FCM::StringRep16 Utils::ToString16(const std::string& str, FCM::PIFCMCallback pCallback)
    {
        FCM::StringRep16 pStrFeatureName = NULL;
        FCM::AutoPtr<FCM::IFCMStringUtils> pStrUtils = GetStringUtilsService(pCallback);
        pStrUtils->ConvertStringRep8to16(str.c_str(), pStrFeatureName);
        
        return pStrFeatureName;
    }


    std::string Utils::ToString(const DOM::FillStyle::GradientSpread& spread)
    {
        std::string res;

        switch (spread)
        {
            case DOM::FillStyle::GRADIENT_SPREAD_EXTEND:
                res = "pad";
                break;

            case DOM::FillStyle::GRADIENT_SPREAD_REFLECT:
                res = "reflect";
                break;

            case DOM::FillStyle::GRADIENT_SPREAD_REPEAT:
                res = "repeat";
                break;

            default:
                res = "none";
                break;
        }

        return res;
    }


    std::string Utils::ToString(const DOM::Utils::COLOR& color)
    {
        char cstr[5];
        std::string colorStr;

        colorStr.append("#");
        sprintf(cstr, "%02x", color.red);
        colorStr.append(cstr);
        sprintf(cstr, "%02x", color.green);
        colorStr.append(cstr);
        sprintf(cstr, "%02x", color.blue);
        colorStr.append(cstr);

        return colorStr;
    }


    void Utils::TransformPoint(
            const DOM::Utils::MATRIX2D& matrix, 
            DOM::Utils::POINT2D& inPoint,
            DOM::Utils::POINT2D& outPoint)
    {
        DOM::Utils::POINT2D loc;

        loc.x = inPoint.x * matrix.a + inPoint.y * matrix.c + matrix.tx;
        loc.y = inPoint.x * matrix.b + inPoint.y * matrix.d + matrix.ty;

        outPoint = loc;
    }

    void Utils::GetParent(const std::string& path, std::string& parent)
    {
        size_t index = path.find_last_of("/\\");
        if((index+1) == path.length())
        {
            parent = path.substr(0, index);
            index = parent.find_last_of("/\\");
        }
        parent = path.substr(0, index + 1);
    }

    void Utils::GetFileName(const std::string& path, std::string& fileName)
    {
        size_t index = path.find_last_of("/\\");
        fileName = path.substr(index + 1, path.length() - index - 1);
    }

    void Utils::GetFileNameWithoutExtension(const std::string& path, std::string& fileName)
    {
        GetFileName(path, fileName);

        // Remove the extension (if any)
        size_t index = fileName.find_last_of(".");
        if (index != std::string::npos)
        {
            fileName = fileName.substr(0, index);
        }
    }

    void Utils::GetFileExtension(const std::string& path, std::string& extension)
    {
        size_t index = path.find_last_of(".");
        extension = "";
        if (index != std::string::npos)
        {
            extension = path.substr(index + 1, path.length());
        }
    }

    void Utils::GetModuleFilePath(std::string& path, FCM::PIFCMCallback pCallback)
    {
#ifdef _WINDOWS

        std::string fullPath;
        FCM::U_Int16* pFilePath = new FCM::U_Int16[MAX_PATH];

        ASSERT(pFilePath);

        ::GetModuleFileName((HINSTANCE)&__ImageBase, pFilePath, MAX_PATH);
        
        fullPath = Utils::ToString(pFilePath, pCallback);

        GetParent(fullPath, path);

        delete[] pFilePath;
        
#else
        Dl_info info;
        if (dladdr((void*)(GetModuleFilePath), &info)) {
            std::string fullPath(info.dli_fname);
            GetParent(fullPath, path);
            GetParent(path, fullPath);
            GetParent(fullPath, path);
            GetParent(path, fullPath);
            path = fullPath;
        }
        else{
            ASSERT(0);
        }
#endif
    }
    
    void Utils::Trace(FCM::PIFCMCallback pCallback, const char* fmt, ...)
    {
        FCM::AutoPtr<FCM::IFCMUnknown> pUnk;
        FCM::AutoPtr<Application::Service::IOutputConsoleService> outputConsoleService;
        FCM::Result tempRes = pCallback->GetService(Application::Service::FLASHAPP_OUTPUT_CONSOLE_SERVICE, pUnk.m_Ptr);
        outputConsoleService = pUnk;
        pUnk.Reset();

        if (outputConsoleService)
        {
            va_list args;
            char buffer[1024];

            va_start(args, fmt);
            vsnprintf(buffer, 1024, fmt, args);
            va_end(args);

            FCM::AutoPtr<FCM::IFCMCalloc> pCalloc = CreateJS::Utils::GetCallocService(pCallback);
            ASSERT(pCalloc.m_Ptr != NULL);

            FCM::StringRep16 outputString = Utils::ToString16(std::string(buffer), pCallback);
            outputConsoleService->Trace(outputString);
            pCalloc->Free(outputString);
        }
    }

    void Utils::Log(const char* fmt, ...)
    {
        va_list args;
        char buffer[1024];

        va_start(args, fmt);
        vsnprintf(buffer, 1024, fmt, args);
        va_end(args);

        //printf(buffer);
    }
}

