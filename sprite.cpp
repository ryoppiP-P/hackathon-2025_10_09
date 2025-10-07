/*==============================================================================

   スプライト描画 [sprite.cpp]
                                                         Author : Youhei Sato
                                                         Date   : 2025/06/06
--------------------------------------------------------------------------------

==============================================================================*/
#include <d3d11.h>
#include <DirectXMath.h>
using namespace DirectX;
#include "direct3d.h"
#include "shader.h"
#include "debug_ostream.h"
#include "texture.h"
#include "sprite.h"

static constexpr int NUM_VERTEX = 6; // 頂点数

static ID3D11Buffer* g_pVertexBuffer = nullptr; // 頂点バッファ

// 注意！初期化で外部から設定されるもの。Release不要。
static ID3D11Device* g_pDevice = nullptr;
static ID3D11DeviceContext* g_pContext = nullptr;

// 頂点構造体
struct Vertex
{
    XMFLOAT3 position; // 頂点座標
    XMFLOAT4 color;    // 頂点カラー
    XMFLOAT2 texcoord; // UV座標
};

// 四角形の頂点データをセットする関数
// v: Vertex配列へのポインタ（6個分必要）
// v4pos[4]: 4頂点座標, v4col[4]: 4頂点カラー, v4uv[4]: 4頂点UV
void SetRectangleVerticesFrom4(
    Vertex* v6,
    const XMFLOAT3 v4pos[4],
    const XMFLOAT4 v4col[4],
    const XMFLOAT2 v4uv[4])
{
    // 1枚目三角形: 0(左上),1(右上),2(右下)
    v6[0].position = v4pos[0]; v6[0].color = v4col[0]; v6[0].texcoord = v4uv[0];
    v6[1].position = v4pos[1]; v6[1].color = v4col[1]; v6[1].texcoord = v4uv[1];
    v6[2].position = v4pos[2]; v6[2].color = v4col[2]; v6[2].texcoord = v4uv[2];
    // 2枚目三角形: 0(左上),2(右下),3(左下)
    v6[3].position = v4pos[0]; v6[3].color = v4col[0]; v6[3].texcoord = v4uv[0];
    v6[4].position = v4pos[2]; v6[4].color = v4col[2]; v6[4].texcoord = v4uv[2];
    v6[5].position = v4pos[3]; v6[5].color = v4col[3]; v6[5].texcoord = v4uv[3];
}

void Sprite_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    // デバイスとデバイスコンテキストのチェック
    if (!pDevice || !pContext) {
        hal::dout << "Sprite_Initialize() : 与えられたデバイスかコンテキストが不正です" << std::endl;
        return;
    }
    g_pDevice = pDevice;
    g_pContext = pContext;

    // 頂点バッファ生成
    D3D11_BUFFER_DESC bd = {};
    bd.Usage = D3D11_USAGE_DYNAMIC;
    bd.ByteWidth = sizeof(Vertex) * NUM_VERTEX;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    g_pDevice->CreateBuffer(&bd, NULL, &g_pVertexBuffer);
}

void Sprite_Finalize(void)
{
    SAFE_RELEASE(g_pVertexBuffer);
}


