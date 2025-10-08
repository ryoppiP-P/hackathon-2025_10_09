#include "player.h"
#include "texture.h"
#include <algorithm>

Player::Player()
    : x(0.0f), y(0.0f), width(32.0f), height(32.0f)
    , velocityX(0.0f), velocityY(0.0f)
    , gravity(800.0f), jumpPower(-400.0f), moveSpeed(200.0f), maxFallSpeed(600.0f)
    , isOnGround(false), canJump(false)
    , animPlayer(nullptr), idleAnim(nullptr), walkAnim(nullptr), jumpAnim(nullptr)
    , mapRef(nullptr)
    , collisionOffsetX(2.0f), collisionOffsetY(2.0f)
    , collisionWidth(28.0f), collisionHeight(30.0f) {
}

Player::~Player() {
    Uninit();
}

HRESULT Player::Init(Map* map) {
    mapRef = map;

    // �e�N�X�`���ǂݍ��݁i�����̂��̂��g�p�j
    int texid_koko = Texture_Load(L"resource/texture/kokosozai.png");

    // �A�j���[�V�����p�^�[���̍쐬
    idleAnim = new AnimPattern(texid_koko, 1, 0.5, { 0, 0 }, { 32, 32 });
    walkAnim = new AnimPattern(texid_koko, 13, 0.1, { 0, 0 }, { 32, 32 });
    jumpAnim = new AnimPattern(texid_koko, 1, 0.1, { 0, 32 }, { 32, 32 });
    // �����̓A�C�h���A�j���[�V����
    animPlayer = new AnimPatternPlayer(idleAnim);

    // �}�b�v����v���C���[�̏����ʒu���擾
    if (mapRef) {
        float spawnX, spawnY;
        if (mapRef->GetPlayerSpawnPosition(spawnX, spawnY)) {
            SetPosition(spawnX, spawnY - height); // �^�C���̏�ɔz�u
        } else {
            // �X�|�[���ʒu��������Ȃ��ꍇ�̓f�t�H���g�ʒu
            SetPosition(100.0f, 400.0f);
        }
    }
    
    return S_OK;
}

void Player::Uninit() {
    if (animPlayer) {
        delete animPlayer;
        animPlayer = nullptr;
    }
    if (idleAnim) {
        delete idleAnim;
        idleAnim = nullptr;
    }
    if (walkAnim) {
        delete walkAnim;
        walkAnim = nullptr;
    }
    if (jumpAnim) {
        delete jumpAnim;
        jumpAnim = nullptr;
    }
}

void Player::Update(double deltaTime) {
    // ���͏���
    HandleInput(deltaTime);

    // �������Z
    UpdatePhysics(deltaTime);

    // �}�b�v�Ƃ̓����蔻�菈��
    HandleMapCollision();

    // �A�j���[�V�����X�V
    UpdateAnimation(deltaTime);
}

void Player::Draw() {
    if (animPlayer) {
        animPlayer->Draw(x, y-320, width, height);
    }
}

void Player::SetPosition(float px, float py) {
    x = px;
    y = py;
}

void Player::GetPosition(float& px, float& py) const {
    px = x;
    py = y;
}

void Player::SetSize(float w, float h) {
    width = w;
    height = h;
}

void Player::GetSize(float& w, float& h) const {
    w = width;
    h = height;
}

void Player::GetCollisionRect(float& cx, float& cy, float& cw, float& ch) const {
    cx = x + collisionOffsetX;
    cy = y + collisionOffsetY;
    cw = collisionWidth;
    ch = collisionHeight;
}

bool Player::CheckMapCollision(float nextX, float nextY) const {
    if (!mapRef) return false;

    float cx, cy, cw, ch;
    cx = nextX + collisionOffsetX;
    cy = nextY + collisionOffsetY;
    cw = collisionWidth;
    ch = collisionHeight;

    return mapRef->CheckCollision(cx, cy, cw, ch);
}

