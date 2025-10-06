///*****************************************************************//**
// * \file   main.cpp
// * \brief
// *
// * \author �e�r���l
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
// //�E�B���h�E���
//static constexpr char WINDOW_CLASS[] = "GameWindow";
//static constexpr char TITLE[] = "�E�B���h�E�\��";
//
////�E�B���h�E�v���V�[�W�� �v���g�^�C�v�錾
//LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
//
//// ���C��
//int APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
//{
//	(void)CoInitializeEx(nullptr, COINIT_MULTITHREADED);
//
//	/* �E�B���h�E�N���X�̓o�^*/
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
//	// �N���C�A���g�̈�̃T�C�Y���������i������left, top, right, bottom�j
//	RECT window_rect = { 0, 0, 1280, 720 };
//	// �E�B���h�E�̃X�^�C��
//	DWORD window_style = WS_OVERLAPPEDWINDOW & ~(WS_THICKFRAME | WS_MAXIMIZEBOX);
//	// �w�肵���N���C�A���g�̈���m�ۂ��邽�߂ɐV���ȋ�`���W���v�Z
//	AdjustWindowRect(&window_rect, window_style, FALSE);
//
//	// �E�B���h�E�̕��ƍ������Z�o
//	int window_width = window_rect.right - window_rect.left;
//	int window_height = window_rect.bottom - window_rect.top;
//
//	// �v���C�}�����j�^�[�̉摜�𑜓x�擾
//	int desktop_width = GetSystemMetrics(SM_CXSCREEN);
//	int desktop_height = GetSystemMetrics(SM_CYSCREEN);
//
//	// �f�X�N�g�b�v�̐^�񒆂ɃE�B���h�E�����������悤�ɍ��W���v�Z
//	int window_x = std::max((desktop_width - window_width) / 2, 0);
//	int window_y = std::max((desktop_height - window_height) / 2, 0);
//
//	/* ���C���E�B���h�E�̍쐬 */
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
//	/* �e�평���� */
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
//	AnimPatternPlayer appBigExplosion(&explosion_anim);  // �� �ǉ��F�唚���p�A�j��
//
//	ShowWindow(hwnd, nCmdShow);
//	UpdateWindow(hwnd);
//
//	SystemTimer_Initialize();
//
//	// ���Ԍv���p
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
//	// �w�i�摜�̕��擾
//	float haikei_width = (float)Texture_GetWidth(texid_haikei);
//	float haikei_height = (float)Texture_GetHeight(texid_haikei);
//
//	// ���̎q�֘A�̕ϐ�
//	double total_elapsed_time = 0.0;
//	bool woman_appeared = false;
//	float woman_x = -200.0f;
//	float woman_y = 400.0f;
//	float woman_target_x = 900.0f;  // �� �C���F�ڕW�ʒu�𖾊m��
//	const float woman_speed = 200.0f;
//	bool woman_stopped = false;     // �� �ǉ��F��~�t���O
//	bool couple_formed = false;
//
//	// �唚���֘A�̕ϐ���ǉ�
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
//	// ���b�Z�[�W���[�v
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
//				// ���o�ߎ��Ԃ��X�V
//				total_elapsed_time += elapsed_time;
//
//				// 18�b�o�߂ő唚���J�n
//				if (total_elapsed_time >= 19.0 && !big_explosion_started) {
//					big_explosion_started = true;
//					big_explosion_time = 0.0;
//
//					// BGM���~���Ĕ��������Đ�
//					StopBGM();
//					PlaySoundEffect(L"resource/audio/explosion.wav", 100);
//				}
//
//				// �唚�����̏���
//				if (big_explosion_started) {
//					big_explosion_time += elapsed_time;
//					appBigExplosion.Update(elapsed_time);
//
//					// �����I���ŃQ�[���I��
//					if (big_explosion_time >= BIG_EXPLOSION_DURATION) {
//						game_ending = true;
//						PostQuitMessage(0);
//					}
//				}
//
//				// 15�b�o�߂ŏ��̎q�o��
//				if (total_elapsed_time >= 15.0 && !woman_appeared && !big_explosion_started) {
//					woman_appeared = true;
//					woman_x = -200.0f;
//				}
//
//				Direct3D_Clear();
//
//				// �唚�����͓��ʂȕ`��
//				if (big_explosion_started) {
//					// ��ʑS�̂𔚔��G�t�F�N�g�ŕ���
//					appBigExplosion.Draw(0, 0, 1280, 720);
//
//					// �ǉ��̔����G�t�F�N�g�i�����ӏ��j
//					appBigExplosion.Draw(200, 100, 400, 400);
//					appBigExplosion.Draw(600, 200, 500, 500);
//					appBigExplosion.Draw(100, 300, 300, 300);
//
//					// �t���b�V������
//					if ((int)(big_explosion_time * 20) % 2 == 0) {
//						// �����t���b�V���i�w�i�e�N�X�`�����������������΂��Ĕ�������j
//						Sprite_Draw(texid_haikei, 0, 0, 1280, 720, 0, 0, 1, 1);
//					}
//				} else {
//					// �ʏ펞�̕`��
//
//#pragma region haikei
//					scroll_x -= scroll_speed * (float)elapsed_time;
//					if (scroll_x <= -(float)window_width * 2)
//						scroll_x += (float)window_width * 2;
//
//					// �w�i
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
//					// 覐΂̏���
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
//					// �����Ă���l�̕`��i�J�b�v���`���O�̂݁j
//					if (!couple_formed) {
//						appRunning.Update(elapsed_time);
//						appRunning.Draw(1000.0f, 400.0f, 140.0f, 200.0f);
//					}
//
//					// ���̎q�̍X�V�ƕ`��
//					if (woman_appeared) {
//						// ���̎q�̈ړ������i�C���j
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
//						// �`�揈��
//						if (couple_formed) {
//							// �J�b�v���̃e�N�X�`����`��
//							Sprite_Draw(texid_couple, woman_x, woman_y, 140, 200, 0, 0,
//								Texture_GetWidth(texid_couple), Texture_GetHeight(texid_couple));
//						} else {
//							// ���̎q�P�̂�`��
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
//// �E�B���h�E�v���V�[�W��
//LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
//{
//	switch (message) {
//	case WM_KEYDOWN:
//		if (wParam == VK_ESCAPE) {
//			SendMessage(hWnd, WM_CLOSE, 0, 0);	//WM_CLOSE���b�Z�[�W�̑��M
//		}
//		break;
//	case WM_CLOSE:	// �E�B���h�E����郁�b�Z�[�W
//		if (MessageBox(hWnd, "�{���ɏI�����Ă�낵���ł����H",
//			"�m�F", MB_OKCANCEL | MB_DEFBUTTON2) == IDOK) {
//			DestroyWindow(hWnd);	// �w��̃E�B���h�E��WM�QDESTROY���b�Z�[�W�𑗂�
//		}
//		break;
//		//����������ƃv���O�������I���ł��Ȃ�
//	case WM_DESTROY: // �E�B���h�E�̔j�����b�Z�[�W
//		PostQuitMessage(0); // WM_QUITX���b�Z�[�W�̑��M
//		break;
//	default:
//		// �ʏ�̃��b�Z�[�W�����͂��̊֐��ɔC����
//		return DefWindowProc(hWnd, message, wParam, lParam);
//	}
//	return 0;
//}