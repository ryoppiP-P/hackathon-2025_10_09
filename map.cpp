#include "map.h"
#include "texture.h"
#include <DirectXMath.h>

Map::Map(int w, int h, float tSize)
    : width(w), height(h), tileSize(tSize) {
    // 2次元配列を初期化
    tiles.resize(height);
    for (int i = 0; i < height; i++) {
        tiles[i].resize(width);
    }
}

Map::~Map() {
    Uninit();
}

HRESULT Map::Init() {
    // テクスチャの読み込み（今はダミーテクスチャを作成）
    // 実際のゲームでは画像ファイルから読み込む

    // デバッグ用の単色テクスチャを作成
    dummyTexID = Texture_Load(L"resource/texture/kokosozai.png");

    // 各タイル用のテクスチャID（後で実際のテクスチャファイルに置き換え）
    groundTexID = dummyTexID;
    brickTexID = dummyTexID;
    coinTexID = dummyTexID;
    pipeTexID = dummyTexID;

    // サンプルマップを生成
    CreateSampleMap();

    return S_OK;
}

void Map::Uninit() {
    // テクスチャの解放は Texture_Finalize() で一括管理されるので
    // 個別に解放する必要はない
}

void Map::Update() {
    // マップ自体の更新処理（アニメーションタイルなどがあれば）
}

void Map::Draw() {
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (!tiles[y][x].isVisible) continue;

            float worldX, worldY;
            TileToWorld(x, y, worldX, worldY);

            int texID = dummyTexID;
            DirectX::XMFLOAT4 color = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

            switch (tiles[y][x].type) {
            case TileType::GROUND:
                texID = groundTexID;
                color = DirectX::XMFLOAT4(0.6f, 0.4f, 0.2f, 1.0f); // 茶色
                break;
            case TileType::BRICK:
                texID = brickTexID;
                color = DirectX::XMFLOAT4(0.8f, 0.3f, 0.3f, 1.0f); // 赤茶色
                break;
            case TileType::COIN:
                texID = coinTexID;
                color = DirectX::XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f); // 黄色
                break;
            case TileType::PIPE:
                texID = pipeTexID;
                color = DirectX::XMFLOAT4(0.2f, 0.8f, 0.2f, 1.0f); // 緑色
                break;
            case TileType::GOAL:
                texID = dummyTexID;
                color = DirectX::XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f); // マゼンタ
                break;
            }

            // Sprite_Draw関数を使用して描画
            // テクスチャ全体を使用する場合（tx=0, ty=0, tw=テクスチャ幅, th=テクスチャ高さ）
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
    // オブジェクトの四隅をタイル座標に変換してチェック
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
    // サンプルのマリオ風ステージを作成

    // 全体を空にする
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            SetTile(x, y, TileType::EMPTY);
        }
    }

    // 地面を設置（下から2段目）
    for (int x = 0; x < width; x++) {
        if (x < width - 10) { // 右端に穴を作る
            SetTile(x, height - 2, TileType::GROUND);
            SetTile(x, height - 1, TileType::GROUND);
        }
    }

    // ブロックをいくつか配置
    if (width > 12 && height > 6) {
        SetTile(10, height - 6, TileType::BRICK);
        SetTile(11, height - 6, TileType::BRICK);
        SetTile(12, height - 6, TileType::BRICK);

        SetTile(15, height - 4, TileType::BRICK);
        SetTile(16, height - 4, TileType::BRICK);

        // コインを配置
        SetTile(10, height - 7, TileType::COIN);
        SetTile(11, height - 7, TileType::COIN);
        SetTile(12, height - 7, TileType::COIN);

        // プレイヤーのスポーン位置
        SetTile(2, height - 4, TileType::PLAYER_SPAWN);

        // 敵のスポーン位置
        if (width > 20) SetTile(20, height - 3, TileType::ENEMY_SPAWN);
        if (width > 30) SetTile(30, height - 3, TileType::ENEMY_SPAWN);

        // ゴール
        SetTile(width - 5, height - 3, TileType::GOAL);
    }
}