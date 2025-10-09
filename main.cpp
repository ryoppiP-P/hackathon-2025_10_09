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

#include <sstream>
#include <DirectXMath.h>
using namespace DirectX;

//�E�B���h�E���
static constexpr char WINDOW_CLASS[] = "GameWindow"; // x151957
static constexpr char TITLE[] = "DirectX2D beta";// �^�C�g���o�[�̃e�L�X�g

Map* g_pMap = nullptr;
Player* g_pPlayer = nullptr;

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
			g_pMap = new Map(40, 20, 32.0f);
			if (FAILED(g_pMap->Init())) {
				PostQuitMessage(0);
			}

			// �v���C���[�̏�����
			g_pPlayer = new Player();
			if (FAILED(g_pPlayer->Init(g_pMap))) {
				PostQuitMessage(0);
			}
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
	//Mouse_Initialize(hwnd);

	//Mouse_SetVisible(false);

	// ���Ԍv���p
	double exec_last_time = 0.0;
	double fps_last_time = 0.0;
	double current_time = 0.0;
	ULONG frame_count = 0;
	double fps = 0.0;
	
	exec_last_time = fps_last_time = SystemTimer_GetTime();
	// ���b�Z�[�W���[�v
	MSG msg;
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
				
				//KeyLogger_Update();
				//Mouse_State ms{};
				//Mouse_GetState(&ms);

				Direct3D_Clear();

				g_pMap->Update();
				g_pPlayer->Update(elapsed_time);
				
				BoxCollider::UpdateAllCollisions();

				g_pPlayer->Draw();
				g_pMap->Draw();


#if defined(DEBUG) || defined(_DEBUG)
				std::stringstream ss;
				ss << "fps:" << fps << std::endl;
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
