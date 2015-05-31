#include "tcpSocket.h"

int tcpIni(const char* ip, int port);
DWORD WINAPI tcpCommunicationStart(LPVOID pParam);
int tcpDestroy();