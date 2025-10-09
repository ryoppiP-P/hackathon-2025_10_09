/*****************************************************************//**
  \file    ƒŠƒUƒ‹ƒg [result.cpp]
  
  \Author  Ryoto Kikuchi
  \data    2025/9/19
 *********************************************************************/
#include "result.h"
#include "key_logger.h"
#include "texture.h"
#include "sprite.h"
#include "Audio.h"
#include "title.h"
#include "scene.h"
#include "direct3d.h"
#include <DirectXMath.h>

using namespace DirectX;

#undef DrawText

static int g_TexResultId = -1;
static int g_TexPressId = -1;
static int g_TexPushAnyKeyId = -1;
static int g_FontTexId = -1;

static int g_CurrentScore = 0;
static int g_HighScore = 0;
static bool g_IsNewRecord = false;


void Result_Initialize() {
    g_TexResultId = Texture_Load(L"resource/texture/result.png");
    g_TexPressId = Texture_Load(L"resource/texture/title_press.png");
    g_FontTexId = Texture_Load(L"resource/texture/consolab_ascii_512.png", false);
}

void Result_Finalize() {
}

void Result_Update(double elapsed_time) {
    if (KeyLogger_IsTrigger(KK_SPACE)) {
        Scene_SetNextScene(SCENE_TITLE);
    }
}

void Result_Draw() {
    // ”wŒi•`‰æ
    if (g_TexResultId >= 0) {
        float bg_width = 1920.0f;
        float bg_height = 1080.0f;

        Sprite_Draw(g_TexResultId,
            0, 0, bg_width, bg_height,
            0, 0, Texture_GetWidth(g_TexResultId), Texture_GetHeight(g_TexResultId));
    }

    float screen_width = (float)Direct3D_GetBackBufferWidth();
    float screen_height = (float)Direct3D_GetBackBufferHeight();


    // ‘€ìˆÄ“à
    if (g_TexPressId >= 0) {
        float text_width = 200.0f;
        float text_height = 50.0f;
        float text_x = (screen_width - text_width) * 0.5f;
        float text_y = screen_height * 0.7f;

        Sprite_Draw(g_TexPressId,
            text_x, text_y, text_width, text_height,
            0, 0, Texture_GetWidth(g_TexPressId), Texture_GetHeight(g_TexPressId));
    }
}