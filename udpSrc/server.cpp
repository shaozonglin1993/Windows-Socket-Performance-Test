
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

    SOCKET serSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP); 
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
    
    sockaddr_in remoteAddr;
    int nAddrLen = sizeof(remoteAddr); 
	char *recvData = (char *)malloc(buffer_size+100);
	char *sendData = (char *)malloc(buffer_size);
	int ret = 0;
	int recv_count = 0;
    while (true)
    { 
		while(buffer_size - ret > 0)
        {
			ret += recvfrom(serSocket, recvData, buffer_size - ret, 0, (sockaddr *)&remoteAddr, &nAddrLen);
			//printf("ret = %d \n",ret);
			//printf("接受到一个连接：%s \r\n", inet_ntoa(remoteAddr.sin_addr));
		}
		ret = 0;
        sendto(serSocket, sendData, buffer_size, 0, (sockaddr *)&remoteAddr, nAddrLen);  
		//printf("recv %d .\n",recv_count++);
    }

    closesocket(serSocket); 
    WSACleanup();
    return 0;
}