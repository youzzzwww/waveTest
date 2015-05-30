#pragma once
#include "dec_interface.h"
#include <Windows.h>
#include <iostream>
#include "structIni.h"

int decoderIni(int sampleRate, int channel);
DWORD WINAPI decoderStart(LPVOID pParam);
int decoderDestroy();
int decoderReset(int sampleRate, int channel);