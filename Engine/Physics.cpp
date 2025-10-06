#include "Physics.h"
#include "GameObject.h"
#include "Transform.h"

namespace Engine {

Rigidbody::Rigidbody() = default;

void Rigidbody::Update(double deltaTime) {
    if (!IsEnabled()) return;
    
    Transform* transform = GetGameObject()->GetTransform();
    XMFLOAT2 pos = transform->GetPosition();
    
    // 重力適用
    if (m_useGravity && !m_isGrounded) {
        m_velocity.y += m_gravity * static_cast<float>(deltaTime);
    }
    
    // 最大速度制限
    if (m_velocity.x > m_maxVelocity.x) m_velocity.x = m_maxVelocity.x;
    if (m_velocity.x < -m_maxVelocity.x) m_velocity.x = -m_maxVelocity.x;
    if (m_velocity.y > m_maxVelocity.y) m_velocity.y = m_maxVelocity.y;
    if (m_velocity.y < -m_maxVelocity.y) m_velocity.y = -m_maxVelocity.y;
    
    // 位置更新
    pos.x += m_velocity.x * static_cast<float>(deltaTime);
    pos.y += m_velocity.y * static_cast<float>(deltaTime);
    
    transform->SetPosition(pos);
}

BoxCollider::BoxCollider() = default;

BoxCollider::Rect BoxCollider::GetWorldRect() const {
    Transform* transform = GetGameObject()->GetTransform();
    XMFLOAT2 pos = transform->GetPosition();
    
    float left = pos.x + m_offset.x - m_size.x * 0.5f;
    float top = pos.y + m_offset.y - m_size.y * 0.5f;
    float right = left + m_size.x;
    float bottom = top + m_size.y;
    
    return {left, top, right, bottom};
}

bool BoxCollider::IsColliding(BoxCollider* other) const {
    auto rect1 = GetWorldRect();
    auto rect2 = other->GetWorldRect();
    
    return !(rect1.right < rect2.left || 
             rect1.left > rect2.right || 
             rect1.bottom < rect2.top || 
             rect1.top > rect2.bottom);
}

} // namespace Engine