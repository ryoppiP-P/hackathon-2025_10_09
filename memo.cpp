//#include "sound.h"
//
//// より安全なグローバル変数管理
//struct SoundSystem {
//    std::thread bgmThread;
//    std::atomic<bool> isPlayingBGM{ false };
//    std::atomic<bool> stopBGMThread{ false };
//    std::atomic<bool> systemShutdown{ false };  // ← 追加
//    int bgmVolume = 100;
//    std::atomic<int> effectCounter{ 0 };
//
//    ~SoundSystem() {
//        // デストラクタで確実に停止
//        systemShutdown = true;
//        if (isPlayingBGM && bgmThread.joinable()) {
//            stopBGMThread = true;
//            mciSendStringW(L"close all", NULL, 0, NULL);
//            bgmThread.detach(); // joinではなくdetachで強制終了回避
//        }
//    }
//};
//
//// 関数内static（より安全）
//SoundSystem& GetSoundSystem() {
//    static SoundSystem soundSystem;
//    return soundSystem;
//}
//
//// BGM関連
//void PlayBGMThread(std::wstring fileName) {
//    auto& sys = GetSoundSystem();
//
//    std::wstring command = L"open \"" + fileName + L"\" type mpegvideo alias bgm";
//    MMRESULT res = mciSendStringW(command.c_str(), NULL, 0, NULL);
//    if (res != 0) {
//        sys.isPlayingBGM = false;
//        return;
//    }
//
//    command = L"play bgm repeat";
//    mciSendStringW(command.c_str(), NULL, 0, NULL);
//
//    // 音量設定
//    command = L"setaudio bgm volume to " + std::to_wstring((sys.bgmVolume * 1000) / 100);
//    mciSendStringW(command.c_str(), NULL, 0, NULL);
//
//    while (!sys.stopBGMThread && !sys.systemShutdown) {
//        std::this_thread::sleep_for(std::chrono::milliseconds(50));
//    }
//
//    // BGM停止
//    mciSendStringW(L"stop bgm", NULL, 0, NULL);
//    mciSendStringW(L"close bgm", NULL, 0, NULL);
//    sys.isPlayingBGM = false;
//}
//
//void PlayBGM(const std::wstring& fileName, int volume) {
//    auto& sys = GetSoundSystem();
//    if (sys.isPlayingBGM || sys.systemShutdown) return;
//
//    sys.stopBGMThread = false;
//    sys.bgmVolume = volume;
//    sys.isPlayingBGM = true;
//    sys.bgmThread = std::thread(PlayBGMThread, fileName);
//}
//
//void StopBGM() {
//    auto& sys = GetSoundSystem();
//    if (sys.isPlayingBGM) {
//        sys.stopBGMThread = true;
//
//        // MCIを先に強制停止
//        mciSendStringW(L"stop bgm", NULL, 0, NULL);
//        mciSendStringW(L"close bgm", NULL, 0, NULL);
//
//        // 短時間でjoin、だめならdetach
//        if (sys.bgmThread.joinable()) {
//            sys.bgmThread.detach(); // joinを避けてdetach
//        }
//        sys.isPlayingBGM = false;
//    }
//}
//
//void SetBGMVolume(int volume) {
//    auto& sys = GetSoundSystem();
//    sys.bgmVolume = volume;
//    if (sys.isPlayingBGM) {
//        std::wstring command = L"setaudio bgm volume to " + std::to_wstring((sys.bgmVolume * 1000) / 100);
//        mciSendStringW(command.c_str(), NULL, 0, NULL);
//    }
//}
//
//// 効果音関連（スレッドプール風に改良）
//void PlaySingleEffect(std::wstring fileName, int volume, int id) {
//    auto& sys = GetSoundSystem();
//    if (sys.systemShutdown) return; // システム終了中は何もしない
//
//    std::wstring alias = L"effect" + std::to_wstring(id);
//
//    // WAVファイルを開く
//    std::wstring command = L"open \"" + fileName + L"\" type waveaudio alias " + alias;
//    MMRESULT res = mciSendStringW(command.c_str(), NULL, 0, NULL);
//    if (res != 0) {
//        return;
//    }
//
//    // 音量設定
//    command = L"setaudio " + alias + L" volume to " + std::to_wstring((volume * 1000) / 100);
//    mciSendStringW(command.c_str(), NULL, 0, NULL);
//
//    // 再生
//    command = L"play " + alias + L" from 0";
//    mciSendStringW(command.c_str(), NULL, 0, NULL);
//
//    // より短い待機時間
//    std::this_thread::sleep_for(std::chrono::milliseconds(800)); // 0.8秒
//
//    // 停止・クローズ
//    mciSendStringW((L"stop " + alias).c_str(), NULL, 0, NULL);
//    mciSendStringW((L"close " + alias).c_str(), NULL, 0, NULL);
//}
//
//void PlaySoundEffect(const std::wstring& fileName, int volume) {
//    auto& sys = GetSoundSystem();
//    if (sys.systemShutdown) return;
//
//    int id = sys.effectCounter++;
//    std::thread(PlaySingleEffect, fileName, volume, id).detach();
//}
//
//void ShutdownSoundSystem() {
//    std::cout << "Shutting down sound system..." << std::endl;
//
//    auto& sys = GetSoundSystem();
//    sys.systemShutdown = true; // 新規スレッド作成を停止
//
//    // BGMを停止
//    StopBGM();
//
//    // 短時間待機
//    std::this_thread::sleep_for(std::chrono::milliseconds(500));
//
//    // 全MCIデバイスをクリア
//    mciSendStringW(L"close all", NULL, 0, NULL);
//
//    std::cout << "Sound system shutdown complete." << std::endl;
//}