#include "sound.h"

static std::thread bgmThread;
static std::atomic<bool> isPlayingBGM{ false };
static std::atomic<bool> stopBGMThread{ false };
static int bgmVolume = 100;
static std::atomic<int> effectCounter{ 0 };

// BGM�֘A
void PlayBGMThread(std::wstring fileName) {
    std::wstring command = L"open \"" + fileName + L"\" type mpegvideo alias bgm";
    MMRESULT res = mciSendStringW(command.c_str(), NULL, 0, NULL);
    if (res != 0) {
        isPlayingBGM = false;
        return;
    }

    command = L"play bgm repeat";
    mciSendStringW(command.c_str(), NULL, 0, NULL);

    // ���ʐݒ�
    command = L"setaudio bgm volume to " + std::to_wstring((bgmVolume * 1000) / 100);
    mciSendStringW(command.c_str(), NULL, 0, NULL);

    while (!stopBGMThread) {
        std::this_thread::sleep_for(std::chrono::milliseconds(50)); // ���Z���Ԋu�ŏI���`�F�b�N
    }

    // BGM��~
    mciSendStringW(L"stop bgm", NULL, 0, NULL);
    mciSendStringW(L"close bgm", NULL, 0, NULL);
    isPlayingBGM = false;
}

void PlayBGM(const std::wstring& fileName, int volume) {
    if (isPlayingBGM) return;

    stopBGMThread = false;
    bgmVolume = volume;
    isPlayingBGM = true;
    bgmThread = std::thread(PlayBGMThread, fileName);
}

void StopBGM() {
    if (isPlayingBGM) {
        stopBGMThread = true;

        // MCI���ɋ�����~
        mciSendStringW(L"stop bgm", NULL, 0, NULL);
        mciSendStringW(L"close bgm", NULL, 0, NULL);

        // �X���b�h�̏I����҂i�Z���ԁj
        if (bgmThread.joinable()) {
            bgmThread.join();
        }
        isPlayingBGM = false;
    }
}

void SetBGMVolume(int volume) {
    bgmVolume = volume;
    if (isPlayingBGM) {
        std::wstring command = L"setaudio bgm volume to " + std::to_wstring((bgmVolume * 1000) / 100);
        mciSendStringW(command.c_str(), NULL, 0, NULL);
    }
}

// ���ʉ��֘A
void PlaySingleEffect(std::wstring fileName, int volume, int id) {
    std::wstring alias = L"effect" + std::to_wstring(id);

    // WAV�t�@�C�����J��
    std::wstring command = L"open \"" + fileName + L"\" type waveaudio alias " + alias;
    MMRESULT res = mciSendStringW(command.c_str(), NULL, 0, NULL);
    if (res != 0) {
        return;
    }

    // ���ʐݒ�
    command = L"setaudio " + alias + L" volume to " + std::to_wstring((volume * 1000) / 100);
    mciSendStringW(command.c_str(), NULL, 0, NULL);

    // �Đ�
    command = L"play " + alias + L" from 0";
    mciSendStringW(command.c_str(), NULL, 0, NULL);

    // �V���v���ɌŒ莞�ԑҋ@�i�����S�j
    std::this_thread::sleep_for(std::chrono::milliseconds(1500)); // 1.5�b�Œ�

    // ��~�E�N���[�Y
    mciSendStringW((L"stop " + alias).c_str(), NULL, 0, NULL);
    mciSendStringW((L"close " + alias).c_str(), NULL, 0, NULL);
}

void PlaySoundEffect(const std::wstring& fileName, int volume) {
    int id = effectCounter++;
    std::thread(PlaySingleEffect, fileName, volume, id).detach();
}

void ShutdownSoundSystem() {
    // BGM���~�iMCI���Ɏ~�߂Ă���X���b�hjoin�j
    StopBGM();

    // �c���Ă�����ʉ��X���b�h�̏I����҂�
    std::this_thread::sleep_for(std::chrono::milliseconds(2000)); // 2�b�ҋ@

    // �O�̂��ߑSMCI�f�o�C�X���N���A
    mciSendStringW(L"close all", NULL, 0, NULL);
}