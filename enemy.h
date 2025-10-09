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
    // 位置とサイズ
    float x, y;           // 現在位置
    float width, height;  // 敵のサイズ
    ENEMY_TYPE type;   //敵の種類

    // 物理パラメータ
    float velocityX, velocityY;  // 速度
    float gravity;               // 重力加速度
    float jumpPower;            // ジャンプ力
    float moveSpeed;            // 移動速度
    float maxFallSpeed;         // 最大落下速度

    // 状態
    bool isOnGround;            // 地面に立っているか
    bool canJump;               // ジャンプできるか
    bool isUse;
    
    // アニメーション
    AnimPatternPlayer* animEnemy;
    AnimPattern* moveAnim;

    // マップ参照
    Map* mapRef;

    // BoxCollider
    BoxCollider* collider;

public:
    Enemy();
    ~Enemy();

    // 初期化・終了処理
    HRESULT Init(Map* map);
    void Uninit();
    void Update(double deltaTime);
    void Draw();

    // 位置・サイズ
    void SetEnemy(float x, float y, float w, float h, ENEMY_TYPE eType);

    // BoxColliderアクセス
    BoxCollider* GetCollider() const { return collider; }

    bool GetIsUse(void) const { return isUse; }
};