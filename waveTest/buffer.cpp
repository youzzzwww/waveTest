#include "structIni.h"

CCircularBuf::CCircularBuf(void)
{
    pCirBuf = NULL;
    lBufSize = 0;
    lMaxRWBufSize = 0;

    hWriteEvent = 0;
    hReadEvent = 0;

    lWritePos = 0; 
    lReadPos = 0;
}

CCircularBuf::~CCircularBuf(void)
{
}

//创建缓冲区
long CCircularBuf::createBuf(long size, const HANDLE writeEvent, const HANDLE readEvent)
{
    if (!size || !writeEvent || !readEvent)
    {
        return -1;
    }
   
    pCirBuf = new char [size * MULTI_BUF_SIZE];
    if (!pCirBuf)
    {
        return -1;
    }
    
    memset(pCirBuf, 0, size * MULTI_BUF_SIZE);
    lMaxRWBufSize = size;
    lBufSize = size * MULTI_BUF_SIZE;

    hWriteEvent = writeEvent;
    hReadEvent = readEvent;

    lWritePos = 0; 
    lReadPos = 0;

    InitializeCriticalSection(&csCirBuf); 
    return 0;
}

//释放缓冲区
long CCircularBuf::releaseBuf(void)
{
    if (!lBufSize)
    {
        return -1;
    }

    EnterCriticalSection(&csCirBuf);
    if (pCirBuf)
    {
        delete []pCirBuf;
        pCirBuf = NULL;
    }

    lMaxRWBufSize = 0;
    lBufSize = 0;

    hWriteEvent = 0;
    hReadEvent = 0;

    lWritePos = 0; 
    lReadPos = 0;
    LeaveCriticalSection(&csCirBuf);
    DeleteCriticalSection(&csCirBuf);

    return 0;
}

//读
long CCircularBuf::readBuf(char *pBuf, const long bytes)
{ 
    long pos1 = 0;
    long pos2 = 0;
    long len1 = 0;
    long len2 = 0;
    long realBytes = 0;

    if (bytes > lMaxRWBufSize || !pBuf)
    {
        return -1;
    }

    EnterCriticalSection(&csCirBuf);
    if (getReadPos(bytes, &pos1, &pos2, &len1, &len2))
    {
        LeaveCriticalSection(&csCirBuf);
        return -1;
    }

    if (pos2)
    {
        memcpy(pBuf, &pCirBuf[pos1], len1);
        realBytes = len1;
        setReadPos(pos1 + len1);
    }
    else
    {
        memcpy(pBuf, &pCirBuf[pos1], len1);
        memcpy(&pBuf[len1], &pCirBuf, len2);
        realBytes = len1 + len2;
        setReadPos(len2);
    }
    LeaveCriticalSection(&csCirBuf);

    return realBytes;
}

//写
long CCircularBuf::writeBuf(const char *pBuf, const long bytes)
{
    long pos1 = 0;
    long pos2 = 0;
    long len1 = 0;
    long len2 = 0;
    long writeBytes = 0;

    if (bytes > lMaxRWBufSize || !pBuf)
    {
        return -1;
    }

    EnterCriticalSection(&csCirBuf);
    if (getWritePos(bytes, &pos1, &pos2, &len1, &len2))
    {
        LeaveCriticalSection(&csCirBuf);
        return -1;
    }

    if (pos2)
    {
        memcpy(&pCirBuf[pos1], pBuf, len1);
        writeBytes = len1;
        setWritePos(pos1 + len1);
    }
    else
    {
        memcpy(&pCirBuf[pos1], pBuf, len1);
        memcpy(&pCirBuf, &pBuf[len1], len2);
        writeBytes = len1 + len2;
        setWritePos(len2);
    }
    LeaveCriticalSection(&csCirBuf);

    return writeBytes;
}

//获取写指针
long CCircularBuf::getWritePos(const long size, long *pos1, long *pos2, 
                               long *len1, long *len2)
{
    if (!pos1 || !pos2 || *len1 || *len2)
    {
        return -1;
    }

    *pos1 = lWritePos;
    if (lWritePos < lReadPos)//写指针在读指针左，不可能循环
    {
        *pos2 = lBufSize;
        *len2 = 0;
        if (lWritePos + size > lReadPos)
        {
            *len1 = lReadPos - lWritePos;
        }
        else
        {
            *len1 = size;
        }
    }
    else//写指针在读指针右，可能循环
    {
        if (lWritePos + size > lBufSize)
        {
            *len1 = lBufSize - lWritePos;
            *pos2 = 0;
            *len2 = size + lWritePos - lBufSize;
            if (*len2 > lReadPos)
            {
                *len2 = lReadPos;
            }
        }
        else
        {
            *len1 = size;
            *pos2 = lBufSize;
            *len2 = 0;
        }
    }

    return 0;
}


