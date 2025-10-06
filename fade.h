/*****************************************************************//**
 * \file   fade.h
 * \brief  ÉtÉFÅ[Éhèàóù
 * 
 * \author ãeíróΩìl
 * \date   2025/9/10
 *********************************************************************/
#ifndef FADE_H
#define FADE_H

#include <DirectXMath.h>

void Fade_Initialize();
void Fade_Finalize();
void Fade_Update(double elapsed_time);
void Fade_Draw();

void Fade_Start(double fade_time, bool isFadeIn, DirectX::XMFLOAT4 fade_color = {0.0f, 0.0f, 0.0f, 1.0f});

enum FadeState {
	FADE_STATE_NONE,
	FADE_STATE_IN,
	FADE_STATE_OUT,
	FADE_STATE_IN_FINISHED,
	FADE_STATE_OUT_FINISHED,
	FADE_STATE_MAX
};

FadeState Fade_GetState();

#endif //FADE_H
