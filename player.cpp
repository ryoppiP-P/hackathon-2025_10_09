#include "player.h"
#include "texture.h"
#include <algorithm>

Player::Player()
    : x(0.0f), y(0.0f), width(32.0f), height(32.0f)
    , velocityX(0.0f), velocityY(0.0f)
    , gravity(800.0f), jumpPower(-400.0f), moveSpeed(200.0f), maxFallSpeed(600.0f)
    , isOnGround(false), canJump(false)
    , animPlayer(nullptr), idleAnim(nullptr), walkAnim(nullptr), jumpAnim(nullptr)
    , mapRef(nullptr), collider(nullptr)
    , collisionOffsetX(2.0f), collisionOffsetY(0.0f)
    , collisionWidth(24.0f), collisionHeight(26.0f) {
}

Player::~Player() {
    Uninit();
}

HRESULT Player::Init(Map* map) {
    mapRef = map;

    // �e�N�X�`���ǂݍ���
    int texid_koko = Texture_Load(L"resource/texture/kokosozai.png");

    // �A�j���[�V�����p�^�[���̍쐬
    idleAnim = new AnimPattern(texid_koko, 1, 0.5, { 0, 0 }, { 32, 32 });
    walkAnim = new AnimPattern(texid_koko, 13, 0.1, { 0, 0 }, { 32, 32 });
    jumpAnim = new AnimPattern(texid_koko, 1, 0.1, { 0, 32 }, { 32, 32 });

    // �����̓A�C�h���A�j���[�V����
    animPlayer = new AnimPatternPlayer(idleAnim);

    // BoxCollider�̍쐬
    collider = new BoxCollider(collisionWidth, collisionHeight, ColliderType::DYNAMIC);
    collider->SetOffset(collisionOffsetX, collisionOffsetY);
    collider->SetLayer(1); // �v���C���[���C���[

    // �R�[���o�b�N�ݒ�
    collider->SetOnCollisionEnter([this](const CollisionInfo& info) {
        OnCollisionWithMap(info);
        });

    collider->SetOnTriggerEnter([this](const CollisionInfo& info) {
        // �g���K�[�̎�ނɂ���ď����𕪊�
        if (info.other->GetLayer() == 10) { // �R�C�����C���[
            OnTriggerWithCoin(info);
        } else if (info.other->GetLayer() == 20) { // �S�[�����C���[
            OnTriggerWithGoal(info);
        }
        });

    // �}�b�v����v���C���[�̏����ʒu���擾
    if (mapRef) {
        float spawnX, spawnY;
        if (mapRef->GetPlayerSpawnPosition(spawnX, spawnY)) {
            SetPosition(spawnX, spawnY - height);
        } else {
            SetPosition(100.0f, 400.0f);
        }
    }

    return S_OK;
}

void Player::Uninit() {
    if (collider) {
        delete collider;
        collider = nullptr;
    }
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

    // BoxCollider�̈ʒu���X�V
    if (collider) {
        collider->SetPosition(x, y);
    }

    // �}�b�v�Ƃ̓����蔻�菈���i���V�X�e�������p�j
    HandleMapCollision();

    // �A�j���[�V�����X�V
    UpdateAnimation(deltaTime);
}

void Player::Draw() {
    if (animPlayer) {
        animPlayer->Draw(x, (y - 320) - 4, width, height);
    }
}

void Player::SetPosition(float px, float py) {
    x = px;
    y = py;
    if (collider) {
        collider->SetPosition(x, y);
    }
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

// BoxCollider�R�[���o�b�N�֐�
void Player::OnCollisionWithMap(const CollisionInfo& info) {
    // �}�b�v�Ƃ̏Փˏ���
    if (info.normalY < -0.5f) {
        // �n�ʂɒ��n
        y -= info.penetrationY;
        velocityY = 0.0f;
        isOnGround = true;
        canJump = true;
    } else if (info.normalY > 0.5f) {
        // �V��ɂԂ�����
        y += info.penetrationY;
        velocityY = 0.0f;
    } else if (info.normalX != 0) {
        // �ǂɂԂ�����
        x -= info.penetrationX * info.normalX;
        velocityX = 0.0f;
    }
}

void Player::OnTriggerWithCoin(const CollisionInfo& info) {
    // �R�C�����擾��������
}

void Player::OnTriggerWithGoal(const CollisionInfo& info) {
    // �S�[���ɓ��B��������
}

// ���V�X�e���̌݊����ێ�
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
        velocityX = 0.0f;
    } else {
        x = nextX;
    }

    // Y�������̓����蔻��
    float nextY = y + velocityY * (1.0f / 60.0f);
    if (CheckMapCollision(x, nextY)) {
        if (velocityY > 0.0f) {
            isOnGround = true;
            canJump = true;
        }
        velocityY = 0.0f;
    } else {
        y = nextY;
        if (velocityY < 0.0f || !CheckGroundCollision()) {
            isOnGround = false;
        }
    }
}

bool Player::CheckGroundCollision() const {
    if (!mapRef) return false;
    float groundCheckY = y + height + 2.0f;
    return CheckMapCollision(x, groundCheckY);
}

void Player::HandleInput(double deltaTime) {
    // ���E�ړ�
    if (KeyLogger_IsPressed(KK_A) || KeyLogger_IsPressed(KK_LEFT)) {
        velocityX = -moveSpeed;
    } else if (KeyLogger_IsPressed(KK_D) || KeyLogger_IsPressed(KK_RIGHT)) {
        velocityX = moveSpeed;
    } else {
        if (isOnGround) {
            velocityX *= 0.8f;
        } else {
            velocityX *= 0.95f;
        }
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

    if (!isOnGround) {
        velocityY += gravity * dt;
        if (velocityY > maxFallSpeed) {
            velocityY = maxFallSpeed;
        }
    }

    if (velocityX > moveSpeed) velocityX = moveSpeed;
    if (velocityX < -moveSpeed) velocityX = -moveSpeed;
}

void Player::UpdateAnimation(double deltaTime) {
    if (!animPlayer) return;

    AnimPattern* targetAnim = nullptr;

    if (!isOnGround) {
        targetAnim = jumpAnim;
    } else if (abs(velocityX) > 10.0f) {
        targetAnim = walkAnim;
    } else {
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

    if (mapRef) {
        float spawnX, spawnY;
        if (mapRef->GetPlayerSpawnPosition(spawnX, spawnY)) {
            SetPosition(spawnX, spawnY - height);
        }
    }
}