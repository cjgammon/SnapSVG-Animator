/**
 * @file  ResourceIDManager.h
 *
 * @brief This file contains declarations for a ResourceIDManager.
 */

#ifndef RESOURCE_ID_MANAGER_H_
#define RESOURCE_ID_MANAGER_H_


#include "FCMTypes.h"
#include <map>
#include <string>

namespace SnapSVGAnimator
{
    class ResourceIDManager 
    {
    public:

        void AutoMapResId(FCM::U_Int32 resourceId, FCM::U_Int32 &newResourceId);
        void GetMappedResId(FCM::U_Int32 resourceId, FCM::U_Int32 &newResourceId);
        void SetResIdMapping(FCM::U_Int32 resourceId, FCM::U_Int32 newResourceId);
        bool HasResource(FCM::U_Int32 resourceId);
        FCM::Result HasResource(const std::string& name, FCM::Boolean& hasResource);
        FCM::U_Int32 CreateUnusedResourceID();
        void IncrementUseCount(FCM::U_Int32 resourceId);
        void DecrementUseCount(FCM::U_Int32 resourceId);
        FCM::U_Int32 GetUseCount(FCM::U_Int32 resourceId);
        void LinkResourceNameAndID(std::string,FCM::U_Int32);
        void GetResourceIDFromName(const std::string& name, FCM::U_Int32& resourceID);
        void GetNameFromResourceID(FCM::U_Int32 resourceID, std::string& name);

        void ClearResIdMap();
        void Reset();
        static ResourceIDManager& GetInstance() {
            static ResourceIDManager s_instance; return s_instance;
        }

    private:
    
        ResourceIDManager();
        void MarksAsUsedResId(FCM::U_Int32 resourceId);

        std::map<FCM::U_Int32,FCM::U_Int32> m_usedResIDs;
        typedef std::map<FCM::U_Int32,FCM::U_Int32> ResourceIDMap;
        ResourceIDMap m_resIDMap;
        FCM::U_Int32 m_largestResID;
        
        typedef std::pair<std::string,FCM::U_Int32> NameIDPair;
        std::map<std::string,FCM::U_Int32> m_resourceNamesToID;
    };

};

#endif // RESOURCE_ID_MANAGER_H_

