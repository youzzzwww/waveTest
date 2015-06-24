#ifndef STRUCTINI_H
#define STRUCTINI_H

#include <string>
#include <Windows.h>

#define MI  17
#define             MULTI_BUF_SIZE              10
#define             MULTI_MIN_BUF_DATA_SIZE     5

class CCircularBuf
{
public:
    CCircularBuf(void);
    ~CCircularBuf(void);

    //创建缓冲区
    long createBuf(const long size, const HANDLE writeEvent, const HANDLE readEvent);

    //释放缓冲区
    long releaseBuf(void);

    //读
    long readBuf(char *pBuf, const long bytes);

    //写
    long writeBuf(const char *pBuf, const long bytes);

private:
    //获取写指针
    long getWritePos(const long size, long *pos1, long *pos2, 
                     long *len1, long *len2);

    //获取读指针
    long getReadPos(const long size, long *pos1, long *pos2, 
                    long *len1, long *len2);

    //修改写指针
    long setWritePos(const long pos);

    //修改读指针
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

class networkImpairment{
public:
	void countIni();
	void addLossNum(int loss_num);
	std::string getFrameNumPerPacket();
	std::string toString();
	int getInterval();
public:
	float total_loss_num;
	float total_packet_num;
	int interval_every_send;
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
	networkImpairment net_impair;
public:
	DoubleBuf(void);
	DoubleBuf(int rtpSize, int playSize);
	~DoubleBuf();
	int setEvent(void);

};

#endif
