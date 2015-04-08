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

/**
 * @file  FCMErrors.h
 *
 * @brief This file contains all the errors used in FCM.
 */

#ifndef FCM_ERRORS_H_
#define FCM_ERRORS_H_

#include "FCMPreConfig.h"


/* -------------------------------------------------- Forward Decl */


/* -------------------------------------------------- Enums */


/* -------------------------------------------------- Macros / Constants */

/**
 * @def   FCM_SUCCESS_CODE
 *
 * @brief Returns True if @a x is success code, else False.
 */
#define FCM_SUCCESS_CODE(x)                     ((x)==FCM_SUCCESS)


/**
 * @def   FCM_FAILURE_CODE
 *
 * @brief Returns True if @a x is not success code, else False. 
 */
#define FCM_FAILURE_CODE(x)                     ((x)!=FCM_SUCCESS)


/**
 * @def   FCM_GENERAL_ERROR_BASE
 *
 * @brief Base code for general errors.
 */
#define FCM_GENERAL_ERROR_BASE                  0x00000000L


/**
 * @def   FCM_SUCCESS
 *
 * @brief Success code
 */
#define FCM_SUCCESS                             (FCM_GENERAL_ERROR_BASE+0x00000000L)


/**
 * @def   FCM_GENERAL_ERROR
 *
 * @brief General error code
 */
#define FCM_GENERAL_ERROR                       (FCM_GENERAL_ERROR_BASE+0x00000001L)


/**
 * @def   FCM_CLS_NOT_DEFINED
 *
 * @brief Error code thrown if class is not defined.
 *
 */
#define FCM_CLS_NOT_DEFINED                     (FCM_GENERAL_ERROR_BASE+0x00000002L)


/**
 * @def   FCM_MEM_NOT_AVAILABLE
 *
 * @brief Error code thrown if memory is not available.
 */
#define FCM_MEM_NOT_AVAILABLE                   (FCM_GENERAL_ERROR_BASE+0x00000003L)


/**
 * @def   FCM_NO_INTERFACE
 *
 * @brief Error code thrown if interface is not available.
 */
#define FCM_NO_INTERFACE                        (FCM_GENERAL_ERROR_BASE+0x00000004L)


/**
 * @def   FCM_NO_CLASSFACTORY
 *
 * @brief Error code thrown if class factory is not available.
 */
#define FCM_NO_CLASSFACTORY                     (FCM_GENERAL_ERROR_BASE+0x00000005L)


/**
 * @def   FCM_PLUGIN_LOAD_FAILED
 *
 * @brief Error code thrown if plugin fails to load.
 *
 */
#define FCM_PLUGIN_LOAD_FAILED                  (FCM_GENERAL_ERROR_BASE+0x00000006L)


/**
 * @def   FCM_REG_CLS_NOT_DEFINED
 *
 * @brief Error code thrown if registration class is not defined.
 */
#define FCM_REG_CLS_NOT_DEFINED                 (FCM_GENERAL_ERROR_BASE+0x00000007L)


/**
 * @def   FCM_PLUGIN_FILE_NOT_FOUND
 *
 * @brief Error code thrown if plugin file is not found.
 */
#define FCM_PLUGIN_FILE_NOT_FOUND               (FCM_GENERAL_ERROR_BASE+0x00000008L)


/**
 * @def   FCM_SERVICE_ALREADY_EXISTS
 *
 * @brief Error code thrown if service already exists.
 */
#define FCM_SERVICE_ALREADY_EXISTS              (FCM_GENERAL_ERROR_BASE+0x00000009L)


/**
 * @def   FCM_SERVICE_NOT_FOUND
 *
 * @brief Error code thrown if service is not found.
 */
#define FCM_SERVICE_NOT_FOUND                   (FCM_GENERAL_ERROR_BASE+0x0000000AL)


/**
 * @def   FCM_NO_AGGREGATION
 *
 * @brief Error code thrown if interface is not found in case of aggregation.
 */
#define FCM_NO_AGGREGATION                      (FCM_GENERAL_ERROR_BASE+0x0000000BL)


/**
 * @def   FCM_INVALID_PARAM
 *
 * @brief Error code thrown if invalid param is passed.
 */
#define FCM_INVALID_PARAM                       (FCM_GENERAL_ERROR_BASE+0x0000000CL)


/**
 * @def   FCM_NEED_INITIALIZE
 *
 * @brief Error code thrown if initialization is needed.
 */
#define FCM_NEED_INITIALIZE                     (FCM_GENERAL_ERROR_BASE+0x0000000DL)


/**
 * @def   FCM_METHOD_NOT_IMPLEMENTED
 *
 * @brief Error code thrown if method is not implemented.
 */
#define FCM_METHOD_NOT_IMPLEMENTED              (FCM_GENERAL_ERROR_BASE+0x0000000EL)


/**
 * @def   FCM_INVALID_STRUCT_SIZE
 *
 * @brief Error code thrown if structure size is invalid.
 */
