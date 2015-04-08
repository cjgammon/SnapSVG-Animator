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

// Must be balanced with FCMPreConfig.h!!!!!
#ifndef FCM_PRE_CONFIG_H_
    #error "Please include FCMPreConfig.h"
#endif


// Always set these
#ifdef FCM_OS_WIN
    #include "win/FCMPostAlign_WIN.h"    
    #pragma warning(pop)
#elif defined(FCM_OS_MAC)
    #include "mac/FCMPostAlign_MAC.h"
#else
    #error "Unsupported platform"
#endif


#ifndef FCM_POST_CONFIG_H_
#define FCM_POST_CONFIG_H_


#endif // FCM_POST_CONFIG_H_


