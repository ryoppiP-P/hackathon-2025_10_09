/*==============================================================================

   �X�v���C�g�`�� [sprite.cpp]
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

static constexpr int NUM_VERTEX = 6; // ���_��

static ID3D11Buffer* g_pVertexBuffer = nullptr; // ���_�o�b�t�@

// ���ӁI�������ŊO������ݒ肳�����́BRelease�s�v�B
static ID3D11Device* g_pDevice = nullptr;
static ID3D11DeviceContext* g_pContext = nullptr;

// ���_�\����
struct Vertex
{
    XMFLOAT3 position; // ���_���W
    XMFLOAT4 color;    // ���_�J���[
    XMFLOAT2 texcoord; // UV���W
};

// �l�p�`�̒��_�f�[�^���Z�b�g����֐�
// v: Vertex�z��ւ̃|�C���^�i6���K�v�j
// v4pos[4]: 4���_���W, v4col[4]: 4���_�J���[, v4uv[4]: 4���_UV
void SetRectangleVerticesFrom4(
    Vertex* v6,
    const XMFLOAT3 v4pos[4],
    const XMFLOAT4 v4col[4],
    const XMFLOAT2 v4uv[4])
{
    // 1���ڎO�p�`: 0(����),1(�E��),2(�E��)
    v6[0].position = v4pos[0]; v6[0].color = v4col[0]; v6[0].texcoord = v4uv[0];
    v6[1].position = v4pos[1]; v6[1].color = v4col[1]; v6[1].texcoord = v4uv[1];
    v6[2].position = v4pos[2]; v6[2].color = v4col[2]; v6[2].texcoord = v4uv[2];
    // 2���ڎO�p�`: 0(����),2(�E��),3(����)
    v6[3].position = v4pos[0]; v6[3].color = v4col[0]; v6[3].texcoord = v4uv[0];
    v6[4].position = v4pos[2]; v6[4].color = v4col[2]; v6[4].texcoord = v4uv[2];
    v6[5].position = v4pos[3]; v6[5].color = v4col[3]; v6[5].texcoord = v4uv[3];
}

void Sprite_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    // �f�o�C�X�ƃf�o�C�X�R���e�L�X�g�̃`�F�b�N
    if (!pDevice || !pContext) {
        hal::dout << "Sprite_Initialize() : �^����ꂽ�f�o�C�X���R���e�L�X�g���s���ł�" << std::endl;
        return;
    }
    g_pDevice = pDevice;
    g_pContext = pContext;

    // ���_�o�b�t�@����
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

    // ���_�o�b�t�@�����b�N����
    D3D11_MAPPED_SUBRESOURCE msr;
    g_pContext->Map(g_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

    // ���_�o�b�t�@�ւ̉��z�|�C���^���擾
    Vertex* v = (Vertex*)msr.pData;

    // ���_������������
    const float SCREEN_WIDTH = (float)Direct3D_GetBackBufferWidth();
    const float SCREEN_HEIGHT = (float)Direct3D_GetBackBufferHeight();

    float tsw = Texture_GetWidth(texid);
    float tsh = Texture_GetHeight(texid);

    // 4���_���̃f�[�^��p��
    XMFLOAT3 pos[4] = {
        { x,      y,      0.0f }, // ����
        { x + width,  y,      0.0f }, // �E��
        { x + width,  y + height,  0.0f }, // �E��
        { x,      y + height,  0.0f }  // ����
    };

    // �摜���̐؂�o���͈͂�UV�ɕϊ�
    float u0 = tx / tsw;
    float v0 = ty / tsh;
    float u1 = (tx + tw) / tsw;
    float v1 = (ty + th) / tsh;

    XMFLOAT2 uv[4] = {
        { u0, v0 }, // ����
        { u1, v0 }, // �E��
        { u1, v1 }, // �E��
        { u0, v1 }  // ����
    };

    XMFLOAT4 col[4] = {
        color, // ����
        color, // �E��
        color, // �E��
        color  // ����
    };

    SetRectangleVerticesFrom4(v, pos, col, uv);

    // ���_�o�b�t�@�̃��b�N������
    g_pContext->Unmap(g_pVertexBuffer, 0);

    // ���_�o�b�t�@��`��p�C�v���C���ɐݒ�
    UINT stride = sizeof(Vertex);
    UINT offset = 0;
    g_pContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);

    // ���_�V�F�[�_�[��World�ϊ��s���ݒ�
    Shader_SetWorldMatrix(XMMatrixIdentity());
    
    // ���_�V�F�[�_�[�ɕϊ��s���ݒ�
    Shader_SetProjectionMatrix(XMMatrixOrthographicOffCenterLH(0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 0.0f, 1.0f));

    Shader_SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });

    g_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    // �e�N�X�`���͊O���ŃZ�b�g�iTexture_SetTexture�Ȃǁj
    Texture_SetTexture(texid);

    // �|���S���`�施�ߔ��s
    g_pContext->Draw(NUM_VERTEX, 0);
}


void Sprite_Draw(
    int texid,
    float x, float y, float width, float height,
    int tx, int ty, int tw, int th, float angle,
    XMFLOAT4 color)
{
    Shader_Begin();

    // ���_�o�b�t�@�����b�N����
    D3D11_MAPPED_SUBRESOURCE msr;
    g_pContext->Map(g_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

    // ���_�o�b�t�@�ւ̉��z�|�C���^���擾
    Vertex* v = (Vertex*)msr.pData;

    // ���_������������
    const float SCREEN_WIDTH = (float)Direct3D_GetBackBufferWidth();
    const float SCREEN_HEIGHT = (float)Direct3D_GetBackBufferHeight();

    float tsw = Texture_GetWidth(texid);
    float tsh = Texture_GetHeight(texid);

    // 4���_���̃f�[�^��p��
    XMFLOAT3 pos[4] = {
        { -0.5f,      -0.5f,      0.0f }, // ����
        { +0.5f,  -0.5f,      0.0f }, // �E��
        { +0.5f,  +0.5f,  0.0f }, // �E��
        { -0.5f,      +0.5f,  0.0f }  // ����
    };

    // �摜���̐؂�o���͈͂�UV�ɕϊ�
    float u0 = tx / tsw;
    float v0 = ty / tsh;
    float u1 = (tx + tw) / tsw;
    float v1 = (ty + th) / tsh;

    XMFLOAT2 uv[4] = {
        { u0, v0 }, // ����
        { u1, v0 }, // �E��
        { u1, v1 }, // �E��
        { u0, v1 }  // ����
    };

    XMFLOAT4 col[4] = {
        color, // ����
        color, // �E��
        color, // �E��
        color  // ����
    };

    SetRectangleVerticesFrom4(v, pos, col, uv);

    // ���_�o�b�t�@�̃��b�N������
    g_pContext->Unmap(g_pVertexBuffer, 0);

    // ���_�o�b�t�@��`��p�C�v���C���ɐݒ�
    UINT stride = sizeof(Vertex);
    UINT offset = 0;
    g_pContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);

    // ���_�V�F�[�_�[��World�ϊ��s���ݒ�
    XMMATRIX s = XMMatrixScaling(width, height, 1.0f);  // �� �\���T�C�Y���g�p
    XMMATRIX r = XMMatrixRotationZ(angle);
    XMMATRIX t = XMMatrixTranslation(x, y, 0.0f);
    Shader_SetWorldMatrix(s * r * t);

    // ���_�V�F�[�_�[�ɕϊ��s���ݒ�
    Shader_SetProjectionMatrix(XMMatrixOrthographicOffCenterLH(0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 0.0f, 1.0f));

    Shader_SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });

    g_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    // �e�N�X�`���͊O���ŃZ�b�g�iTexture_SetTexture�Ȃǁj
    Texture_SetTexture(texid);

    // �|���S���`�施�ߔ��s
    g_pContext->Draw(NUM_VERTEX, 0);
}