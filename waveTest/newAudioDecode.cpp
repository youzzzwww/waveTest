#include "newAudioDecode.h"

#define IN_BUFF_SIZE 320
#define OUT_BUFF_SIZE 65536

long handle;
unsigned char in[IN_BUFF_SIZE],out[OUT_BUFF_SIZE];
int prev_sample_rate;
int prev_channel;

int decoderIni(int sampleRate, int channel)
{
	prev_sample_rate = sampleRate;
	prev_channel = channel;
	//初始化解码器
	handle = createDecoder(channel, sampleRate);
	if(handle<0)
	{
		printf("create encoder fail\n");
		return 0;
	}
	memset(in, 0, IN_BUFF_SIZE * sizeof(char));
	memset(out, 0, OUT_BUFF_SIZE * sizeof(char));
	return 0;
}
int decoderReset(int sampleRate, int channel)
{
	if(sampleRate == prev_sample_rate && channel == prev_channel)
		return 0;
	else
	{
		decoderDestroy();
		decoderIni(sampleRate, channel);
	}
	return 0;
}

DWORD WINAPI decoderStart(LPVOID pParam)
{
	DoubleBuf* doubleBuf = (DoubleBuf*)pParam;
	int ret=-1,decoder_size=0;
	int read_len=0,bfi_len=0;
	int i;
	int bfi[]={0,0,0,0};

	//循环读取数据，然后解码
	while(1)
	{
		if(read_len = doubleBuf->rtpBuf.readBuf((char*)in,3))
		{
			decoder_size = Get_FrameLength(in);
			while(read_len < decoder_size)
			{
				read_len += doubleBuf->rtpBuf.readBuf((char*)in+read_len , decoder_size-read_len);
				if( read_len <decoder_size )
					Sleep(20);
			}			
			ret = doDecoder(handle, in, out, bfi);
			
			//解码后的数据读入输出缓冲
			if(ret>0)
			{
				doubleBuf->playBuf.writeBuf((const char*)out, ret);
			}
			//重新初始化
			ret=-1;
		}
		else
			Sleep(80);
	}
	return 0;
}
int decoderDestroy()
{
	if(-1!=handle) exitDecoder(handle);
	return 1;
}