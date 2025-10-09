#include "map.h"
#include "texture.h"
#include <DirectXMath.h>

// Tileコンストラクタ・デストラクタ
Tile::Tile(TileType t) : type(t), collider(nullptr) {
    switch (t) {
    case TileType::EMPTY:
        isCollidable = false;
        isVisible = false;
        break;
    case TileType::GROUND:
    case TileType::BRICK:
    case TileType::PIPE:
        isCollidable = true;
        isVisible = true;
        break;
    case TileType::COIN:
    case TileType::GOAL:
        isCollidable = false;
        isVisible = true;
        break;
    case TileType::ENEMY_SPAWN:
    case TileType::PLAYER_SPAWN:
        isCollidable = false;
        isVisible = false;
        break;
    }
}

Tile::~Tile() {
}

Map::Map(int w, int h, float tSize)
    : width(w), height(h), tileSize(tSize) {
    tiles.resize(height);
    for (int i = 0; i < height; i++) {
        tiles[i].resize(width);
    }
}

Map::~Map() {
    Uninit();
}

HRESULT Map::Init() {
    // テクスチャの読み込み
    dummyTexID = Texture_Load(L"resource/texture/kokosozai.png");

    groundTexID = dummyTexID;
    brickTexID = dummyTexID;
    coinTexID = dummyTexID;
    pipeTexID = dummyTexID;

    // サンプルマップを生成
    CreateSampleMap();

    // BoxColliderを生成
    CreateTileColliders();

    return S_OK;
}

void Map::Uninit() {
    ClearColliders();
}

void Map::Update() {
    UpdateColliders();
}

void Map::Draw() {
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (!tiles[y][x].isVisible) continue;

            float worldX = (float)x * tileSize;
            float worldY = (float)y * tileSize;

            int texID = dummyTexID;
            DirectX::XMFLOAT4 color = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

            switch (tiles[y][x].type) {
            case TileType::GROUND:
                texID = groundTexID;
                color = DirectX::XMFLOAT4(0.6f, 0.4f, 0.2f, 1.0f);
                break;
            case TileType::BRICK:
                texID = brickTexID;
                color = DirectX::XMFLOAT4(0.8f, 0.3f, 0.3f, 1.0f);
                break;
            case TileType::COIN:
                texID = coinTexID;
                color = DirectX::XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f);
                break;
            case TileType::PIPE:
                texID = pipeTexID;
                color = DirectX::XMFLOAT4(0.2f, 0.8f, 0.2f, 1.0f);
                break;
            case TileType::GOAL:
                texID = dummyTexID;
                color = DirectX::XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f);
                break;
            }

            int texWidth = 32;
            int texHeight = 32;

            Sprite_Draw(
                texID,
                worldX, worldY, tileSize, tileSize,
                0, 160, texWidth, texHeight,
                color
            );
        }
    }
}

void Map::CreateTileColliders() {
    ClearColliders(); // 既存のコライダーを削除

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            float worldX = (float)x * tileSize;
            float worldY = (float)y * tileSize;

            switch (tiles[y][x].type) {
            case TileType::GROUND:
            case TileType::BRICK:
            case TileType::PIPE:
            {
                // 静的な障害物コライダー
                BoxCollider* collider = new BoxCollider(tileSize, tileSize, ColliderType::STATIC);
                collider->SetPosition(worldX, worldY);
                collider->SetLayer(0); // マップレイヤー
                mapColliders.push_back(collider);
                break;
            }
            case TileType::COIN:
            {
                // コイン用トリガーコライダー
                BoxCollider* collider = new BoxCollider(tileSize, tileSize, ColliderType::TRIGGER);
                collider->SetPosition(worldX, worldY);
                collider->SetLayer(10); // コインレイヤー
                coinColliders.push_back(collider);
                break;
            }
            case TileType::GOAL:
            {
                // ゴール用トリガーコライダー
                BoxCollider* collider = new BoxCollider(tileSize, tileSize, ColliderType::TRIGGER);
                collider->SetPosition(worldX, worldY);
                collider->SetLayer(20); // ゴールレイヤー
                goalColliders.push_back(collider);
                break;
            }
            }
        }
    }
}

