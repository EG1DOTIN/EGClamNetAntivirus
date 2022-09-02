#include "common.h"
#include "sendrecv.h"
#include <wx\string.h>
#include <wx\datetime.h>
#include "datetime.h"

WSADATA wsaData;

int MainProcessURl20(const std::string& mkey, int& result)
{
	const int bufLen = 1024;
	std::string mainurl("http://eg1.in/MachineKey20.aspx?MachineKey=");
	mainurl.append(mkey);
	char *szUrl = (char*)(mainurl.c_str());

	SuccessAppWinLogW(L"Requested url : %S", mainurl.c_str());

	long fileSize;
	char *memBuffer, *headerBuffer;
	//FILE *fp;

	memBuffer = headerBuffer = NULL;

	if (WSAStartup(0x101, &wsaData) != 0)
	{
		ErrorAppWinLogW(L"Error in WSAStartup: %s", wxSysErrorMsg(WSAGetLastError()));
		return -1;
	}

	memBuffer = ReadURL2(szUrl, fileSize, &headerBuffer);

	SuccessAppWinLogW(L"MemBuffer Contents: %S", memBuffer);

	//Take Respone in string
	wxString Result;

	//Calc response from membuffer or header buffer
	{
		wxString MemBufferStr(memBuffer);

		if (MemBufferStr.Contains(wxT("MachineKey20")))
		{
			wxString SubStrAfterEqualSign = MemBufferStr.AfterFirst('=').AfterFirst('=');
			Result = SubStrAfterEqualSign.substr(0, 4);
		}
		else
		{
			SuccessAppWinLogW(L"Request Processed: Not any useful information found !!");
			WSACleanup();
			return 1;
		}
	}


	// define result from response
	if (Result == wxT("True"))
	{
		result = 1;

	}
	else
	{
		result = 0;
	}

	SuccessAppWinLogW(L"Result From the EG1: %s", Result.wc_str());

	//printf("returned from readUrl\n");
	//printf("data returned:\n%s", memBuffer);
	if (fileSize != 0)
	{
		//printf("Got some data\n");
		//fp = fopen("downloaded.file", "wb");
		//fwrite(memBuffer, 1, fileSize, fp);
		//fclose(fp);
		delete(memBuffer);
		delete(headerBuffer);
	}

	WSACleanup();
	return 0;
}


int MainProcessURl(const std::string& rkey, const std::string& mkey, int& result, int& daysusing)
{
	const int bufLen = 1024;
	std::string mainurl("http://eg1.in/Checking-RegisterKey.aspx?RegisterKey=");
	mainurl.append(rkey);
	mainurl.append("&MachineKey=");
	mainurl.append(mkey);
	char *szUrl = (char*)(mainurl.c_str());

	SuccessAppWinLogW(L"Requested url : %S", mainurl.c_str());

	long fileSize;
	char *memBuffer, *headerBuffer;
	//FILE *fp;

	memBuffer = headerBuffer = NULL;

	if (WSAStartup(0x101, &wsaData) != 0)
	{
		ErrorAppWinLogW(L"Error in WSAStartup: %s", wxSysErrorMsg(WSAGetLastError()));
		return -1;
	}

	memBuffer = ReadURL2(szUrl, fileSize, &headerBuffer);

	SuccessAppWinLogW(L"MemBuffer Contents: %S", memBuffer);

	//Take Respone in string
	wxString rkeyResult, rkeyDate;

	//Calc response from membuffer or header buffer
	{
		wxString MemBufferStr(memBuffer);
		
		if (MemBufferStr.Contains(wxT("RegisterKeyCheck")))
		{
			wxString SubStrAfterEqualSign = MemBufferStr.AfterFirst('=').AfterFirst('=');
			rkeyResult = SubStrAfterEqualSign.substr(0, 4);

			if (rkeyResult == wxT("True"))
			{
				if (SubStrAfterEqualSign.Contains(wxT("EntryDate")))
				{
					rkeyDate = SubStrAfterEqualSign.AfterFirst('=').substr(0, 10);
					OnlyDateStruct onlydatestruct;
					ParseDateInOnlyDateStruct(rkeyDate, onlydatestruct);
					daysusing = GetNumberOfDaysFromToday(onlydatestruct.m_day, onlydatestruct.m_month, onlydatestruct.m_year);
				}
				else
					daysusing = 0;
			}

		}
		else
		{
			SuccessAppWinLogW(L"Request Processed: Not any useful information found !!");
			WSACleanup();
			return 1;
		}
	}


	// define result from response
	if (rkeyResult ==wxT("True"))
	{
		result = 1;

	}
	if (rkeyResult == wxT("Fals"))
	{
		result = 0;
	}
	if (rkeyResult == wxT("Regi"))
	{
		result = -1;
	}
	
	SuccessAppWinLogW(L"Result From the EG1: %s", rkeyResult.wc_str());

	//printf("returned from readUrl\n");
	//printf("data returned:\n%s", memBuffer);
	if (fileSize != 0)
	{
		//printf("Got some data\n");
		//fp = fopen("downloaded.file", "wb");
		//fwrite(memBuffer, 1, fileSize, fp);
		//fclose(fp);
		delete(memBuffer);
		delete(headerBuffer);
	}

	WSACleanup();
	return 0;
}


