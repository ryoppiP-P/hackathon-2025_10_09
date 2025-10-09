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

    // テクスチャ読み込み
    int texid_koko = Texture_Load(L"resource/texture/kokosozai.png");

    // アニメーションパターンの作成
    idleAnim = new AnimPattern(texid_koko, 1, 0.5, { 0, 0 }, { 32, 32 });
    walkAnim = new AnimPattern(texid_koko, 13, 0.1, { 0, 0 }, { 32, 32 });
    jumpAnim = new AnimPattern(texid_koko, 1, 0.1, { 0, 32 }, { 32, 32 });

    // 初期はアイドルアニメーション
    animPlayer = new AnimPatternPlayer(idleAnim);

    // BoxColliderの作成
    collider = new BoxCollider(collisionWidth, collisionHeight, ColliderType::DYNAMIC);
    collider->SetOffset(collisionOffsetX, collisionOffsetY);
    collider->SetLayer(1); // プレイヤーレイヤー

    // コールバック設定
    collider->SetOnCollisionEnter([this](const CollisionInfo& info) {
        OnCollisionWithMap(info);
        });

    collider->SetOnTriggerEnter([this](const CollisionInfo& info) {
        // トリガーの種類によって処理を分岐
        if (info.other->GetLayer() == 10) { // コインレイヤー
            OnTriggerWithCoin(info);
        } else if (info.other->GetLayer() == 20) { // ゴールレイヤー
            OnTriggerWithGoal(info);
        }
        });

    // マップからプレイヤーの初期位置を取得
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
    // 入力処理
    HandleInput(deltaTime);

    // 物理演算
    UpdatePhysics(deltaTime);

    // BoxColliderの位置を更新
    if (collider) {
        collider->SetPosition(x, y);
    }

    // マップとの当たり判定処理（旧システムも併用）
    HandleMapCollision();

    // アニメーション更新
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

// BoxColliderコールバック関数
void Player::OnCollisionWithMap(const CollisionInfo& info) {
    // マップとの衝突処理
    if (info.normalY < -0.5f) {
        // 地面に着地
        y -= info.penetrationY;
        velocityY = 0.0f;
        isOnGround = true;
        canJump = true;
    } else if (info.normalY > 0.5f) {
        // 天井にぶつかった
        y += info.penetrationY;
        velocityY = 0.0f;
    } else if (info.normalX != 0) {
        // 壁にぶつかった
        x -= info.penetrationX * info.normalX;
        velocityX = 0.0f;
    }
}

void Player::OnTriggerWithCoin(const CollisionInfo& info) {
    // コインを取得した処理
}

void Player::OnTriggerWithGoal(const CollisionInfo& info) {
    // ゴールに到達した処理
}

// 旧システムの互換性維持
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

    // X軸方向の当たり判定
    float nextX = x + velocityX * (1.0f / 60.0f);
    if (CheckMapCollision(nextX, y)) {
        velocityX = 0.0f;
    } else {
        x = nextX;
    }

    // Y軸方向の当たり判定
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
    // 左右移動
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

    // ジャンプ
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