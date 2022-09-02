#ifndef SE_LOG_HEADER
#define SE_LOG_HEADER

#include <iostream>
#include <ShlObj.h>
void GetAppDataFolderPath2(std::wstring& strOut);
void WriteLogsW2(const WCHAR *filename, const WCHAR *fmt, ...);

#define SuccessCommonLogsW(...)		 WriteLogsW2(L"SuccessCommonLogs.txt",__VA_ARGS__)
#define ErrorCommonLogsW(...)		 WriteLogsW2(L"ErrorCommonLogs.txt",__VA_ARGS__)
#endif