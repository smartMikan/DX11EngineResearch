﻿#pragma once

///////////////////////////////////////////////////////////////////////////////
// Filename: soundclass.h
///////////////////////////////////////////////////////////////////////////////
#ifndef _SOUNDCLASS_H_
#define _SOUNDCLASS_H_

#include <math.h>

/////////////
// LINKING //
/////////////
#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "winmm.lib")


//////////////
// INCLUDES //
//////////////
#include <windows.h>
#include <mmsystem.h>
#include <dsound.h>
#include <stdio.h>
#include <d3d11.h>

///////////////////////////////////////////////////////////////////////////////
// Class name: SoundClass
///////////////////////////////////////////////////////////////////////////////
class SoundClass
{
private:
	//The WaveHeaderType structure used here is for the .wav file format. 
	//When loading in .wav files I first read in the header to determine the required information for loading in the .wav audio data.
	//If you are using a different format you will want to replace this header with the one required for your audio format.
	struct WaveHeaderType
	{
		char chunkId[4];
		unsigned long chunkSize;
		char format[4];
		char subChunkId[4];
		unsigned long subChunkSize;
		unsigned short audioFormat;
		unsigned short numChannels;
		unsigned long sampleRate;
		unsigned long bytesPerSecond;
		unsigned short blockAlign;
		unsigned short bitsPerSample;
		char dataChunkId[4];
		unsigned long dataSize;
	};
public:
	SoundClass();
	SoundClass(const SoundClass&);
	~SoundClass();

	
	//The Initialize function will initialize DirectSound and load in the .wav audio file and then play it once. 
	//Shutdown will release the .wav file and shutdown DirectSound.

	bool Initialize(HWND);
	void Shutdown();

	DWORD TransToDSVolume(DWORD inputVolume);

private:
	bool InitializeDirectSound(HWND);
	void ShutdownDirectSound();

	bool LoadWaveFile(const WCHAR * filename, IDirectSoundBuffer8**);
	void ShutdownWaveFile(IDirectSoundBuffer8**);

	bool PlayWaveFile();
	
private:
	IDirectSound8* m_DirectSound;
	IDirectSoundBuffer* m_primaryBuffer;

	//Note that I only have one secondary buffer as this project only loads in one sound.
	IDirectSoundBuffer8* m_secondaryBuffer1;
};

#endif


