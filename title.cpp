/*****************************************************************//**
 * \file   title.cpp
 * \brief  タイトル画面の制御
 * 
 * \author 菊池凌斗
 * \date   2025/9/12
 *********************************************************************/
#include "title.h"
#include "key_logger.h"
#include "texture.h"
#include "sprite.h"
#include "Audio.h"
#include "scene.h"
#include "direct3d.h"
#include <DirectXMath.h>

using namespace DirectX;

static int g_TexTitleBackGroundId = -1;
static int g_TexTitleLogoId = -1;
static int g_TexTitlePressId = -1;
static double g_BlinkTimer = 0.0;
static bool g_ShowPushAnyKey = true;

void Title_Initialize() {
	// テクスチャの読み込み
	g_TexTitleBackGroundId = Texture_Load(L"resource/texture/title_bg.jpg");
	g_TexTitleLogoId = Texture_Load(L"resource/texture/title_logo.png");
	g_TexTitlePressId = Texture_Load(L"resource/texture/title_press.png");
	
	// 初期状態の設定
	g_BlinkTimer = 0.0;
	g_ShowPushAnyKey = true;
}

void Title_Finalize() {
}

void Title_Update(double elapsed_time) {
	// キー入力チェック
	if (KeyLogger_IsTrigger(KK_SPACE)) {
		Scene_SetNextScene(SCENE_GAME_ZAKO);
	}
}

void Title_Draw() {
	// タイトル背景の描画
	if (g_TexTitleBackGroundId >= 0) {
		float screen_width = (float)Direct3D_GetBackBufferWidth();
		float screen_height = (float)Direct3D_GetBackBufferHeight();

		float bg_width = 1920.0f;
		float bg_height = 1080.0f;

		Sprite_Draw(g_TexTitleBackGroundId,
			0, 0, bg_width, bg_height,
			0, 0, Texture_GetWidth(g_TexTitleBackGroundId), Texture_GetHeight(g_TexTitleBackGroundId));
	}

	// タイトルロゴの描画
	if (g_TexTitleLogoId >= 0) {
		float screen_width = (float)Direct3D_GetBackBufferWidth();
		float screen_height = (float)Direct3D_GetBackBufferHeight();
		
		float logo_width = 400.0f;
		float logo_height = 100.0f;
		float logo_x = (screen_width - logo_width) * 0.5f;
		float logo_y = screen_height * 0.3f;
		
		Sprite_Draw(g_TexTitleLogoId,
			logo_x, logo_y, logo_width, logo_height,
			0, 0, Texture_GetWidth(g_TexTitleLogoId), Texture_GetHeight(g_TexTitleLogoId));
	}
	
	// "Push Any Key" の点滅描画
	if (g_TexTitlePressId >= 0) {
		float screen_width = (float)Direct3D_GetBackBufferWidth();
		float screen_height = (float)Direct3D_GetBackBufferHeight();
		
		float text_width = 200.0f;
		float text_height = 50.0f;
		float text_x = (screen_width - text_width) * 0.5f;
		float text_y = screen_height * 0.7f;
		
		Sprite_Draw(g_TexTitlePressId,
			text_x, text_y, text_width, text_height,
			0, 0, Texture_GetWidth(g_TexTitlePressId), Texture_GetHeight(g_TexTitlePressId));
	}
}
