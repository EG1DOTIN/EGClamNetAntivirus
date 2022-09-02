#ifndef SEND_RECV_HEADER
#define SEND_RECV_HEADER

#include <windows.h>
#include <string>
#include <stdio.h>

using namespace std;

#pragma comment(lib,"ws2_32.lib")

void MParseURL(char *mUrl, string &serverName, string &filepath, string &filename);
SOCKET ConnectToServer(char *szServerName, WORD portNum);
int GetHeaderLength(char *content);
char *ReadURL2(char *szUrl, long &bytesReturnedOut, char **headerOut);
int MainProcessURl(const std::string& rkey, const std::string& mkey, int& result, int& daysusing);

int MainProcessURl20(const std::string& mkey, int& result);
#endif