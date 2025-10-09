#pragma once

#include <vector>
#include <functional>

// �����蔻��̎��
enum class ColliderType {
    STATIC,     // �ÓI�I�u�W�F�N�g�i�����Ȃ��j
    DYNAMIC,    // ���I�I�u�W�F�N�g�i�����j
    TRIGGER     // �g���K�[�i�ʂ蔲���\�A�C�x���g�����j
};

// �����蔻�茋��
struct CollisionInfo {
    bool hasCollision;
    float penetrationX;  // X���̏d�Ȃ��
    float penetrationY;  // Y���̏d�Ȃ��
    float normalX;       // �Փ˖ʂ̖@��X
    float normalY;       // �Փ˖ʂ̖@��Y
    class BoxCollider* other;  // �Փˑ���
};

class BoxCollider {
private:
    float x, y;          // �ʒu
    float width, height; // �T�C�Y
    float offsetX, offsetY; // �I�t�Z�b�g�i�e�I�u�W�F�N�g����̑��Έʒu�j

    ColliderType type;
    bool enabled;        // �����蔻��̗L��/����
    int layer;          // ���C���[�i�ǂ̃��C���[�ƏՓ˂��邩�𐧌�j

    // �R�[���o�b�N�֐�
    std::function<void(const CollisionInfo&)> onCollisionEnter;
    std::function<void(const CollisionInfo&)> onCollisionStay;
    std::function<void(const CollisionInfo&)> onCollisionExit;
    std::function<void(const CollisionInfo&)> onTriggerEnter;
    std::function<void(const CollisionInfo&)> onTriggerStay;
    std::function<void(const CollisionInfo&)> onTriggerExit;

    // �Ǘ��p
    static std::vector<BoxCollider*> allColliders;
    bool isInCollision;

public:
    BoxCollider(float w = 32.0f, float h = 32.0f, ColliderType t = ColliderType::DYNAMIC);
    ~BoxCollider();

    // �ʒu�E�T�C�Y�ݒ�
    void SetPosition(float px, float py);
    void GetPosition(float& px, float& py) const;
    void SetSize(float w, float h);
    void GetSize(float& w, float& h) const;
    void SetOffset(float ox, float oy);
    void GetOffset(float& ox, float& oy) const;

    // ���ۂ̓����蔻��{�b�N�X�̍��W���擾
    void GetBounds(float& left, float& top, float& right, float& bottom) const;

    // �����蔻��ݒ�
    void SetType(ColliderType t) { type = t; }
    ColliderType GetType() const { return type; }
    void SetEnabled(bool enable) { enabled = enable; }
    bool IsEnabled() const { return enabled; }
    void SetLayer(int l) { layer = l; }
    int GetLayer() const { return layer; }

    // �R�[���o�b�N�ݒ�
    void SetOnCollisionEnter(std::function<void(const CollisionInfo&)> callback) { onCollisionEnter = callback; }
    void SetOnCollisionStay(std::function<void(const CollisionInfo&)> callback) { onCollisionStay = callback; }
    void SetOnCollisionExit(std::function<void(const CollisionInfo&)> callback) { onCollisionExit = callback; }
    void SetOnTriggerEnter(std::function<void(const CollisionInfo&)> callback) { onTriggerEnter = callback; }
    void SetOnTriggerStay(std::function<void(const CollisionInfo&)> callback) { onTriggerStay = callback; }
    void SetOnTriggerExit(std::function<void(const CollisionInfo&)> callback) { onTriggerExit = callback; }

    // �����蔻��`�F�b�N
    bool CheckCollision(const BoxCollider& other) const;
    CollisionInfo GetCollisionInfo(const BoxCollider& other) const;

    // �_�Ƃ̓����蔻��
    bool ContainsPoint(float px, float py) const;

    // �ړ����̓����蔻��i�X�C�[�v�e�X�g�j
    CollisionInfo SweepTest(float deltaX, float deltaY, const BoxCollider& other) const;

    // �ÓI���\�b�h
    static void UpdateAllCollisions();
    static std::vector<BoxCollider*> GetCollidersInLayer(int layer);
    static void ClearAllColliders();
};