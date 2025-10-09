/*****************************************************************//**
 * \file   main.cpp
 * \brief  
 * 
 * \author �e�r���l
 * \date   2025/4/25
 *********************************************************************/
#include <SDKDDKVer.h>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <algorithm>
#include "direct3d.h"
#include "shader.h"
#include "texture.h"
#include "sprite.h"
#include "sprite_anim.h"
#include "system_timer.h"
#include "debug_text.h"
#include "key_logger.h"
#include <Xinput.h>
#pragma comment(lib, "xinput.lib")
#include "mouse.h"
#include "map.h"
#include "player.h"
<<<<<<< HEAD
#include "camera.h"
#include "scene.h"
=======
#include "enemy.h"
>>>>>>> 7af28c146c241fe12d89092bfc43f3fb843f69b1

#include <sstream>
#include <DirectXMath.h>
using namespace DirectX;

//�E�B���h�E���
static constexpr char WINDOW_CLASS[] = "GameWindow"; // x151957
static constexpr char TITLE[] = "DirectX2D beta";// �^�C�g���o�[�̃e�L�X�g

Map* g_pMap = nullptr;
Player* g_pPlayer = nullptr;
Enemy* g_pEnemy[ENEMY_MAX] = {};

// �V�[������
static Scene g_Scene = SCENE_TITLE;
static Scene g_NextScene = SCENE_TITLE;
static bool g_ChangeRequest = false;


