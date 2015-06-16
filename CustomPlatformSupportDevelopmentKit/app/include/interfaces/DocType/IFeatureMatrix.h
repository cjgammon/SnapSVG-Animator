/******************************************************************************
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
******************************************************************************/

/**
 * @file  IFeatureMatrix.h
 *
 * @brief This file contains interface for IFeatureMatrix. 
 *        IFeatureMatrix provides methods to query the capabilities
 *        of a particular doc type.
 */

#ifndef IFEATURE_MATRIX_H_
#define IFEATURE_MATRIX_H_

#include "FCMPreConfig.h"
#include "FCMPluginInterface.h"


/* -------------------------------------------------- Forward Decl */


/* -------------------------------------------------- Enums */


/* -------------------------------------------------- Macros/Constants */

namespace DocType
{
    /**
     * @brief Defines the universally-unique interface ID for 
     *        IFeatureMatrix
     *
     * @note  Textual Representation:  {745F0985-43A9-4F11-BB66-6021D52223D7}
     */
    FCM::ConstFCMIID ID_IFEATURE_MATRIX =
        {0x745f0985, 0x43a9, 0x4f11, {0xbb, 0x66, 0x60, 0x21, 0xd5, 0x22, 0x23, 0xd7}};
}


/* -------------------------------------------------- Structs/Unions */


/* -------------------------------------------------- Class Decl */

namespace DocType
{

    /**
     * @class IFeatureMatrix
     *
     * @brief Defines an interface that represents the feature matrix for the associated 
     *        document type.
     */
    BEGIN_DECLARE_INTERFACE(IFeatureMatrix, ID_IFEATURE_MATRIX)

         /**  
          * @brief This function indicates whether a feature is supported or not.
          *
          * @param inFeatureName (IN) 
          *        Name of the feature. 
          *
          * @param isSupported (OUT) 
          *        This parameter is updated based on whether the feature is supported or not.
          *        For any value of inFeatureName not mentioned in the list below, isSupported
          *        should be set to FALSE.
          *
          * @return On success, FCM_SUCCESS is returned; else an error code is returned.
          *
          * @note  The following are the possible valid values for the inFeatureName:
          *         "Stroke", 
          *         "Fill", 
          *         "Text", 
          *         "Scene", 
          *         "Components",
          *         "Debug", 
          *         "SwfObjects", 
          *         "3D", 
          *         "ScriptLinkage", 
          *         "9Slice", 
          *         "Symbol",
          *         "Filters", 
          *         "DropShadowFilter", 
          *         "GlowFilter", 
          *         "BlurFilter", 
          *         "BevelFilter",
          *         "GradientGlowFilter", 
          *         "GradientBevelFilter", 
          *         "AdjustColorFilter", 
          *         "Compiler", 
          *         "Video", 
          *         "FrameScript", 
          *         "RuntimeSharedLibraries", 
          *         "Accessibility", 
          *         "Sound", 
          *         "ActionScript", 
          *         "Layer", 
          *         "ClassicTween".
          */
         virtual FCM::Result _FCMCALL IsSupported(
             CStringRep16 inFeatureName, 
             FCM::Boolean& isSupported) = 0;


