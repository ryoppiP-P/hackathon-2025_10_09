/*****************************************************************//**
 * \file   SceneManager.h
 * \brief  シーン管理システム
 * 
 * \author 菊池凌斗
 * \date   2025/10/6
 *********************************************************************/
#pragma once

#include <vector>
#include <memory>
#include <functional>

namespace Engine {

class GameObject;

class Scene {
public:
    Scene() = default;
    virtual ~Scene() = default;

    // ライフサイクル
    virtual void Start() {}
    virtual void Update(double deltaTime);
    virtual void Draw();
    virtual void Destroy();

    // GameObject管理
    GameObject* CreateGameObject(const std::string& name = "GameObject");
    void DestroyGameObject(GameObject* gameObject);
    
    template<typename T>
    std::vector<T*> FindObjectsOfType();

private:
    std::vector<std::unique_ptr<GameObject>> m_gameObjects;
    std::vector<GameObject*> m_toDestroy;
};

class SceneManager {
public:
    static SceneManager& Instance();

    void LoadScene(std::unique_ptr<Scene> scene);
    Scene* GetCurrentScene() { return m_currentScene.get(); }
    
    void Update(double deltaTime);
    void Draw();

private:
    SceneManager() = default;
    std::unique_ptr<Scene> m_currentScene;
};

// テンプレート実装
template<typename T>
std::vector<T*> Scene::FindObjectsOfType() {
    std::vector<T*> result;
    for (auto& gameObject : m_gameObjects) {
        if (T* component = gameObject->GetComponent<T>()) {
            result.push_back(component);
        }
    }
    return result;
}

} // namespace Engine