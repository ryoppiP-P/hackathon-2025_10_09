#include "GameObject.h"
#include "Transform.h"
#include <algorithm>

namespace Engine {

GameObject::GameObject(const std::string& name) : m_name(name) {
    m_transform = AddComponent<Transform>();
}

GameObject::~GameObject() = default;

void GameObject::Start() {
    if (m_started) return;
    
    for (auto& component : m_components) {
        if (component->IsEnabled()) {
            component->Start();
        }
    }
    m_started = true;
}

void GameObject::Update(double deltaTime) {
    if (!m_active) return;
    
    if (!m_started) Start();
    
    for (auto& component : m_components) {
        if (component->IsEnabled()) {
            component->Update(deltaTime);
        }
    }
}

void GameObject::Draw() {
    if (!m_active) return;
    
    for (auto& component : m_components) {
        if (component->IsEnabled()) {
            component->Draw();
        }
    }
}

void GameObject::Destroy() {
    m_components.clear();
}

} // namespace Engine