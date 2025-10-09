#pragma once

#include <DirectXMath.h>
#include <Windows.h>

class Player;
class Map;

class Camera {
private:
    float x, y;                    // カメラ位置（左上角）
    float screenWidth, screenHeight;

    // 追従設定
    Player* targetPlayer;

    // ズーム
    float zoom; // ズーム倍率（2.0f = 2倍拡大）

    // 境界制限
    float minX, minY, maxX, maxY;
    bool boundsEnabled;
public:
    Camera();
    ~Camera();

    HRESULT Init(float screenW, float screenH);
    void Uninit();
    void Update(double deltaTime);

    // ターゲット設定
    void SetTarget(Player* player);

    // ズーム機能
    void SetZoom(float z) { zoom = z; }
    float GetZoom() const { return zoom; }

    // 境界制限機能
    void SetBoundsFromMap(Map* map);
    void SetBoundsEnabled(bool enabled) { boundsEnabled = enabled; }

    // 位置取得
    float GetX() const { return x; }
    float GetY() const { return y; }
    void GetPosition(float& px, float& py) const { px = x; py = y; }

    // 座標変換（シンプル化）
    void GetDrawPosition(float worldX, float worldY, float& drawX, float& drawY) const;
    void GetDrawSize(float worldW, float worldH, float& drawW, float& drawH) const;

private:
    void UpdateFollowing();
    void ApplyBounds();  // 境界制限を適用
};

extern Camera* g_pCamera;