//�E�B���h�E�v���V�[�W�� �v���g�^�C�v�錾
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// ���C��
int APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	(void)CoInitializeEx(nullptr, COINIT_MULTITHREADED);

	/* �E�B���h�E�N���X�̓o�^*/
	WNDCLASSEX wcex{};

	wcex.cbSize = sizeof(WNDCLASSEX); 
	wcex.lpfnWndProc = WndProc;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = nullptr; //���j���[�͍��Ȃ�
	wcex.lpszClassName = WINDOW_CLASS;
	wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);

	RegisterClassEx(&wcex);

	// �N���C�A���g�̈�̃T�C�Y���������i������left, top, right, bottom�j
	RECT window_rect = { 0, 0, 1920, 1080 };
	// �E�B���h�E�̃X�^�C�� - �t���X�N���[���p�ɕύX
	DWORD window_style =/* WS_POPUP;*/ WS_OVERLAPPEDWINDOW & ~(WS_THICKFRAME | WS_MAXIMIZEBOX);
	// �w�肵���N���C�A���g�̈���m�ۂ��邽�߂ɐV���ȋ�`���W���v�Z
	AdjustWindowRect(&window_rect, window_style, FALSE);

	// �v���C�}�����j�^�[�̉摜�𑜓x�擾
	int desktop_width = GetSystemMetrics(SM_CXSCREEN);
	int desktop_height = GetSystemMetrics(SM_CYSCREEN);

	// �t���X�N���[���p�̐ݒ�
	int window_width = desktop_width;
	int window_height = desktop_height;
	int window_x = 0;
	int window_y = 0;

	/* ���C���E�B���h�E�̍쐬 */
	HWND hwnd = CreateWindow(
		WINDOW_CLASS, 
		TITLE,
		window_style,
		window_x,
		window_y,
		window_width,
		window_height,
		nullptr,
		nullptr,
		hInstance,
		nullptr
	);

	/* �e�평���� */
	if (!Direct3D_Initialize(hwnd)) {
		PostQuitMessage(0);
	} else {
		if (!Shader_Initialize(Direct3D_GetDevice(), Direct3D_GetDeviceContext())) {
			PostQuitMessage(0);
		} else {
			Texture_Initialize(Direct3D_GetDevice(), Direct3D_GetDeviceContext());
			Sprite_Initialize(Direct3D_GetDevice(), Direct3D_GetDeviceContext());
			// �}�b�v�̏�����
<<<<<<< HEAD
			g_pMap = new Map(400, 30, 32.0f);
			if (FAILED(g_pMap->Init())) {
=======
			g_pMap = new Map(130, 30, 32.0f);
			/*if (FAILED(g_pMap->Init())) {
				PostQuitMessage(0);
			}*/
			if (FAILED(g_pMap->Init(MapStage::STAGE1))) {
>>>>>>> 7af28c146c241fe12d89092bfc43f3fb843f69b1
				PostQuitMessage(0);
			}
			
			// �v���C���[�̏�����
			g_pPlayer = new Player();
			if (FAILED(g_pPlayer->Init(g_pMap))) {
				PostQuitMessage(0);
			}
<<<<<<< HEAD

			// �J�����̏�����
			g_pCamera = new Camera();
			if (FAILED(g_pCamera->Init(1920, 1080))) {
				PostQuitMessage(0);
			}

			// �J�����Ƀv���C���[��ݒ�
			g_pCamera->SetTarget(g_pPlayer);

			// �}�b�v�T�C�Y�ɉ����ăJ�����̈ړ��͈͂�ݒ�
			float mapPixelWidth = g_pMap->GetWidth() * g_pMap->GetTileSize();
			float mapPixelHeight = g_pMap->GetHeight() * g_pMap->GetTileSize();
=======
 
			for (int i = 0; i < ENEMY_MAX; i++)
			{
				g_pEnemy[i] = new Enemy();
				if (FAILED(g_pEnemy[i]->Init(g_pMap))) {
					PostQuitMessage(0);
				}
			}
>>>>>>> 7af28c146c241fe12d89092bfc43f3fb843f69b1
		}
	}

	hal::DebugText dt(Direct3D_GetDevice(), Direct3D_GetDeviceContext(),
		L"resource/texture/consolab_ascii_512.png",
		Direct3D_GetBackBufferWidth(), Direct3D_GetBackBufferHeight(),
		0.0f, 0.0f,
		0, 0,
		0.0f, 0.0f
	);

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	SystemTimer_Initialize();
	KeyLogger_Initialize();

	// ���Ԍv���p
	double exec_last_time = 0.0;
	double fps_last_time = 0.0;
	double current_time = 0.0;
	ULONG frame_count = 0;
	double fps = 0.0;
	
	exec_last_time = fps_last_time = SystemTimer_GetTime();
	// ���b�Z�[�W���[�v
	MSG msg;
	bool sceneInitialized = false;

	do {
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {	// �E�B���h�E���b�Z�[�W�����Ă�����
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		} else {	// �Q�[���̏���
			current_time = SystemTimer_GetTime();
			double elapsed_time = current_time - fps_last_time;	// �o�ߎ���
			if ((elapsed_time) >= 1.0) {
				fps = frame_count / elapsed_time;
				fps_last_time = current_time;
				frame_count = 0;	// �J�E���g���N���A
			}

			elapsed_time = current_time - exec_last_time;
			if (elapsed_time >= (1.0 / 60.0)) {	// 1/60�b���ƂɎ��s
				exec_last_time = current_time;	// ��������������ۑ�

				Direct3D_Clear();
				KeyLogger_Update();

<<<<<<< HEAD
				// �V�[���ύX�`�F�b�N
				if (g_ChangeRequest) {
					// ���݂̃V�[���̏I������
					switch (g_Scene) {
					case SCENE_TITLE:
						Title_Finalize();
						break;
					case SCENE_GAME_ZAKO:
						if (g_pPlayer) {
							g_pPlayer->Uninit();
							delete g_pPlayer;
							g_pPlayer = nullptr;
						}
						if (g_pMap) {
							g_pMap->Uninit();
							delete g_pMap;
							g_pMap = nullptr;
						}
						if (g_pCamera) {
							g_pCamera->Uninit();
							delete g_pCamera;
							g_pCamera = nullptr;
						}
						break;
					case SCENE_RESULT:
						Result_Finalize();
						break;
					}

					// �V�[���؂�ւ�
					Scene_ChangeScene();
					sceneInitialized = false;
					g_ChangeRequest = false;
				}

				// �V�����V�[���̏������i��x�����j
				if (!sceneInitialized) {
					switch (g_Scene) {
					case SCENE_TITLE:
						Title_Initialize();
						break;
					case SCENE_GAME_ZAKO:
						// �}�b�v�̏�����
						g_pMap = new Map(400, 30, 32.0f);
						if (FAILED(g_pMap->Init())) {
							PostQuitMessage(0);
						}

						// �v���C���[�̏�����
						g_pPlayer = new Player();
						if (FAILED(g_pPlayer->Init(g_pMap))) {
							PostQuitMessage(0);
						}

						// �J�����̏�����
						g_pCamera = new Camera();
						if (FAILED(g_pCamera->Init(1920, 1080))) {
							PostQuitMessage(0);
						}

						// �J�����Ƀv���C���[��ݒ�
						g_pCamera->SetTarget(g_pPlayer);
						
						// ���E������ݒ�
						g_pCamera->SetBoundsFromMap(g_pMap);

						break;
					case SCENE_RESULT:
						Result_Initialize();
						break;
					}
					sceneInitialized = true;
				}

				// ���݂̃V�[���̍X�V
				switch (g_Scene) {
				case SCENE_TITLE:
					Title_Update(elapsed_time);
					break;
				case SCENE_GAME_ZAKO:
					if (g_pMap) g_pMap->Update();
					if (g_pPlayer) g_pPlayer->Update(elapsed_time);
					if (g_pCamera) g_pCamera->Update(elapsed_time);

					if (KeyLogger_IsPressed(KK_F1)) {
						Scene_SetNextScene(SCENE_RESULT);
					}
					break;
				case SCENE_RESULT:
					Result_Update(elapsed_time);
					break;
=======
				g_pMap->Update();
				g_pPlayer->Update(elapsed_time);

				for (int i = 0; i < ENEMY_MAX; i++)
				{
					if (g_pEnemy[i])
					{
						if (g_pEnemy[i]->GetIsUse())
						{
							g_pEnemy[i]->Update(elapsed_time);
						}
					}
>>>>>>> 7af28c146c241fe12d89092bfc43f3fb843f69b1
				}
				
				BoxCollider::UpdateAllCollisions();

<<<<<<< HEAD
				// �`��
				switch (g_Scene) {
				case SCENE_TITLE:
					Title_Draw();
					break;
				case SCENE_GAME_ZAKO:
					if (g_pMap) g_pMap->Draw();
					if (g_pPlayer) g_pPlayer->Draw();
					break;
				case SCENE_RESULT:
					Result_Draw();
					break;
				}
=======
				g_pPlayer->Draw();
				g_pMap->Draw();

				for (int i = 0; i < ENEMY_MAX; i++)
				{
					if (g_pEnemy[i])
					{
						if (g_pEnemy[i]->GetIsUse())
						{
							g_pEnemy[i]->Draw();
						}
					}
				}

>>>>>>> 7af28c146c241fe12d89092bfc43f3fb843f69b1

#if defined(DEBUG) || defined(_DEBUG)
				std::stringstream ss;
				ss << "FPS: " << fps << std::endl;
				ss << "Scene: " << g_Scene << std::endl;  // ���݂̃V�[����\��

				if (g_pPlayer) {
					float px, py;
					g_pPlayer->GetPosition(px, py);
					ss << "Player: (" << (int)px << ", " << (int)py << ")" << std::endl;
				}

				if (g_pCamera) {
					float cx, cy;
					g_pCamera->GetPosition(cx, cy);
					ss << "Camera: (" << (int)cx << ", " << (int)cy << ")" << std::endl;

					if (g_pPlayer) {
						float px, py, drawX, drawY;
						g_pPlayer->GetPosition(px, py);
						g_pCamera->GetDrawPosition(px, py, drawX, drawY);
						ss << "Player Screen: (" << (int)drawX << ", " << (int)drawY << ")" << std::endl;
						ss << "Expected Center: (960, 540)" << std::endl;
					}
				}

				dt.SetText(ss.str().c_str());
				dt.Draw();
				dt.Clear();
#endif

				Direct3D_Present();

				frame_count++;
			}
		}
	} while (msg.message != WM_QUIT);

	if (g_pPlayer) {
		g_pPlayer->Uninit();
		delete g_pPlayer;
		g_pPlayer = nullptr;
	}
	if (g_pMap) {
		g_pMap->Uninit();
		delete g_pMap;
		g_pMap = nullptr;
	}
	if (g_pCamera) {
		g_pCamera->Uninit();
		delete g_pCamera;
		g_pCamera = nullptr;
	}
	Sprite_Finalize();
	Texture_Finalize();
	Shader_Finalize();
	Direct3D_Finalize();	// Direct3D�̏I������
	//Mouse_Finalize();

	return (int)msg.wParam;
}


// �E�B���h�E�v���V�[�W��

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_ACTIVATEAPP:
		Keyboard_ProcessMessage(message, wParam, lParam);
		Mouse_ProcessMessage(message, wParam, lParam);
		break;
	case WM_KEYDOWN:
		Keyboard_ProcessMessage(message, wParam, lParam);
		if (wParam == VK_ESCAPE) {
			SendMessage(hWnd, WM_CLOSE, 0, 0);	//WM_CLOSE���b�Z�[�W�̑��M
		}
		break;
	case WM_SYSKEYDOWN:
	case WM_KEYUP:
	case WM_SYSKEYUP:
		Keyboard_ProcessMessage(message, wParam, lParam);
		break;
    case WM_INPUT:
    case WM_MOUSEMOVE:
    case WM_LBUTTONDOWN:
    case WM_LBUTTONUP:
    case WM_RBUTTONDOWN:
    case WM_RBUTTONUP:
    case WM_MBUTTONDOWN:
    case WM_MBUTTONUP:
    case WM_MOUSEWHEEL:
    case WM_XBUTTONDOWN:
    case WM_XBUTTONUP:
    case WM_MOUSEHOVER:
		Mouse_ProcessMessage(message, wParam, lParam);
		break;
	case WM_CLOSE:	// �E�B���h�E����郁�b�Z�[�W
		if (MessageBox(hWnd, "�{���ɏI�����Ă�낵���ł����H",
			"�m�F", MB_OKCANCEL | MB_DEFBUTTON2) == IDOK) {
			DestroyWindow(hWnd);	// �w��̃E�B���h�E��WM�QDESTROY���b�Z�[�W�𑗂�
		}
		break;
		//����������ƃv���O�������I���ł��Ȃ�
	case WM_DESTROY: // �E�B���h�E�̔j�����b�Z�[�W
		PostQuitMessage(0); // WM_QUITX���b�Z�[�W�̑��M
		break;
	default:
		// �ʏ�̃��b�Z�[�W�����͂��̊֐��ɔC����
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

void Scene_SetNextScene(Scene scene) {
	g_NextScene = scene;
	g_ChangeRequest = true;
}

void Scene_ChangeScene() {
	// �V�[���̐؂�ւ�
 	g_Scene = g_NextScene;
}

Scene Scene_GetCurrentScene() {
	return g_Scene;
}