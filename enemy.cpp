#include "enemy.h"

static float move = 0;

Enemy::Enemy()
	: x(0.0f), y(0.0f), width(32.0f), height(32.0f), type(ENEMY_TYPE_01)
		, velocityX(0.0f), velocityY(0.0f)
		, gravity(800.0f), jumpPower(-400.0f), moveSpeed(3.0f), maxFallSpeed(600.0f)
		, isOnGround(false), canJump(false), isUse(false)
		, animEnemy(nullptr), moveAnim(nullptr)
		, mapRef(nullptr), collider(nullptr) {
}

Enemy::~Enemy()
{
	Uninit();
}

HRESULT Enemy::Init(Map* map)
{
    mapRef = map;
	
    // �e�N�X�`���ǂݍ���
    int texid_enemy = Texture_Load(L"resource/texture/enemy.png");

    // �A�j���[�V�����p�^�[���̍쐬
    moveAnim = new AnimPattern(texid_enemy, 2, 0.1, { 0, 0 }, { 32, 32 });

    // �����̓A�C�h���A�j���[�V����
    animEnemy = new AnimPatternPlayer(moveAnim);

    // BoxCollider�̍쐬
    collider = new BoxCollider(width, height, ColliderType::DYNAMIC);
    collider->SetLayer(1); // �v���C���[���C���[

	velocityX = moveSpeed;

    // �}�b�v����v���C���[�̏����ʒu���擾
    for (int y = 0; y < mapRef->GetHeight(); y++)
    {
        for (int x = 0; x < mapRef->GetWidth(); x++)
        {
            TileType t = mapRef->GetTile(x, y);
			switch (t)
			{
			case TileType::ENEMY_SPAWN01:
				SetEnemy(mapRef->GetTileSize() * x, mapRef->GetTileSize() * y, 32.0f, 32.0f, ENEMY_TYPE_01);
				break;
			case TileType::ENEMY_SPAWN02:
				SetEnemy(mapRef->GetTileSize() * x, mapRef->GetTileSize() * y, 32.0f, 32.0f, ENEMY_TYPE_02);
				break;
			case TileType::ENEMY_SPAWN03:
				SetEnemy(mapRef->GetTileSize() * x, mapRef->GetTileSize() * y, 32.0f, 32.0f, ENEMY_TYPE_03);
				break;
			case TileType::ENEMY_SPAWN04:
				SetEnemy(mapRef->GetTileSize() * x, mapRef->GetTileSize() * y, 32.0f, 32.0f, ENEMY_TYPE_04);
				break;
			default:
				break;
			}
        }
    }
    return S_OK;
}

void Enemy::Uninit()
{
}

void Enemy::Update(double deltaTime)
{
	if (isUse)
	{
		switch (type)
		{
		case ENEMY_TYPE_01:

			x += velocityX;

			break;
		case ENEMY_TYPE_02:
			break;
		case ENEMY_TYPE_03:
			break;
		case ENEMY_TYPE_04:
			break;
		default:
			break;
		}
		animEnemy->Update(deltaTime);
	}
}

void Enemy::Draw()
{
	animEnemy->Draw(x, y, width, height);
}

void Enemy::SetEnemy(float ex, float ey, float w, float h, ENEMY_TYPE eType)
{
	x = ex;
	y = ey;
	width = w;
	height = h;
	type = eType;

	isUse = true;
}
