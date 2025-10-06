/*****************************************************************//**
 * \file   CollisionManager.h
 * \brief  “–‚½‚è”»’èŠÇ—ƒVƒXƒeƒ€
 * 
 * \author ‹e’r—½“l
 * \date   2025/10/6
 *********************************************************************/
#pragma once

#include <vector>
#include "Physics.h"

namespace Engine {

class CollisionManager {
public:
    static CollisionManager& Instance();
    
    void RegisterCollider(BoxCollider* collider, const std::string& tag = "");
    void UnregisterCollider(BoxCollider* collider);
    void CheckCollisions();
    
    std::vector<BoxCollider*> GetCollidersWithTag(const std::string& tag);

private:
    struct ColliderInfo {
        BoxCollider* collider;
        std::string tag;
    };
    
    std::vector<ColliderInfo> m_colliders;
    
    void HandlePlayerCollision(BoxCollider* player, BoxCollider* other, const std::string& otherTag);
};

} // namespace Engine