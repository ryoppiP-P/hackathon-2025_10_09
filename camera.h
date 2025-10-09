#pragma once

#include <DirectXMath.h>
#include <Windows.h>

class Player;
class Map;

class Camera {
private:
    float x, y;                    // �J�����ʒu�i����p�j
    float screenWidth, screenHeight;

    // �Ǐ]�ݒ�
    Player* targetPlayer;

    // �Y�[��
    float zoom; // �Y�[���{���i2.0f = 2�{�g��j

    // ���E����
    float minX, minY, maxX, maxY;
    bool boundsEnabled;
public:
    Camera();
    ~Camera();

    HRESULT Init(float screenW, float screenH);
    void Uninit();
    void Update(double deltaTime);

    // �^�[�Q�b�g�ݒ�
    void SetTarget(Player* player);

    // �Y�[���@�\
    void SetZoom(float z) { zoom = z; }
    float GetZoom() const { return zoom; }

    // ���E�����@�\
    void SetBoundsFromMap(Map* map);
    void SetBoundsEnabled(bool enabled) { boundsEnabled = enabled; }

    // �ʒu�擾
    float GetX() const { return x; }
    float GetY() const { return y; }
    void GetPosition(float& px, float& py) const { px = x; py = y; }

    // ���W�ϊ��i�V���v�����j
    void GetDrawPosition(float worldX, float worldY, float& drawX, float& drawY) const;
    void GetDrawSize(float worldW, float worldH, float& drawW, float& drawH) const;

private:
    void UpdateFollowing();
    void ApplyBounds();  // ���E������K�p
};

extern Camera* g_pCamera;