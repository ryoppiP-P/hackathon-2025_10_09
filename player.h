#pragma once

#include "sprite_anim.h"
#include "map.h"
#include "key_logger.h"
#include "box_collider.h"
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

    // BoxCollider
    BoxCollider* collider;

    // kesu
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

    // 位置・サイズ
    void SetPosition(float px, float py);
    void GetPosition(float& px, float& py) const;
    void SetSize(float w, float h);
    void GetSize(float& w, float& h) const;

    // BoxColliderアクセス
    BoxCollider* GetCollider() const { return collider; }

    // 当たり判定コールバック
    void OnCollisionWithMap(const CollisionInfo& info);
    void OnTriggerWithCoin(const CollisionInfo& info);
    void OnTriggerWithGoal(const CollisionInfo& info);

    // 旧システム（互換性のため残す）
    void GetCollisionRect(float& cx, float& cy, float& cw, float& ch) const;
    bool CheckMapCollision(float nextX, float nextY) const;
    void HandleMapCollision();
    bool CheckGroundCollision() const;

    // 入力・物理・アニメーション
    void HandleInput(double deltaTime);
    void UpdatePhysics(double deltaTime);
    void UpdateAnimation(double deltaTime);

    // リセット
    void Reset();
};