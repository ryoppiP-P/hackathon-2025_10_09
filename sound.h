#ifndef SOUND_H
#define SOUND_H

#include <windows.h>
#include <mmsystem.h>
#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <atomic>
#include <chrono>
#pragma comment(lib, "winmm.lib")


void PlayBGMThread(std::wstring fileName);
void PlayBGM(const std::wstring& fileName, int volume = 100);
void StopBGM();
void SetBGMVolume(int volume);
void PlaySingleEffect(std::wstring fileName, int volume, int id);
void PlaySoundEffect(const std::wstring& fileName, int volume = 100);
void InitSoundSystem();
void ShutdownSoundSystem();

#endif //SOUND_H