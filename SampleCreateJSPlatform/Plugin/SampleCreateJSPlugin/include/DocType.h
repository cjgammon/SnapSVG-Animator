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

/*
 * @file  DocType.h
 *
 * @brief This file contains declarations for a DocType plugin.
 */


#ifndef DOC_TYPE_H_
#define DOC_TYPE_H_

#include <map>

#include "Version.h"
#include "FCMTypes.h"
#include "FCMPluginInterface.h"
#include "FlashFCMPublicIDs.h"
#include "DocType/IDocType.h"
#include "DocType/IFeatureMatrix.h"
#include <string>
#include "PluginConfiguration.h"

/* -------------------------------------------------- Forward Decl */

using namespace FCM;
using namespace DocType;

namespace CreateJS
{
    class CDocType;
    class FeatureMatrix;
    class Value;
    class Property;
    class Feature;
    class FeatureDocumentHandler;
}

 
namespace CreateJS
{
    typedef std::map<std::string, Value*> StrValueMap;
    typedef std::map<std::string, Property*> StrPropertyMap;
    typedef std::map<std::string, Feature*> StrFeatureMap;
};


/* -------------------------------------------------- Class Decl */

namespace CreateJS
{
	class CDocType : public DocType::IDocType, public FCM::FCMObjectBase
    {
        BEGIN_INTERFACE_MAP(CDocType, SAMPLE_PLUGIN_VERSION)
            INTERFACE_ENTRY(IDocType)    
        END_INTERFACE_MAP
            
    public:

        virtual FCM::Result _FCMCALL GetFeatureMatrix(DocType::PIFeatureMatrix& pFeatureMatrix);

        CDocType();

        ~CDocType();

    private:

        DocType::PIFeatureMatrix m_fm;
    };

    
    class FeatureMatrix : public DocType::IFeatureMatrix, public FCM::FCMObjectBase
    {
        BEGIN_MULTI_INTERFACE_MAP(FeatureMatrix, SAMPLE_PLUGIN_VERSION)
            INTERFACE_ENTRY(IFeatureMatrix)
        END_INTERFACE_MAP

    public:

        virtual FCM::Result _FCMCALL IsSupported(
            CStringRep16 inFeatureName, 
            FCM::Boolean& isSupported);

        virtual FCM::Result _FCMCALL IsSupported(
            CStringRep16 inFeatureName, 
            CStringRep16 inPropName, 
            FCM::Boolean& isSupported);

        virtual FCM::Result _FCMCALL IsSupported(
            CStringRep16 inFeatureName, 
            CStringRep16 inPropName, 
            CStringRep16 inValName, 
            FCM::Boolean& isSupported);

        virtual FCM::Result _FCMCALL GetDefaultValue(
            CStringRep16 inFeatureName, 
            CStringRep16 inPropName,
            FCM::VARIANT& outDefVal);
        
        FeatureMatrix();

        ~FeatureMatrix();

        void Init(FCM::PIFCMCallback pCallback);

    private:

        FCM::Result StartElement(
            const std::string name,
            const std::map<std::string, std::string>& attrs);
        
        FCM::Result EndElement(const std::string name);            

        Feature* FindFeature(const std::string& inFeatureName);

        Feature* UpdateFeature(const std::map<std::string, std::string>& inAttrs);

        Property* UpdateProperty(Feature* inFeature, const std::map<std::string,std::string>& inAttrs);

        Value* UpdateValue(Property* inProperty, const std::map<std::string, std::string>& inAttrs);
            
    private:

        StrFeatureMap mFeatures;

        Feature* mCurrentFeature;

        Property* mCurrentProperty;

        bool m_bInited;
        
        friend class FeatureDocumentHandler;
    };


    class Value
    {
    public:

        Value(bool supported);

        ~Value();

        bool IsSupported();

    private:
        bool mbSupported;
    };


    class Property
    {
    public:
        Property(const std::string& def, bool supported);

        ~Property();

        Value* FindValue(const std::string& inValueName);

        bool AddValue(const std::string& valueName, Value* pValue);

        bool IsSupported();

        std::string GetDefault();
        
    private:
        std::string mDefault;
        bool mbSupported;
        StrValueMap mValues;
    };


    class Feature
    {

    public:

        Feature(bool supported);

        ~Feature();

        Property* FindProperty(const std::string& inPropertyName);

        bool AddProperty(const std::string& name, Property* pProperty);

        bool IsSupported();

    private:

        bool mbSupported;

        StrPropertyMap mProperties;
    };

    FCM::Result RegisterDocType(FCM::PIFCMDictionary pPlugins, const std::string& resPath);
};


#endif // DOC_TYPE_H_

