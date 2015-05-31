#include "tcpCommunication.h"

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
	tcp_trans = tcp_server->Accept();
	Data* data = new Data("hello");
	tcp_trans->Send(data);

	return 0;
}
int tcpDestroy()
{
	tcp_trans->close();
	tcp_server->close();
	delete tcp_trans,tcp_server;

	return 0;
}