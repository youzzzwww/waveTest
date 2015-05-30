#ifndef STRUCTINI_H
#define STRUCTINI_H

#include <Windows.h>

#define             MULTI_BUF_SIZE              10
#define             MULTI_MIN_BUF_DATA_SIZE     5

class CCircularBuf
{
public:
    CCircularBuf(void);
    ~CCircularBuf(void);

    //����������
    long createBuf(const long size, const HANDLE writeEvent, const HANDLE readEvent);

    //�ͷŻ�����
    long releaseBuf(void);

    //��
    long readBuf(char *pBuf, const long bytes);

    //д
    long writeBuf(const char *pBuf, const long bytes);

private:
    //��ȡдָ��
    long getWritePos(const long size, long *pos1, long *pos2, 
                     long *len1, long *len2);

    //��ȡ��ָ��
    long getReadPos(const long size, long *pos1, long *pos2, 
                    long *len1, long *len2);

    //�޸�дָ��
    long setWritePos(const long pos);

    //�޸Ķ�ָ��
    long setReadPos(const long pos);


private:
    char *pCirBuf;
    long lBufSize;
    long lMaxRWBufSize;

    long lWritePos;
    long lReadPos;

    HANDLE hWriteEvent;
    HANDLE hReadEvent;

    CRITICAL_SECTION csCirBuf;
};

class DoubleBuf
{
public:
	CCircularBuf rtpBuf;
	CCircularBuf playBuf;
	HANDLE hRtpWrite;
	HANDLE hRtpRead;
	HANDLE hPlayWrite;
	HANDLE hPlayRead;
public:
	DoubleBuf(void);
	DoubleBuf(int rtpSize, int playSize);
	int setEvent(void);

};
#endif
//#ifndef _STRUCTINI_
//#define _STRUCTINI_
//
//struct newaudioBuffer
//{
//	unsigned char* dataBuffer;
//	int data_size;
//	unsigned char* bfiBuffer;
//	int bfi_size;
//};
//struct waveBuffer
//{
//	unsigned char* dataBuffer;
//	int size;
//};
//struct Buffer
//{
//	newaudioBuffer newaudio_buffer;
//	waveBuffer wave_buffer;
//};
//
//int bufferIni(struct Buffer *buffer);
//int bufferFree(struct Buffer *buffer);
//
////#define SamplesPerSec 48000
////#define BitsPerSample 16
////#define Channels 1
//#define MI  17
//
//#endif
#define MI  17