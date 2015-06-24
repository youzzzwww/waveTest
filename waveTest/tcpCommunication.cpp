#include "tcpCommunication.h"
#include "structIni.h"

TcpServer* tcp_server;
TcpTransmission* tcp_trans;

int tcpIni(const char* ip, int port)
{
	tcp_server = new TcpServer();
	tcp_server->setAddr(ip, port);
	tcp_server->Listen();

	return 0;
}
DWORD WINAPI tcpCommunicationStart(LPVOID pParam)
{
	networkImpairment* net_impair = (networkImpairment*)pParam;
	tcp_trans = tcp_server->Accept();
	Data* data = new Data();

	net_impair->interval_every_send = 1000;
	while(1)
	{
		data->setString(net_impair->getFrameNumPerPacket());
		tcp_trans->Send(data);
		Sleep(net_impair->getInterval());
	}

	return 0;
}
int tcpDestroy()
{
	tcp_trans->close();
	tcp_server->close();
	delete tcp_trans,tcp_server;

	return 0;
}