#define FCM_INVALID_STRUCT_SIZE                 (FCM_GENERAL_ERROR_BASE+0x0000000FL)


/**
 * @def   FCM_OPERATION_CANCELLED
 *
 * @brief Error code thrown if operation is cancelled.
 */
#define FCM_OPERATION_CANCELLED                 (FCM_GENERAL_ERROR_BASE+0x00000010L)


/**
 * @def   FCM_DICT_ERROR_BASE
 *
 * @brief Base code for dictionary errors.
 */
#define FCM_DICT_ERROR_BASE                     0x00002000L


/**
 * @def   FCM_DICT_ERR_NOTFOUND
 *
 * @brief Error code thrown for lookup of a nonexistent key.
 */
#define    FCM_DICT_ERR_NOTFOUND                (FCM_DICT_ERROR_BASE+0x00000001L)


/**
 * @def   FCM_DICT_ERR_WRONGTYPE
 *
 * @brief Error code thrown if key exists but holds a value of a different type than requested.
 */
#define    FCM_DICT_ERR_WRONGTYPE               (FCM_DICT_ERROR_BASE+0x00000002L)


/**
 * @def   FCM_DICT_ERR_CANTWRITE
 *
 * @brief Error code thrown if attempted to put or remove in a dictionary where that is 
 *        not allowed (e.g. structure dicts).
 */
#define    FCM_DICT_ERR_CANTWRITE               (FCM_DICT_ERROR_BASE+0x00000003L)


/**
 * @def   FCM_DICT_ERR_BUFFERTOOSMALL
 *
 * @brief Error code thrown if value won't fit into the buffer you passed.
 */
#define    FCM_DICT_ERR_BUFFERTOOSMALL          (FCM_DICT_ERROR_BASE+0x00000004L)


/**
 * @def   FCM_DICT_ERR_EXISING_KEY_FOUND
 *
 * @brief Error code thrown if the key is already existing in the dictionary.
 */
#define    FCM_DICT_ERR_EXISING_KEY_FOUND       (FCM_DICT_ERROR_BASE+0x00000005L)


/**
 * @def   FCM_DICT_ERR_INVALID_KEY_TYPE
 *
 * @brief Error code thrown if type is not found in FCMDictionaryRecType.
 */
#define    FCM_DICT_ERR_INVALID_KEY_TYPE        (FCM_DICT_ERROR_BASE+0x00000006L)


/**
 * @def   FCM_DICT_ERR_KEY_NOT_FOUND
 *
 * @brief Error code thrown if key is not found
 */
#define    FCM_DICT_ERR_KEY_NOT_FOUND           (FCM_DICT_ERROR_BASE+0x00000007L)


/**
 * @def   FCM_REG_DICT_ERR_NOTFOUND
 *
 * @brief Error code thrown if registry entry is not found in dictionary.
 */
#define FCM_REG_DICT_ERR_NOTFOUND               (FCM_DICT_ERROR_BASE+0x00000008L)


/**
 * @def   FCM_NOTIFY_ERR_BASE
 *
 * @brief Base code for notification errors.
 */
#define FCM_NOTIFY_ERR_BASE                     0x00004000L


/**
 * @def   FCM_NOTIFY_ERR_NOTIFCLIENT_NOT_FOUND
 *
 * @brief Error code thrown if notification client is not found.
 */
#define FCM_NOTIFY_ERR_NOTIFCLIENT_NOT_FOUND    (FCM_NOTIFY_ERR_BASE+0x00000001L)


/**
 * @def   FCM_NOTIFY_ERR_CLIENT_ALREADY_REG
 *
 * @brief Error code thrown if client is already registered for notificationID.
 */
#define FCM_NOTIFY_ERR_CLIENT_ALREADY_REG       (FCM_NOTIFY_ERR_BASE+0x00000002L)


/**
 * @def   FCM_NOTIFY_ERR_INVALID_CLIENT
 *
 * @brief Error code thrown if client is invalid.
 */
#define FCM_NOTIFY_ERR_INVALID_CLIENT           (FCM_NOTIFY_ERR_BASE+0x00000003L)


/**
 * @def   FCM_DOM_ERR_BASE
 *
 * @brief Base code for DOM errors.
 */
#define FCM_DOM_ERR_BASE                        0x00008000L


/**
 * @def   FCM_DOM_ERR_DOC_OPEN_FAILED
 *
 * @brief Error code thrown when failed to open the document.
 */
#define FCM_DOM_ERR_DOC_OPEN_FAILED             (FCM_DOM_ERR_BASE+0x00000001L)


/**
 * @def   FCM_DOM_ERR_DOC_CLOSE_FAILED
 *
 * @brief Error code thrown when failed to close the document.
 */
#define FCM_DOM_ERR_DOC_CLOSE_FAILED            (FCM_DOM_ERR_BASE+0x00000002L)