        /**  
         * @brief This function indicates whether a property of a feature is supported or not.
         *
         * @param inFeatureName (IN) 
         *        Name of the feature
         *
         * @param inPropName (IN)
         *        Name of the property related to feature(inFeatureName)
         *
         * @param isSupported (OUT) 
         *        This parameter is updated based on whether feature is supported or not
         *        For any cobination of <inFeatureName, inPropName> not mentioned in the list below,
         *        isSupported should be set to FALSE.
         *
         * @return On success, FCM_SUCCESS is returned; else an error code is returned.
         *
         * @note  The following are the possible valid combinations of 
         *        <inFeatureName, inPropName>:
         *          <"Stroke", "Style">,
         *          <"Stroke", "Scale">,
         *          <"Stroke", "VariableWidth">,
         *
         *          <"Fill", "Filltype">,
         *          <"Fill", "GradientFlow">,
         *          <"Fill", "LinearRGB">,
         *
         *          <"Text", "RenderAsHTML">,
         *          <"Text", "TextType">,
         *          <"Text", "TextFormat">,
         *          <"Text", "FontEmbed">,
         *          <"Text", "LetterSpacing">,
         *          <"Text", "AutoKern">,
         *          <"Text", "AntiAlias">,
         *          <"Text", "Selectable">,
         *          <"Text", "Border">,
         *          <"Text", "SuperScript">,
         *          <"Text", "SubScript">,
         *          <"Text", "Margins">,
         *          <"Text", "LineType">,
         *          <"Text", "TextLinkage">,
         *          <"Text", "Scrollable">
         *
         *          <"SwfObjects", "ImportEmbeddedSWF">
         *
         *          <"ScriptLinkage", "Declaration">,
         *          <"ScriptLinkage", "Definition">,
         *
         *          <"Symbol", "SymbolType">,
         *          <"Symbol", "BlendModes">,
         *          <"Symbol", "ColorEffect">,
         *          <"Symbol", "Render">,
         *          <"Symbol", "BitmapBackground">,
         *          <"Symbol", "Tracking">,
         *
         *          <"DropShadowFilter", "Quality">,
         *          <"DropShadowFilter", "Angle">,
         *          <"DropShadowFilter", "Distance">,
         *          <"DropShadowFilter", "Knockout">,
         *          <"DropShadowFilter", "Inner">,
         *          <"DropShadowFilter", "HideObject">,
         *
         *          <"GlowFilter", "Quality">,
         *          <"GlowFilter", "Knockout">,
         *          <"GlowFilter", "Inner">,
         *
         *          <"Sound", "Effect">,
         *          <"Sound", "Sync">,
         *          <"Sound", "FileFormat">,
         *          <"Sound", "Compression">,
         *
         *          <"Layer", "MaskLayer">,
         *          <"Layer", "GuideLayer">,
         *
         *          <"ClassicTween", "Rotate">,
         *          <"ClassicTween", "Scale">
         */
        virtual FCM::Result _FCMCALL IsSupported(
            CStringRep16 inFeatureName, 
            CStringRep16 inPropName, 
            FCM::Boolean& isSupported) = 0;


