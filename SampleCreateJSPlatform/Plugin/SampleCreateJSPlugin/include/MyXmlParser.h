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
 * @file  MyXMLParser.h
 *
 * @brief This file contains declarations for a sample .
 */


#ifndef MY_XML_PARSER_H_
#define MY_XML_PARSER_H_

#include "xercesc/parsers/SAXParser.hpp"
#include "xercesc/sax/DocumentHandler.hpp"

/* -------------------------------------------------- Forward Decl */

using namespace xercesc;

namespace CreateJS
{
    class FeatureMatrix;
}


/* -------------------------------------------------- Enums */


/* -------------------------------------------------- Macros / Constants */


/* -------------------------------------------------- Structs / Unions */


/* -------------------------------------------------- Class Decl */

namespace CreateJS
{
    class FeatureDocumentHandler : public DocumentHandler 
    {
    public:
        FeatureDocumentHandler(CreateJS::FeatureMatrix *pFeatureMat);
        
        virtual void characters(const XMLCh* const chars, const XMLSize_t length) {}
        virtual void startDocument() {}
        virtual void resetDocument() {}
        virtual void endDocument() {}
        virtual void ignorableWhitespace(const XMLCh* const chars, const XMLSize_t length) {}
        virtual void processingInstruction(const XMLCh* const target, const XMLCh* const data) {}
        virtual void setDocumentLocator(const Locator* const locator) {}
        virtual void startElement(const XMLCh* const name, AttributeList& attrs);
        virtual void endElement(const XMLCh* const name);

    private:
        CreateJS::FeatureMatrix *m_pFeatureMat;
    };
}

#endif // MY_XML_PARSER_H_

