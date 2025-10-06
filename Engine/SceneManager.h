/*****************************************************************//**
 * \file   SceneManager.h
 * \brief  �V�[���Ǘ��V�X�e��
 * 
 * \author �e�r���l
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

    // ���C�t�T�C�N��
    virtual void Start() {}
    virtual void Update(double deltaTime);
    virtual void Draw();
    virtual void Destroy();

    // GameObject�Ǘ�
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

// �e���v���[�g����
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