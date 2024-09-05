#include "pch.h"
#include <iostream>

#include <WinSock2.h>
#include <MSWSock.h>
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

void HandleError(const char* cause)
{
	int32 errCode = ::WSAGetLastError();
	cout << cause << " ErrorCode : " << errCode << endl;
}

int main()
{

	int32 num = 0x12345678;

	// 윈속 초기화 (ws_32 라이브러리 초기화)
	// 관련 정보가 wsaData에 채워짐
	WSAData wsaData;
	if (::WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		return 0;

	// ad : Address Family (AF_INET = IPv4, AF_INET6 = IPv6)
	// type : TCP(SOCK_STREAM) vs UDP(SOCK_DGRAM)
	// protocol : 0
	// return : descriptor
	SOCKET clientSocket = ::socket(AF_INET, SOCK_DGRAM, 0);
	if (clientSocket == INVALID_SOCKET)
	{
		HandleError("Socket");
		return 0;
	}

	// 연결할 목적지는 ? (ip주소 + port) -> XX 아파트 YY 호
	SOCKADDR_IN serverAddr; // IPv4
	::memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	//serverAddr.sin_addr.s_addr = ::inet_addr("127.0.0.1");
	::inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);
	serverAddr.sin_port = ::htons(7777); // 80 : HTTP
	
	// host to network short
	// Little-Endian vs Big-Endian
	// ex) 0x12345678 4바이트 정수
	// low [0x78][0x56][0x34][0x12] high < little  일반적인 기기에서 많이 사용 (인텔칩)
	// low [0x12][0x34][0x56][0x78] high < big = network 공식 표준

	// Connected UDP
	::connect(clientSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr));


	while (true)
	{
		//for (int32 i = 0; i < 10; i++)
		//{
			char sendBuffer[100] = "Hello World!";

			// 나의 IP주소 + 포트 번호 설정

			// Unconnected UDP
			/*int32 resultCode = ::sendto(clientSocket, sendBuffer, sizeof(sendBuffer), 0,
				(SOCKADDR*)&serverAddr, sizeof(serverAddr));*/

			// Connected UDP
			int32 resultCode = send(clientSocket, sendBuffer, sizeof(sendBuffer), 0);

			if (resultCode == SOCKET_ERROR) {
				HandleError("sendto");
				return 0;
			}

			cout << "Send Data! Len = " << sizeof(sendBuffer) << endl;
		//}
		
		SOCKADDR_IN recvAddr;
		::memset(&recvAddr, 0, sizeof(recvAddr));
		int32 addrLen = sizeof(recvAddr);

		char recvBuffer[1000];

		// Unconnected UDP
		/*int32 recvLen = ::recvfrom(clientSocket, recvBuffer, sizeof(recvBuffer), 0, 
			(SOCKADDR*)&recvAddr, &addrLen);*/

		//Connected UDP
		int32 recvLen = ::recv(clientSocket, recvBuffer, sizeof(recvBuffer), 0);

		if (recvLen <= 0)
		{
			HandleError("Recvfrom");
			return 0;
		}

		cout << "Recv Data! Data = " << recvBuffer << endl;
		cout << "Recv Data! Len = " << recvLen << endl;

		this_thread::sleep_for(1s);
	}
	// ---------------
	// 소켓 리소스 반환
	::closesocket(clientSocket);

	// 윈속 종료
	::WSACleanup();
}