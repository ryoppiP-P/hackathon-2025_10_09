/*****************************************************************//**
 * \file   GameObject.h
 * \brief  Unity風ゲームオブジェクトシステム
 * 
 * \author 菊池凌斗
 * \date   2025/10/6
 *********************************************************************/
#pragma once

#include <vector>
#include <memory>
#include <string>
#include <DirectXMath.h>
#include "Component.h"
#include "Transform.h"

using namespace DirectX;

namespace Engine {

class GameObject {
public:
    GameObject(const std::string& name = "GameObject");
    virtual ~GameObject();

    // コンポーネント管理
    template<typename T, typename... Args>
    T* AddComponent(Args&&... args);
    
    template<typename T>
    T* GetComponent();
    
    template<typename T>
    void RemoveComponent();

    // Transform（必須コンポーネント）
    Transform* GetTransform() { return m_transform; }

    // ライフサイクル
    virtual void Start();
    virtual void Update(double deltaTime);
    virtual void Draw();
    virtual void Destroy();

    // プロパティ
    bool IsActive() const { return m_active; }
    void SetActive(bool active) { m_active = active; }
    
    const std::string& GetName() const { return m_name; }
    void SetName(const std::string& name) { m_name = name; }

private:
    std::string m_name;
    bool m_active = true;
    bool m_started = false;
    
    Transform* m_transform;
    std::vector<std::unique_ptr<Component>> m_components;
};

// テンプレート実装
template<typename T, typename... Args>
T* GameObject::AddComponent(Args&&... args) {
    static_assert(std::is_base_of_v<Component, T>, "T must inherit from Component");
    
    auto component = std::make_unique<T>(std::forward<Args>(args)...);
    T* ptr = component.get();
    component->SetGameObject(this);
    m_components.push_back(std::move(component));
    return ptr;
}

template<typename T>
T* GameObject::GetComponent() {
    for (auto& component : m_components) {
        if (T* ptr = dynamic_cast<T*>(component.get())) {
            return ptr;
        }
    }
    return nullptr;
}

template<typename T>
void GameObject::RemoveComponent() {
    m_components.erase(
        std::remove_if(m_components.begin(), m_components.end(),
            [](const std::unique_ptr<Component>& component) {
                return dynamic_cast<T*>(component.get()) != nullptr;
            }),
        m_components.end()
    );
}

} // namespace Engine