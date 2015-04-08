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
 * @file  IMatrix2D.h
 *
 * @brief This file contains the interface for IMatrix2D. This interface provides 
 *        a way to deceompose a 2-dimensional transformation matrix.
 */

#ifndef IMatrix2D_H_
#define IMatrix2D_H_

#include "FCMPreConfig.h"
#include "FCMTypes.h"
#include "IFCMUnknown.h"
#include "Utils/DOMTypes.h"


/* -------------------------------------------------- Forward Decl */


/* -------------------------------------------------- Enums */


/* -------------------------------------------------- Macros / Constants */

namespace DOM
{
    namespace Utils
    {
        /**
         * @brief Defines the universally-unique Interface ID for IMatrix2D.
         *
         * @note  Textual Representation: {0BEBDD0F-ED7A-4410-959D-152DA4EEC2EF}
         */
        FCM::ConstFCMIID IID_IMATRIX2D =
            {0xbebdd0f, 0xed7a, 0x4410, {0x95, 0x9d, 0x15, 0x2d, 0xa4, 0xee, 0xc2, 0xef}};
    }
}


/* -------------------------------------------------- Structs / Unions */


/* -------------------------------------------------- Class Decl */

namespace DOM
{
    namespace Utils
    {
        /**
        * @class IMatrix2D
        *
        * @brief Defines an interface that decomposes a Matrix2D.
        */
        BEGIN_DECLARE_INTERFACE(IMatrix2D, IID_IMATRIX2D)

            /**
             * @brief  This function sets the Matrix2D which will be decomposed.
             *
             * @param  matrix (IN)
             *         Matrix2D object.
             *
             * @return On success, FCM_SUCCESS is returned; else an error code is returned.
             */
            virtual FCM::Result _FCMCALL SetMatrix(const MATRIX2D& matrix) = 0;


            /**
             * @brief  This function returns the rotation of Matrix2D set.
             *
             * @param  rotation (OUT)
             *         Rotation obtained by decomposing the matrix set.
             *
             * @return On success, FCM_SUCCESS is returned; else an error code is returned.
             */
            virtual FCM::Result _FCMCALL GetRotation(FCM::Float& rotation) = 0;


            /**
             * @brief  This function returns the scale of Matrix2D set.
             *
             * @param  scale (OUT)
             *         Scale obtained by decomposing the matrix set.
             *
             * @return On success, FCM_SUCCESS is returned; else an error code is returned.
             */
            virtual FCM::Result _FCMCALL GetScale(POINT2D& scale) = 0;


            /**
             * @brief  This function returns the skew of Matrix2D set.
             *
             * @param  skew (OUT)
             *         Skew obtained by decomposing the matrix set.
             *
             * @return On success, FCM_SUCCESS is returned; else an error code is returned.
             */
            virtual FCM::Result _FCMCALL GetSkew(POINT2D& skew) = 0;

        END_DECLARE_INTERFACE
    }
};


/* -------------------------------------------------- Inline / Functions */


#include "FCMPostConfig.h"

#endif // IMatrix2D_H_

