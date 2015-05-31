#pragma once
#include "package.h"
#pragma comment(lib,"ws2_32.lib") 

class TcpTransmission
{
public:
	TcpTransmission(){};
	TcpTransmission(SOCKET soc)
	{this->soc = soc;}

	bool socketInitial();
	void Bind();
	virtual void setAddr(const char* ip, int port);
	virtual void Connect(){};
	int Recv(Data* data);
	int Send(Data* data);
	int close();
protected:
	SOCKADDR_IN addr;
	SOCKET soc;
};
class TcpClient:public TcpTransmission
{
public:
	virtual void setAddr(const char* ip, int port);
	int Send(Data* data);
	virtual void Connect();
};
class TcpServer:public TcpTransmission
{
public:
	virtual void setAddr(const char* ip, int port);
	void Listen();
	TcpTransmission* Accept();
};