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

#ifdef FCM_ALIGNMENT
    
    #if defined(__GNUC__)
        #if defined(FCM_OS_64) 
            #pragma pack(push, FCMAlign, 16)
        #else
            #error "Unsupported platform bit depth"
        #endif
    #else
        #error "Unsupported mac compiler"
    #endif

#endif // FCM_ALIGNMENT

