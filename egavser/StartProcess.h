#ifndef START_PROCESS_HEADER
#define START_PROCESS_HEADER

#include "Windows.h"
#include "Winsvc.h"
#include "time.h"
#include <WtsApi32.h>
#include <UserEnv.h>
#include <tlhelp32.h>
#include <tchar.h>
#include <stdio.h>
#include <Shlwapi.h>
#include <iostream>

using namespace std;


#pragma comment(lib,"WtsApi32.lib")
#pragma comment(lib,"UserEnv.lib")
#pragma comment(lib,"Shlwapi.lib")

DWORD GetCurrentActiveUserSessionId();
//bool IsProcessRunningFromUser(const WCHAR *procesName, int sessionid);
bool IsProcessRunningFromCurrentActiveUser(const WCHAR* processName);
void RunExeAsUserSession(WCHAR* exeFullPath, int sessionid);
PHANDLE GetCurrentUserToken();
BOOL RunExeFromCurrentUser(const wstring& processPath, const wstring& arguments);
bool StartInteractiveProcess(LPTSTR cmd, LPCTSTR cmdDir);
void ImpersonateActiveUserAndRun(WCHAR* path, WCHAR* args = L"");
BOOL IsProcessRunningFromUserExploreId(const WCHAR *procesName, DWORD explorerId);
size_t GetAllExploreProcess(DWORD* ExploId);
#endif