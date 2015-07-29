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

#include "FCMPluginInterface.h"

#include "DocType.h"
#include "Publisher.h"
#include "Utils.h"

namespace SnapSVGAnimator
{
    BEGIN_MODULE(SnapSVGAnimatorModule)

        BEGIN_CLASS_ENTRY

            CLASS_ENTRY(CLSID_DocType, CDocType)
            CLASS_ENTRY(CLSID_FeatureMatrix, FeatureMatrix)
            CLASS_ENTRY(CLSID_Publisher, CPublisher)
            CLASS_ENTRY(CLSID_ResourcePalette, ResourcePalette)
            CLASS_ENTRY(CLSID_TimelineBuilder, TimelineBuilder)
            CLASS_ENTRY(CLSID_TimelineBuilderFactory, TimelineBuilderFactory)

        END_CLASS_ENTRY

        public:
            void SetResPath(const std::string& resPath) {m_resPath = resPath;}
            const std::string& GetResPath() {return m_resPath;}

        private:
            std::string m_resPath;

    END_MODULE

    
    SnapSVGAnimatorModule g_snapSVGAnimatorModule;

    extern "C" FCMPLUGIN_IMP_EXP FCM::Result PluginBoot(FCM::PIFCMCallback pCallback)
    {
        FCM::Result res;
        std::string langCode;
        std::string modulePath;

        res = g_snapSVGAnimatorModule.init(pCallback);

        Utils::GetModuleFilePath(modulePath, pCallback);
        Utils::GetLanguageCode(pCallback, langCode);

        g_snapSVGAnimatorModule.SetResPath(modulePath + "../res/" + langCode + "/");
        return res;
    }

    extern "C" FCMPLUGIN_IMP_EXP FCM::Result PluginGetClassInfo(
        FCM::PIFCMCalloc pCalloc, 
        FCM::PFCMClassInterfaceInfo* ppClassInfo)
    {
        return g_snapSVGAnimatorModule.getClassInfo(pCalloc, ppClassInfo);
    }

    extern "C" FCMPLUGIN_IMP_EXP FCM::Result PluginGetClassObject(
        FCM::PIFCMUnknown pUnkOuter, 
        FCM::ConstRefFCMCLSID clsid, 
        FCM::ConstRefFCMIID iid, 
        FCM::PPVoid pAny)
    {
        return g_snapSVGAnimatorModule.getClassObject(pUnkOuter, clsid, iid, pAny);
    }

    // Register the plugin - Register plugin as both DocType and Publisher
    extern "C" FCMPLUGIN_IMP_EXP FCM::Result PluginRegister(FCM::PIFCMPluginDictionary pPluginDict)
    {
        FCM::Result res = FCM_SUCCESS;

        AutoPtr<IFCMDictionary> pDictionary = pPluginDict;

        AutoPtr<IFCMDictionary> pPlugins;
        pDictionary->AddLevel((const FCM::StringRep8)kFCMComponent, pPlugins.m_Ptr);
    
        res = RegisterDocType(pPlugins, g_snapSVGAnimatorModule.GetResPath());
        if (FCM_FAILURE_CODE(res))
        {
            return res;
        }
        
        res = RegisterPublisher(pPlugins, CLSID_DocType);

        return res;
    }

    extern "C" FCMPLUGIN_IMP_EXP FCM::U_Int32 PluginCanUnloadNow(void)
    {
        return g_snapSVGAnimatorModule.canUnloadNow();
    }

    extern "C" FCMPLUGIN_IMP_EXP FCM::Result PluginShutdown()
    {
        g_snapSVGAnimatorModule.finalize();

        return FCM_SUCCESS;
    }

};