void Sprite_Draw(
    int texid,
    float x, float y, float width, float height,
    int tx, int ty, int tw, int th,
    XMFLOAT4 color)
{
    Shader_Begin();

    // 頂点バッファをロックする
    D3D11_MAPPED_SUBRESOURCE msr;
    g_pContext->Map(g_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

    // 頂点バッファへの仮想ポインタを取得
    Vertex* v = (Vertex*)msr.pData;

    // 頂点情報を書き込み
    const float SCREEN_WIDTH = (float)Direct3D_GetBackBufferWidth();
    const float SCREEN_HEIGHT = (float)Direct3D_GetBackBufferHeight();

    float tsw = Texture_GetWidth(texid);
    float tsh = Texture_GetHeight(texid);

    // 4頂点分のデータを用意
    XMFLOAT3 pos[4] = {
        { x,      y,      0.0f }, // 左上
        { x + width,  y,      0.0f }, // 右上
        { x + width,  y + height,  0.0f }, // 右下
        { x,      y + height,  0.0f }  // 左下
    };

    // 画像内の切り出し範囲をUVに変換
    float u0 = tx / tsw;
    float v0 = ty / tsh;
    float u1 = (tx + tw) / tsw;
    float v1 = (ty + th) / tsh;

    XMFLOAT2 uv[4] = {
        { u0, v0 }, // 左上
        { u1, v0 }, // 右上
        { u1, v1 }, // 右下
        { u0, v1 }  // 左下
    };

    XMFLOAT4 col[4] = {
        color, // 左上
        color, // 右上
        color, // 右下
        color  // 左下
    };

    SetRectangleVerticesFrom4(v, pos, col, uv);

    // 頂点バッファのロックを解除
    g_pContext->Unmap(g_pVertexBuffer, 0);

    // 頂点バッファを描画パイプラインに設定
    UINT stride = sizeof(Vertex);
    UINT offset = 0;
    g_pContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);

    // 頂点シェーダーにWorld変換行列を設定
    Shader_SetWorldMatrix(XMMatrixIdentity());
    
    // 頂点シェーダーに変換行列を設定
    Shader_SetProjectionMatrix(XMMatrixOrthographicOffCenterLH(0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 0.0f, 1.0f));

    Shader_SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });

    g_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    // テクスチャは外部でセット（Texture_SetTextureなど）
    Texture_SetTexture(texid);

    // ポリゴン描画命令発行
    g_pContext->Draw(NUM_VERTEX, 0);
}


void Sprite_Draw(
    int texid,
    float x, float y, float width, float height,
    int tx, int ty, int tw, int th, float angle,
    XMFLOAT4 color)
{
    Shader_Begin();

    // 頂点バッファをロックする
    D3D11_MAPPED_SUBRESOURCE msr;
    g_pContext->Map(g_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

    // 頂点バッファへの仮想ポインタを取得
    Vertex* v = (Vertex*)msr.pData;

    // 頂点情報を書き込み
    const float SCREEN_WIDTH = (float)Direct3D_GetBackBufferWidth();
    const float SCREEN_HEIGHT = (float)Direct3D_GetBackBufferHeight();

    float tsw = Texture_GetWidth(texid);
    float tsh = Texture_GetHeight(texid);

    // 4頂点分のデータを用意
    XMFLOAT3 pos[4] = {
        { -0.5f,      -0.5f,      0.0f }, // 左上
        { +0.5f,  -0.5f,      0.0f }, // 右上
        { +0.5f,  +0.5f,  0.0f }, // 右下
        { -0.5f,      +0.5f,  0.0f }  // 左下
    };

    // 画像内の切り出し範囲をUVに変換
    float u0 = tx / tsw;
    float v0 = ty / tsh;
    float u1 = (tx + tw) / tsw;
    float v1 = (ty + th) / tsh;

    XMFLOAT2 uv[4] = {
        { u0, v0 }, // 左上
        { u1, v0 }, // 右上
        { u1, v1 }, // 右下
        { u0, v1 }  // 左下
    };

    XMFLOAT4 col[4] = {
        color, // 左上
        color, // 右上
        color, // 右下
        color  // 左下
    };

    SetRectangleVerticesFrom4(v, pos, col, uv);

    // 頂点バッファのロックを解除
    g_pContext->Unmap(g_pVertexBuffer, 0);

    // 頂点バッファを描画パイプラインに設定
    UINT stride = sizeof(Vertex);
    UINT offset = 0;
    g_pContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);

    // 頂点シェーダーにWorld変換行列を設定
    XMMATRIX s = XMMatrixScaling(width, height, 1.0f);  // ← 表示サイズを使用
    XMMATRIX r = XMMatrixRotationZ(angle);
    XMMATRIX t = XMMatrixTranslation(x, y, 0.0f);
    Shader_SetWorldMatrix(s * r * t);

    // 頂点シェーダーに変換行列を設定
    Shader_SetProjectionMatrix(XMMatrixOrthographicOffCenterLH(0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 0.0f, 1.0f));

    Shader_SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });

    g_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    // テクスチャは外部でセット（Texture_SetTextureなど）
    Texture_SetTexture(texid);

    // ポリゴン描画命令発行
    g_pContext->Draw(NUM_VERTEX, 0);
}