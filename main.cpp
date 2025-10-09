/*****************************************************************//**
 * \file   main.cpp
 * \brief  
 * 
 * \author 菊池凌斗
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

//ウィンドウ情報
static constexpr char WINDOW_CLASS[] = "GameWindow"; // x151957
static constexpr char TITLE[] = "DirectX2D beta";// タイトルバーのテキスト

Map* g_pMap = nullptr;
Player* g_pPlayer = nullptr;

//ウィンドウプロシージャ プロトタイプ宣言
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// メイン
int APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	(void)CoInitializeEx(nullptr, COINIT_MULTITHREADED);

	/* ウィンドウクラスの登録*/
	WNDCLASSEX wcex{};

	wcex.cbSize = sizeof(WNDCLASSEX); 
	wcex.lpfnWndProc = WndProc;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = nullptr; //メニューは作らない
	wcex.lpszClassName = WINDOW_CLASS;
	wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);

	RegisterClassEx(&wcex);

	// クライアント領域のサイズを持った（左からleft, top, right, bottom）
	RECT window_rect = { 0, 0, 1920, 1080 };
	// ウィンドウのスタイル - フルスクリーン用に変更
	DWORD window_style =/* WS_POPUP;*/ WS_OVERLAPPEDWINDOW & ~(WS_THICKFRAME | WS_MAXIMIZEBOX);
	// 指定したクライアント領域を確保するために新たな矩形座標を計算
	AdjustWindowRect(&window_rect, window_style, FALSE);

	// プライマリモニターの画像解像度取得
	int desktop_width = GetSystemMetrics(SM_CXSCREEN);
	int desktop_height = GetSystemMetrics(SM_CYSCREEN);

	// フルスクリーン用の設定
	int window_width = desktop_width;
	int window_height = desktop_height;
	int window_x = 0;
	int window_y = 0;


	/* メインウィンドウの作成 */
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

	/* 各種初期化 */
	if (!Direct3D_Initialize(hwnd)) {
		PostQuitMessage(0);
	} else {
		if (!Shader_Initialize(Direct3D_GetDevice(), Direct3D_GetDeviceContext())) {
			PostQuitMessage(0);
		} else {
			Texture_Initialize(Direct3D_GetDevice(), Direct3D_GetDeviceContext());
			Sprite_Initialize(Direct3D_GetDevice(), Direct3D_GetDeviceContext());
			// マップの初期化
			g_pMap = new Map(40, 20, 32.0f);
			if (FAILED(g_pMap->Init())) {
				PostQuitMessage(0);
			}

			// プレイヤーの初期化
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

	// 時間計測用
	double exec_last_time = 0.0;
	double fps_last_time = 0.0;
	double current_time = 0.0;
	ULONG frame_count = 0;
	double fps = 0.0;
	
	exec_last_time = fps_last_time = SystemTimer_GetTime();
	// メッセージループ
	MSG msg;
	do {
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {	// ウィンドウメッセージが来ていたら
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		} else {	// ゲームの処理
			current_time = SystemTimer_GetTime();
			double elapsed_time = current_time - fps_last_time;	// 経過時間
			if ((elapsed_time) >= 1.0) {
				fps = frame_count / elapsed_time;
				fps_last_time = current_time;
				frame_count = 0;	// カウントをクリア
			}

			elapsed_time = current_time - exec_last_time;
			if (elapsed_time >= (1.0 / 60.0)) {	// 1/60秒ごとに実行
				exec_last_time = current_time;	// 処理した時刻を保存
				
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
	Direct3D_Finalize();	// Direct3Dの終了処理
	//Mouse_Finalize();

	return (int)msg.wParam;
}


// ウィンドウプロシージャ

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
			SendMessage(hWnd, WM_CLOSE, 0, 0);	//WM_CLOSEメッセージの送信
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
	case WM_CLOSE:	// ウィンドウを閉じるメッセージ
		if (MessageBox(hWnd, "本当に終了してよろしいですか？",
			"確認", MB_OKCANCEL | MB_DEFBUTTON2) == IDOK) {
			DestroyWindow(hWnd);	// 指定のウィンドウにWM＿DESTROYメッセージを送る
		}
		break;
		//↓これ消すとプログラムを終了できない
	case WM_DESTROY: // ウィンドウの破棄メッセージ
		PostQuitMessage(0); // WM_QUITXメッセージの送信
		break;
	default:
		// 通常のメッセージ処理はこの関数に任せる
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
