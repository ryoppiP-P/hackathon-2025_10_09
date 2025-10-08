#pragma once

#include <vector>
#include <memory>
#include "direct3d.h"
#include "sprite.h"
#include "texture.h"

// タイルの種類
enum class TileType : int {
    EMPTY = 0,      // 空きタイル
    GROUND = 1,     // 地面
    BRICK = 2,      // ブロック
    COIN = 3,       // コイン
    PIPE = 4,       // 土管
    ENEMY_SPAWN = 5, // 敵の出現位置
    GOAL = 6,       // ゴール
    PLAYER_SPAWN = 7 // プレイヤーの初期位置
};

// タイル情報を保持する構造体
struct Tile {
    TileType type;
    bool isCollidable;  // 当たり判定があるか
    bool isVisible;     // 描画するか

    Tile(TileType t = TileType::EMPTY) : type(t) {
        // タイプに応じて当たり判定と可視性を設定
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
};

class Map {
private:
    int width;          // マップの幅（タイル数）
    int height;         // マップの高さ（タイル数）
    float tileSize;     // 1タイルのサイズ（ピクセル）

    std::vector<std::vector<Tile>> tiles;  // 2次元配列でタイルを管理

    // 各タイル用のテクスチャID
    int groundTexID;
    int brickTexID;
    int coinTexID;
    int pipeTexID;
    int dummyTexID;     // デバッグ用の単色テクスチャ

public:
    Map(int w, int h, float tSize = 32.0f);
    ~Map();

    // 初期化・終了処理
    HRESULT Init();
    void Uninit();
    void Update();
    void Draw();

    // マップ操作
    void SetTile(int x, int y, TileType type);
    TileType GetTile(int x, int y) const;
    bool IsCollidable(int x, int y) const;

    // 座標変換
    void WorldToTile(float worldX, float worldY, int& tileX, int& tileY) const;
    void TileToWorld(int tileX, int tileY, float& worldX, float& worldY) const;

    // 当たり判定チェック
    bool CheckCollision(float x, float y, float w, float h) const;

    // プレイヤーやエネミーの初期位置を取得
    bool GetPlayerSpawnPosition(float& x, float& y) const;
    std::vector<std::pair<float, float>> GetEnemySpawnPositions() const;

    // サンプルマップの生成
    void CreateSampleMap();

    // ゲッター
    int GetWidth() const { return width; }
    int GetHeight() const { return height; }
    float GetTileSize() const { return tileSize; }
};