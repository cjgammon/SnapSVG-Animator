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

/**
 * @file  HTTPServer.h
 *
 * @brief This file contains the implementation for a HTTP server using Mongoose.
 */

#ifndef HTTP_SERVER_H_
#define HTTP_SERVER_H_

#ifdef USE_HTTP_SERVER

#include <string>
#ifdef _WINDOWS
#include <xmemory>
#endif
#include "mongoose.h"
#include "FCMErrors.h"
#include "FCMTypes.h"

/* -------------------------------------------------- Forward Decl */


/* -------------------------------------------------- Enums */


/* -------------------------------------------------- Macros / Constants */


/* -------------------------------------------------- Structs / Unions */

struct ServerConfigParam
{
    std::string root;
    int port;
};

/* -------------------------------------------------- Class Decl */

namespace CreateJS
{
    class HTTPServer
    {
    public:
        static HTTPServer* GetInstance();
        virtual ~HTTPServer();

        FCM::Result Start();
        void Stop();

        void SetConfig(const ServerConfigParam& config);

    private:
        HTTPServer();

        static int BeginRequestHandler(struct mg_connection *conn);

    private:
        static std::auto_ptr<HTTPServer> m_instance;
        ServerConfigParam m_config;
        struct mg_context *m_context;
    };

}

#endif // USE_HTTP_SERVER

#endif // HTTP_SERVER_H_
