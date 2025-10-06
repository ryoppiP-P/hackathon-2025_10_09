/*==============================================================================

   �V�F�[�_�[ [shader.h]
														 Author : Youhei Sato
														 Date   : 2025/05/15
--------------------------------------------------------------------------------

==============================================================================*/
#ifndef SHADER_H
#define	SHADER_H

#include <d3d11.h>
#include <DirectXMath.h>

bool Shader_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
void Shader_Finalize();

void Shader_SetMatrix(const DirectX::XMMATRIX& matrix);
void Shader_SetPosition(const DirectX::XMFLOAT4& position);
void Shader_SetColor(const DirectX::XMFLOAT4& color);
void Shader_Begin();

#endif // SHADER_H
