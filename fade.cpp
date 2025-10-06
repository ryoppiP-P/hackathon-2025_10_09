/*********************************************************************
 * \file    fade.cpp[フェード処理]
 * 
 * \Author  Ryoto Kikuchi
 * \data    2025/9/16
 *********************************************************************/
#include "fade.h"
#include "sprite.h"
#include "texture.h"
using namespace DirectX;
#include "direct3d.h"

static FadeState g_FadeState = FADE_STATE_NONE;
static double g_FadeTime = 0.0;
static double g_AccumulatedTime = 0.0;
static double g_FadeStartTime = 0.0;
static XMFLOAT4 g_FadeColor = { 0.0f, 0.0f, 0.0f, 0.0f };
static int g_TexId = -1;

void Fade_Initialize() {
	g_FadeState = FADE_STATE_NONE;
	g_AccumulatedTime = 0.0;

	g_TexId = Texture_Load(L"resource/texture/white.png");
}

void Fade_Finalize() {
}

void Fade_Update(double elapsed_time) {
	g_AccumulatedTime += elapsed_time;
	
	if (g_FadeState == FADE_STATE_NONE || g_FadeState == FADE_STATE_IN_FINISHED || g_FadeState == FADE_STATE_OUT_FINISHED) {
		return;
	}

	double lifeTime = g_AccumulatedTime - g_FadeStartTime;

	float alpha = (float)(lifeTime / g_FadeTime);
	
	if (alpha > 1.0f) alpha = 1.0f;
	if (alpha < 0.0f) alpha = 0.0f;

	g_FadeColor.w = g_FadeState == FADE_STATE_IN ? 1.0f - alpha : alpha;

	if (lifeTime >= g_FadeTime) {
		g_FadeState = g_FadeState == FADE_STATE_IN ? FADE_STATE_IN_FINISHED : FADE_STATE_OUT_FINISHED;
		g_FadeColor.w = g_FadeState == FADE_STATE_IN_FINISHED ? 0.0f : 1.0f;
	}
}

void Fade_Draw() {
	if (g_FadeState == FADE_STATE_NONE) {
		return;
	}
	
	Sprite_Draw(g_TexId, 0.0f, 0.0f,
		Direct3D_GetBackBufferWidth(),
		Direct3D_GetBackBufferHeight(),
		0, 0, 8, 8,
		g_FadeColor);
}

void Fade_Start(double fade_time, bool isFadeIn, DirectX::XMFLOAT4 fade_color) {
	g_FadeTime = fade_time;
	g_FadeState = isFadeIn ? FADE_STATE_IN : FADE_STATE_OUT;
	g_FadeStartTime = g_AccumulatedTime;
	g_FadeColor = fade_color;
	
	g_FadeColor.w = isFadeIn ? 1.0f : 0.0f;
}

FadeState Fade_GetState() {
	return g_FadeState;
}
