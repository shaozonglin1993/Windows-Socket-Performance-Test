
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
#include <winsock2.h>
#include "ndds/ndds_cpp.h"
#include "clock/clock_highResolution.h"

#pragma comment(lib, "ws2_32.lib") 

int main(int argc, char* argv[])
{
	int buffer_size = 0;
	int count = 0;
	if(argc >= 2)
	{
		count = atoi(argv[1]);
	}

	if(argc >= 3)
	{
		buffer_size = atoi(argv[2]);
	}

    WSADATA wsaData;
    WORD sockVersion = MAKEWORD(2,2);
    if(WSAStartup(sockVersion, &wsaData) != 0)
    {
        return 0;
    }

    SOCKET serSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); 
    if(serSocket == INVALID_SOCKET)
    {
        printf("socket error !");
        return 0;
    }

    sockaddr_in serAddr;
    serAddr.sin_family = AF_INET;
    serAddr.sin_port = htons(8080);
    serAddr.sin_addr.S_un.S_addr = INADDR_ANY;

    if(bind(serSocket, (sockaddr *)&serAddr, sizeof(serAddr)) == SOCKET_ERROR)
    {
        printf("bind error !");
        closesocket(serSocket);
        return 0;
    }
    
	if (listen(serSocket, 1) == SOCKET_ERROR)
	{
		printf("listen error !");
		return 0;
	}

    sockaddr_in remoteAddr;
    int nAddrLen = sizeof(remoteAddr); 
	char *recvData = (char *)malloc(buffer_size+100);
	char *sendData = (char *)malloc(buffer_size);
	int ret = 0;
	int recv_count = 0;
	int send_size = 0;
	SOCKET client;
	printf("waitting con!!!\n");
	client = accept(serSocket, (SOCKADDR *)&remoteAddr, &nAddrLen);
	if(client == INVALID_SOCKET)
	{
		printf("sceppt error\n");
	}
	else
	{
		printf("accept success!\n");
	}
    while (true)
    { 
		while(buffer_size - ret > 0)
        {
			ret += recv(client, recvData + ret, buffer_size - ret, 0);
		}
		while(send_size < buffer_size)
		{
			send_size += send(client, sendData + send_size, buffer_size - send_size, 0);
		}
		send_size = 0;
		ret = 0;
    }
    closesocket(serSocket); 
	closesocket(client); 
    WSACleanup();
    return 0;
}