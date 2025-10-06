#include "texture.h"
#include <string>
#include "direct3d.h"
#include "DirectXTex.h"
using namespace DirectX;

static constexpr int TEXTURE_MAX = 1024;

struct Texture {
	std::wstring filename;
	unsigned int width = 0;
	unsigned int height = 0;
	ID3D11ShaderResourceView* pTexture = nullptr;
};

static Texture g_Textures[TEXTURE_MAX];

// ���ӁI�������ŊO������ݒ肳�����́BRelease�s�v�B
static ID3D11Device* g_pDevice = nullptr;
static ID3D11DeviceContext* g_pContext = nullptr;

void Texture_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	// �f�o�C�X�ƃf�o�C�X�R���e�L�X�g�̕ۑ�
	g_pDevice = pDevice;
	g_pContext = pContext;
}

void Texture_Finalize()
{
	Texture_AllRelease();
}

int Texture_Load(const wchar_t* pFileName, bool bMipMap)
{
	// ���ɓǂݍ���ł����t�@�C���Ή�
	for (int i = 0; i < TEXTURE_MAX; i++) {

		if (!g_Textures[i].pTexture) continue;

		if (g_Textures[i].filename == pFileName) {
			return i;
		}
	}

	for (int i = 0; i < TEXTURE_MAX; i++) {
		// �󂢂Ă���ꏊ��T��
		if (g_Textures[i].pTexture) continue;
		// �e�N�X�`���̃t�@�C������̓ǂݍ���
		TexMetadata metadata;
		ScratchImage image;

		LoadFromWICFile(pFileName, WIC_FLAGS_NONE, &metadata, image);

		g_Textures[i].width = (unsigned int)metadata.width;
		g_Textures[i].height = (unsigned int)metadata.height;

		if (bMipMap) {
			// �~�b�v�}�b�v���쐬����
			ScratchImage mipChain;
			GenerateMipMaps(image.GetImages(), image.GetImageCount(), image.GetMetadata(),
				TEX_FILTER_DEFAULT, 0, mipChain);
			// TEX_FILTER_BOX | TEX_FILTER_FORCE_NON_WIC
			image = std::move(mipChain);
			metadata = image.GetMetadata();
		}

		// �V�F�[�_�[���\�[�X�r���[�̍쐬
		HRESULT hr = CreateShaderResourceView(
			g_pDevice, image.GetImages(), image.GetImageCount(),
			metadata, &g_Textures[i].pTexture);

		if (FAILED(hr)) {
			MessageBox(nullptr, "�e�N�X�`���̓ǂݍ��݂Ɏ��s���܂���", "�G���[", MB_OK | MB_ICONERROR);
			break;
		}

		// �t�@�C������ۑ�
		g_Textures[i].filename = pFileName;

		return i;
	}

	return -1;
}

void Texture_AllRelease()
{
	for (Texture& t : g_Textures) {
		SAFE_RELEASE(t.pTexture);
	}
}

void Texture_SetTexture(int texture_id)
{
	if (texture_id < 0) {
		//
		return;
	}
	// �e�N�X�`���ݒ�
	g_pContext->PSSetShaderResources(0, 1, &g_Textures[texture_id].pTexture);
}

DirectX::XMUINT2 Texture_GetSize(int texture_id)
{
	return { g_Textures[texture_id].width, g_Textures[texture_id].height };
}

const unsigned int& Texture_GetWidth(int texture_id)
{
	return g_Textures[texture_id].width;
}

const unsigned int& Texture_GetHeight(int texture_id)
{
	return g_Textures[texture_id].height;
}
