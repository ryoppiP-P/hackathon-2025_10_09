/*****************************************************************//**
 * \file   main.cpp
 * \brief  �}���I��2D�v���b�g�t�H�[�}�[
 *
 * \author �e�r���l
 * \date   2025/10/6
 *********************************************************************/
#include <SDKDDKVer.h>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "direct3d.h"
#include "shader.h"
#include "texture.h"
#include "sprite.h"
#include "system_timer.h"
#include "Audio.h"

// Engine includes
#include "Engine/SceneManager.h"
#include "Engine/Time.h"
#include "Engine/Input.h"
#include "Engine/CollisionManager.h"
#include "GameScenes/MarioScene.h"

//�E�B���h�E���
static constexpr char WINDOW_CLASS[] = "MarioGame";
static constexpr char TITLE[] = "Super Mario Bros. Style Game";

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
    wcex.lpszMenuName = nullptr;
    wcex.lpszClassName = WINDOW_CLASS;
    wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);

    RegisterClassEx(&wcex);

    // �E�B���h�E�쐬
    RECT window_rect = { 0, 0, 1280, 720 };
    DWORD window_style = WS_OVERLAPPEDWINDOW & ~(WS_THICKFRAME | WS_MAXIMIZEBOX);
    AdjustWindowRect(&window_rect, window_style, FALSE);

    int window_width = window_rect.right - window_rect.left;
    int window_height = window_rect.bottom - window_rect.top;

    int desktop_width = GetSystemMetrics(SM_CXSCREEN);
    int desktop_height = GetSystemMetrics(SM_CYSCREEN);

    int window_x = std::max((desktop_width - window_width) / 2, 0);
    int window_y = std::max((desktop_height - window_height) / 2, 0);

    HWND hwnd = CreateWindow(
        WINDOW_CLASS, TITLE, window_style,
        window_x, window_y, window_width, window_height,
        nullptr, nullptr, hInstance, nullptr
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
            InitAudio();
        }
    }

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    SystemTimer_Initialize();

    // �V�[���}�l�[�W���[�̏�����
    auto& sceneManager = Engine::SceneManager::Instance();
    sceneManager.LoadScene(std::make_unique<MarioScene>());

    // BGM�Đ�
    int bgmId = LoadAudio("resource/audio/mario_bgm.wav");
    if (bgmId >= 0) {
        PlayAudio(bgmId, true);
    }

    // ���C�����[�v
    double exec_last_time = SystemTimer_GetTime();
    MSG msg;
    
    do {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        } else {
            double current_time = SystemTimer_GetTime();
            double deltaTime = current_time - exec_last_time;
            
            if (deltaTime >= (1.0 / 60.0)) {
                exec_last_time = current_time;

                // �G���W���X�V
                Engine::Time::Update(deltaTime);
                
                // �V�[���X�V
                sceneManager.Update(deltaTime);

                // �`��
                Direct3D_Clear();
                sceneManager.Draw();
                Direct3D_Present();
            }
        }
    } while (msg.message != WM_QUIT);

    // �I������
    UninitAudio();
    Sprite_Finalize();
    Texture_Finalize();
    Shader_Finalize();
    Direct3D_Finalize();

    return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message) {
    case WM_KEYDOWN:
        if (wParam == VK_ESCAPE) {
            SendMessage(hWnd, WM_CLOSE, 0, 0);
        }
        break;
    case WM_CLOSE:
        if (MessageBox(hWnd, "�Q�[�����I�����܂����H",
            "�m�F", MB_OKCANCEL | MB_DEFBUTTON2) == IDOK) {
            DestroyWindow(hWnd);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}