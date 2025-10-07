/*==============================================================================

   スプライト描画 [sprite.h]
														 Author : Youhei Sato
														 Date   : 2025/06/06
--------------------------------------------------------------------------------

==============================================================================*/
#ifndef SPRITE_H
#define SPRITE_H

#include <d3d11.h>
#include <DirectXMath.h>

void Sprite_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
void Sprite_Finalize(void);
void Sprite_Draw(
    int texid,
    float x, float y, float width, float height,
    int tx, int ty, int tw, int th,
    DirectX::XMFLOAT4 color = {1.0f, 1.0f, 1.0f, 1.0f});

void Sprite_Draw(
    int texid,
    float x, float y, float width, float height,
    int tx, int ty, int tw, int th, float angle,
    DirectX::XMFLOAT4 color = { 1.0f, 1.0f, 1.0f, 1.0f });

#endif // SPRITE_H
