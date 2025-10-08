#pragma once

#include "sprite_anim.h"
#include "map.h"
#include "key_logger.h"
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

    // �����蔻��̃I�t�Z�b�g�i�����p�j
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

    // �ʒu�ݒ�
    void SetPosition(float px, float py);
    void GetPosition(float& px, float& py) const;

    // �T�C�Y�ݒ�
    void SetSize(float w, float h);
    void GetSize(float& w, float& h) const;

    // �����蔻��p�̈ʒu�ƃT�C�Y���擾
    void GetCollisionRect(float& cx, float& cy, float& cw, float& ch) const;

    // �}�b�v�Ƃ̓����蔻��
    bool CheckMapCollision(float nextX, float nextY) const;
    void HandleMapCollision();

    // �n�ʃ`�F�b�N
    bool CheckGroundCollision() const;

    // ���͏���
    void HandleInput(double deltaTime);

    // �������Z
    void UpdatePhysics(double deltaTime);

    // �A�j���[�V�����X�V
    void UpdateAnimation(double deltaTime);

    // ���Z�b�g�i�Q�[���I�[�o�[���Ȃǁj
    void Reset();
};