        /**  
         * @brief This function indicates whether a value is supported or not.
         *
         * @param inFeatureName (IN) 
         *        Name of the feature
         *
         * @param inPropName (IN)
         *        Name of the property related to feature(inFeatureName)
         *
         * @param inValName (IN)
         *        Name of the value assosciated with a property(inPropName)
         *        For any cobination of <inFeatureName, inPropName, inValName> not mentioned 
         *        in the list below, isSupported should be set to FALSE.
         *
         * @param isSupported (OUT)
         *        This param is updated according to whether value for the property is supported
         *
         * @return On success, FCM_SUCCESS is returned; else an error code is returned.
         *
         * @note  The following are the valid possible combinations of 
         *        <inFeatureName, inPropName, inValName>:
         *
         *          <"Stroke", "Style", "Solid">,
         *          <"Stroke", "Style", "Hairline">,
         *          <"Stroke", "Style", "Dashed">,
         *          <"Stroke", "Style", "Dotted">,
         *          <"Stroke", "Style", "Ragged">,
         *          <"Stroke", "Style", "Stippled">,
         *          <"Stroke", "Style", "Hatched">,
         *
         *          <"Stroke", "Scale", "Normal">,
         *          <"Stroke", "Scale", "Horizontal">,
         *          <"Stroke", "Scale", "Vertical">,
         *          <"Stroke", "Scale", "None">,
         *
         *          <"Fill", "Filltype", "NoFill">,
         *          <"Fill", "Filltype", "SolidFill">,
         *          <"Fill", "Filltype", "LinearGradientFill">,
         *          <"Fill", "Filltype", "RadialGradientFill">,
         *          <"Fill", "Filltype", "BitmapFill">,
         *
         *          <"Fill", "GradientFlow", "Extend">,
         *          <"Fill", "GradientFlow", "Reflect">,
         *          <"Fill", "GradientFlow", "Repeat">,
         *
         *          <"Text", "TextType", "StaticText">,
         *          <"Text", "TextType", "DynamicText">,
         *          <"Text", "TextType", "InputText">,
         *
         *          <"Text", "TextFormat", "LeftAlign">,
         *          <"Text", "TextFormat", "Justify">,
         *          <"Text", "TextFormat", "RightAlign">,
         *          <"Text", "TextFormat", "CentreAlign">,
         *
         *          <"Text", "LineType", "SingleLine">,
         *          <"Text", "LineType", "MultiLine">,
         *          <"Text", "LineType", "MultiLineNoWrap">,
         *          <"Text", "LineType", "Password">,
         *
         *          <"Symbol", "SymbolType", "MovieClip">,
         *          <"Symbol", "SymbolType", "Button">,
         *          <"Symbol", "SymbolType", "Graphic">,
         *
         *          <"Symbol", "BlendModes", "Normal">,
         *          <"Symbol", "BlendModes", "Layer">,
         *          <"Symbol", "BlendModes", "Darken">,
         *          <"Symbol", "BlendModes", "Add">,
         *          <"Symbol", "BlendModes", "Multiply">,
         *          <"Symbol", "BlendModes", "Lighten">,
         *          <"Symbol", "BlendModes", "Screen">,
         *          <"Symbol", "BlendModes", "Overlay">,
         *          <"Symbol", "BlendModes", "HardLight">,
         *          <"Symbol", "BlendModes", "Subtract">,
         *          <"Symbol", "BlendModes", "Difference">,
         *          <"Symbol", "BlendModes", "Invert">,
         *          <"Symbol", "BlendModes", "Alpha">,
         *          <"Symbol", "BlendModes", "Erase">,
         *
         *          <"Symbol", "ColorEffect", "None">,
         *          <"Symbol", "ColorEffect", "Brightness">,
         *          <"Symbol", "ColorEffect", "Tint">,
         *          <"Symbol", "ColorEffect", "Alpha">,
         *          <"Symbol", "ColorEffect", "Advanced">,
         *
         *          <"Symbol", "Render", "Original">,
         *          <"Symbol", "Render", "CacheAsBitmap">,
         *          <"Symbol", "Render", "ExportAsBitmap">,
         *
         *          <"Symbol", "BitmapBackground", "Transparent">,
         *          <"Symbol", "BitmapBackground", "Opaque">,
         *
         *          <"Symbol", "Tracking", "TrackAsButton">,
         *          <"Symbol", "Tracking", "TrackAsMenuItem">,
         *
         *          <"Sound", "Sync", "Event">,
         *          <"Sound", "Sync", "Start">,
         *          <"Sound", "Sync", "Stop">,
         *          <"Sound", "Sync", "Stream">,
         *
         *          <"Sound", "FileFormat", "AIF">,
         *          <"Sound", "FileFormat", "WAV">,
         *          <"Sound", "FileFormat", "MP3">,
         *          <"Sound", "FileFormat", "ASND">,
         *          <"Sound", "FileFormat", "SND">,
         *          <"Sound", "FileFormat", "AU">,
         *          <"Sound", "FileFormat", "SD2">,
         *          <"Sound", "FileFormat", "OGG">,
         *          <"Sound", "FileFormat", "OGA">,
         *          <"Sound", "FileFormat", "FLAC">,
         *
         *          <"Sound", "Compression", "Default">,
         *          <"Sound", "Compression", "ADPCM">,
         *          <"Sound", "Compression", "MP3">,
         *          <"Sound", "Compression", "Raw">,
         *          <"Sound", "Compression", "Speech">,
         *
         *          <"ClassicTween", "Rotate", "None">,
         *          <"ClassicTween", "Rotate", "Auto">,
         *          <"ClassicTween", "Rotate", "CW">,
         *          <"ClassicTween", "Rotate", "CCW">
         */
        virtual FCM::Result _FCMCALL IsSupported(
            CStringRep16 inFeatureName, 
            CStringRep16 inPropName, 
            CStringRep16 inValName, 
            FCM::Boolean& isSupported) = 0;


        /**  
         * @brief This function gives a default value for an unsupported property.
         *
         * @param inFeatureName (IN) 
         *        Name of the feature
         *
         * @param inPropName (IN)
         *        Name of the property related to the feature(inFeatureName)
         *
         * @param outDefVal (OUT) 
         *        This param is updated with the default value of unsupported property
         *
         * @return On success, FCM_SUCCESS is returned; otherwise an error code is returned.
         */
        virtual FCM::Result _FCMCALL GetDefaultValue(
            CStringRep16 inFeatureName, 
            CStringRep16 inPropName,
            FCM::VARIANT& outDefVal) = 0;

    END_DECLARE_INTERFACE 
} 

#include "FCMPostConfig.h"

#endif // IFEATURE_MATRIX_H_

