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

    // テクスチャ読み込み（既存のものを使用）
    int texid_koko = Texture_Load(L"resource/texture/kokosozai.png");

    // アニメーションパターンの作成
    idleAnim = new AnimPattern(texid_koko, 1, 0.5, { 0, 0 }, { 32, 32 });
    walkAnim = new AnimPattern(texid_koko, 13, 0.1, { 0, 0 }, { 32, 32 });
    jumpAnim = new AnimPattern(texid_koko, 1, 0.1, { 0, 32 }, { 32, 32 });
    // 初期はアイドルアニメーション
    animPlayer = new AnimPatternPlayer(idleAnim);

    // マップからプレイヤーの初期位置を取得
    if (mapRef) {
        float spawnX, spawnY;
        if (mapRef->GetPlayerSpawnPosition(spawnX, spawnY)) {
            SetPosition(spawnX, spawnY - height); // タイルの上に配置
        } else {
            // スポーン位置が見つからない場合はデフォルト位置
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
    // 入力処理
    HandleInput(deltaTime);

    // 物理演算
    UpdatePhysics(deltaTime);

    // マップとの当たり判定処理
    HandleMapCollision();

    // アニメーション更新
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

    // X軸方向の当たり判定
    float nextX = x + velocityX * (1.0f / 60.0f);
    if (CheckMapCollision(nextX, y)) {
        velocityX = 0.0f; // 壁にぶつかったら停止
    } else {
        x = nextX;
    }

    // Y軸方向の当たり判定
    float nextY = y + velocityY * (1.0f / 60.0f);
    if (CheckMapCollision(x, nextY)) {
        if (velocityY > 0.0f) {
            // 下方向（落下中）に衝突 = 地面に着地
            isOnGround = true;
            canJump = true;
        }
        velocityY = 0.0f;
    } else {
        y = nextY;

        // 地面から離れた場合
        if (velocityY < 0.0f || !CheckGroundCollision()) {
            isOnGround = false;
        }
    }
}

bool Player::CheckGroundCollision() const {
    if (!mapRef) return false;

    // 少し下の位置をチェック
    float groundCheckY = y + height + 2.0f;
    return CheckMapCollision(x, groundCheckY);
}

void Player::HandleInput(double deltaTime) {
    float dt = (float)deltaTime;

    // 左右移動
    if (KeyLogger_IsPressed(KK_A) || KeyLogger_IsPressed(KK_LEFT)) {
        velocityX = -moveSpeed;
    } else if (KeyLogger_IsPressed(KK_D) || KeyLogger_IsPressed(KK_RIGHT)) {
        velocityX = moveSpeed;
    } else {
        // 摩擦による減速
        if (isOnGround) {
            velocityX *= 0.8f; // 地面での摩擦
        } else {
            velocityX *= 0.95f; // 空中での抵抗
        }

        // 微小な速度は0にする
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

    // 重力を適用（地面にいない場合）
    if (!isOnGround) {
        velocityY += gravity * dt;

        // 最大落下速度を制限
        if (velocityY > maxFallSpeed) {
            velocityY = maxFallSpeed;
        }
    }

    // 速度制限
    if (velocityX > moveSpeed) velocityX = moveSpeed;
    if (velocityX < -moveSpeed) velocityX = -moveSpeed;
}

void Player::UpdateAnimation(double deltaTime) {
    if (!animPlayer || !animPlayer->IsValid()) return;

    // 現在の状態に応じてアニメーションを切り替え
    AnimPattern* targetAnim = nullptr;

    if (!isOnGround) {
        // 空中にいる場合
        targetAnim = jumpAnim;
    } else if (abs(velocityX) > 10.0f) {
        // 移動中
        targetAnim = walkAnim;
    } else {
        // 停止中
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

    // 初期位置に戻す
    if (mapRef) {
        float spawnX, spawnY;
        if (mapRef->GetPlayerSpawnPosition(spawnX, spawnY)) {
            SetPosition(spawnX, spawnY - height);
        }
    }
}