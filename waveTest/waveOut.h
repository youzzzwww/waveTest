#pragma once
#include <Windows.h>
#include <iostream>
#include <mmsystem.h>
#pragma comment(lib,"Winmm.lib")
#include "structIni.h"

#define BLOCK_SIZE 8192
#define BLOCK_COUNT 20

void CALLBACK waveOutProc(HWAVEOUT, UINT, DWORD, DWORD, DWORD);
WAVEHDR* allocateBlocks(int size, int count);
void freeBlocks(WAVEHDR* blockArray);
void writeAudio(unsigned char*  data, int size);

int myWaveOutInitalize(int sampleRate, int bitsPerSample, int channels);
int myWaveOutReset(int sampleRate, int bitsPerSample, int channels);
int myWaveOutDestroy();
DWORD WINAPI myWaveOutWrite(LPVOID pParam);
