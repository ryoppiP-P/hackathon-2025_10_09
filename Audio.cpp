#include <xaudio2.h>
#include <assert.h>
#include "audio.h"

#pragma comment(lib, "winmm.lib")


static IXAudio2* g_Xaudio{};
static IXAudio2MasteringVoice* g_MasteringVoice{};


void InitAudio()
{
	// XAudio����
	XAudio2Create(&g_Xaudio, 0);

	// �}�X�^�����O�{�C�X����
	g_Xaudio->CreateMasteringVoice(&g_MasteringVoice);
}


void UninitAudio()
{
	g_MasteringVoice->DestroyVoice();
	g_Xaudio->Release();
}









struct AUDIO
{
	IXAudio2SourceVoice*	SourceVoice{};
	BYTE*					SoundData{};

	int						Length{};
	int						PlayLength{};
};

#define AUDIO_MAX 100
static AUDIO g_Audio[AUDIO_MAX]{};



int LoadAudio(const char *FileName)
{
	int index = -1;

	for (int i = 0; i < AUDIO_MAX; i++)
	{
		if (g_Audio[i].SourceVoice == nullptr)
		{
			index = i;
			break;
		}
	}

	if (index == -1)
		return -1;




	// �T�E���h�f�[�^�Ǎ�
	WAVEFORMATEX wfx = { 0 };

	{
		HMMIO hmmio = NULL;
		MMIOINFO mmioinfo = { 0 };
		MMCKINFO riffchunkinfo = { 0 };
		MMCKINFO datachunkinfo = { 0 };
		MMCKINFO mmckinfo = { 0 };
		UINT32 buflen;
		LONG readlen;


		hmmio = mmioOpen((LPSTR)FileName, &mmioinfo, MMIO_READ);
		assert(hmmio);

		riffchunkinfo.fccType = mmioFOURCC('W', 'A', 'V', 'E');
		mmioDescend(hmmio, &riffchunkinfo, NULL, MMIO_FINDRIFF);

		mmckinfo.ckid = mmioFOURCC('f', 'm', 't', ' ');
		mmioDescend(hmmio, &mmckinfo, &riffchunkinfo, MMIO_FINDCHUNK);

		if (mmckinfo.cksize >= sizeof(WAVEFORMATEX))
		{
			mmioRead(hmmio, (HPSTR)&wfx, sizeof(wfx));
		}
		else
		{
			PCMWAVEFORMAT pcmwf = { 0 };
			mmioRead(hmmio, (HPSTR)&pcmwf, sizeof(pcmwf));
			memset(&wfx, 0x00, sizeof(wfx));
			memcpy(&wfx, &pcmwf, sizeof(pcmwf));
			wfx.cbSize = 0;
		}
		mmioAscend(hmmio, &mmckinfo, 0);

		datachunkinfo.ckid = mmioFOURCC('d', 'a', 't', 'a');
		mmioDescend(hmmio, &datachunkinfo, &riffchunkinfo, MMIO_FINDCHUNK);



		buflen = datachunkinfo.cksize;
		g_Audio[index].SoundData = new unsigned char[buflen];
		readlen = mmioRead(hmmio, (HPSTR)g_Audio[index].SoundData, buflen);


		g_Audio[index].Length = readlen;
		g_Audio[index].PlayLength = readlen / wfx.nBlockAlign;


		mmioClose(hmmio, 0);
	}


	// �T�E���h�\�[�X����
	g_Xaudio->CreateSourceVoice(&g_Audio[index].SourceVoice, &wfx);
	assert(g_Audio[index].SourceVoice);


	return index;
}




void UnloadAudio(int Index)
{
	g_Audio[Index].SourceVoice->Stop();
	g_Audio[Index].SourceVoice->DestroyVoice();

	delete[] g_Audio[Index].SoundData;
	g_Audio[Index].SoundData = nullptr;
}





void PlayAudio(int Index, bool Loop)
{
	g_Audio[Index].SourceVoice->Stop();
	g_Audio[Index].SourceVoice->FlushSourceBuffers();


	// �o�b�t�@�ݒ�
	XAUDIO2_BUFFER bufinfo;

	memset(&bufinfo, 0x00, sizeof(bufinfo));
	bufinfo.AudioBytes = g_Audio[Index].Length;
	bufinfo.pAudioData = g_Audio[Index].SoundData;
	bufinfo.PlayBegin = 0;
	bufinfo.PlayLength = g_Audio[Index].PlayLength;

	// ���[�v�ݒ�
	if (Loop)
	{
		bufinfo.LoopBegin = 0;
		bufinfo.LoopLength = g_Audio[Index].PlayLength;
		bufinfo.LoopCount = XAUDIO2_LOOP_INFINITE;
	}

	g_Audio[Index].SourceVoice->SubmitSourceBuffer(&bufinfo, NULL);


	// �Đ�
	g_Audio[Index].SourceVoice->Start();

}



