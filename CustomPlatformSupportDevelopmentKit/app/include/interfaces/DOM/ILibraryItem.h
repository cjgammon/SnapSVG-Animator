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
 * @file  ILibraryItem.h
 *
 * @brief This file contains the interface for ILibraryItem. 
 *        Each Library item is a reusable resource residing in the library. 
 */

#ifndef ILIBRARY_ITEM_H_
#define ILIBRARY_ITEM_H_

#include "FCMPreConfig.h"
#include "FCMPluginInterface.h"


/* -------------------------------------------------- Forward Decl */

namespace FCM
{
    FORWARD_DECLARE_INTERFACE(IFCMDictionary);
};


/* -------------------------------------------------- Enums */


/* -------------------------------------------------- Macros / Constants */

/**
 * @def   kLibProp_LinkageClass_DictKey
 *
 * @brief Dictionary key for linkage class name. 
 */
#define kLibProp_LinkageClass_DictKey                   "LinkageClass"


/**
 * @def   kLibProp_LinkageClass_DictType
 *
 * @brief Type of value for linkage class.
 */
#define kLibProp_LinkageClass_DictType                  FCM::kFCMDictType_StringRep8


/**
 * @def   kLibProp_LinkageBaseClass_DictKey
 *
 * @brief Dictionary key for linkage class base name. 
 */
#define kLibProp_LinkageBaseClass_DictKey               "LinkageBaseClass"


/**
 * @def   kLibProp_LinkageBaseClass_DictType
 *
 * @brief Type of value for linkage base class.
 */
#define kLibProp_LinkageBaseClass_DictType              FCM::kFCMDictType_StringRep8


/**
 * @def   kLibProp_LinkageIdentifier_DictKey
 *
 * @brief Dictionary key for linkage identifier. 
 */
#define kLibProp_LinkageIdentifier_DictKey              "LinkageIdentifier"


/**
 * @def   kLibProp_LinkageIdentifier_DictType
 *
 * @brief Type of value for linkage identifer.
 */
#define kLibProp_LinkageIdentifier_DictType             FCM::kFCMDictType_StringRep8


/**
 * @def   kLibProp_LinkageURL_DictKey
 *
 * @brief Dictionary key for linkage URL. 
 */
#define kLibProp_LinkageURL_DictKey                     "LinkageURL"


/**
 * @def   kLibProp_LinkageURL_DictType
 *
 * @brief Type of value for linkage URL.
 */
#define kLibProp_LinkageURL_DictType                    FCM::kFCMDictType_StringRep8


/**
 * @def   kLibProp_LinkageExportForRS_DictKey
 *
 * @brief Dictionary key for 'Linkage Export For Runtime Sharing'.
 */
#define kLibProp_LinkageExportForRS_DictKey             "LinkageExportForRS"


/**
 * @def   kLibProp_LinkageExportForRS_DictType
 *
 * @brief Type of value for 'Linkage Export For Runtime Sharing'.
 */
#define kLibProp_LinkageExportForRS_DictType            FCM::kFCMDictType_Bool


/**
 * @def   kLibProp_LinkageImportForRS_DictKey
 *
 * @brief Dictionary key for 'Import For Runtime Sharing'.
 */
#define kLibProp_LinkageImportForRS_DictKey             "LinkageImportForRS"


/**
 * @def   kLibProp_LinkageImportForRS_DictType
 *
 * @brief Type of value for 'Import For Runtime Sharing'.
 */
#define kLibProp_LinkageImportForRS_DictType            FCM::kFCMDictType_Bool


/**
 * @def   kLibProp_LinkageExportInFirstFrame_DictKey
 *
 * @brief Dictionary key for 'Export In First Frame'.
 */
#define kLibProp_LinkageExportInFirstFrame_DictKey      "LinkageExportInFirstFrame"


/**
 * @def   kLibProp_LinkageExportInFirstFrame_DictType
 *
 * @brief Type of value for 'Export In First Frame'.
 */
#define kLibProp_LinkageExportInFirstFrame_DictType     FCM::kFCMDictType_Bool


