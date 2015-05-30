#pragma comment(lib,"decoder_NewAudio.lib")

#ifndef _DEC_INTERFACE_H
#define _DEC_INTERFACE_H


/**创建解码器
  *
  *numOfChannels为声道数
  *samplingRate为采样频率，单位Hz
  *bfi  坏帧指示，用于误码隐藏
  *返回值为编码器句柄
  *
**/
extern "C"{
_declspec(dllexport) long __stdcall createDecoder(int numOfChannels, long samplingRate);


/**解码一帧
  *
  *handle为编码器句柄
  *in为输入缓存(待编码的一帧原始语音)
  *out为码流缓存(存放编码后的码流)
  *返回值为重建样点数目
  *
**/
_declspec(dllexport) int __stdcall doDecoder(long handle, unsigned char* in, unsigned char *out, int *bfi);

/************************************************************************/
/* 返回值：获取当前帧的长度
     *in  输入码流数据，只需3个字节                                            */
/************************************************************************/
_declspec(dllexport) short __stdcall Get_FrameLength(unsigned char *in);




/**销毁编码器
  *
  *handle为编码器句柄
  *
**/
_declspec(dllexport) void __stdcall exitDecoder(long handle);

};

#endif


