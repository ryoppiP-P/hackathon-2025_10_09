/*****************************************************************//**
 * \file   Physics.h
 * \brief  物理演算コンポーネント
 * 
 * \author 菊池凌斗
 * \date   2025/10/6
 *********************************************************************/
#pragma once

#include "Component.h"
#include <DirectXMath.h>

using namespace DirectX;

namespace Engine {

class Rigidbody : public Component {
public:
    Rigidbody();
    virtual ~Rigidbody() = default;

    virtual void Update(double deltaTime) override;

    // 速度
    XMFLOAT2 GetVelocity() const { return m_velocity; }
    void SetVelocity(float x, float y) { m_velocity = {x, y}; }
    void SetVelocity(const XMFLOAT2& velocity) { m_velocity = velocity; }
    void AddVelocity(float x, float y) { m_velocity.x += x; m_velocity.y += y; }

    // 重力
    bool UseGravity() const { return m_useGravity; }
    void SetUseGravity(bool useGravity) { m_useGravity = useGravity; }
    float GetGravity() const { return m_gravity; }
    void SetGravity(float gravity) { m_gravity = gravity; }

    // 地面判定
    bool IsGrounded() const { return m_isGrounded; }
    void SetGrounded(bool grounded) { m_isGrounded = grounded; }

    // 最大速度制限
    void SetMaxVelocity(float maxX, float maxY) { m_maxVelocity = {maxX, maxY}; }

private:
    XMFLOAT2 m_velocity = {0.0f, 0.0f};
    XMFLOAT2 m_maxVelocity = {500.0f, 1000.0f};
    float m_gravity = 980.0f;  // ピクセル/秒^2
    bool m_useGravity = true;
    bool m_isGrounded = false;
};

class BoxCollider : public Component {
public:
    BoxCollider();
    virtual ~BoxCollider() = default;

    void SetSize(float width, float height) { m_size = {width, height}; }
    void SetOffset(float x, float y) { m_offset = {x, y}; }
    
    XMFLOAT2 GetSize() const { return m_size; }
    XMFLOAT2 GetOffset() const { return m_offset; }
    
    // ワールド座標での当たり判定矩形を取得
    struct Rect {
        float left, top, right, bottom;
    };
    Rect GetWorldRect() const;

    bool IsColliding(BoxCollider* other) const;

private:
    XMFLOAT2 m_size = {32.0f, 32.0f};
    XMFLOAT2 m_offset = {0.0f, 0.0f};
};

} // namespace Engine