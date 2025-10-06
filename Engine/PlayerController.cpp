#include "PlayerController.h"
#include "GameObject.h"
#include "Transform.h"
#include "SpriteRenderer.h"
#include "Input.h"

namespace Engine {

PlayerController::PlayerController() = default;

void PlayerController::Start() {
    m_rigidbody = GetGameObject()->GetComponent<Rigidbody>();
    m_collider = GetGameObject()->GetComponent<BoxCollider>();
}

void PlayerController::Update(double deltaTime) {
    if (!IsEnabled()) return;
    
    HandleInput();
    UpdateAnimation();
    
    m_wasGrounded = m_rigidbody->IsGrounded();
}

void PlayerController::HandleInput() {
    XMFLOAT2 velocity = m_rigidbody->GetVelocity();
    m_isMoving = false;
    
    // 左右移動
    if (Input::GetKey(VK_LEFT) || Input::GetKey('A')) {
        velocity.x = -m_moveSpeed;
        m_facingRight = false;
        m_isMoving = true;
    }
    else if (Input::GetKey(VK_RIGHT) || Input::GetKey('D')) {
        velocity.x = m_moveSpeed;
        m_facingRight = true;
        m_isMoving = true;
    }
    else {
        velocity.x = 0.0f;
    }
    
    // ジャンプ
    if ((Input::GetKeyDown(VK_SPACE) || Input::GetKeyDown('W')) && m_rigidbody->IsGrounded()) {
        velocity.y = -m_jumpPower;
    }
    
    m_rigidbody->SetVelocity(velocity);
    
    // スプライトの向き調整
    auto renderer = GetGameObject()->GetComponent<SpriteRenderer>();
    if (renderer) {
        Transform* transform = GetGameObject()->GetTransform();
        XMFLOAT2 scale = transform->GetScale();
        scale.x = m_facingRight ? 1.0f : -1.0f;
        transform->SetScale(scale);
    }
}

void PlayerController::UpdateAnimation() {
    AnimState newState = AnimState::Idle;
    
    if (!m_rigidbody->IsGrounded()) {
        if (m_rigidbody->GetVelocity().y < 0) {
            newState = AnimState::Jumping;
        } else {
            newState = AnimState::Falling;
        }
    }
    else if (m_isMoving) {
        newState = AnimState::Walking;
    }
    
    if (newState != m_currentAnimState) {
        m_currentAnimState = newState;
        // アニメーション切り替え処理
    }
}

} // namespace Engine