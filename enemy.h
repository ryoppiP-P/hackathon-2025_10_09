#pragma once

#include "sprite_anim.h"
#include "map.h"
#include "key_logger.h"
#include "box_collider.h"
#include <DirectXMath.h>

constexpr int ENEMY_MAX = 100;

enum ENEMY_TYPE {
    ENEMY_TYPE_01,
    ENEMY_TYPE_02,
    ENEMY_TYPE_03,
    ENEMY_TYPE_04
};

class Enemy {
private:
    // �ʒu�ƃT�C�Y
    float x, y;           // ���݈ʒu
    float width, height;  // �G�̃T�C�Y
    ENEMY_TYPE type;   //�G�̎��

    // �����p�����[�^
    float velocityX, velocityY;  // ���x
    float gravity;               // �d�͉����x
    float jumpPower;            // �W�����v��
    float moveSpeed;            // �ړ����x
    float maxFallSpeed;         // �ő嗎�����x

    // ���
    bool isOnGround;            // �n�ʂɗ����Ă��邩
    bool canJump;               // �W�����v�ł��邩
    bool isUse;
    
    // �A�j���[�V����
    AnimPatternPlayer* animEnemy;
    AnimPattern* moveAnim;

    // �}�b�v�Q��
    Map* mapRef;

    // BoxCollider
    BoxCollider* collider;

public:
    Enemy();
    ~Enemy();

    // �������E�I������
    HRESULT Init(Map* map);
    void Uninit();
    void Update(double deltaTime);
    void Draw();

    // �ʒu�E�T�C�Y
    void SetEnemy(float x, float y, float w, float h, ENEMY_TYPE eType);

    // BoxCollider�A�N�Z�X
    BoxCollider* GetCollider() const { return collider; }

    bool GetIsUse(void) const { return isUse; }
};