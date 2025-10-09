#pragma once

#include <DirectXMath.h>
#include <Windows.h>

class Player;

class Camera {
private:
    float x, y;                    // �J�����ʒu�i����p�j
    float screenWidth, screenHeight;

    // �Ǐ]�ݒ�
    Player* targetPlayer;

    // ���E����
    float minX, minY, maxX, maxY;
    bool boundsEnabled;

    // �V�F�C�N����
    float shakeIntensity, shakeDuration, shakeTimer;
    float shakeOffsetX, shakeOffsetY;

public:
    Camera();
    ~Camera();

    HRESULT Init(float screenW, float screenH);
    void Uninit();
    void Update(double deltaTime);

    // �^�[�Q�b�g�ݒ�
    void SetTarget(Player* player);

    // �ʒu�擾
    float GetX() const { return x; }
    float GetY() const { return y; }
    void GetPosition(float& px, float& py) const { px = x; py = y; }

    // ���W�ϊ��i�V���v�����j
    void GetDrawPosition(float worldX, float worldY, float& drawX, float& drawY) const;

private:
    void UpdateFollowing();
};

extern Camera* g_pCamera;