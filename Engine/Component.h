/*****************************************************************//**
 * \file   Component.h
 * \brief  基底コンポーネントクラス
 * 
 * \author 菊池凌斗
 * \date   2025/10/6
 *********************************************************************/
#pragma once

namespace Engine {

class GameObject;

class Component {
public:
    Component() = default;
    virtual ~Component() = default;

    virtual void Start() {}
    virtual void Update(double deltaTime) {}
    virtual void Draw() {}

    GameObject* GetGameObject() const { return m_gameObject; }
    void SetGameObject(GameObject* gameObject) { m_gameObject = gameObject; }

    bool IsEnabled() const { return m_enabled; }
    void SetEnabled(bool enabled) { m_enabled = enabled; }

protected:
    GameObject* m_gameObject = nullptr;
    bool m_enabled = true;
};

} // namespace Engine