void Player::HandleMapCollision() {
    if (!mapRef) return;

    // X�������̓����蔻��
    float nextX = x + velocityX * (1.0f / 60.0f);
    if (CheckMapCollision(nextX, y)) {
        velocityX = 0.0f; // �ǂɂԂ��������~
    } else {
        x = nextX;
    }

    // Y�������̓����蔻��
    float nextY = y + velocityY * (1.0f / 60.0f);
    if (CheckMapCollision(x, nextY)) {
        if (velocityY > 0.0f) {
            // �������i�������j�ɏՓ� = �n�ʂɒ��n
            isOnGround = true;
            canJump = true;
        }
        velocityY = 0.0f;
    } else {
        y = nextY;

        // �n�ʂ��痣�ꂽ�ꍇ
        if (velocityY < 0.0f || !CheckGroundCollision()) {
            isOnGround = false;
        }
    }
}

bool Player::CheckGroundCollision() const {
    if (!mapRef) return false;

    // �������̈ʒu���`�F�b�N
    float groundCheckY = y + height + 2.0f;
    return CheckMapCollision(x, groundCheckY);
}

void Player::HandleInput(double deltaTime) {
    float dt = (float)deltaTime;

    // ���E�ړ�
    if (KeyLogger_IsPressed(KK_A) || KeyLogger_IsPressed(KK_LEFT)) {
        velocityX = -moveSpeed;
    } else if (KeyLogger_IsPressed(KK_D) || KeyLogger_IsPressed(KK_RIGHT)) {
        velocityX = moveSpeed;
    } else {
        // ���C�ɂ�錸��
        if (isOnGround) {
            velocityX *= 0.8f; // �n�ʂł̖��C
        } else {
            velocityX *= 0.95f; // �󒆂ł̒�R
        }

        // �����ȑ��x��0�ɂ���
        if (abs(velocityX) < 10.0f) {
            velocityX = 0.0f;
        }
    }

    // �W�����v
    if ((KeyLogger_IsPressed(KK_SPACE) || KeyLogger_IsPressed(KK_W) || KeyLogger_IsPressed(KK_UP))
        && canJump && isOnGround) {
        velocityY = jumpPower;
        isOnGround = false;
        canJump = false;
    }
}

void Player::UpdatePhysics(double deltaTime) {
    float dt = (float)deltaTime;

    // �d�͂�K�p�i�n�ʂɂ��Ȃ��ꍇ�j
    if (!isOnGround) {
        velocityY += gravity * dt;

        // �ő嗎�����x�𐧌�
        if (velocityY > maxFallSpeed) {
            velocityY = maxFallSpeed;
        }
    }

    // ���x����
    if (velocityX > moveSpeed) velocityX = moveSpeed;
    if (velocityX < -moveSpeed) velocityX = -moveSpeed;
}

void Player::UpdateAnimation(double deltaTime) {
    if (!animPlayer || !animPlayer->IsValid()) return;

    // ���݂̏�Ԃɉ����ăA�j���[�V������؂�ւ�
    AnimPattern* targetAnim = nullptr;

    if (!isOnGround) {
        // �󒆂ɂ���ꍇ
        targetAnim = jumpAnim;
    } else if (abs(velocityX) > 10.0f) {
        // �ړ���
        targetAnim = walkAnim;
    } else {
        // ��~��
        targetAnim = idleAnim;
    }

    animPlayer->SetPattern(targetAnim);

    animPlayer->Update(deltaTime);
}

void Player::Reset() {
    velocityX = 0.0f;
    velocityY = 0.0f;
    isOnGround = false;
    canJump = false;

    // �����ʒu�ɖ߂�
    if (mapRef) {
        float spawnX, spawnY;
        if (mapRef->GetPlayerSpawnPosition(spawnX, spawnY)) {
            SetPosition(spawnX, spawnY - height);
        }
    }
}