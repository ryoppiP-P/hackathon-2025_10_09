/*****************************************************************//**
 * \file   Transform.h
 * \brief  Transform ÉRÉìÉ|Å[ÉlÉìÉg
 * 
 * \author ãeíróΩìl
 * \date   2025/10/6
 *********************************************************************/
#pragma once

#include "Component.h"
#include <DirectXMath.h>

using namespace DirectX;

namespace Engine {

class Transform : public Component {
public:
    Transform();
    virtual ~Transform() = default;

    // Position
    XMFLOAT2 GetPosition() const { return m_position; }
    void SetPosition(float x, float y) { m_position = {x, y}; }
    void SetPosition(const XMFLOAT2& position) { m_position = position; }
    
    // Scale
    XMFLOAT2 GetScale() const { return m_scale; }
    void SetScale(float x, float y) { m_scale = {x, y}; }
    void SetScale(const XMFLOAT2& scale) { m_scale = scale; }
    
    // Rotation (in radians)
    float GetRotation() const { return m_rotation; }
    void SetRotation(float rotation) { m_rotation = rotation; }

    // Movement functions
    void Translate(float x, float y);
    void Translate(const XMFLOAT2& translation);
    void Rotate(float deltaRotation);
    void Scale(float scaleX, float scaleY);

private:
    XMFLOAT2 m_position = {0.0f, 0.0f};
    XMFLOAT2 m_scale = {1.0f, 1.0f};
    float m_rotation = 0.0f;
};

} // namespace Engine