void Map::UpdateColliders() {
    // 必要に応じてコライダーの更新処理
    // 例：アニメーションするタイルがあれば位置更新など
}

void Map::ClearColliders() {
    for (BoxCollider* collider : mapColliders) {
        delete collider;
    }
    mapColliders.clear();

    for (BoxCollider* collider : coinColliders) {
        delete collider;
    }
    coinColliders.clear();

    for (BoxCollider* collider : goalColliders) {
        delete collider;
    }
    goalColliders.clear();
}

// 残りのメソッドは既存のまま
void Map::SetTile(int x, int y, TileType type) {
    if (x >= 0 && x < width && y >= 0 && y < height) {
        tiles[y][x] = Tile(type);
    }
}

TileType Map::GetTile(int x, int y) const {
    if (x >= 0 && x < width && y >= 0 && y < height) {
        return tiles[y][x].type;
    }
    return TileType::EMPTY;
}

bool Map::IsCollidable(int x, int y) const {
    if (x >= 0 && x < width && y >= 0 && y < height) {
        return tiles[y][x].isCollidable;
    }
    return false;
}

void Map::WorldToTile(float worldX, float worldY, int& tileX, int& tileY) const {
    tileX = (int)(worldX / tileSize);
    tileY = (int)(worldY / tileSize);
}

void Map::TileToWorld(int tileX, int tileY, float& worldX, float& worldY) const {
    worldX = (float)tileX * tileSize;
    worldY = (float)tileY * tileSize;
}

bool Map::CheckCollision(float x, float y, float w, float h) const {
    int left, top, right, bottom;
    WorldToTile(x, y, left, top);
    WorldToTile(x + w, y + h, right, bottom);

    for (int ty = top; ty <= bottom; ty++) {
        for (int tx = left; tx <= right; tx++) {
            if (IsCollidable(tx, ty)) {
                return true;
            }
        }
    }
    return false;
}

bool Map::GetPlayerSpawnPosition(float& x, float& y) const {
    for (int ty = 0; ty < height; ty++) {
        for (int tx = 0; tx < width; tx++) {
            if (tiles[ty][tx].type == TileType::PLAYER_SPAWN) {
                TileToWorld(tx, ty, x, y);
                return true;
            }
        }
    }
    return false;
}

std::vector<std::pair<float, float>> Map::GetEnemySpawnPositions() const {
    std::vector<std::pair<float, float>> positions;
    for (int ty = 0; ty < height; ty++) {
        for (int tx = 0; tx < width; tx++) {
            if (tiles[ty][tx].type == TileType::ENEMY_SPAWN) {
                float x, y;
                TileToWorld(tx, ty, x, y);
                positions.push_back(std::make_pair(x, y));
            }
        }
    }
    return positions;
}

void Map::CreateSampleMap() {
    // 既存のCreateSampleMapと同じ
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            SetTile(x, y, TileType::EMPTY);
        }
    }

    for (int x = 0; x < width; x++) {
        if (x < width - 10) {
            SetTile(x, height - 2, TileType::GROUND);
            SetTile(x, height - 1, TileType::GROUND);
        }
    }

    if (width > 12 && height > 6) {
        SetTile(10, height - 6, TileType::BRICK);
        SetTile(11, height - 6, TileType::BRICK);
        SetTile(12, height - 6, TileType::BRICK);

        SetTile(15, height - 4, TileType::BRICK);
        SetTile(16, height - 4, TileType::BRICK);

        SetTile(10, height - 7, TileType::COIN);
        SetTile(11, height - 7, TileType::COIN);
        SetTile(12, height - 7, TileType::COIN);

        SetTile(2, height - 4, TileType::PLAYER_SPAWN);

        if (width > 20) SetTile(20, height - 3, TileType::ENEMY_SPAWN);
        if (width > 30) SetTile(30, height - 3, TileType::ENEMY_SPAWN);

        SetTile(width - 5, height - 3, TileType::GOAL);
    }
}