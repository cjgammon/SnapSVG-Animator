/*************************************************************************
* ADOBE CONFIDENTIAL
* ___________________
*
*  Copyright [2014] Adobe Systems Incorporated
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
 * PLUGIN DEVELOPERS MUST CHANGE VALUES OF ALL THE MACROS AND CONSTANTS IN THIS FILE 
 * IN ORDER TO AVOID ANY CLASH WITH OTHER PLUGINS.
 */


#ifndef _PLUGIN_CONFIGURATION_H_
#define _PLUGIN_CONFIGURATION_H_

#include "FCMTypes.h"

#define PUBLISHER_NAME						"SnapSVGAnimator"
#define PUBLISHER_UNIVERSAL_NAME			"com.svg.SnapSVGAnimator"

/* The value of the PUBLISH_SETTINGS_UI_ID has to be the HTML extension ID used for Publish settings dialog*/
#define PUBLISH_SETTINGS_UI_ID				"com.svg.SnapSVGAnimator.PublishSettings"

#define DOCTYPE_NAME						"SnapSVGAnimator"
#define DOCTYPE_UNIVERSAL_NAME				"com.svg.SnapSVGAnimator"

/* This string must be localized for various locales and stored in the "res" in their respective lang folder. */
#define DOCTYPE_DESCRIPTION					"This document can be used to author content for SnapSVG animator."

/* The value of RUNTIME_FOLDER_NAME must be the name of the runtime folder present in EclipseProject/ExtensionContent. */
#define RUNTIME_FOLDER_NAME                 "SnapSVGAnimator"

/* Sub-folder within RUNTIME_FOLDER_NAME where the minified runtime resides */
#define RUNTIME_MINIFIED_SUBFOLDER_NAME     "js"

/* Minified runtime file name */
#define RUNTIME_MINIFIED_FILE_NAME          "main.min.js"

namespace SnapSVGAnimator
{
    // {C457B30A-A9EA-4FE6-B468-334F8F57075B}
    const FCM::FCMCLSID CLSID_DocType =
        {0xc457b30a, 0xa9ea, 0x4fe6, {0xb4, 0x68, 0x33, 0x4f, 0x8f, 0x57, 0x7, 0x5b}};

    // {F4CCD217-1ED5-4CD6-ADAA-CC2AEEE88076}
    const FCM::FCMCLSID CLSID_FeatureMatrix =
        {0xf4ccd217, 0x1ed5, 0x4cd6, {0xad, 0xaa, 0xcc, 0x2a, 0xee, 0xe8, 0x80, 0x76}};

    // {A87CD23C-344C-40E6-B003-BEED6580F099}
    const FCM::FCMCLSID CLSID_Publisher =
        {0xa87cd23c, 0x344c, 0x40e6, {0xb0, 0x3, 0xbe, 0xed, 0x65, 0x80, 0xf0, 0x99}};


    // {FBB6EAD4-D1EF-4614-B892-62DDB2DFC32C}
    const FCM::FCMCLSID CLSID_ResourcePalette =
        {0xfbb6ead4, 0xd1ef, 0x4614, {0xb8, 0x92, 0x62, 0xdd, 0xb2, 0xdf, 0xc3, 0x2c}};

    // {89CC21D3-1A92-44D3-8C0E-D0766DF45387}
    const FCM::FCMCLSID CLSID_TimelineBuilder =
        {0x89cc21d3, 0x1a92, 0x44d3, {0x8c, 0xe, 0xd0, 0x76, 0x6d, 0xf4, 0x53, 0x87}};

    // {A1BAE394-B460-42DA-BF7A-EEA68BC2E275}
    const FCM::FCMCLSID CLSID_TimelineBuilderFactory =
        {0xa1bae394, 0xb460, 0x42da, {0xbf, 0x7a, 0xee, 0xa6, 0x8b, 0xc2, 0xe2, 0x75}};
}


#endif // _PLUGIN_CONFIGURATION_H_
