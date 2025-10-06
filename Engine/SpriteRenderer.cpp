#include "SpriteRenderer.h"
#include "GameObject.h"
#include "Transform.h"
#include "../texture.h"
#include "../sprite.h"

namespace Engine {

SpriteRenderer::SpriteRenderer() = default;

void SpriteRenderer::SetTexture(const std::wstring& texturePath) {
    m_textureId = Texture_Load(texturePath.c_str());
    if (m_srcWidth == 0 || m_srcHeight == 0) {
        m_srcWidth = Texture_GetWidth(m_textureId);
        m_srcHeight = Texture_GetHeight(m_textureId);
    }
}

void SpriteRenderer::SetTexture(int textureId) {
    m_textureId = textureId;
    if (m_srcWidth == 0 || m_srcHeight == 0) {
        m_srcWidth = Texture_GetWidth(m_textureId);
        m_srcHeight = Texture_GetHeight(m_textureId);
    }
}

void SpriteRenderer::SetColor(float r, float g, float b, float a) {
    m_color = {r, g, b, a};
}

void SpriteRenderer::SetSourceRect(int x, int y, int width, int height) {
    m_srcX = x;
    m_srcY = y;
    m_srcWidth = width;
    m_srcHeight = height;
}

void SpriteRenderer::Draw() {
    if (m_textureId < 0) return;
    
    Transform* transform = GetGameObject()->GetTransform();
    XMFLOAT2 pos = transform->GetPosition();
    XMFLOAT2 scale = transform->GetScale();
    
    float width = m_size.x * scale.x;
    float height = m_size.y * scale.y;
    
    Sprite_Draw(
        m_textureId,
        pos.x, pos.y, width, height,
        m_srcX, m_srcY, m_srcWidth, m_srcHeight,
        m_color
    );
}

} // namespace Engine