#include "sound.h"

static std::thread bgmThread;
static std::atomic<bool> isPlayingBGM{ false };
static std::atomic<bool> stopBGMThread{ false };
static int bgmVolume = 100;
static std::atomic<int> effectCounter{ 0 };

// BGM関連
void PlayBGMThread(std::wstring fileName) {
    std::wstring command = L"open \"" + fileName + L"\" type mpegvideo alias bgm";
    MMRESULT res = mciSendStringW(command.c_str(), NULL, 0, NULL);
    if (res != 0) {
        isPlayingBGM = false;
        return;
    }

    command = L"play bgm repeat";
    mciSendStringW(command.c_str(), NULL, 0, NULL);

    // 音量設定
    command = L"setaudio bgm volume to " + std::to_wstring((bgmVolume * 1000) / 100);
    mciSendStringW(command.c_str(), NULL, 0, NULL);

    while (!stopBGMThread) {
        std::this_thread::sleep_for(std::chrono::milliseconds(50)); // より短い間隔で終了チェック
    }

    // BGM停止
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

        // MCIを先に強制停止
        mciSendStringW(L"stop bgm", NULL, 0, NULL);
        mciSendStringW(L"close bgm", NULL, 0, NULL);

        // スレッドの終了を待つ（短時間）
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

// 効果音関連
void PlaySingleEffect(std::wstring fileName, int volume, int id) {
    std::wstring alias = L"effect" + std::to_wstring(id);

    // WAVファイルを開く
    std::wstring command = L"open \"" + fileName + L"\" type waveaudio alias " + alias;
    MMRESULT res = mciSendStringW(command.c_str(), NULL, 0, NULL);
    if (res != 0) {
        return;
    }

    // 音量設定
    command = L"setaudio " + alias + L" volume to " + std::to_wstring((volume * 1000) / 100);
    mciSendStringW(command.c_str(), NULL, 0, NULL);

    // 再生
    command = L"play " + alias + L" from 0";
    mciSendStringW(command.c_str(), NULL, 0, NULL);

    // シンプルに固定時間待機（より安全）
    std::this_thread::sleep_for(std::chrono::milliseconds(1500)); // 1.5秒固定

    // 停止・クローズ
    mciSendStringW((L"stop " + alias).c_str(), NULL, 0, NULL);
    mciSendStringW((L"close " + alias).c_str(), NULL, 0, NULL);
}

void PlaySoundEffect(const std::wstring& fileName, int volume) {
    int id = effectCounter++;
    std::thread(PlaySingleEffect, fileName, volume, id).detach();
}

void ShutdownSoundSystem() {
    // BGMを停止（MCIを先に止めてからスレッドjoin）
    StopBGM();

    // 残っている効果音スレッドの終了を待つ
    std::this_thread::sleep_for(std::chrono::milliseconds(2000)); // 2秒待機

    // 念のため全MCIデバイスをクリア
    mciSendStringW(L"close all", NULL, 0, NULL);
}