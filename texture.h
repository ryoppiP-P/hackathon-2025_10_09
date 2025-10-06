/*****************************************************************//**
 * \file   texture.h
 * \brief  
 * 
 * \author �e�r���l
 * \date   2025/6/6
 *********************************************************************/
#pragma once


#ifndef TEXTURE_H
#define TEXTURE_H

#include <d3d11.h>
#include <DirectXMath.h>

void Texture_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
void Texture_Finalize();

// �e�N�X�`���̓ǂݍ���
// 
// ����
// pFileName ... �摜�t�@�C����
// bMipMap ... MipMap����鎞��true
//
// �߂�l �e�N�X�`���Ǘ��ԍ��B�ǂݍ��߂Ȃ������ꍇ��-1
//
int Texture_Load(const wchar_t* pFileName, bool bMipMap = true);

void Texture_AllRelease();

void Texture_SetTexture(int texture_id);

DirectX::XMUINT2 Texture_GetSize(int texture_id);

const unsigned int& Texture_GetWidth(int texture_id);
const unsigned int& Texture_GetHeight(int texture_id);

#endif	// TEXTURE_H