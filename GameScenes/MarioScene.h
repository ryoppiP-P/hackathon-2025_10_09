/*****************************************************************//**
 * \file   MarioScene.h
 * \brief  マリオ風プラットフォーマーシーン
 * 
 * \author 菊池凌斗
 * \date   2025/10/6
 *********************************************************************/
#pragma once

#include "SceneManager.h"
#include "GameObject.h"
#include "SpriteRenderer.h"
#include "Physics.h"
#include "PlayerController.h"

class MarioScene : public Engine::Scene {
public:
    virtual void Start() override;
    virtual void Update(double deltaTime) override;

private:
    void CreatePlayer();
    void CreateGround();
    void CreatePlatforms();
    void CreateEnemies();
    void CreateCoins();
    void CreateBackground();

    Engine::GameObject* m_player = nullptr;
    Engine::GameObject* m_camera = nullptr;
    std::vector<Engine::GameObject*> m_platforms;
    std::vector<Engine::GameObject*> m_enemies;
    std::vector<Engine::GameObject*> m_coins;
    
    int m_score = 0;
    int m_lives = 3;
};