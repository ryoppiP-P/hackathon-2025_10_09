#include "MainScene.h"
#include "../Engine/Transform.h"
#include "../texture.h"
#include "../sprite_anim.h"

void MainScene::Start() {
    // 背景作成
    m_background = CreateGameObject("Background");
    auto bgRenderer = m_background->AddComponent<Engine::SpriteRenderer>();
    bgRenderer->SetTexture(L"resource/texture/haikei.jpg");
    bgRenderer->SetSize(1280, 720);

    // プレイヤー作成
    m_player = CreateGameObject("Player");
    m_player->GetTransform()->SetPosition(1000.0f, 400.0f);
    
    auto playerRenderer = m_player->AddComponent<Engine::SpriteRenderer>();
    playerRenderer->SetTexture(L"resource/texture/runningman001.png");
    playerRenderer->SetSize(140, 200);
    
    auto playerAnimator = m_player->AddComponent<Engine::Animator>();
    // アニメーションパターンを設定
}

void MainScene::Update(double deltaTime) {
    Scene::Update(deltaTime);
    
    m_totalTime += deltaTime;
    m_meteorSpawnTimer += deltaTime;
    
    // 隕石生成
    if (m_meteorSpawnTimer > 1.0) {
        m_meteorSpawnTimer = 0.0;
        CreateMeteor();
    }
}

void MainScene::CreateMeteor() {
    auto meteor = CreateGameObject("Meteor");
    float startX = 100 + (rand() % (1280 - 200));
    meteor->GetTransform()->SetPosition(startX, -50);
    
    auto renderer = meteor->AddComponent<Engine::SpriteRenderer>();
    renderer->SetTexture(L"resource/texture/meteor.png");
    renderer->SetSize(32, 32);
    
    // 隕石の動きのコンポーネントを追加
    meteor->AddComponent<MeteorBehavior>();
}