void MParseURL(char *mUrl, string &serverName, string &filepath, string &filename)
{
	string::size_type n;
	string url = mUrl;

	if (url.substr(0, 7) == "http://")
		url.erase(0, 7);

	if (url.substr(0, 8) == "https://")
		url.erase(0, 8);

	n = url.find('/');
	if (n != string::npos)
	{
		serverName = url.substr(0, n);
		filepath = url.substr(n);
		n = filepath.rfind('/');
		filename = filepath.substr(n + 1);
	}

	else
	{
		serverName = url;
		filepath = "/";
		filename = "";
	}
}

SOCKET ConnectToServer(char *szServerName, WORD portNum)
{
	struct hostent *hp;
	unsigned int addr;
	struct sockaddr_in server;
	SOCKET conn;

	conn = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (conn == INVALID_SOCKET)
		return NULL;

	if (inet_addr(szServerName) == INADDR_NONE)
	{
		hp = gethostbyname(szServerName);
	}
	else
	{
		addr = inet_addr(szServerName);
		hp = gethostbyaddr((char*)&addr, sizeof(addr), AF_INET);
	}

	if (hp == NULL)
	{
		closesocket(conn);
		return NULL;
	}

	server.sin_addr.s_addr = *((unsigned long*)hp->h_addr);
	server.sin_family = AF_INET;
	server.sin_port = htons(portNum);
	if (connect(conn, (struct sockaddr*)&server, sizeof(server)))
	{
		closesocket(conn);
		return NULL;
	}
	return conn;
}

int GetHeaderLength(char *content)
{
	const char *srchStr1 = "\r\n\r\n", *srchStr2 = "\n\r\n\r";
	char *findPos;
	int ofset = -1;

	findPos = strstr(content, srchStr1);
	if (findPos != NULL)
	{
		ofset = findPos - content;
		ofset += strlen(srchStr1);
	}

	else
	{
		findPos = strstr(content, srchStr2);
		if (findPos != NULL)
		{
			ofset = findPos - content;
			ofset += strlen(srchStr2);
		}
	}
	return ofset;
}

char *ReadURL2(char *szUrl, long &bytesReturnedOut, char **headerOut)
{
	const int bufSize = 512;
	char readBuffer[bufSize], sendBuffer[bufSize], tmpBuffer[bufSize];
	char *tmpResult = NULL, *result;
	SOCKET conn;
	string server, filepath, filename;
	long totalBytesRead, thisReadSize, headerLen;

	MParseURL(szUrl, server, filepath, filename);

	///////////// step 1, connect //////////////////////
	conn = ConnectToServer((char*)server.c_str(), 80);

	///////////// step 2, send GET request /////////////
	sprintf(tmpBuffer, "GET %s HTTP/1.0", filepath.c_str());
	strcpy(sendBuffer, tmpBuffer);
	strcat(sendBuffer, "\r\n");
	sprintf(tmpBuffer, "Host: %s", server.c_str());
	strcat(sendBuffer, tmpBuffer);
	strcat(sendBuffer, "\r\n");
	strcat(sendBuffer, "\r\n");
	send(conn, sendBuffer, strlen(sendBuffer), 0);

	//    SetWindowText(edit3Hwnd, sendBuffer);
	printf("Buffer being sent:\n%s", sendBuffer);

	///////////// step 3 - get received bytes ////////////////
	// Receive until the peer closes the connection
	totalBytesRead = 0;
	wxBusyCursor bgcrsr;
	while (1)
	{
		memset(readBuffer, 0, bufSize);
		thisReadSize = recv(conn, readBuffer, bufSize, 0);

		if (thisReadSize <= 0)
			break;

		tmpResult = (char*)realloc(tmpResult, thisReadSize + totalBytesRead);

		memcpy(tmpResult + totalBytesRead, readBuffer, thisReadSize);
		totalBytesRead += thisReadSize;
	}

	headerLen = GetHeaderLength(tmpResult);
	long contenLen = totalBytesRead - headerLen;
	result = new char[contenLen + 1];
	memcpy(result, tmpResult + headerLen, contenLen);
	result[contenLen] = 0x0;
	char *myTmp;

	myTmp = new char[headerLen + 1];
	strncpy(myTmp, tmpResult, headerLen);
	myTmp[headerLen] = NULL;
	delete(tmpResult);
	*headerOut = myTmp;

	bytesReturnedOut = contenLen;
	closesocket(conn);
	return(result);
}