///*****************************************************************//**
// * \file   main.cpp
// * \brief
// *
// * \author 菊池凌斗
// * \date   2025/4/25
// *********************************************************************/
//#include <SDKDDKVer.h>
//#define WIN32_LEAN_AND_MEAN
//#include <Windows.h>
//#include <algorithm>
//#include "direct3d.h"
//#include "shader.h"
//#include "texture.h"
//#include "sprite.h"
//#include "sprite_anim.h"
//#include "system_timer.h"
//#include "debug_text.h"
//#include "meteor.h"
//#include <sstream>
//#include <ctime>
//
// //ウィンドウ情報
//static constexpr char WINDOW_CLASS[] = "GameWindow";
//static constexpr char TITLE[] = "ウィンドウ表示";
//
////ウィンドウプロシージャ プロトタイプ宣言
//LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
//
//// メイン
//int APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
//{
//	(void)CoInitializeEx(nullptr, COINIT_MULTITHREADED);
//
//	/* ウィンドウクラスの登録*/
//	WNDCLASSEX wcex{};
//
//	wcex.cbSize = sizeof(WNDCLASSEX);
//	wcex.lpfnWndProc = WndProc;
//	wcex.hInstance = hInstance;
//	wcex.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
//	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
//	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
//	wcex.lpszMenuName = nullptr;
//	wcex.lpszClassName = WINDOW_CLASS;
//	wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);
//
//	RegisterClassEx(&wcex);
//
//	// クライアント領域のサイズを持った（左からleft, top, right, bottom）
//	RECT window_rect = { 0, 0, 1280, 720 };
//	// ウィンドウのスタイル
//	DWORD window_style = WS_OVERLAPPEDWINDOW & ~(WS_THICKFRAME | WS_MAXIMIZEBOX);
//	// 指定したクライアント領域を確保するために新たな矩形座標を計算
//	AdjustWindowRect(&window_rect, window_style, FALSE);
//
//	// ウィンドウの幅と高さを算出
//	int window_width = window_rect.right - window_rect.left;
//	int window_height = window_rect.bottom - window_rect.top;
//
//	// プライマリモニターの画像解像度取得
//	int desktop_width = GetSystemMetrics(SM_CXSCREEN);
//	int desktop_height = GetSystemMetrics(SM_CYSCREEN);
//
//	// デスクトップの真ん中にウィンドウが生成されるように座標を計算
//	int window_x = std::max((desktop_width - window_width) / 2, 0);
//	int window_y = std::max((desktop_height - window_height) / 2, 0);
//
//	/* メインウィンドウの作成 */
//	HWND hwnd = CreateWindow(
//		WINDOW_CLASS,
//		TITLE,
//		window_style,
//		window_x,
//		window_y,
//		window_width,
//		window_height,
//		nullptr,
//		nullptr,
//		hInstance,
//		nullptr
//	);
//
//	/* 各種初期化 */
//	if (!Direct3D_Initialize(hwnd)) {
//		PostQuitMessage(0);
//	} else {
//		if (!Shader_Initialize(Direct3D_GetDevice(), Direct3D_GetDeviceContext())) {
//			PostQuitMessage(0);
//		} else {
//			Texture_Initialize(Direct3D_GetDevice(), Direct3D_GetDeviceContext());
//			Sprite_Initialize(Direct3D_GetDevice(), Direct3D_GetDeviceContext());
//		}
//	}
//
//	hal::DebugText dt(Direct3D_GetDevice(), Direct3D_GetDeviceContext(),
//		L"resource/texture/consolab_ascii_512.png",
//		Direct3D_GetBackBufferWidth(), Direct3D_GetBackBufferHeight(),
//		0.0f, 0.0f,
//		0, 0,
//		0.0f, 0.0f
//	);
//
//	int texid_running = Texture_Load(L"resource/texture/runningman001.png");
//	int texid_haikei = Texture_Load(L"resource/texture/haikei.jpg");
//	int texid_meteor = Texture_Load(L"resource/texture/meteor.png");
//	int texid_explosion = Texture_Load(L"resource/texture/explosion.png");
//	int texid_woman = Texture_Load(L"resource/texture/itosinohito.png");
//	int texid_couple = Texture_Load(L"resource/texture/sine.png");
//
//	AnimPattern running_anim(texid_running, 10, 5, 0.1, { 0,0 }, { 140,200 });
//	AnimPatternPlayer appRunning(&running_anim);
//
//	AnimPattern explosion_anim(texid_explosion, 15, 5, 0.1, { 0,0 }, { 128,128 });
//	AnimPatternPlayer appExplosion(&explosion_anim);
//	AnimPatternPlayer appBigExplosion(&explosion_anim);  // ← 追加：大爆発用アニメ
//
//	ShowWindow(hwnd, nCmdShow);
//	UpdateWindow(hwnd);
//
//	SystemTimer_Initialize();
//
//	// 時間計測用
//	double exec_last_time = 0.0;
//	double fps_last_time = 0.0;
//	double current_time = 0.0;
//	ULONG frame_count = 0;
//	double fps = 0.0;
//
//	std::vector<Meteor> meteor;
//
//	float scroll_x = 0.0f;
//	const float scroll_speed = 200.0f;
//
//	// 背景画像の幅取得
//	float haikei_width = (float)Texture_GetWidth(texid_haikei);
//	float haikei_height = (float)Texture_GetHeight(texid_haikei);
//
//	// 女の子関連の変数
//	double total_elapsed_time = 0.0;
//	bool woman_appeared = false;
//	float woman_x = -200.0f;
//	float woman_y = 400.0f;
//	float woman_target_x = 900.0f;  // ← 修正：目標位置を明確化
//	const float woman_speed = 200.0f;
//	bool woman_stopped = false;     // ← 追加：停止フラグ
//	bool couple_formed = false;
//
//	// 大爆発関連の変数を追加
//	bool big_explosion_started = false;
//	double big_explosion_time = 0.0;
//	const double BIG_EXPLOSION_DURATION = 3.0;
//	bool game_ending = false;
//
//	exec_last_time = fps_last_time = SystemTimer_GetTime();
//	static double meteor_spawn_timer = 0.0;
//	srand((unsigned int)time(nullptr));
//
//	PlayBGM(L"resource/audio/BGM.wav", 80);
//
//	// メッセージループ
//	MSG msg;
//	do {
//		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
//			TranslateMessage(&msg);
//			DispatchMessage(&msg);
//		} else {
//			current_time = SystemTimer_GetTime();
//			double elapsed_time = current_time - fps_last_time;
//			if ((elapsed_time) >= 1.0) {
//				fps = frame_count / elapsed_time;
//				fps_last_time = current_time;
//				frame_count = 0;
//			}
//
//			elapsed_time = current_time - exec_last_time;
//			if (elapsed_time >= (1.0 / 60.0)) {
//				exec_last_time = current_time;
//
//				// 総経過時間を更新
//				total_elapsed_time += elapsed_time;
//
//				// 18秒経過で大爆発開始
//				if (total_elapsed_time >= 19.0 && !big_explosion_started) {
//					big_explosion_started = true;
//					big_explosion_time = 0.0;
//
//					// BGMを停止して爆発音を再生
//					StopBGM();
//					PlaySoundEffect(L"resource/audio/explosion.wav", 100);
//				}
//
//				// 大爆発中の処理
//				if (big_explosion_started) {
//					big_explosion_time += elapsed_time;
//					appBigExplosion.Update(elapsed_time);
//
//					// 爆発終了でゲーム終了
//					if (big_explosion_time >= BIG_EXPLOSION_DURATION) {
//						game_ending = true;
//						PostQuitMessage(0);
//					}
//				}
//
//				// 15秒経過で女の子登場
//				if (total_elapsed_time >= 15.0 && !woman_appeared && !big_explosion_started) {
//					woman_appeared = true;
//					woman_x = -200.0f;
//				}
//
//				Direct3D_Clear();
//
//				// 大爆発中は特別な描画
//				if (big_explosion_started) {
//					// 画面全体を爆発エフェクトで覆う
//					appBigExplosion.Draw(0, 0, 1280, 720);
//
//					// 追加の爆発エフェクト（複数箇所）
//					appBigExplosion.Draw(200, 100, 400, 400);
//					appBigExplosion.Draw(600, 200, 500, 500);
//					appBigExplosion.Draw(100, 300, 300, 300);
//
//					// フラッシュ効果
//					if ((int)(big_explosion_time * 20) % 2 == 0) {
//						// 白いフラッシュ（背景テクスチャを小さく引き延ばして白くする）
//						Sprite_Draw(texid_haikei, 0, 0, 1280, 720, 0, 0, 1, 1);
//					}
//				} else {
//					// 通常時の描画
//
//#pragma region haikei
//					scroll_x -= scroll_speed * (float)elapsed_time;
//					if (scroll_x <= -(float)window_width * 2)
//						scroll_x += (float)window_width * 2;
//
//					// 背景
//					Sprite_Draw(
//						texid_haikei,
//						-scroll_x + -(window_width * 2), 0.0f,
//						window_width, window_height,
//						0, 0, haikei_width, haikei_height
//					);
//
//					Sprite_Draw(
//						texid_haikei,
//						-scroll_x + -window_width, 0.0f,
//						window_width, window_height,
//						haikei_width, 0, -haikei_width, haikei_height
//					);
//
//					Sprite_Draw(
//						texid_haikei,
//						-scroll_x, 0.0f,
//						window_width, window_height,
//						0, 0, haikei_width, haikei_height
//					);
//#pragma endregion
//
//					// 隕石の処理
//					meteor_spawn_timer += elapsed_time;
//
//					for (auto& m : meteor) {
//						m.Update(elapsed_time);
//					}
//
//					meteor.erase(
//						std::remove_if(meteor.begin(), meteor.end(),
//							[](const Meteor& m) { return m.IsFinished(); }),
//						meteor.end()
//					);
//
//					if (meteor_spawn_timer > 1.0) {
//						meteor_spawn_timer = 0.0;
//
//						float startX = 100 + (rand() % (1280 - 200));
//						float startY = -50;
//						float vx = -450.0f - rand() % 10;
//						float vy = 450.0f + rand() % 10;
//
//						meteor.emplace_back(startX, startY, vx, vy, texid_meteor, &explosion_anim);
//					}
//
//					for (auto& m : meteor) {
//						m.Draw();
//					}
//
//					// 走っている人の描画（カップル形成前のみ）
//					if (!couple_formed) {
//						appRunning.Update(elapsed_time);
//						appRunning.Draw(1000.0f, 400.0f, 140.0f, 200.0f);
//					}
//
//					// 女の子の更新と描画
//					if (woman_appeared) {
//						// 女の子の移動処理（修正）
//						if (!woman_stopped && woman_x < woman_target_x) {
//							woman_x += woman_speed * (float)elapsed_time;
//
//							if (woman_x >= woman_target_x) {
//								woman_x = woman_target_x;
//								woman_stopped = true;
//								couple_formed = true;
//							}
//						}
//
//						// 描画処理
//						if (couple_formed) {
//							// カップルのテクスチャを描画
//							Sprite_Draw(texid_couple, woman_x, woman_y, 140, 200, 0, 0,
//								Texture_GetWidth(texid_couple), Texture_GetHeight(texid_couple));
//						} else {
//							// 女の子単体を描画
//							Sprite_Draw(texid_woman, woman_x, woman_y, 140, 200, 0, 0,
//								Texture_GetWidth(texid_woman), Texture_GetHeight(texid_woman));
//						}
//					}
//				}
//
//#if defined(DEBUG) || defined(_DEBUG)
//				std::stringstream ss;
//				ss << "fps:" << fps << std::endl;
//				dt.SetText(ss.str().c_str());
//
//				dt.Draw();
//				dt.Clear();
//#endif
//
//				Direct3D_Present();
//
//				frame_count++;
//			}
//		}
//	} while (msg.message != WM_QUIT);
//
//	ShutdownSoundSystem();
//	Sprite_Finalize();
//	Texture_Finalize();
//	Shader_Finalize();
//	Direct3D_Finalize();
//
//	return (int)msg.wParam;
//}
//
//// ウィンドウプロシージャ
//LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
//{
//	switch (message) {
//	case WM_KEYDOWN:
//		if (wParam == VK_ESCAPE) {
//			SendMessage(hWnd, WM_CLOSE, 0, 0);	//WM_CLOSEメッセージの送信
//		}
//		break;
//	case WM_CLOSE:	// ウィンドウを閉じるメッセージ
//		if (MessageBox(hWnd, "本当に終了してよろしいですか？",
//			"確認", MB_OKCANCEL | MB_DEFBUTTON2) == IDOK) {
//			DestroyWindow(hWnd);	// 指定のウィンドウにWM＿DESTROYメッセージを送る
//		}
//		break;
//		//↓これ消すとプログラムを終了できない
//	case WM_DESTROY: // ウィンドウの破棄メッセージ
//		PostQuitMessage(0); // WM_QUITXメッセージの送信
//		break;
//	default:
//		// 通常のメッセージ処理はこの関数に任せる
//		return DefWindowProc(hWnd, message, wParam, lParam);
//	}
//	return 0;
//}