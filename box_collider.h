#pragma once

#include <vector>
#include <functional>

// 当たり判定の種類
enum class ColliderType {
    STATIC,     // 静的オブジェクト（動かない）
    DYNAMIC,    // 動的オブジェクト（動く）
    TRIGGER     // トリガー（通り抜け可能、イベント発生）
};

// 当たり判定結果
struct CollisionInfo {
    bool hasCollision;
    float penetrationX;  // X軸の重なり量
    float penetrationY;  // Y軸の重なり量
    float normalX;       // 衝突面の法線X
    float normalY;       // 衝突面の法線Y
    class BoxCollider* other;  // 衝突相手
};

class BoxCollider {
private:
    float x, y;          // 位置
    float width, height; // サイズ
    float offsetX, offsetY; // オフセット（親オブジェクトからの相対位置）

    ColliderType type;
    bool enabled;        // 当たり判定の有効/無効
    int layer;          // レイヤー（どのレイヤーと衝突するかを制御）

    // コールバック関数
    std::function<void(const CollisionInfo&)> onCollisionEnter;
    std::function<void(const CollisionInfo&)> onCollisionStay;
    std::function<void(const CollisionInfo&)> onCollisionExit;
    std::function<void(const CollisionInfo&)> onTriggerEnter;
    std::function<void(const CollisionInfo&)> onTriggerStay;
    std::function<void(const CollisionInfo&)> onTriggerExit;

    // 管理用
    static std::vector<BoxCollider*> allColliders;
    bool isInCollision;

public:
    BoxCollider(float w = 32.0f, float h = 32.0f, ColliderType t = ColliderType::DYNAMIC);
    ~BoxCollider();

    // 位置・サイズ設定
    void SetPosition(float px, float py);
    void GetPosition(float& px, float& py) const;
    void SetSize(float w, float h);
    void GetSize(float& w, float& h) const;
    void SetOffset(float ox, float oy);
    void GetOffset(float& ox, float& oy) const;

    // 実際の当たり判定ボックスの座標を取得
    void GetBounds(float& left, float& top, float& right, float& bottom) const;

    // 当たり判定設定
    void SetType(ColliderType t) { type = t; }
    ColliderType GetType() const { return type; }
    void SetEnabled(bool enable) { enabled = enable; }
    bool IsEnabled() const { return enabled; }
    void SetLayer(int l) { layer = l; }
    int GetLayer() const { return layer; }

    // コールバック設定
    void SetOnCollisionEnter(std::function<void(const CollisionInfo&)> callback) { onCollisionEnter = callback; }
    void SetOnCollisionStay(std::function<void(const CollisionInfo&)> callback) { onCollisionStay = callback; }
    void SetOnCollisionExit(std::function<void(const CollisionInfo&)> callback) { onCollisionExit = callback; }
    void SetOnTriggerEnter(std::function<void(const CollisionInfo&)> callback) { onTriggerEnter = callback; }
    void SetOnTriggerStay(std::function<void(const CollisionInfo&)> callback) { onTriggerStay = callback; }
    void SetOnTriggerExit(std::function<void(const CollisionInfo&)> callback) { onTriggerExit = callback; }

    // 当たり判定チェック
    bool CheckCollision(const BoxCollider& other) const;
    CollisionInfo GetCollisionInfo(const BoxCollider& other) const;

    // 点との当たり判定
    bool ContainsPoint(float px, float py) const;

    // 移動時の当たり判定（スイープテスト）
    CollisionInfo SweepTest(float deltaX, float deltaY, const BoxCollider& other) const;

    // 静的メソッド
    static void UpdateAllCollisions();
    static std::vector<BoxCollider*> GetCollidersInLayer(int layer);
    static void ClearAllColliders();
};