/**
 * @def   kLibProp_LinkageExportForAS_DictKey
 *
 * @brief Dictionary key for 'Export For Actionscript'.
 */
#define kLibProp_LinkageExportForAS_DictKey             "LinkageExportForAS"


/**
 * @def   kLibProp_LinkageExportForAS_DictType
 *
 * @brief Type of value for 'Export For Actionscript'.
 */
#define kLibProp_LinkageExportForAS_DictType            FCM::kFCMDictType_Bool


/**
 * @def   kLibProp_SymbolType_DictKey
 *
 * @brief Dictionary key for the property "type" of the Symbol. 
 */
#define kLibProp_SymbolType_DictKey                     "SymbolType"


/**
 * @def   kLibProp_SymbolType_DictType
 *
 * @brief Type of value of the property "type" of the Symbol.
 */
#define kLibProp_SymbolType_DictType					FCM::kFCMDictType_StringRep8

/**
* @def   kLibProp_SymbolType_Button
*
* @brief Dictionary value for the key kLibProp_SymbolType_DictKey if the symbol is Button
*/
#define kLibProp_SymbolType_Button                     "Button"

/**
* @def   kLibProp_SymbolType_MovieClip
*
* @brief Dictionary value for the key kLibProp_SymbolType_DictKey if the symbol is Movie Clip
*/
#define kLibProp_SymbolType_MovieClip                  "MovieClip"

/**
* @def   kLibProp_SymbolType_Graphic
*
* @brief Dictionary value for the key kLibProp_SymbolType_DictKey if the symbol is Graphic
*/
#define kLibProp_SymbolType_Graphic                    "Graphic"


namespace DOM
{
    /**
     * @brief Defines the universally-unique interface ID for 
     *        ILibraryItem.
     *
     * @note  Textual Representation: {2EA5B91D-26C8-45A4-8BDC-41AD1F2C7B73}
     */
    FCM::ConstFCMIID IID_ILIBRARY_ITEM =
        {0x2ea5b91d, 0x26c8, 0x45a4, {0x8b, 0xdc, 0x41, 0xad, 0x1f, 0x2c, 0x7b, 0x73}};
}


/* -------------------------------------------------- Structs / Unions */


/* -------------------------------------------------- Class Decl */

namespace DOM
{
    /**
    * @class ILibraryItem
    *
    * @brief Defines the interface that represents a library item in the library. 
    */
    BEGIN_DECLARE_INTERFACE(ILibraryItem, IID_ILIBRARY_ITEM)
        
        /**
         * @brief  Returns the name of the item.
         * 
         * @param  ppName (OUT)
         *         Name of the library item with the full path in Library.
         *
         * @return On success, FCM_SUCCESS is returned, else an error code is returned.
         *
         * @note   The memory allocated for 'ppName' in this function must be freed 
         *         by the caller using IFCMCalloc::Free().
         */
        virtual FCM::Result _FCMCALL GetName(FCM::StringRep16* ppName) = 0;


       /**
        * @brief  GetProperties returns a set of property names and their values.
        *
        * @param  pPropDict (OUT)
        *         Dictionary containing the preperties and their values. The possible properties 
        *         int the dictionary are as follows:
        *           kLibProp_LinkageClass_DictKey,
        *           kLibProp_LinkageBaseClass_DictKey,
        *           kLibProp_LinkageIdentifier_DictKey,
        *           kLibProp_LinkageURL_DictKey,
        *           kLibProp_LinkageExportForRS_DictKey,
        *           kLibProp_LinkageImportForRS_DictKey,
        *           kLibProp_LinkageExportInFirstFrame_DictKey,
        *           kLibProp_LinkageExportForAS_DictKey,
		*           kLibProp_SymbolType_DictKey
        *
        * @return On success, FCM_SUCCESS is returned, else an error code is returned.
        */    
        virtual FCM::Result _FCMCALL GetProperties(PIFCMDictionary& pPropDict) = 0;
    
    END_DECLARE_INTERFACE
};


/* -------------------------------------------------- Inline / Functions */


#include "FCMPostConfig.h"

#endif // ILIBRARY_ITEM_H_

