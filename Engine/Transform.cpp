#include "Transform.h"

namespace Engine {

Transform::Transform() = default;

void Transform::Translate(float x, float y) {
    m_position.x += x;
    m_position.y += y;
}

void Transform::Translate(const XMFLOAT2& translation) {
    Translate(translation.x, translation.y);
}

void Transform::Rotate(float deltaRotation) {
    m_rotation += deltaRotation;
}

void Transform::Scale(float scaleX, float scaleY) {
    m_scale.x *= scaleX;
    m_scale.y *= scaleY;
}

} // namespace Engine