//获取读指针
long CCircularBuf::getReadPos(const long size, long *pos1, long *pos2, 
                              long *len1, long *len2)
{
    if (!pos1 || !pos2 || *len1 || *len2)
    {
        return -1;
    }

    *pos1 = lReadPos;
    if (lReadPos <= lWritePos)//读指针在写指针左，不可能循环
    {
        *pos2 = lBufSize;
        *len2 = 0;
        if (lReadPos + size > lWritePos)
        {
            *len1 = lWritePos - lReadPos;
        }
        else
        {
            *len1 = size;
        }
    }
    else//读指针在写指针右，可能循环
    {
        if (lReadPos + size > lBufSize)
        {
            *len1 = lBufSize - lReadPos;
            *pos2 = 0;
            *len2 = size + lReadPos - lBufSize;
            if (*len2 > lWritePos)
            {
                *len2 = lWritePos;
            }
        }
        else
        {
            *len1 = size;
            *pos2 = lBufSize;
            *len2 = 0;
        }
    }
    
    return 0;
}


//修改写指针
long CCircularBuf::setWritePos(const long pos)
{
    if (pos >= lBufSize)
    {
        lWritePos = pos - lBufSize;
    }
    else
    {
        lWritePos = pos;
    }

    if (lReadPos > lWritePos)
    {
        if (lBufSize - lReadPos + lWritePos > MULTI_MIN_BUF_DATA_SIZE * lMaxRWBufSize)
        {
            SetEvent(hReadEvent);
        }
    }
    else
    {
        if (lWritePos - lReadPos > MULTI_MIN_BUF_DATA_SIZE * lMaxRWBufSize)
        {
            SetEvent(hReadEvent);
        }
    }

    return 0;
}

//修改读指针
long CCircularBuf::setReadPos(const long pos)
{    
    if (pos >= lBufSize)
    {
        lReadPos = pos - lBufSize;
    }
    else
    {
        lReadPos = pos;
    }

    if (lReadPos > lWritePos)
    {
        if (lBufSize - lReadPos + lWritePos < MULTI_MIN_BUF_DATA_SIZE * lMaxRWBufSize)
        {
            SetEvent(hWriteEvent);
        }
    }
    else
    {
        if (lWritePos - lReadPos < MULTI_MIN_BUF_DATA_SIZE * lMaxRWBufSize)
        {
            SetEvent(hWriteEvent);
        }
    }
    
    return 0;
}
//int bufferIni(struct Buffer *buffer)
//{
//	buffer->newaudio_buffer.dataBuffer = (unsigned char*)malloc(1);
//	buffer->newaudio_buffer.bfiBuffer = (unsigned char*)malloc(1);
//	buffer->newaudio_buffer.data_size = 0;
//	buffer->newaudio_buffer.bfi_size = 0;
//	buffer->wave_buffer.dataBuffer = (unsigned char*)malloc(1);
//	buffer->wave_buffer.size = 0;
//	return 0;
//}
//int bufferFree(struct Buffer *buffer)
//{
//	buffer->newaudio_buffer.data_size = 0;
//	buffer->newaudio_buffer.bfi_size = 0;
//	buffer->wave_buffer.size = 0;
//	free(buffer->newaudio_buffer.dataBuffer);
//	free(buffer->newaudio_buffer.bfiBuffer);
//	free(buffer->wave_buffer.dataBuffer);
//	return 0;
//}
DoubleBuf::DoubleBuf(int rtpSize, int playSize)
{
	setEvent();
	this->rtpBuf.createBuf(rtpSize, hRtpWrite, hRtpRead);
	this->playBuf.createBuf(playSize, hPlayWrite, hPlayRead);
}

int DoubleBuf::setEvent(void)
{
	hRtpWrite = CreateEvent(NULL, FALSE, FALSE, NULL);
	hRtpRead = CreateEvent(NULL, FALSE, FALSE, NULL);
	hPlayWrite = CreateEvent(NULL, FALSE, FALSE, NULL);
	hPlayRead = CreateEvent(NULL, FALSE, FALSE, NULL);
	return 1;
}