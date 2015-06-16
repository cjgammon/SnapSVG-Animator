/*************************************************************************
* ADOBE CONFIDENTIAL
* ___________________
*
*  Copyright [2015] Adobe Systems Incorporated
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

#ifdef USE_HTTP_SERVER

#include "HTTPServer.h"
#include "Utils.h"

namespace CreateJS
{

    /* -------------------------------------------------- Constants */

    /* -------------------------------------------------- Static Functions */

    /* -------------------------------------------------- HTTPServer */

    std::auto_ptr<HTTPServer> HTTPServer::m_instance;

    HTTPServer::HTTPServer() : m_context(NULL)
    {
    }

    HTTPServer::~HTTPServer()
    {
        Stop();
    }

    HTTPServer* HTTPServer::GetInstance()
    {
        if (!m_instance.get())
        {
            m_instance.reset(new HTTPServer());
        }

        return m_instance.get();
    }

    void HTTPServer::SetConfig(const ServerConfigParam& config)
    {
        m_config = config;
    }

    FCM::Result HTTPServer::Start()
    {
        FCM::Result res = FCM_GENERAL_ERROR;
        std::string portStr;
        mg_callbacks callbacks;

        portStr = Utils::ToString(m_config.port);

        const char *options[] = {
            "document_root", m_config.root.c_str(),
            "listening_ports", portStr.c_str(),
            NULL
        };

        memset(&callbacks, 0, sizeof(mg_callbacks));
        callbacks.begin_request = BeginRequestHandler;

        // Start the server
        m_context = mg_start(&callbacks, NULL, options);

        if (m_context)
        {
            res = FCM_SUCCESS;
        }

        return res;
    }

    void HTTPServer::Stop()
    {
        if (m_context)
        {
            // Stop the server
            mg_stop(m_context);
            m_context = NULL;
        }
    }

    int HTTPServer::BeginRequestHandler(struct mg_connection *conn) 
    {
        // Inform Mongoose that it needs to handle the request
        return 0;
    }
};

#endif // USE_HTTP_SERVER
