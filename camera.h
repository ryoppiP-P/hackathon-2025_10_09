#pragma once

#include <DirectXMath.h>
#include <Windows.h>

class Player;

class Camera {
private:
    float x, y;                    // カメラ位置（左上角）
    float screenWidth, screenHeight;

    // 追従設定
    Player* targetPlayer;

    // 境界制限
    float minX, minY, maxX, maxY;
    bool boundsEnabled;

    // シェイク効果
    float shakeIntensity, shakeDuration, shakeTimer;
    float shakeOffsetX, shakeOffsetY;

public:
    Camera();
    ~Camera();

    HRESULT Init(float screenW, float screenH);
    void Uninit();
    void Update(double deltaTime);

    // ターゲット設定
    void SetTarget(Player* player);

    // 位置取得
    float GetX() const { return x; }
    float GetY() const { return y; }
    void GetPosition(float& px, float& py) const { px = x; py = y; }

    // 座標変換（シンプル化）
    void GetDrawPosition(float worldX, float worldY, float& drawX, float& drawY) const;

private:
    void UpdateFollowing();
};

extern Camera* g_pCamera;