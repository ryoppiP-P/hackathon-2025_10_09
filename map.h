#pragma once

#include <vector>
#include <memory>
#include "direct3d.h"
#include "sprite.h"
#include "texture.h"
#include "box_collider.h"

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
    bool isCollidable = false;  // �����蔻�肪���邩
    bool isVisible = false;     // �`�悷�邩
    BoxCollider* collider;  // �e�^�C���̃R���C�_�[

    Tile(TileType t = TileType::EMPTY);
    ~Tile();
};

class Map {
private:
    int width = 0;          // �}�b�v�̕��i�^�C�����j
    int height = 0;         // �}�b�v�̍����i�^�C�����j
    float tileSize= 0.0f;     // 1�^�C���̃T�C�Y�i�s�N�Z���j

    std::vector<std::vector<Tile>> tiles;  // 2�����z��Ń^�C�����Ǘ�

    // �e�^�C���p�̃e�N�X�`��ID
    int groundTexID = 0;
    int brickTexID = 0;
    int coinTexID = 0;
    int pipeTexID = 0;

    // �R���C�_�[�Ǘ�
    std::vector<BoxCollider*> mapColliders;    // �n�ʁE�Ǘp
    std::vector<BoxCollider*> coinColliders;   // �R�C���p
    std::vector<BoxCollider*> goalColliders;   // �S�[���p

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

    // �X�|�[���ʒu
    bool GetPlayerSpawnPosition(float& x, float& y) const;
    std::vector<std::pair<float, float>> GetEnemySpawnPositions() const;

    // BoxCollider�֘A
    void CreateTileColliders();      // �^�C���p�R���C�_�[����
    void UpdateColliders();          // �R���C�_�[�X�V
    void ClearColliders();          // �R���C�_�[�폜

    // �T���v���}�b�v�̐���
    void CreateSampleMap();

    // �Q�b�^�[
    int GetWidth() const { return width; }
    int GetHeight() const { return height; }
    float GetTileSize() const { return tileSize; }
};