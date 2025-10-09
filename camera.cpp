#include "camera.h"
#include "player.h"
#include <algorithm>
#include <cmath>
#include <random>

Camera* g_pCamera = nullptr;

Camera::Camera()
    : x(0.0f), y(0.0f)
    , screenWidth(1920.0f), screenHeight(1080.0f)
    , targetPlayer(nullptr)
    , minX(0.0f), minY(0.0f), maxX(10000.0f), maxY(10000.0f)
    , boundsEnabled(true)
    , shakeIntensity(0.0f), shakeDuration(0.0f), shakeTimer(0.0f)
    , shakeOffsetX(0.0f), shakeOffsetY(0.0f) {
}

Camera::~Camera() {
    Uninit();
}

HRESULT Camera::Init(float screenW, float screenH) {
    screenWidth = screenW;
    screenHeight = screenH;
    return S_OK;
}

void Camera::Uninit() {
    targetPlayer = nullptr;
}

void Camera::Update(double deltaTime) {
    UpdateFollowing();
}

void Camera::SetTarget(Player* player) {
    targetPlayer = player;
}

void Camera::GetDrawPosition(float worldX, float worldY, float& drawX, float& drawY) const {
    // 左上原点の座標系での変換
    // カメラ位置(x,y)は「ワールドの左上角」を表す
    drawX = worldX - x + shakeOffsetX;
    drawY = worldY - y + shakeOffsetY;
}


void Camera::UpdateFollowing() {
    if (!targetPlayer) return;

    float playerX, playerY;
    targetPlayer->GetPosition(playerX, playerY);

    x = playerX - (screenWidth * 0.5f);
    y = playerY - (screenHeight * 0.5f);
}