#pragma once

#include "sprite_anim.h"
#include "map.h"
#include "key_logger.h"
#include <DirectXMath.h>

class Player {
private:
    // 位置とサイズ
    float x, y;           // 現在位置
    float width, height;  // プレイヤーのサイズ

    // 物理パラメータ
    float velocityX, velocityY;  // 速度
    float gravity;               // 重力加速度
    float jumpPower;            // ジャンプ力
    float moveSpeed;            // 移動速度
    float maxFallSpeed;         // 最大落下速度

    // 状態
    bool isOnGround;            // 地面に立っているか
    bool canJump;               // ジャンプできるか

    // アニメーション
    AnimPatternPlayer* animPlayer;
    AnimPattern* idleAnim;
    AnimPattern* walkAnim;
    AnimPattern* jumpAnim;

    // マップ参照
    Map* mapRef;

    // 当たり判定のオフセット（調整用）
    float collisionOffsetX, collisionOffsetY;
    float collisionWidth, collisionHeight;

public:
    Player();
    ~Player();

    // 初期化・終了処理
    HRESULT Init(Map* map);
    void Uninit();
    void Update(double deltaTime);
    void Draw();

    // 位置設定
    void SetPosition(float px, float py);
    void GetPosition(float& px, float& py) const;

    // サイズ設定
    void SetSize(float w, float h);
    void GetSize(float& w, float& h) const;

    // 当たり判定用の位置とサイズを取得
    void GetCollisionRect(float& cx, float& cy, float& cw, float& ch) const;

    // マップとの当たり判定
    bool CheckMapCollision(float nextX, float nextY) const;
    void HandleMapCollision();

    // 地面チェック
    bool CheckGroundCollision() const;

    // 入力処理
    void HandleInput(double deltaTime);

    // 物理演算
    void UpdatePhysics(double deltaTime);

    // アニメーション更新
    void UpdateAnimation(double deltaTime);

    // リセット（ゲームオーバー時など）
    void Reset();
};