#ifndef COMMON_HEADER
#define COMMON_HEADER

#include <Windows.h>
#include <iostream>

#pragma comment(lib,"psapi.lib")
#pragma comment(lib,"userenv.lib")

#define DELETE_POINTER(TypeName,varName) delete varName; varName = (TypeName*)(NULL)

#define DELETE_POINTER_ARRAY(TypeName,varName) delete[] varName; varName =(TypeName*)(NULL)

void GetUsersProfileDir(std::wstring& strout);

void GetAppDataFolderPath(std::wstring& AppDataFolderPath);

void GetHistoryFolderPath(std::wstring& strOut);

void GetLocalAppDataFolderPath(std::wstring& strOut);

void GetRoamingAppDataFolderPath(std::wstring& strOut);

void GetSystemFolderPath(std::wstring& strOut);

void GetSystemx86FolderPath(std::wstring& strOut);

void WriteLogsW(const WCHAR *filename, const WCHAR *fmt, ...);

void GetLastErrorAsString(DWORD errorMessageID, std::string& ErrorInStdStr);

#define ErrorServiceLogW(...) WriteLogsW(L"ErrorServiceLogs.txt",__VA_ARGS__)
#define ErrorBlockerLogW(...) WriteLogsW(L"ErrorBlockerLogs.txt",__VA_ARGS__)
#define ErrorAppConLogW(...) WriteLogsW(L"ErrorConsoleLogs.txt",__VA_ARGS__)
#define ErrorAppWinLogW(...) WriteLogsW(L"ErrorWindowLogs.txt",__VA_ARGS__)
#define ErrorAppTrayLogW(...) WriteLogsW(L"ErrorTrayLogs.txt",__VA_ARGS__)
#define ErrorShellExecLogsW(...) WriteLogsW(L"ErrorCommandLogs.txt",__VA_ARGS__)
#define ErrorFunctionLogsW(...) WriteLogsW(L"ErrorFunctionLogs.txt",__VA_ARGS__)
#define ErrorInternetLogsW(...) WriteLogsW(L"ErrorInternetLogsW.txt",__VA_ARGS__)
#define ErrorAppUpdateLogsW(...) WriteLogsW(L"ErrorAppUpdateLogsW.txt",__VA_ARGS__)

#define SuccessFunctionLogsW(...) WriteLogsW(L"SuccessFunctionLogs.txt",__VA_ARGS__)
#define SuccessServiceLogW(...) WriteLogsW(L"SuccessServiceLogs.txt",__VA_ARGS__)
#define SuccessAppConLogW(...) WriteLogsW(L"SuccessAppConLogs.txt",__VA_ARGS__)
#define SuccessAppWinLogW(...) WriteLogsW(L"SuccessAppWinLogs.txt",__VA_ARGS__)
#define SuccessAppTrayLogW(...) WriteLogsW(L"SuccessAppTrayLogs.txt",__VA_ARGS__)
#define SuccessBlockerLogW(...) WriteLogsW(L"SuccessBlockerLogs.txt",__VA_ARGS__)
#define SuccessInternetLogsW(...) WriteLogsW(L"SuccessInternetLogsW.txt",__VA_ARGS__)
#define SuccessAppUpdateLogsW(...) WriteLogsW(L"SuccessAppUpdateLogsW.txt",__VA_ARGS__)


static bool Is_Base64(unsigned char c);
void Base64_Encode(unsigned char const* bytes_to_encode, unsigned int in_len, std::string& EncodedBytes);
void Base64_Decode(std::string const& encoded_string, std::string& DecodedBytes);
void MyEncode64(const std::string& strInput, std::string& strOutput);
int Pos(const std::string& str, char chr);
void MyDecode64(const std::string& strIn, std::string& strOut);
//CHAR *EncodeInUTF8(const WCHAR *wstr, int &size_needed);
//WCHAR *DecodeInUTF8(const CHAR *str, int &size_needed);
void Base64EncodeN(std::wstring const& wstr, std::string& EncodedStrN);
void Base64DecodeN(std::string const& str, std::wstring& DecodedStrN);
DWORD ExecuteCmdUACSilentW(const WCHAR* lpCommand, const WCHAR* lpCommandParm, const WCHAR* lpVerb = L"runas");
DWORD ExecuteExeUACSilentW(const WCHAR* lpCommand, const WCHAR* lpCommandParm, const WCHAR* lpDirectory, const WCHAR* lpVerb = L"runas");

DWORD ExecuteExeUACSilent1W(const WCHAR* lpCommand, const WCHAR* lpCommandParm, const WCHAR* lpDirectory, bool showConsole, const WCHAR* lpVerb = L"runas");


DWORD GetServiceStatusW(WCHAR* name, int &status);
bool IsServiceRunningW(WCHAR* servicename);
bool IsProcessRunningW(const WCHAR *procesName);
bool IsApplicationRunning(const WCHAR *procesName, DWORD &expid);
void GetProgramFilesFolderPath(std::wstring& ProgramFilesDirPath);
BOOL SetPrivilege(
	HANDLE hToken,          // access token handle
	LPCTSTR lpszPrivilege,  // name of privilege to enable/disable
	BOOL bEnablePrivilege   // to enable or disable privilege
	);
BOOL StartProcessAndBypassUAC(WCHAR* applicationName, int id);
DWORD ProtectProcess(void);
bool IsProcessRunningFromUser(const WCHAR *procesName, int sessionid);
WCHAR* GetFileNameFromFullPathW(WCHAR * path);
void GetFolderPathFromFileFullPathW(WCHAR* fileFullpath, std::wstring& folderPath);
bool GetProcessFileName(HANDLE hProc, std::wstring& ProcessFileName);
void RemoveWhiteSpace(std::string& stdstr);
#endif