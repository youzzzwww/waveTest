#include "tcpSocket.h"

bool TcpTransmission::socketInitial()
{
	WSADATA wsa;  
    WSAStartup(MAKEWORD(2,2),&wsa);
	if((soc = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP)) <= 0)  
    {  
        wprintf(L"Create tcp socket fail!\n");  
        return false;  
    } 
	return true;
}
void TcpTransmission::setAddr(const char* ip, int port)
{
	this->socketInitial();

	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.S_un.S_addr = inet_addr(ip);
}
void TcpTransmission::Bind()
{
	if( bind(soc,(struct sockaddr *)&addr,sizeof(addr))==SOCKET_ERROR )
		wprintf(L"bind error: %d\n", WSAGetLastError());
}
int TcpTransmission::Send(Data* data)
{
	int iResult = send(soc, data->getData(), data->getLength(), 0);
	if (iResult == SOCKET_ERROR) {
		wprintf(L"sendto failed with error: %d\n", WSAGetLastError());
		closesocket(soc);
		WSACleanup();
	}
	return iResult;
}
int TcpTransmission::Recv(Data* data)
{
	int iResult=0;
	char buf[2048];

	iResult = recv(soc, buf, 2048, 0);
	if (iResult == SOCKET_ERROR) {
        wprintf(L"recvfrom failed with error %d\n", WSAGetLastError());
		return iResult;
    }
	data->setData(buf, iResult);
	return iResult;
}
int TcpTransmission::close()
{
	int iResult = closesocket(soc);
    if (iResult == SOCKET_ERROR) {
        wprintf(L"closesocket failed with error %d\n", WSAGetLastError());
        return iResult;
    }
	WSACleanup();
	return 0;
}
void TcpClient::setAddr(char const* ip, int port)
{
	TcpTransmission::setAddr(ip, port);
}
void TcpClient::Connect()
{
	int iResult = connect(soc, (sockaddr *)&addr, sizeof(addr));
	if (iResult == SOCKET_ERROR) {
		wprintf(L"connect failed with error: %d\n", WSAGetLastError());
		closesocket(soc);
		WSACleanup();
	}
}
void TcpServer::setAddr(char const* ip, int port)
{
	char* ip_bind;
	if( !strcmp(ip, "") )
		ip_bind = "127.0.0.1";
	else ip_bind = (char*)ip;
	TcpTransmission::setAddr(ip_bind, port);
}
void TcpServer::Listen()
{
	this->Bind();
	if(listen(soc, 5) == SOCKET_ERROR)
	{
		printf("listen error !");
	}
}
TcpTransmission* TcpServer::Accept()
{
	SOCKET sClient;
	sockaddr_in remoteAddr;
	int nAddrlen = sizeof(remoteAddr);
	sClient = accept(soc, (SOCKADDR *)&remoteAddr, &nAddrlen);
	TcpTransmission* tcp_connect = new TcpTransmission(sClient);
	return tcp_connect;
}