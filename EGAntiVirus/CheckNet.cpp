#include "CheckNet.h"
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include "common.h"


int CheckInternet()
{
	WSADATA wsaData;
	SOCKET ConnectSocket = INVALID_SOCKET;
	struct addrinfo *result = NULL, *ptr = NULL, hints;
	char *sendbuf = "this is a test";
//	char recvbuf[DEFAULT_BUFLEN];
	int iResult;
	int recvbuflen = DEFAULT_BUFLEN;


	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0)
	{
		std::string err;
		GetLastErrorAsString(iResult, err);
		ErrorInternetLogsW(L"WSAStartup failed with error: %S\n", err.c_str());
		return 1;
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	// Resolve the server address and port
	iResult = getaddrinfo("www.google.com", DEFAULT_PORT, &hints, &result);
	if (iResult != 0)
	{
		std::string err;
		GetLastErrorAsString(iResult, err);
		ErrorInternetLogsW(L"getaddrinfo failed with error: %S\n", err.c_str());
		WSACleanup();
		return 1;
	}

	// Attempt to connect to an address until one succeeds
	for (ptr = result; ptr != NULL; ptr = ptr->ai_next)
	{

		// Create a SOCKET for connecting to server
		ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
		if (ConnectSocket == INVALID_SOCKET)
		{
			ErrorInternetLogsW(L"socket failed with error: %ld\n", WSAGetLastError());
			WSACleanup();
			return 1;
		}

		// Connect to server.
		iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (iResult == SOCKET_ERROR)
		{
			closesocket(ConnectSocket);
			ConnectSocket = INVALID_SOCKET;
			continue;
		}
		break;
	}

	freeaddrinfo(result);

	if (ConnectSocket == INVALID_SOCKET)
	{
		ErrorInternetLogsW(L"Unable to connect to server!\n");
		WSACleanup();
		return 1;
	}

	// Send an initial buffer
	iResult = send(ConnectSocket, sendbuf, (int)strlen(sendbuf), 0);
	if (iResult == SOCKET_ERROR)
	{
		ErrorInternetLogsW(L"send failed with error: %d\n", WSAGetLastError());
		closesocket(ConnectSocket);
		WSACleanup();
		return 1;
	}

	SuccessInternetLogsW(L"Bytes Sent: %ld\n", iResult);

	// shutdown the connection since no more data will be sent
	iResult = shutdown(ConnectSocket, SD_SEND);
	if (iResult == SOCKET_ERROR)
	{
		ErrorInternetLogsW(L"shutdown failed with error: %d\n", WSAGetLastError());
		closesocket(ConnectSocket);
		WSACleanup();
		return 1;
	}

	// Receive until the peer closes the connection
	/*  do {

	iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
	if ( iResult > 0 )
	printf("Bytes received: %d\n", iResult);
	else if ( iResult == 0 )
	printf("Connection closed\n");
	else
	printf("recv failed with error: %d\n", WSAGetLastError());

	} while( iResult > 0 );
	*/
	// cleanup
	closesocket(ConnectSocket);
	WSACleanup();

	SuccessInternetLogsW(L"Success... Connected to internet.");
	return 0;
}
