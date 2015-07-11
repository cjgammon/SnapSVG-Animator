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
 * @file  Version.h
 *
 * @brief This file contains version of the sample plugin. This version will be used as the class implementation version.
 */

#ifndef VERSION_H_
#define VERSION_H_



/* -------------------------------------------------- Forward Decl */


/* -------------------------------------------------- Enums */


/* -------------------------------------------------- Macros / Constants */

/**
     * @def   SAMPLE_PLUGIN_VERSION_MAJOR
     *
     * @brief Major version of the plugin
     */
    #define SAMPLE_PLUGIN_VERSION_MAJOR                 0x01
    

    /**
     * @def   SAMPLE_PLUGIN_VERSION_MINOR
     *
     * @brief Minor version of the plugin
     */
    #define SAMPLE_PLUGIN_VERSION_MINOR                 0x00
    

    /**
     * @def   SAMPLE_PLUGIN_VERSION_MAINTENANCE
     *
     * @brief Plugin version's maintenance number. 
     */
    #define SAMPLE_PLUGIN_VERSION_MAINTENANCE           0x00
    

    /**
     * @def   SAMPLE_PLUGIN_VERSION_BUILD
     *
     * @brief Plugin version's build number. 
     */
    #define SAMPLE_PLUGIN_VERSION_BUILD                 0x00

    
    /**
     * @def   SAMPLE_PLUGIN_VERSION
     *
     * @brief Complete plugin version.  
     */
    #define SAMPLE_PLUGIN_VERSION ( (SAMPLE_PLUGIN_VERSION_MAJOR << 24) | (SAMPLE_PLUGIN_VERSION_MINOR << 16) | \
                                    (SAMPLE_PLUGIN_VERSION_MAINTENANCE << 8) | (SAMPLE_PLUGIN_VERSION_BUILD) )


/* -------------------------------------------------- Structs / Unions */


/* -------------------------------------------------- Class Decl */


#endif // VERSION_H_

