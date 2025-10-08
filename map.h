#pragma once

#include <vector>
#include <memory>
#include "direct3d.h"
#include "sprite.h"
#include "texture.h"

// �^�C���̎��
enum class TileType : int {
    EMPTY = 0,      // �󂫃^�C��
    GROUND = 1,     // �n��
    BRICK = 2,      // �u���b�N
    COIN = 3,       // �R�C��
    PIPE = 4,       // �y��
    ENEMY_SPAWN = 5, // �G�̏o���ʒu
    GOAL = 6,       // �S�[��
    PLAYER_SPAWN = 7 // �v���C���[�̏����ʒu
};

// �^�C������ێ�����\����
struct Tile {
    TileType type;
    bool isCollidable;  // �����蔻�肪���邩
    bool isVisible;     // �`�悷�邩

    Tile(TileType t = TileType::EMPTY) : type(t) {
        // �^�C�v�ɉ����ē����蔻��Ɖ�����ݒ�
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
    int width;          // �}�b�v�̕��i�^�C�����j
    int height;         // �}�b�v�̍����i�^�C�����j
    float tileSize;     // 1�^�C���̃T�C�Y�i�s�N�Z���j

    std::vector<std::vector<Tile>> tiles;  // 2�����z��Ń^�C�����Ǘ�

    // �e�^�C���p�̃e�N�X�`��ID
    int groundTexID;
    int brickTexID;
    int coinTexID;
    int pipeTexID;
    int dummyTexID;     // �f�o�b�O�p�̒P�F�e�N�X�`��

public:
    Map(int w, int h, float tSize = 32.0f);
    ~Map();

    // �������E�I������
    HRESULT Init();
    void Uninit();
    void Update();
    void Draw();

    // �}�b�v����
    void SetTile(int x, int y, TileType type);
    TileType GetTile(int x, int y) const;
    bool IsCollidable(int x, int y) const;

    // ���W�ϊ�
    void WorldToTile(float worldX, float worldY, int& tileX, int& tileY) const;
    void TileToWorld(int tileX, int tileY, float& worldX, float& worldY) const;

    // �����蔻��`�F�b�N
    bool CheckCollision(float x, float y, float w, float h) const;

    // �v���C���[��G�l�~�[�̏����ʒu���擾
    bool GetPlayerSpawnPosition(float& x, float& y) const;
    std::vector<std::pair<float, float>> GetEnemySpawnPositions() const;

    // �T���v���}�b�v�̐���
    void CreateSampleMap();

    // �Q�b�^�[
    int GetWidth() const { return width; }
    int GetHeight() const { return height; }
    float GetTileSize() const { return tileSize; }
};