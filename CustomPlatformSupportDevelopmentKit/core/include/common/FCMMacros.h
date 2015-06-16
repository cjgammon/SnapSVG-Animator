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
 * @file  FCMMacros.h
 *
 * @brief This file contains the macros used in FCM.
 */

#ifndef FCM_MACROS_H_
#define FCM_MACROS_H_

#include "FCMPreConfig.h"


/* -------------------------------------------------- Forward Decl */


/* -------------------------------------------------- Enums */


/* -------------------------------------------------- Macros / Constants */

namespace FCM
{
    /**
     * @def   FCM_VERSION_MAJOR
     *
     * @brief Major version of the framework.
     */
    #define FCM_VERSION_MAJOR                   0x01
    

    /**
     * @def   FCM_VERSION_MINOR
     *
     * @brief Minor version of the framework.
     */
    #define FCM_VERSION_MINOR                   0x02
    

    /**
     * @def   FCM_VERSION_MAINTENANCE
     *
     * @brief Maintenance number of the framework.
     */
    #define FCM_VERSION_MAINTENANCE             0x00
    

    /**
     * @def   FCM_VERSION_BUILD
     *
     * @brief Build number of the framework.
     */
    #define FCM_VERSION_BUILD                   0x00

    
    /**
     * @def   FCM_VERSION
     *
     * @brief Complete version of the framework.
     */
    #define FCM_VERSION ((FCM_VERSION_MAJOR << 24) | (FCM_VERSION_MINOR << 16) | \
                         (FCM_VERSION_MAINTENANCE << 8) | (FCM_VERSION_BUILD))


    /**
     * @def   DEFINE_FCMIID
     *
     * @brief Defines interface with name @a name and ID whose textual representation is
     *        @a l - @a w1 - @a w2 - @a b1 @a b2 - @a b3 @a b4 @a b5 @a b6 @a b7 @a b8.  
     */
    #define DEFINE_FCMIID    (name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \
    const FCMID name = {l, w1, w2, {b1, b2,  b3,  b4,  b5,  b6,  b7,  b8}}
    
    
    /**
     * @def   IID
     *
     * @brief Utility macro to define interface ID easily.  
      */
    #define IID(ifx) IID_##ifx
    
    
    /**
     * @def   _FCM_SIMPLEMAPENTRY
     *
     * @brief This macro makes debugging asserts easier.
     */
    #define _FCM_SIMPLEMAPENTRY ((_FCM_CREATORARGFUNC*)0)
    


    /**
     * @def   FORWARD_DECLARE_INTERFACE
     *
     * @brief Used to forward declare an interface @a ifx.
     */
    #define FORWARD_DECLARE_INTERFACE(ifx) \
    class ifx; \
    typedef ifx* P##ifx;



     /**
     * @def   BEGIN_DECLARE_INTERFACE_COMMON
     *
     * @brief Used to begin any interface @a ifx with ID @a iid.
     */
    #define BEGIN_DECLARE_INTERFACE_COMMON(ifx, iid) \
    FORWARD_DECLARE_INTERFACE(ifx) \
    const FCM::FCMIID IID_##ifx = (iid); \
    using namespace FCM;
    
    
     /**
     * @def   BEGIN_DECLARE_INTERFACE_INHERIT
     *
     * @brief Used to begin the interface @a ifx with ID @a iid , which inherits from @a baseifx.
     */
    #define BEGIN_DECLARE_INTERFACE_INHERIT(ifx, iid, baseifx) \
    BEGIN_DECLARE_INTERFACE_COMMON (ifx, iid) \
    class ifx : public baseifx {  public:  inline static FCM::FCMIID GetIID() {return IID(ifx);}
    


     /**
     * @def   BEGIN_DECLARE_INTERFACE
     *
     * @brief Used to begin the interface @a ifx with ID @a iid,  which inherits from IFCMUnknown.
     */
    #define BEGIN_DECLARE_INTERFACE(ifx, iid) BEGIN_DECLARE_INTERFACE_INHERIT(ifx, iid, FCM::IFCMUnknown);


    /**
     * @def   BEGIN_DECLARE_BASE_INTERFACE
     *
     * @brief Used to begin the interface @a ifx with ID @a iid,  which inherits from IFCMUnknown.
     */    
    #define BEGIN_DECLARE_BASE_INTERFACE(ifx, iid) \
    BEGIN_DECLARE_INTERFACE_COMMON (ifx, iid) \
    class ifx { public: inline static FCM::FCMIID GetIID() {return IID(ifx);}
      

    /**
     * @def   END_DECLARE_INTERFACE
     *
     * @brief Ends an interface.
     */ 
    #define END_DECLARE_INTERFACE    };
        
    
    /**
     * @def   _FCMCALL
     *
     * @brief Defines calling convention for interface methods.
     */
    #if defined(__GNUC__) 
        #define _FCMCALL
    #else
        #define _FCMCALL __stdcall
    #endif


    /**
     * @def   FCM_ADDREF
     *
     * @brief If @a intPtr exists, increment its reference count.
     */
    #define FCM_ADDREF(intPtr) if(intPtr){(intPtr)->AddRef();}
    

     /**
     * @def   FCM_RELEASE
     *
     * @brief If @a intPtr exists, decrement its reference count.
     */
    #define FCM_RELEASE(intPtr) if(intPtr){(intPtr)->Release();(intPtr)=0;}
    
    
    /**
     * @def   _FCM_PACKING
     *
     * @brief Used for internal purpose.
     */
    #define _FCM_PACKING 1


    /**
     * @def   offsetofclass
     *
     * @brief Utility macro to get offset of a derived class. Used for internal purposes.
     */
    #define offsetofclass(base, derived) \
                ((FCM::S_Int64)(static_cast<base*>((derived*)_FCM_PACKING))-_FCM_PACKING)


    /**
     * @def   offsetofclasscustom
     *
     * @brief Custom Class offset. Used for internal purposes.
     */
    #define offsetofclasscustom(base,custom, derived) \
                ((FCM::S_Int64)(static_cast<base*>((custom*)((derived*)_FCM_PACKING)))-_FCM_PACKING)


    /**
     * @def   offsetofmem
     *
     * @brief Memory Offset
     */
    #define offsetofmem(m,s) \
        (FCM::S_Int64)&(((s *)0)->m)

    

    /**
     * @def   BEGIN_INTERFACE_MAP
     *
     * @brief Format to begin Interface map.
     */
    #define BEGIN_INTERFACE_MAP(impl,implVersion) \
            public:  \
            virtual FCM::PIFCMCallback GetCallback()=0; \
            typedef impl _ClassImpl; \
            static FCM::U_Int32 GetVersion(){ return (FCM::U_Int32)implVersion;} \
            static FCM::FCMInterfaceMap* GetInterfaceMap() \
            { static FCM::FCMInterfaceMap _pInterfaceMap [] ={ \
            INTERFACE_ENTRY(IFCMUnknown)
    

    /**
     * @def   BEGIN_MULTI_INTERFACE_MAP
     *
     * @brief Format to Begin multi-interface map. This should be used if a implementation 
     *        class is inheriting from multiple interfaces. 
     */
    #define BEGIN_MULTI_INTERFACE_MAP(impl,implVersion) \
        public:  \
        virtual FCM::PIFCMCallback GetCallback()=0; \
        typedef impl _ClassImpl; \
        static FCM::U_Int32 GetVersion(){ return (FCM::U_Int32)implVersion;} \
        static FCM::FCMInterfaceMap* GetInterfaceMap() \
        { static FCM::FCMInterfaceMap _pInterfaceMap [] ={
        

    /**
     * @def   INTERFACE_ENTRY
     *
     * @brief Add an interface.
     */
    #define INTERFACE_ENTRY(ifx) \
            {IID(ifx),offsetofclass(ifx,_ClassImpl),_FCM_SIMPLEMAPENTRY},


    /**
     * @def   INTERFACE_ENTRY_CUSTOM
     *
     * @brief Add custom interface. Used to avoid the "diamond problem" of 
     *        multiple inheritance.
     */
    #define INTERFACE_ENTRY_CUSTOM(ifx,custom) \
            {IID(ifx),offsetofclasscustom(ifx,custom,_ClassImpl),_FCM_SIMPLEMAPENTRY},


    /**
     * @def   INTERFACE_ENTRY_AGGREGATE
     *
     * @brief Add interface with aggregration.
     */
    #define INTERFACE_ENTRY_AGGREGATE(ifx, punk)\
    {IID(ifx),offsetofmem(punk,_ClassImpl),_Delegate},

    
    /**
     * @def   END_INTERFACE_MAP
     *
     * @brief Format to end the interface map.
     */
    #define END_INTERFACE_MAP \
            {FCM::FCMIID_NULL,0,_FCM_SIMPLEMAPENTRY}}; \
            return _pInterfaceMap;}

    /**
     * @def   BEGIN_MODULE
     *
     * @brief Format to mark the beginning of a module. A module contains a list of class map with name @a ursModule.
     */
    #define BEGIN_MODULE(usrModule) \
            class usrModule : public FCM::FCMPluginModule {


    /**
     * @def   BEGIN_CLASS_ENTRY
     *
     * @brief Format to mark the begining of class map.
     */
    #define BEGIN_CLASS_ENTRY \
            public: \
            FCM::Result init(FCM::PIFCMCallback pCallback) { FCM::Result res = FCMPluginModule::init(pCallback); 


    /**
     * @def   CLASS_ENTRY
     *
     * @brief Add class entry with class id @a clsid and classobject @a ursClass to classFactory.
     */
    #define CLASS_ENTRY(clsid,ursClass)        \
            addClassEntry(clsid,&FCMClassFactory<ursClass>::GetFactory,&ursClass::GetInterfaceMap,ursClass::GetVersion());


    /**
     * @def   END_CLASS_ENTRY
     *
     * @brief Format to mark the end of a class map.
     */
    #define END_CLASS_ENTRY return res; }


    /**
     * @def   END_MODULE
     *
     * @brief Format to end the module.
     */
    #define END_MODULE    };


    /**
     * @def   DllExport
     *
     * @brief Used to export a function from a plugin
     */
    #if defined(__GNUC__)
        #ifndef DllExport
            #define DllExport        __attribute__((visibility("default")))
        #endif
    #else
        #ifndef DllExport 
            #define DllExport        __declspec( dllexport )
        #endif
    #endif


    /**
     * @def   FCMPLUGIN_IMP_EXP
     *
     * @brief Used to export a function from the plugin.
     */
    #if defined( __FCM_INTERNAL_PLUGIN__)
        #define FCMPLUGIN_IMP_EXP
    #else 
        #define FCMPLUGIN_IMP_EXP DllExport
    #endif //__FCM_INTERNAL_PLUGIN__

};


/* -------------------------------------------------- Structs / Unions */


/* -------------------------------------------------- Class Decl */


/* -------------------------------------------------- Inline / Functions */


#include "FCMPostConfig.h"

#endif //FCM_MACROS_H_
