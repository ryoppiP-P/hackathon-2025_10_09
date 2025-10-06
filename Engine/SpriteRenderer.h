/*****************************************************************//**
 * \file   SpriteRenderer.h
 * \brief  スプライト描画コンポーネント
 * 
 * \author 菊池凌斗
 * \date   2025/10/6
 *********************************************************************/
#pragma once

#include "Component.h"
#include <DirectXMath.h>
#include <string>

using namespace DirectX;

namespace Engine {

class SpriteRenderer : public Component {
public:
    SpriteRenderer();
    virtual ~SpriteRenderer() = default;

    void SetTexture(const std::wstring& texturePath);
    void SetTexture(int textureId);
    
    void SetColor(float r, float g, float b, float a = 1.0f);
    void SetColor(const XMFLOAT4& color) { m_color = color; }
    
    void SetSize(float width, float height) { m_size = {width, height}; }
    void SetSize(const XMFLOAT2& size) { m_size = size; }
    
    void SetSourceRect(int x, int y, int width, int height);
    
    virtual void Draw() override;

    // プロパティ
    XMFLOAT4 GetColor() const { return m_color; }
    XMFLOAT2 GetSize() const { return m_size; }
    int GetTextureId() const { return m_textureId; }

private:
    int m_textureId = -1;
    XMFLOAT4 m_color = {1.0f, 1.0f, 1.0f, 1.0f};
    XMFLOAT2 m_size = {100.0f, 100.0f};
    
    // ソーステクスチャ座標
    int m_srcX = 0;
    int m_srcY = 0;
    int m_srcWidth = 0;
    int m_srcHeight = 0;
};

} // namespace Engine