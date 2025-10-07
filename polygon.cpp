/*==============================================================================

   ポリゴン描画 [polygon.cpp]
                                                         Author : Youhei Sato
                                                         Date   : 2025/05/15
--------------------------------------------------------------------------------

==============================================================================*/
#include <d3d11.h>
#include <DirectXMath.h>
using namespace DirectX;
#include "direct3d.h"
#include "shader.h"
#include "debug_ostream.h"
#include "polygon.h"

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
//void SetRectangleVerticesFrom4(
//    Vertex* v6,
//    const XMFLOAT3 v4pos[4],
//    const XMFLOAT4 v4col[4],
//    const XMFLOAT2 v4uv[4])
//{
//    // 1枚目三角形: 0(左上),1(右上),2(右下)
//    v6[0].position = v4pos[0]; v6[0].color = v4col[0]; v6[0].texcoord = v4uv[0];
//    v6[1].position = v4pos[1]; v6[1].color = v4col[1]; v6[1].texcoord = v4uv[1];
//    v6[2].position = v4pos[2]; v6[2].color = v4col[2]; v6[2].texcoord = v4uv[2];
//    // 2枚目三角形: 0(左上),2(右下),3(左下)
//    v6[3].position = v4pos[0]; v6[3].color = v4col[0]; v6[3].texcoord = v4uv[0];
//    v6[4].position = v4pos[2]; v6[4].color = v4col[2]; v6[4].texcoord = v4uv[2];
//    v6[5].position = v4pos[3]; v6[5].color = v4col[3]; v6[5].texcoord = v4uv[3];
//}

void Polygon_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    // デバイスとデバイスコンテキストのチェック
    if (!pDevice || !pContext) {
        hal::dout << "Polygon_Initialize() : 与えられたデバイスかコンテキストが不正です" << std::endl;
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

void Polygon_Finalize(void)
{
    SAFE_RELEASE(g_pVertexBuffer);
}

void Polygon_Draw(void)
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

    constexpr float W = 512.0f;
    constexpr float H = 512.0f;

    float x = 64.0f;
    float y = 64.0f;
    static float ga = 0.0f;

    // 4頂点分のデータを用意
    XMFLOAT3 pos[4] = {
        { x,      y,      0.0f }, // 左上
        { x + W,  y,      0.0f }, // 右上
        { x + W,  y + H,  0.0f }, // 右下
        { x,      y + H,  0.0f }  // 左下
    };
    XMFLOAT2 uv[4] = {
        { 0.0f + ga, 0.0f + ga }, // 左上
        { 4.0f + ga, 0.0f + ga }, // 右上
        { 4.0f + ga, 4.0f + ga }, // 右下
        { 0.0f + ga, 4.0f + ga }  // 左下
    }; ga += 0.001f;
    XMFLOAT4 col[4] = {
        { 1.0f, 1.0f, 1.0f, 1.0f }, // 左上
        { 1.0f, 1.0f, 1.0f, 1.0f }, // 右上
        { 1.0f, 1.0f, 1.0f, 1.0f }, // 右下
        { 1.0f, 1.0f, 1.0f, 1.0f }  // 左下
    };

    //SetRectangleVerticesFrom4(v, pos, col, uv);

    // 頂点バッファのロックを解除
    g_pContext->Unmap(g_pVertexBuffer, 0);

    // 頂点バッファを描画パイプラインに設定
    UINT stride = sizeof(Vertex);
    UINT offset = 0;
    g_pContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);

    // 頂点シェーダーに変換行列を設定
    Shader_SetProjectionMatrix(XMMatrixOrthographicOffCenterLH(0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 0.0f, 1.0f));

    static float da = 0.0f;
    Shader_SetColor({ 1.0f, 1.0f, 1.0f, da });
    da = std::max(da + 0.01f, 1.0f);

    g_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    // テクスチャは外部でセット（Texture_SetTextureなど）

    // ポリゴン描画命令発行
    g_pContext->Draw(NUM_VERTEX, 0);
}