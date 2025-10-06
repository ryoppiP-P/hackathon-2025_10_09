/*****************************************************************//**
 * \file   PlayerController.h
 * \brief  プレイヤー操作コンポーネント
 * 
 * \author 菊池凌斗
 * \date   2025/10/6
 *********************************************************************/
#pragma once

#include "Component.h"
#include "Physics.h"

namespace Engine {

class PlayerController : public Component {
public:
    PlayerController();
    virtual ~PlayerController() = default;

    virtual void Start() override;
    virtual void Update(double deltaTime) override;

    // パラメータ
    void SetMoveSpeed(float speed) { m_moveSpeed = speed; }
    void SetJumpPower(float power) { m_jumpPower = power; }

private:
    void HandleInput();
    void UpdateAnimation();

    Rigidbody* m_rigidbody = nullptr;
    BoxCollider* m_collider = nullptr;
    
    float m_moveSpeed = 200.0f;
    float m_jumpPower = 400.0f;
    
    bool m_facingRight = true;
    bool m_isMoving = false;
    bool m_wasGrounded = false;

    // アニメーション状態
    enum class AnimState {
        Idle,
        Walking,
        Jumping,
        Falling
    };
    AnimState m_currentAnimState = AnimState::Idle;
};

} // namespace Engine