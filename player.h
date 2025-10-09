#pragma once

#include "sprite_anim.h"
#include "map.h"
#include "key_logger.h"
#include "box_collider.h"
#include <DirectXMath.h>

class Player {
private:
    // �ʒu�ƃT�C�Y
    float x, y;           // ���݈ʒu
    float width, height;  // �v���C���[�̃T�C�Y

    // �����p�����[�^
    float velocityX, velocityY;  // ���x
    float gravity;               // �d�͉����x
    float jumpPower;            // �W�����v��
    float moveSpeed;            // �ړ����x
    float maxFallSpeed;         // �ő嗎�����x

    // ���
    bool isOnGround;            // �n�ʂɗ����Ă��邩
    bool canJump;               // �W�����v�ł��邩

    // �A�j���[�V����
    AnimPatternPlayer* animPlayer;
    AnimPattern* idleAnim;
    AnimPattern* walkAnim;
    AnimPattern* jumpAnim;

    // �}�b�v�Q��
    Map* mapRef;

    // BoxCollider
    BoxCollider* collider;

    // kesu
    float collisionOffsetX, collisionOffsetY;
    float collisionWidth, collisionHeight;

public:
    Player();
    ~Player();

    // �������E�I������
    HRESULT Init(Map* map);
    void Uninit();
    void Update(double deltaTime);
    void Draw();

    // �ʒu�E�T�C�Y
    void SetPosition(float px, float py);
    void GetPosition(float& px, float& py) const;
    void SetSize(float w, float h);
    void GetSize(float& w, float& h) const;

    // BoxCollider�A�N�Z�X
    BoxCollider* GetCollider() const { return collider; }

    // �����蔻��R�[���o�b�N
    void OnCollisionWithMap(const CollisionInfo& info);
    void OnTriggerWithCoin(const CollisionInfo& info);
    void OnTriggerWithGoal(const CollisionInfo& info);

    // ���V�X�e���i�݊����̂��ߎc���j
    void GetCollisionRect(float& cx, float& cy, float& cw, float& ch) const;
    bool CheckMapCollision(float nextX, float nextY) const;
    void HandleMapCollision();
    bool CheckGroundCollision() const;

    // ���́E�����E�A�j���[�V����
    void HandleInput(double deltaTime);
    void UpdatePhysics(double deltaTime);
    void UpdateAnimation(double deltaTime);

    // ���Z�b�g
    void Reset();
};