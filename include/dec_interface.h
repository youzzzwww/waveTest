#pragma comment(lib,"decoder_NewAudio.lib")

#ifndef _DEC_INTERFACE_H
#define _DEC_INTERFACE_H


/**����������
  *
  *numOfChannelsΪ������
  *samplingRateΪ����Ƶ�ʣ���λHz
  *bfi  ��ָ֡ʾ��������������
  *����ֵΪ���������
  *
**/
extern "C"{
_declspec(dllexport) long __stdcall createDecoder(int numOfChannels, long samplingRate);


/**����һ֡
  *
  *handleΪ���������
  *inΪ���뻺��(�������һ֡ԭʼ����)
  *outΪ��������(��ű���������)
  *����ֵΪ�ؽ�������Ŀ
  *
**/
_declspec(dllexport) int __stdcall doDecoder(long handle, unsigned char* in, unsigned char *out, int *bfi);

/************************************************************************/
/* ����ֵ����ȡ��ǰ֡�ĳ���
     *in  �����������ݣ�ֻ��3���ֽ�                                            */
/************************************************************************/
_declspec(dllexport) short __stdcall Get_FrameLength(unsigned char *in);




/**���ٱ�����
  *
  *handleΪ���������
  *
**/
_declspec(dllexport) void __stdcall exitDecoder(long handle);

};

#endif


