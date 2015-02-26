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

#include "MyXmlParser.h"
#include "IFCMStringUtils.h"
#include "Utils.h"
#include "DocType.h"
#include <map>

namespace CreateJS
{
    typedef std::pair <std::string, std::string> Pair_Attrib;

    FeatureDocumentHandler::FeatureDocumentHandler(CreateJS::FeatureMatrix *pFeatureMat) : 
        DocumentHandler(), 
        m_pFeatureMat(pFeatureMat)
    {
    }


    void FeatureDocumentHandler::startElement(const XMLCh* const name, AttributeList& attrs)
    {
        char elementName[1024];

        XMLString::transcode(name, elementName, 1024 - 1);
        std::string elemName(elementName);
        std::map<std::string, std::string> attributes;
        
        for (unsigned int c = 0; c < attrs.getLength() ; c++) 
        {
            char attributeName[1024];
            char attributeValue [1024];
            XMLString::transcode(attrs.getName(c), attributeName, 1024 - 1);
            XMLString::transcode(attrs.getValue(c), attributeValue, 1024 - 1);
            std::string attribName(attributeName);
            std::string attribVal(attributeValue);

            attributes.insert(Pair_Attrib(attribName, attribVal));
        }
        m_pFeatureMat->StartElement(elemName, attributes);
    }
    
    void FeatureDocumentHandler::endElement(const XMLCh* const name)
    {
        char elementName[1024];
        XMLString::transcode(name, elementName, 1024 - 1);
        std::string elemName(elementName);
        m_pFeatureMat->EndElement(elemName);
    }

};
