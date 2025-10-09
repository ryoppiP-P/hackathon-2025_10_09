#include "camera.h"
#include "player.h"
#include "map.h"
#include <algorithm>
#include <cmath>
#include <random>

Camera* g_pCamera = nullptr;

Camera::Camera()
    : x(0.0f), y(0.0f)
    , screenWidth(1920.0f), screenHeight(1080.0f)
    , targetPlayer(nullptr)
	, zoom(2.0f)
    , minX(0.0f), minY(0.0f), maxX(10000.0f), maxY(10000.0f)
    , boundsEnabled(false) {
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
    if (boundsEnabled) {
        ApplyBounds();
    }
}

void Camera::SetTarget(Player* player) {
    targetPlayer = player;
}

// �}�b�v���狫�E�������ݒ�
void Camera::SetBoundsFromMap(Map* map) {
    if (!map) {
        boundsEnabled = false;
        return;
    }

    // �}�b�v�̃s�N�Z���T�C�Y���v�Z
    float mapPixelWidth = map->GetWidth() * map->GetTileSize();
    float mapPixelHeight = map->GetHeight() * map->GetTileSize();

    // �Y�[�����l�������J�����̉��͈�
    float viewWidth = screenWidth / zoom;
    float viewHeight = screenHeight / zoom;

    // �J�����̋��E��ݒ�
    minX = 0.0f;
    minY = 0.0f;
    maxX = std::max(0.0f, mapPixelWidth - viewWidth);   // �}�b�v�� - ��ʕ�
    maxY = std::max(0.0f, mapPixelHeight - viewHeight); // �}�b�v�� - ��ʍ�

    boundsEnabled = true;
}

void Camera::GetDrawPosition(float worldX, float worldY, float& drawX, float& drawY) const {
    // ���㌴�_�̍��W�n�ł̕ϊ�
    // �J�����ʒu(x,y)�́u���[���h�̍���p�v��\��
    drawX = (worldX - x) * zoom;
    drawY = (worldY - y) * zoom;
}

// �Y�[���Ή��̃T�C�Y�ϊ�
void Camera::GetDrawSize(float worldW, float worldH, float& drawW, float& drawH) const {
    drawW = worldW * zoom;
    drawH = worldH * zoom;
}

void Camera::UpdateFollowing() {
    if (!targetPlayer) return;

    float playerX, playerY;
    targetPlayer->GetPosition(playerX, playerY);

    x = playerX - (screenWidth * 0.5f) / zoom;
    y = playerY - (screenHeight * 0.5f) / zoom;
}

// ���E������K�p
void Camera::ApplyBounds() {
    x = std::max(minX, std::min(x, maxX));
    y = std::max(minY, std::min(y, maxY));
}