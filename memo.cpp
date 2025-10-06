//#include "sound.h"
//
//// �����S�ȃO���[�o���ϐ��Ǘ�
//struct SoundSystem {
//    std::thread bgmThread;
//    std::atomic<bool> isPlayingBGM{ false };
//    std::atomic<bool> stopBGMThread{ false };
//    std::atomic<bool> systemShutdown{ false };  // �� �ǉ�
//    int bgmVolume = 100;
//    std::atomic<int> effectCounter{ 0 };
//
//    ~SoundSystem() {
//        // �f�X�g���N�^�Ŋm���ɒ�~
//        systemShutdown = true;
//        if (isPlayingBGM && bgmThread.joinable()) {
//            stopBGMThread = true;
//            mciSendStringW(L"close all", NULL, 0, NULL);
//            bgmThread.detach(); // join�ł͂Ȃ�detach�ŋ����I�����
//        }
//    }
//};
//
//// �֐���static�i�����S�j
//SoundSystem& GetSoundSystem() {
//    static SoundSystem soundSystem;
//    return soundSystem;
//}
//
//// BGM�֘A
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
//    // ���ʐݒ�
//    command = L"setaudio bgm volume to " + std::to_wstring((sys.bgmVolume * 1000) / 100);
//    mciSendStringW(command.c_str(), NULL, 0, NULL);
//
//    while (!sys.stopBGMThread && !sys.systemShutdown) {
//        std::this_thread::sleep_for(std::chrono::milliseconds(50));
//    }
//
//    // BGM��~
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
//        // MCI���ɋ�����~
//        mciSendStringW(L"stop bgm", NULL, 0, NULL);
//        mciSendStringW(L"close bgm", NULL, 0, NULL);
//
//        // �Z���Ԃ�join�A���߂Ȃ�detach
//        if (sys.bgmThread.joinable()) {
//            sys.bgmThread.detach(); // join�������detach
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
//// ���ʉ��֘A�i�X���b�h�v�[�����ɉ��ǁj
//void PlaySingleEffect(std::wstring fileName, int volume, int id) {
//    auto& sys = GetSoundSystem();
//    if (sys.systemShutdown) return; // �V�X�e���I�����͉������Ȃ�
//
//    std::wstring alias = L"effect" + std::to_wstring(id);
//
//    // WAV�t�@�C�����J��
//    std::wstring command = L"open \"" + fileName + L"\" type waveaudio alias " + alias;
//    MMRESULT res = mciSendStringW(command.c_str(), NULL, 0, NULL);
//    if (res != 0) {
//        return;
//    }
//
//    // ���ʐݒ�
//    command = L"setaudio " + alias + L" volume to " + std::to_wstring((volume * 1000) / 100);
//    mciSendStringW(command.c_str(), NULL, 0, NULL);
//
//    // �Đ�
//    command = L"play " + alias + L" from 0";
//    mciSendStringW(command.c_str(), NULL, 0, NULL);
//
//    // ���Z���ҋ@����
//    std::this_thread::sleep_for(std::chrono::milliseconds(800)); // 0.8�b
//
//    // ��~�E�N���[�Y
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
//    sys.systemShutdown = true; // �V�K�X���b�h�쐬���~
//
//    // BGM���~
//    StopBGM();
//
//    // �Z���ԑҋ@
//    std::this_thread::sleep_for(std::chrono::milliseconds(500));
//
//    // �SMCI�f�o�C�X���N���A
//    mciSendStringW(L"close all", NULL, 0, NULL);
//
//    std::cout << "Sound system shutdown complete." << std::endl;
//}