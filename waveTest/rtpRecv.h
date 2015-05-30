#pragma once
#include "ortp/ortp.h"
#include <time.h>
#pragma comment(lib,"oRTP.lib")
#include "structIni.h"


int rtpRecvInitalize(int port=8000, bool jitter_adapt=1, int jittcomp=40);
int rtpRecv(unsigned char* buffer,int &size);
DWORD WINAPI rtpRecv(LPVOID pParam);
int rtpDestory();