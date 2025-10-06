/*****************************************************************//**
 * \file   MainScene.h
 * \brief  ÉÅÉCÉìÉQÅ[ÉÄÉVÅ[Éì
 * 
 * \author ãeíróΩìl
 * \date   2025/10/6
 *********************************************************************/
#pragma once

#include "../Engine/SceneManager.h"
#include "../Engine/GameObject.h"
#include "../Engine/SpriteRenderer.h"
#include "../Engine/Animator.h"

class MainScene : public Engine::Scene {
public:
    virtual void Start() override;
    virtual void Update(double deltaTime) override;

private:
    Engine::GameObject* m_player = nullptr;
    Engine::GameObject* m_background = nullptr;
    std::vector<Engine::GameObject*> m_meteors;
    
    double m_meteorSpawnTimer = 0.0;
    double m_totalTime = 0.0;
};