/**
 * @def   FCM_DOM_ERR_LAYER_HAS_NO_PARENT
 *
 * @brief Error code thrown when layer doesn't have a parent and client requested for one.
 */
#define FCM_DOM_ERR_LAYER_HAS_NO_PARENT         (FCM_DOM_ERR_BASE+0x00000003L)


/**
 * @def   FCM_DOM_ERR_LAYER_HAS_NO_FRAMES
 *
 * @brief Error code thrown when layer has no frames.
 */
#define FCM_DOM_ERR_LAYER_HAS_NO_FRAMES         (FCM_DOM_ERR_BASE+0x00000004L)


/**
 * @def   FCM_DOM_ERR_LAYER_INVALID_FRAMEINDEX
 *
 * @brief Error code thrown if frameindex is invalid. Valid range is from 0 to (FrameDuration-1).
 */
#define FCM_DOM_ERR_LAYER_INVALID_FRAMEINDEX    (FCM_DOM_ERR_BASE+0x00000005L)                


/**
 * @def   FCM_DOM_ERR_FRAME_NO_FRAMESCRIPT
 *
 * @brief Error code thrown when frame doesn't have script associated with it.
 */
#define FCM_DOM_ERR_FRAME_NO_FRAMESCRIPT        (FCM_DOM_ERR_BASE+0x00000006L)


/**
 * @def   FCM_DOM_ERR_INTERNAL
 *
 * @brief Error code thrown if internal error occurs and user has to abort.
 */
#define FCM_DOM_ERR_INTERNAL                    (FCM_DOM_ERR_BASE+0x00000007L)


/**
 * @def   FCM_DOM_ERR_INVALID_OBJECT
 *
 * @brief Error code thrown if the underlying object is not alive or it has changed.
 */
#define FCM_DOM_ERR_INVALID_OBJECT              (FCM_DOM_ERR_BASE+0x00000008L) 


/**
 * @def   FCM_IO_STREAM_ERR_BASE
 *
 * @brief Base code for IO errors.
 */
#define FCM_IO_STREAM_ERR_BASE                  0x0000C000L


/**
 * @def   FCM_IO_STREAM_ERR_STREAM_NOT_OPEN
 *
 * @brief Error code thrown if file can't be opened.
 */
#define FCM_IO_STREAM_ERR_STREAM_NOT_OPEN       (FCM_IO_STREAM_ERR_BASE+0x00000001L)


/**
 * @def   FCM_IO_STREAM_ERR_STREAM_ALREADY_OPEN
 *
 * @brief Error code thrown if file is already opened.
 */
#define FCM_IO_STREAM_ERR_STREAM_ALREADY_OPEN   (FCM_IO_STREAM_ERR_BASE+0x00000002L)


/**
 * @def   FCM_IO_STREAM_ERR_READ_PAST_END
 *
 * @brief Eror code thrown if trying to read beyond end of file.
 */
#define FCM_IO_STREAM_ERR_READ_PAST_END         (FCM_IO_STREAM_ERR_BASE+0x00000003L)


/**
 * @def   FCM_IO_STREAM_ERR_BAD_ACCESS_MODE
 *
 * @brief Error code thrown if bad access is encountered.
 */
#define FCM_IO_STREAM_ERR_BAD_ACCESS_MODE       (FCM_IO_STREAM_ERR_BASE+0x00000004L)


/**
 * @def   FCM_IO_STREAM_ERR_WROTE_PAST_END
 *
 * @brief Error code thrown when trying to write to beyond stream end.
 */
#define FCM_IO_STREAM_ERR_WROTE_PAST_END        (FCM_IO_STREAM_ERR_BASE+0x00000005L)


/**
 * @def   FCM_EXPORT_ERR_BASE
 *
 * @brief Base code for export errors.
 */
#define FCM_EXPORT_ERR_BASE                     0x0000D000L


/**
 * @def   FCM_EXPORT_FAILED
 *
 * @brief Error code thrown when export fails.
 */
#define FCM_EXPORT_FAILED                       (FCM_EXPORT_ERR_BASE+0x00000001L)



/**
 * @def   FCM_FLASH_APPLICATION_ERR_BASE
 *
 * @brief Base code for FlashApplication errors.
 */
#define FCM_FLASH_APPLICATION_ERR_BASE                  0x0000E000L

/**
 * @def   FCM_FLASH_APPLICATION_ERR_NO_OUTPUT_CONSOLE
 *
 * @brief Error code thrown when output console is not found.
 */
#define FCM_FLASH_APPLICATION_ERR_NO_OUTPUT_CONSOLE        (FCM_FLASH_APPLICATION_ERR_BASE+0x00000001L)



/* -------------------------------------------------- Structs / Unions */


/* -------------------------------------------------- Class Decl */


/* -------------------------------------------------- Inline / Functions */


#include "FCMPostConfig.h"

#endif //FCM_ERRORS_H_
