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

#include "PluginConfiguration.h"
#include "Utils.h"

#ifdef _WINDOWS
#ifdef USE_HTTP_SERVER
    #include <WinSock.h>
#endif
    #include "Windows.h"
    #include "ShellApi.h"
    #include "ShlObj.h"
#endif

#ifdef __APPLE__
    #include "CoreFoundation/CoreFoundation.h"
    #include <dlfcn.h>
#ifdef USE_HTTP_SERVER    
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <sys/stat.h>    
#endif    
    #include <copyfile.h>
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

namespace SnapSVGAnimator
{
    static std::string comma = ",";
}


/* -------------------------------------------------- Static Functions */


/* -------------------------------------------------- Utils */

namespace SnapSVGAnimator
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

    void Utils::RemoveTrailingZeroes(char *str) 
    {
        char *ptr;

        ptr = str + strlen(str) - 1;

        while (*ptr == '0')
        {
            *ptr-- = '\0';
        }

        // If all digits after decimal are 0, remove point as well.
        if (*ptr == '.') 
        {        
            *ptr = '\0';
        }
    }

    std::string Utils::ToString(const double& in, int precision)
    {
        char buffer[32];

        ASSERT(precision >= 1 && precision <= 6);
        char* precisionFormat[] = {"%.1f", "%.2f", "%.3f", "%.4f", "%.5f", "%.6f"};

        sprintf(buffer, precisionFormat[precision - 1], in);

        RemoveTrailingZeroes(buffer);

        std::string str(buffer);
        return str;
    }
    
    std::string Utils::ToString(const float& in, int precision)
    {
        char buffer[32];

        ASSERT(precision >= 1 && precision <= 6);
        const char* precisionFormat[] = {"%.1f", "%.2f", "%.3f", "%.4f", "%.5f", "%.6f"};

        sprintf(buffer, precisionFormat[precision - 1], in);

        RemoveTrailingZeroes(buffer);

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
    
    std::string Utils::ToString(const DOM::Utils::MATRIX2D& matrix, FCM::U_Int8 precision)
    {
        std::string matrixString = "";

        matrixString.append(ToString(matrix.a, precision));
        matrixString.append(comma);
        matrixString.append(ToString(matrix.b, precision));
        matrixString.append(comma);
        matrixString.append(ToString(matrix.c, precision));
        matrixString.append(comma);
        matrixString.append(ToString(matrix.d, precision));
        matrixString.append(comma);
        matrixString.append(ToString(matrix.tx, precision));
        matrixString.append(comma);
        matrixString.append(ToString(matrix.ty, precision));

        return matrixString;
    }

    std::string Utils::ToString(const DOM::Utils::COLOR_MATRIX& colorMatrix, FCM::U_Int8 precision)
    {
        std::string matrixString = "";

        for (FCM::U_Int32 i = 0; i < 4; i++)
        {
            // Multiplicative factor
            matrixString.append(ToString(colorMatrix.matrix[i][i], precision));
            matrixString.append(comma);

            // Additive factor
            matrixString.append(ToString(colorMatrix.matrix[i][4], precision));
            matrixString.append(comma);
        }

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

    std::string Utils::ToString(const DOM::FrameElement::AAMode& aaMode)
    {
        std::string str;

        switch (aaMode)
        {
            case DOM::FrameElement::ANTI_ALIAS_MODE_DEVICE:
                str = "device";
                break;

            case DOM::FrameElement::ANTI_ALIAS_MODE_BITMAP:
                str = "bitmap";
                break;
           
            case DOM::FrameElement::ANTI_ALIAS_MODE_STANDARD:
                str = "standard";
                break;
           
            case DOM::FrameElement::ANTI_ALIAS_MODE_ADVANCED:
                str = "advanced";
                break;

            case DOM::FrameElement::ANTI_ALIAS_MODE_CUSTOM:
                str = "custom";
                break;

            default:
                ASSERT(0);
                break;
        }
        return str;
    }


    std::string Utils::ToString(const DOM::FrameElement::OrientationMode& mode)
    {
        std::string str;

        switch (mode)
        {
            case DOM::FrameElement::ORIENTATION_MODE_HORIZONTAL:
                str = "horizontal";
                break;

            case DOM::FrameElement::ORIENTATION_MODE_VERTICAL:
                str = "vertical";
                break;
           
            default:
                ASSERT(0);
                break;
        }
        return str;
    }

    std::string Utils::ToString(const DOM::FrameElement::TextFlow& flow)
    {
        std::string str;

        switch (flow)
        {
            case DOM::FrameElement::TEXT_FLOW_LEFT_TO_RIGHT:
                str = "leftToRight";
                break;

            case DOM::FrameElement::TEXT_FLOW_RIGHT_TO_LEFT:
                str = "rightToLeft";
                break;

            default:
                ASSERT(0);
                break;
        }
        return str;
    }

    std::string Utils::ToString(const DOM::FrameElement::LineMode& mode)
    {
        std::string str;

        switch (mode)
        {
            case DOM::FrameElement::LINE_MODE_SINGLE:
                str = "single";
                break;

            case DOM::FrameElement::LINE_MODE_MULTI:
                str = "multi";
                break;
           
            case DOM::FrameElement::LINE_MODE_MULTI_NO_WRAP:
                str = "multiNoWrap";
                break;
           
            default:
                ASSERT(0);
                break;
        }
        return str;
    }
    

    std::string Utils::ToString(const DOM::FrameElement::AlignMode& mode)
    {
        std::string str;

        switch (mode)
        {
             case DOM::FrameElement::ALIGN_MODE_LEFT:
                 str = "left";
                 break;

             case DOM::FrameElement::ALIGN_MODE_RIGHT:
                 str = "right";
                 break;

             case DOM::FrameElement::ALIGN_MODE_CENTER:
                 str = "center";
                 break;

             case DOM::FrameElement::ALIGN_MODE_JUSTIFY:
                 str = "justify";
                 break;

             default:
                 ASSERT(0);
                 break;
        }

        return str;
    }

    std::string Utils::ToString(const DOM::FrameElement::BaseLineShiftStyle& lineShiftStyle)
    {
        std::string str;

        switch (lineShiftStyle)
        {
             case DOM::FrameElement::BASE_LINE_SHIFT_STYLE_NORMAL:
                 str = "normal";
                 break;

             case DOM::FrameElement::BASE_LINE_SHIFT_STYLE_SUPERSCRIPT:
                 str = "superscript";
                 break;

             case DOM::FrameElement::BASE_LINE_SHIFT_STYLE_SUBSCRIPT:
                 str = "subscript";
                 break;

             default:
                 ASSERT(0);
                 break;
        }

        return str;
    }

    std::string Utils::ToString(const DOM::Utils::RECT& rect, FCM::U_Int8 precision)
    {
        std::string rectString = "";

        rectString.append(ToString(rect.topLeft.x, precision));
        rectString.append(comma);
        rectString.append(ToString(rect.topLeft.y, precision));
        rectString.append(comma);
        rectString.append(ToString(rect.bottomRight.x, precision));
        rectString.append(comma);
        rectString.append(ToString(rect.bottomRight.y, precision));

        return rectString;
    }

    FCM::StringRep16 Utils::ToString16(const std::string& str, FCM::PIFCMCallback pCallback)
    {
        FCM::StringRep16 pStrFeatureName = NULL;
        FCM::AutoPtr<FCM::IFCMStringUtils> pStrUtils = GetStringUtilsService(pCallback);
        pStrUtils->ConvertStringRep8to16(str.c_str(), pStrFeatureName);
        
        return pStrFeatureName;
    }


    // It is assumed that the versionStr is of the form "a.b.c.d".
    FCM::U_Int32 Utils::ToVersion(const std::string& versionStr)
    {
        FCM::U_Int32 ver = 0;
        FCM::S_Int32 shift = 24;
        size_t pos = 0;

        size_t nextPos = versionStr.find(".", pos);
        while ((shift >= 0) && (nextPos != pos))
        {
            FCM::U_Int32 val = atoi(versionStr.substr(pos, nextPos).c_str());

            ver = ver | (val << shift);
            pos = nextPos + 1;
            nextPos = versionStr.find(".", pos);
            shift -= 8;
        }

        return ver;
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

    FCM::Boolean Utils::FileExists(const std::string& path, FCM::PIFCMCallback pCallback)
    {
        std::fstream file;

        // TBD: This is a work-around. Use direct OS calls to determine if file exists.
        Utils::OpenFStream(path, file, std::ios_base::in, pCallback);
        if (file.is_open())
        {
            file.close();
            return true;
        }
        return false;
    }

    // Creates a directory. If the directory already exists or is successfully created, success
    // is returned; otherwise an error code is returned.
    FCM::Result Utils::CreateDir(const std::string& path, FCM::PIFCMCallback pCallback)
    {
#ifdef _WINDOWS

        FCM::Result res = FCM_SUCCESS;
        BOOL ret;
        FCM::StringRep16 pFullPath;

        pFullPath = Utils::ToString16(path, pCallback);
        ASSERT(pFullPath);

        ret = ::CreateDirectory(pFullPath, NULL);
        if (ret == FALSE)
        {
            DWORD err = GetLastError();
            if (err != ERROR_ALREADY_EXISTS)
            {
                res = FCM_GENERAL_ERROR;
            }
        }

        FCM::AutoPtr<FCM::IFCMCalloc> pCalloc = Utils::GetCallocService(pCallback);
        ASSERT(pCalloc.m_Ptr != NULL);  
        pCalloc->Free(pFullPath);

        return res;

#else
        struct stat sb;
        
        // Does the directory exist?
        if (stat(path.c_str(), &sb) == 0 && S_ISDIR(sb.st_mode))
        {
            return FCM_SUCCESS;
        }
        
        int err = mkdir(path.c_str(), 0777);
        if ((err == 0) || (err == EEXIST))
        {

            return FCM_SUCCESS;
        }

        return FCM_GENERAL_ERROR;
#endif
    }

    FCM::Result Utils::GetAppTempDir(FCM::PIFCMCallback pCallback, std::string& path)
    {
#ifdef _WINDOWS

        PWSTR pPath;
        FCM::Result ret;

        if (::SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, NULL, &pPath) == S_OK) 
        {
            path = Utils::ToString((FCM::CStringRep16)pPath, pCallback);
            path += "\\Temp\\";
            path += PUBLISHER_NAME;
            path += "\\";
            ret = CreateDir(path, pCallback);
        }
        else
        {
            ret = FCM_GENERAL_ERROR;
        }

        return ret;

#else
        FCM::Result ret = FCM_GENERAL_ERROR;
        
        // TODO: We are here using a deprecated function and will have to modernize it.
        char* name = tempnam(NULL, NULL);
        if (name)
        {
            std::string fullPath;
            fullPath = name;
            delete name;
            Utils::GetParent(fullPath, path);
            path += PUBLISHER_NAME;
            path += "/";
            ret = CreateDir(path, pCallback);
            ret = FCM_SUCCESS;
        }
        
        return ret;

#endif
    }

    void Utils::OpenFStream(const std::string& outputFileName, std::fstream &file, std::ios_base::openmode mode, FCM::PIFCMCallback pCallback)
    {
 
#ifdef _WINDOWS
        FCM::StringRep16 pFilePath = Utils::ToString16(outputFileName, pCallback);

        file.open(pFilePath,mode);

        FCM::AutoPtr<FCM::IFCMCalloc> pCalloc = Utils::GetCallocService(pCallback);
        ASSERT(pCalloc.m_Ptr != NULL);  
        pCalloc->Free(pFilePath);
#else
       file.open(outputFileName.c_str(),mode);
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
            vsnprintf(buffer, 1023, fmt, args);
            buffer[1023] = 0;
            va_end(args);

            FCM::AutoPtr<FCM::IFCMCalloc> pCalloc = SnapSVGAnimator::Utils::GetCallocService(pCallback);
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


    // Removes the folder all its contents
    FCM::Result Utils::Remove(const std::string& folder, FCM::PIFCMCallback pCallback)
    {

#ifdef _WINDOWS

        SHFILEOPSTRUCT sf;
        std::wstring wstr;

        memset(&sf, 0, sizeof(sf));

        sf.hwnd = NULL;
        sf.wFunc = FO_DELETE;
        sf.fFlags = FOF_NOCONFIRMATION | FOF_NOCONFIRMMKDIR | FOF_NOERRORUI | FOF_SILENT;
        FCM::StringRep16 folderStr = Utils::ToString16(folder, pCallback);;
        wstr = folderStr;
        wstr.append(1, '\0');
        sf.pFrom = wstr.c_str();
        sf.pTo = NULL;

        FCM::AutoPtr<FCM::IFCMCalloc> pCalloc = GetCallocService(pCallback);
        int n = SHFileOperation(&sf);
        if (n != 0)
        {
            pCalloc->Free(folderStr);
            return FCM_GENERAL_ERROR;
        }

        pCalloc->Free(folderStr);

#else
        std::string delFolder = folder + "/";
        std::string cmd = "rm -rf " + delFolder;
        std::system(cmd.c_str());
#endif

        return FCM_SUCCESS;
    }


    // Copies a source folder to a destination folder. In other words, dstFolder contains
    // the srcFolder after the operation.
    FCM::Result Utils::CopyDir(const std::string& srcFolder, const std::string& dstFolder, FCM::PIFCMCallback pCallback)
    {
#ifdef _WINDOWS

        SHFILEOPSTRUCT sf;
        std::wstring srcWstr;
        std::wstring dstWstr;

        memset(&sf, 0, sizeof(sf));

        sf.hwnd = NULL;
        sf.wFunc = FO_COPY;
        sf.fFlags = FOF_NOCONFIRMATION | FOF_NOCONFIRMMKDIR | FOF_NOERRORUI | FOF_SILENT;

        FCM::StringRep16 srcFolderStr = Utils::ToString16(srcFolder, pCallback);
        srcWstr = srcFolderStr;
        srcWstr.append(1, '\0');
        sf.pFrom = srcWstr.c_str();
        FCM::StringRep16 dstFolderStr = Utils::ToString16(dstFolder, pCallback);
        dstWstr = dstFolderStr;
        dstWstr.append(1, '\0');
        sf.pFrom = srcWstr.c_str();
        sf.pTo = dstWstr.c_str();

        FCM::AutoPtr<FCM::IFCMCalloc> pCalloc = GetCallocService(pCallback);
        int n = SHFileOperation(&sf);
        if (n != 0)
        {
            pCalloc->Free(srcFolderStr);
            pCalloc->Free(dstFolderStr);
            return FCM_GENERAL_ERROR;
        }

        pCalloc->Free(srcFolderStr);
        pCalloc->Free(dstFolderStr);
#else

        copyfile(srcFolder.c_str(), dstFolder.c_str(), NULL, COPYFILE_ALL | COPYFILE_RECURSIVE);
#endif
        return FCM_SUCCESS;
    }


    // Copies a source file to a destination folder. 
    FCM::Result Utils::CopyAFile(const std::string& srcFile, const std::string& dstFolder, FCM::PIFCMCallback pCallback)
    {
#ifdef _WINDOWS

        std::wstring srcWstr;
        std::wstring dstWstr;

        FCM::StringRep16 srcFileStr = Utils::ToString16(srcFile, pCallback);
        srcWstr = srcFileStr;
        srcWstr.append(1, '\0');

        FCM::StringRep16 dstFolderStr = Utils::ToString16(dstFolder, pCallback);
        dstWstr = dstFolderStr;
        dstWstr.append(1, '\0');

        ::CopyFile(srcWstr.c_str(), dstWstr.c_str(), false);

        FCM::AutoPtr<FCM::IFCMCalloc> pCalloc = GetCallocService(pCallback);

        pCalloc->Free(srcFileStr);
        pCalloc->Free(dstFolderStr);


#else

        copyfile(srcFile.c_str(), dstFolder.c_str(), NULL, COPYFILE_ALL);
#endif
        return FCM_SUCCESS;
    }


    bool Utils::ReadString(
        const FCM::PIFCMDictionary pDict,
        FCM::StringRep8 key, 
        std::string &retString)
    {
        FCM::U_Int32 valueLen;
        FCM::FCMDictRecTypeID type;

        FCM::Result res = pDict->GetInfo(key, type, valueLen);
        if (FCM_FAILURE_CODE(res))
        {
            return false;
        }

        FCM::StringRep8 strValue = new char[valueLen];
        res = pDict->Get(key, type, (FCM::PVoid)strValue, valueLen);
        if (FCM_FAILURE_CODE(res))
        {
            delete [] strValue;
            return false;
        }

        retString = strValue;

        delete [] strValue;
        return true;
    }


    bool Utils::ToBool(const std::string& str)
    {
        std::string tempStr = str;
        std::transform(tempStr.begin(), tempStr.end(), tempStr.begin(), ::tolower);
        if (tempStr == "true")
            return true;
        return false;
    }

    DataPrecision Utils::ToPrecision(const std::string& str)
    {
        DataPrecision precision;

        std::string compactDataStr = str;
        std::transform(compactDataStr.begin(), compactDataStr.end(), compactDataStr.begin(), ::tolower);

        if (compactDataStr == "low")
            precision = PRECISION_5;
        else if (compactDataStr == "medium")
            precision = PRECISION_4;
        else if (compactDataStr == "high")
            precision = PRECISION_3;
        else if (compactDataStr == "veryhigh")
            precision = PRECISION_2;
        else
            precision = PRECISION_6;

        return precision;
    }

#ifdef USE_HTTP_SERVER

    void Utils::LaunchBrowser(const std::string& outputFileName, int port, FCM::PIFCMCallback pCallback)
    {

#ifdef _WINDOWS

        std::wstring output = L"http://localhost:";
        std::wstring tail;
        tail.assign(outputFileName.begin(), outputFileName.end());
        FCM::StringRep16 portStr = Utils::ToString16(Utils::ToString(port), pCallback);
        output += portStr;
        output += L"/";
        output += tail;
        ShellExecute(NULL, L"open", output.c_str(), NULL, NULL, SW_SHOWNORMAL);

        FCM::AutoPtr<FCM::IFCMCalloc> pCalloc = GetCallocService(pCallback);
        pCalloc->Free(portStr);
#else

        std::string output = "http://localhost:";
        output += Utils::ToString(port);
        output += "/";
        output += outputFileName;
        std::string str = "/usr/bin/open " + output;
        popen(str.c_str(), "r");
        
#endif // _WINDOWS

    }

    int Utils::GetUnusedLocalPort()
    {
        sockaddr_in client;

        InitSockAddr(&client);

        SOCKET sock = socket(PF_INET, SOCK_STREAM, 0);
        
        // Look for a port in the private port range
        int minPortNumber = 49152;
        int maxPortNumber = 65535;
        int defaultPortNumber = 50000;
        
        int port = defaultPortNumber;
        
        int nTries = 0;
        int maxTries = 10;
        srand ((int)time(NULL));

        // Try connect
        while (nTries++ <= maxTries) 
        {
            client.sin_port = htons(port);

            int result = connect(sock, (struct sockaddr *) &client, sizeof(client));

            CLOSE_SOCKET(sock);

            // Connect unsuccessful, port is available.
            if (result != 0) 
            {
                break;  
            }

            // Retry at a random port number in the valid range
            port = minPortNumber + rand() % (maxPortNumber - minPortNumber - 1);
        }
        
        if (nTries > maxTries) 
        {
            port = -1;
        }
        
        return port;
    }

    void Utils::InitSockAddr(sockaddr_in* sockAddr)
    {
        ASSERT(sockAddr);

        memset(sockAddr, 0, sizeof(struct sockaddr_in));
        sockAddr->sin_family = AF_INET;
        char *ipAddressStr = (char*)"127.0.0.1";

#ifdef _MAC
        sockAddr->sin_addr.s_addr = inet_addr(ipAddressStr);
#endif
        
#ifdef _WINDOWS
        sockAddr->sin_addr.S_un.S_addr = inet_addr(ipAddressStr);
#endif

    }

#endif // USE_HTTP_SERVER
}

