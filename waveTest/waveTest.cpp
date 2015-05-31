#include "rtpRecv.h"
#include "waveOut.h"
#include "newAudioDecode.h"
#include "waveFileWrite.h"

#include "tcpCommunication.h"
#include <iostream>
using namespace std;

int sampleRate=48000;
int bitsPerSample=16;
int channel=1;

int main(int argc, char* argv[])
{
	DWORD threadID_tcpTrans,threadID_waveOut,threadID_rtpRecv,threadID_decoder;
	HANDLE hThread_tcpTrans,hThread_waveOut,hThread_rtpRecv,hThread_decoder;

	DoubleBuf *buffer = new DoubleBuf(3200, 655360);
	rtpRecvInitalize(30998);
	myWaveOutInitalize(sampleRate, bitsPerSample, channel);
	decoderIni(sampleRate, channel);
	tcpIni("", 30996);
	
	hThread_tcpTrans = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)tcpCommunicationStart, 0, 0, &threadID_tcpTrans);
	hThread_rtpRecv = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)rtpRecv, buffer, 0, &threadID_rtpRecv);
	hThread_decoder = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)decoderStart, buffer, 0, &threadID_decoder);
	hThread_waveOut = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)myWaveOutWrite, buffer, 0, &threadID_waveOut);

	while( WaitForSingleObject(hThread_rtpRecv, INFINITE)!=WAIT_OBJECT_0 )
	{
		Sleep(80);
	}
	tcpDestroy();
	decoderDestroy();
	myWaveOutDestroy();
	rtpDestory();
	CloseHandle(hThread_waveOut);
	CloseHandle(hThread_rtpRecv);
	CloseHandle(hThread_decoder);
	return 0;

}