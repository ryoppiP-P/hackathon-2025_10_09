#include "box_collider.h"
#include "sprite.h"
#include "texture.h"
#include <algorithm>
#include <cmath>

std::vector<BoxCollider*> BoxCollider::allColliders;

BoxCollider::BoxCollider(float w, float h, ColliderType t)
    : x(0.0f), y(0.0f), width(w), height(h)
    , offsetX(0.0f), offsetY(0.0f)
    , type(t), enabled(true), layer(0)
    , isInCollision(false) {

    allColliders.push_back(this);
}

BoxCollider::~BoxCollider() {
    // vector���玩�����폜
    auto it = std::find(allColliders.begin(), allColliders.end(), this);
    if (it != allColliders.end()) {
        allColliders.erase(it);
    }
}

void BoxCollider::SetPosition(float px, float py) {
    x = px;
    y = py;
}

void BoxCollider::GetPosition(float& px, float& py) const {
    px = x;
    py = y;
}

void BoxCollider::SetSize(float w, float h) {
    width = w;
    height = h;
}

void BoxCollider::GetSize(float& w, float& h) const {
    w = width;
    h = height;
}

void BoxCollider::SetOffset(float ox, float oy) {
    offsetX = ox;
    offsetY = oy;
}

void BoxCollider::GetOffset(float& ox, float& oy) const {
    ox = offsetX;
    oy = offsetY;
}

void BoxCollider::GetBounds(float& left, float& top, float& right, float& bottom) const {
    left = x + offsetX;
    top = y + offsetY;
    right = left + width;
    bottom = top + height;
}

bool BoxCollider::CheckCollision(const BoxCollider& other) const {
    if (!enabled || !other.enabled) return false;

    float left1, top1, right1, bottom1;
    float left2, top2, right2, bottom2;

    GetBounds(left1, top1, right1, bottom1);
    other.GetBounds(left2, top2, right2, bottom2);

    return !(right1 <= left2 || left1 >= right2 || bottom1 <= top2 || top1 >= bottom2);
}

CollisionInfo BoxCollider::GetCollisionInfo(const BoxCollider& other) const {
    CollisionInfo info;
    info.hasCollision = false;
    info.penetrationX = 0.0f;
    info.penetrationY = 0.0f;
    info.normalX = 0.0f;
    info.normalY = 0.0f;
    info.other = const_cast<BoxCollider*>(&other);

    if (!CheckCollision(other)) {
        return info;
    }

    float left1, top1, right1, bottom1;
    float left2, top2, right2, bottom2;

    GetBounds(left1, top1, right1, bottom1);
    other.GetBounds(left2, top2, right2, bottom2);

    info.hasCollision = true;

    // �d�Ȃ�ʂ��v�Z
    float overlapX = std::min(right1, right2) - std::max(left1, left2);
    float overlapY = std::min(bottom1, bottom2) - std::max(top1, top2);

    // ��菬�����d�Ȃ�̕����𕪗������Ƃ���
    if (overlapX < overlapY) {
        info.penetrationX = overlapX;
        info.penetrationY = 0.0f;
        info.normalX = (left1 < left2) ? -1.0f : 1.0f;
        info.normalY = 0.0f;
    } else {
        info.penetrationX = 0.0f;
        info.penetrationY = overlapY;
        info.normalX = 0.0f;
        info.normalY = (top1 < top2) ? -1.0f : 1.0f;
    }

    return info;
}

bool BoxCollider::ContainsPoint(float px, float py) const {
    if (!enabled) return false;

    float left, top, right, bottom;
    GetBounds(left, top, right, bottom);

    return (px >= left && px < right && py >= top && py < bottom);
}

CollisionInfo BoxCollider::SweepTest(float deltaX, float deltaY, const BoxCollider& other) const {
    CollisionInfo info;
    info.hasCollision = false;
    info.other = const_cast<BoxCollider*>(&other);

    // ���݈ʒu�ł̓����蔻��
    if (CheckCollision(other)) {
        return GetCollisionInfo(other);
    }

    // �ړ���̈ʒu�ł̓����蔻��
    BoxCollider tempCollider = *this;
    tempCollider.SetPosition(x + deltaX, y + deltaY);

    if (tempCollider.CheckCollision(other)) {
        info = tempCollider.GetCollisionInfo(other);
        // �ړ��ʂ���Փˎ��Ԃ��t�Z�i�ȈՔŁj
        // ��萳�m�Ȏ����ł͘A���I�ȏՓˌ��o���K�v
    }

    return info;
}

void BoxCollider::UpdateAllCollisions() {
    // �S�ẴR���C�_�[���m�̓����蔻����`�F�b�N
    for (size_t i = 0; i < allColliders.size(); i++) {
        for (size_t j = i + 1; j < allColliders.size(); j++) {
            BoxCollider* collider1 = allColliders[i];
            BoxCollider* collider2 = allColliders[j];

            if (!collider1->enabled || !collider2->enabled) continue;

            CollisionInfo info = collider1->GetCollisionInfo(*collider2);

            if (info.hasCollision) {
                if (collider1->type == ColliderType::TRIGGER || collider2->type == ColliderType::TRIGGER) {
                    // �g���K�[�R�[���o�b�N
                    if (collider1->onTriggerEnter) collider1->onTriggerEnter(info);
                    if (collider2->onTriggerEnter) {
                        CollisionInfo reverseInfo = info;
                        reverseInfo.other = collider1;
                        reverseInfo.normalX = -info.normalX;
                        reverseInfo.normalY = -info.normalY;
                        collider2->onTriggerEnter(reverseInfo);
                    }
                } else {
                    // �ʏ�̏Փ˃R�[���o�b�N
                    if (collider1->onCollisionEnter) collider1->onCollisionEnter(info);
                    if (collider2->onCollisionEnter) {
                        CollisionInfo reverseInfo = info;
                        reverseInfo.other = collider1;
                        reverseInfo.normalX = -info.normalX;
                        reverseInfo.normalY = -info.normalY;
                        collider2->onCollisionEnter(reverseInfo);
                    }
                }
            }
        }
    }
}

std::vector<BoxCollider*> BoxCollider::GetCollidersInLayer(int layer) {
    std::vector<BoxCollider*> result;
    for (BoxCollider* collider : allColliders) {
        if (collider->layer == layer) {
            result.push_back(collider);
        }
    }
    return result;
}

void BoxCollider::ClearAllColliders() {
    allColliders.clear();
}