#include "MarioScene.h"
#include "Transform.h"
#include "CollisionManager.h"
#include "Input.h"
#include "texture.h"

void MarioScene::Start() {
    CreateBackground();
    CreateGround();
    CreatePlatforms();
    CreatePlayer();
    CreateEnemies();
    CreateCoins();
}

void MarioScene::Update(double deltaTime) {
    Engine::Input::Update();
    Scene::Update(deltaTime);
    
    // 当たり判定チェック
    Engine::CollisionManager::Instance().CheckCollisions();
    
    // カメラ追従
    if (m_player && m_camera) {
        auto playerPos = m_player->GetTransform()->GetPosition();
        auto cameraPos = m_camera->GetTransform()->GetPosition();
        
        // プレイヤーに追従するカメラ
        cameraPos.x = playerPos.x - 640.0f; // 画面中央にプレイヤーを配置
        if (cameraPos.x < 0) cameraPos.x = 0; // 左端制限
        
        m_camera->GetTransform()->SetPosition(cameraPos);
    }
}

void MarioScene::CreatePlayer() {
    m_player = CreateGameObject("Player");
    m_player->GetTransform()->SetPosition(100.0f, 500.0f);
    
    // スプライト
    auto renderer = m_player->AddComponent<Engine::SpriteRenderer>();
    renderer->SetTexture(L"resource/texture/mario.png");
    renderer->SetSize(32, 32);
    
    // 物理
    auto rigidbody = m_player->AddComponent<Engine::Rigidbody>();
    rigidbody->SetGravity(980.0f);
    
    auto collider = m_player->AddComponent<Engine::BoxCollider>();
    collider->SetSize(28, 30);
    
    // プレイヤーコントローラー
    auto controller = m_player->AddComponent<Engine::PlayerController>();
    controller->SetMoveSpeed(200.0f);
    controller->SetJumpPower(400.0f);
    
    // 当たり判定登録
    Engine::CollisionManager::Instance().RegisterCollider(collider, "Player");
}

void MarioScene::CreateGround() {
    // 地面ブロック
    for (int x = 0; x < 100; ++x) {
        auto ground = CreateGameObject("Ground");
        ground->GetTransform()->SetPosition(x * 32.0f, 720.0f - 32.0f);
        
        auto renderer = ground->AddComponent<Engine::SpriteRenderer>();
        renderer->SetTexture(L"resource/texture/ground.png");
        renderer->SetSize(32, 32);
        
        auto collider = ground->AddComponent<Engine::BoxCollider>();
        collider->SetSize(32, 32);
        
        Engine::CollisionManager::Instance().RegisterCollider(collider, "Ground");
        m_platforms.push_back(ground);
    }
}

void MarioScene::CreatePlatforms() {
    // プラットフォーム例
    struct PlatformData {
        float x, y;
        int width;
    };
    
    std::vector<PlatformData> platforms = {
        {300.0f, 600.0f, 4},
        {500.0f, 500.0f, 3},
        {800.0f, 400.0f, 5},
        {1200.0f, 550.0f, 4}
    };
    
    for (auto& data : platforms) {
        for (int i = 0; i < data.width; ++i) {
            auto platform = CreateGameObject("Platform");
            platform->GetTransform()->SetPosition(data.x + i * 32.0f, data.y);
            
            auto renderer = platform->AddComponent<Engine::SpriteRenderer>();
            renderer->SetTexture(L"resource/texture/platform.png");
            renderer->SetSize(32, 32);
            
            auto collider = platform->AddComponent<Engine::BoxCollider>();
            collider->SetSize(32, 32);
            
            Engine::CollisionManager::Instance().RegisterCollider(collider, "Platform");
            m_platforms.push_back(platform);
        }
    }
}

void MarioScene::CreateEnemies() {
    std::vector<XMFLOAT2> enemyPositions = {
        {400.0f, 650.0f},
        {600.0f, 650.0f},
        {900.0f, 650.0f}
    };
    
    for (auto& pos : enemyPositions) {
        auto enemy = CreateGameObject("Enemy");
        enemy->GetTransform()->SetPosition(pos);
        
        auto renderer = enemy->AddComponent<Engine::SpriteRenderer>();
        renderer->SetTexture(L"resource/texture/goomba.png");
        renderer->SetSize(32, 32);
        
        auto rigidbody = enemy->AddComponent<Engine::Rigidbody>();
        rigidbody->SetVelocity(-50.0f, 0.0f); // 左に移動
        
        auto collider = enemy->AddComponent<Engine::BoxCollider>();
        collider->SetSize(28, 28);
        
        Engine::CollisionManager::Instance().RegisterCollider(collider, "Enemy");
        m_enemies.push_back(enemy);
    }
}

void MarioScene::CreateCoins() {
    std::vector<XMFLOAT2> coinPositions = {
        {350.0f, 550.0f},
        {550.0f, 450.0f},
        {850.0f, 350.0f}
    };
    
    for (auto& pos : coinPositions) {
        auto coin = CreateGameObject("Coin");
        coin->GetTransform()->SetPosition(pos);
        
        auto renderer = coin->AddComponent<Engine::SpriteRenderer>();
        renderer->SetTexture(L"resource/texture/coin.png");
        renderer->SetSize(24, 24);
        
        auto collider = coin->AddComponent<Engine::BoxCollider>();
        collider->SetSize(24, 24);
        
        Engine::CollisionManager::Instance().RegisterCollider(collider, "Coin");
        m_coins.push_back(coin);
    }
}

void MarioScene::CreateBackground() {
    auto bg = CreateGameObject("Background");
    auto renderer = bg->AddComponent<Engine::SpriteRenderer>();
    renderer->SetTexture(L"resource/texture/sky.png");
    renderer->SetSize(3200, 720); // 長い背景
    
    // カメラオブジェクト
    m_camera = CreateGameObject("Camera");
    m_camera->GetTransform()->SetPosition(0.0f, 0.0f);
}