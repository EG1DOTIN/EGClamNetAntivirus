#include "StartProcess.h"
#include "common.h"


DWORD GetCurrentActiveUserSessionId()
{
	PWTS_SESSION_INFO pSessionInfo = 0;
	DWORD dwCount = 0, dwSessionId = 0;

	WTSEnumerateSessions(WTS_CURRENT_SERVER_HANDLE, 0, 1, &pSessionInfo, &dwCount);

	int dataSize = sizeof(WTS_SESSION_INFO);

	for (DWORD i = 0; i < dwCount; ++i)
	{
		WTS_SESSION_INFO si = pSessionInfo[i];
		if (WTSActive == si.State)
		{
			dwSessionId = si.SessionId;
			break;
		}
	}

	WTSFreeMemory(pSessionInfo);

	return dwSessionId;
}

bool IsProcessRunningFromCurrentActiveUser(const WCHAR* processName)
{
	DWORD dwcurrentsessid = GetCurrentActiveUserSessionId();
	return IsProcessRunningFromUser(processName, dwcurrentsessid);
}

BOOL IsProcessRunningFromUserExploreId(const WCHAR *procesName, DWORD explorerId)
{
	DWORD sessionidprocess;
	if (0 == ProcessIdToSessionId(explorerId, &sessionidprocess))
	{
		//ErrorFunctionLogsW(L"IsProcessRunningFromUser: Error in ProcessIdToSessionId: %u", GetLastError());
		return -1;
	}
	bool res = IsProcessRunningFromUser(procesName, sessionidprocess);

	return (BOOL)(res);
}


void RunExeAsUserSession(WCHAR* exeFullPath, int sessionid)
{
	WCHAR* exeName = GetFileNameFromFullPathW(exeFullPath);

	if (IsProcessRunningFromUser(exeName, sessionid))
	{
		//WriteLogA("exe already running .");
		return;
	}

	HANDLE hToken;
	WTSQueryUserToken(sessionid, &hToken);

	HANDLE hUserTokenDup;
	SECURITY_ATTRIBUTES sa;
	memset(&sa, 0, sizeof(sa));
	sa.nLength = sizeof(sa);

	// copy the access token of the winlogon process; the newly created token will be a primary token
	if (!DuplicateTokenEx(hToken, TOKEN_ASSIGN_PRIMARY | TOKEN_ALL_ACCESS, &sa, SECURITY_IMPERSONATION_LEVEL::SecurityIdentification, TOKEN_TYPE::TokenPrimary, &hUserTokenDup))
	{
		std::string errstr;
		GetLastErrorAsString(GetLastError(), errstr);
		ErrorFunctionLogsW(L"RunExeAsUserSession: Error in DuplicateTokenEx: %S", errstr.c_str());
		//CloseHandle(hProcess);
		CloseHandle(hToken);
		return;
	}

	SetPrivilege(hUserTokenDup,/* access token handle*/
		SE_TCB_NAME,/* name of privilege to enable/disable*/
		1 /* to enable or disable privilege*/);

	void* lpEnvironment = NULL;

	// Get all necessary environment variables of logged in user
	// to pass them to the process
	BOOL resultEnv = CreateEnvironmentBlock(&lpEnvironment, hToken, FALSE);
	if (resultEnv == 0)
	{
		std::string errstr;
		GetLastErrorAsString(GetLastError(), errstr);
		ErrorFunctionLogsW(L"RunExeAsUserSession: Error in CreateEnvironmentBlock: %S", errstr.c_str());
		CloseHandle(hToken);
		CloseHandle(hUserTokenDup);
		return;
	}

	PROCESS_INFORMATION procInfo;
	STARTUPINFOW si;
	memset(&si, 0, sizeof(si));
	si.cb = INT32(sizeof(si));

	si.lpDesktop = L"winsta0\\default"; // interactive window station parameter; basically this indicates that the process created can display a GUI on the desktop

	// flags that specify the priority and creation method of the process
	int dwCreationFlags = NORMAL_PRIORITY_CLASS | CREATE_UNICODE_ENVIRONMENT;
	wstring EXEFolderPath;
	GetFolderPathFromFileFullPathW(exeFullPath, EXEFolderPath);
	const WCHAR* exeFolderPath = EXEFolderPath.c_str();
	if (0 == CreateProcessAsUserW(hUserTokenDup, exeFullPath, NULL, NULL, NULL, false, dwCreationFlags, lpEnvironment, exeFolderPath, &si, &procInfo))
	{
		std::string errstr;
		GetLastErrorAsString(GetLastError(), errstr);
		ErrorFunctionLogsW(L"RunExeAsUserSession: Function CreateProcessAsUserW Failed : %S", errstr.c_str());
		CloseHandle(hToken);
		CloseHandle(hUserTokenDup);
		return;
	}
}

PHANDLE GetCurrentUserToken()
{
	HANDLE currentToken;
	PHANDLE primaryToken = 0;

	int dwSessionId = 0;
	PHANDLE hUserToken = 0;
	PHANDLE hTokenDup = 0;

	PWTS_SESSION_INFO pSessionInfo = 0;
	DWORD dwCount = 0;

	// Get the list of all terminal sessions 
	WTSEnumerateSessions(WTS_CURRENT_SERVER_HANDLE, 0, 1,
		&pSessionInfo, &dwCount);

	int dataSize = sizeof(WTS_SESSION_INFO);

	// look over obtained list in search of the active session
	for (DWORD i = 0; i < dwCount; ++i)
	{
		WTS_SESSION_INFO si = pSessionInfo[i];
		if (WTSActive == si.State)
		{
			// If the current session is active – store its ID
			dwSessionId = si.SessionId;
			break;
		}
	}

	WTSFreeMemory(pSessionInfo);

	// Get token of the logged in user by the active session ID
	BOOL bRet = WTSQueryUserToken(dwSessionId, &currentToken);
	if (bRet == false)
	{
		std::string errstr;
		GetLastErrorAsString(GetLastError(), errstr);

		ErrorFunctionLogsW(_T("GetCurrentUserToken: Error in WTSQueryUserToken: %S"), errstr.c_str());
		return 0;
	}

	bRet = DuplicateTokenEx(currentToken,
		TOKEN_ASSIGN_PRIMARY | TOKEN_ALL_ACCESS,
		0, SecurityImpersonation, TokenPrimary, primaryToken);
	if (bRet == false)
	{
		std::string errstr;
		GetLastErrorAsString(GetLastError(), errstr);

		ErrorFunctionLogsW(_T("GetCurrentUserToken: Error in DuplicateTokenEx: %S"), errstr.c_str());
		return 0;
	}

	return primaryToken;
}

BOOL RunExeFromCurrentUser(const wstring& processPath, const wstring& arguments)
{
	// Get token of the current user
	HANDLE primaryToken = GetCurrentUserToken();
	if (primaryToken == 0)
	{
		ErrorFunctionLogsW(_T("RunExeFromCurrentUser: failed GetCurrentUserToken.."));
		return FALSE;
	}
	STARTUPINFOW StartupInfo;
	PROCESS_INFORMATION processInfo;
	StartupInfo.cb = sizeof(STARTUPINFO);

	SECURITY_ATTRIBUTES Security1;
	SECURITY_ATTRIBUTES Security2;

	wstring command = L"\"" + processPath + L"\"";
	if (arguments.length() != 0)
	{
		command += L" " + arguments;
	}

	void* lpEnvironment = NULL;

	// Get all necessary environment variables of logged in user
	// to pass them to the process
	BOOL resultEnv = CreateEnvironmentBlock(&lpEnvironment,
		primaryToken, FALSE);
	if (resultEnv == 0)
	{
		DWORD nError = GetLastError();
		ErrorFunctionLogsW(_T("RunExeFromCurrentUser: Error: CreateEnvironmentBlock : %u"), nError);
	}

	// Start the process on behalf of the current user 
	BOOL result = CreateProcessAsUserW(primaryToken, 0,
		(LPWSTR)(command.c_str()), &Security1,
		&Security2, FALSE, CREATE_NO_WINDOW | NORMAL_PRIORITY_CLASS |
		CREATE_UNICODE_ENVIRONMENT, lpEnvironment, 0,
		&StartupInfo, &processInfo);
	DestroyEnvironmentBlock(lpEnvironment);
	CloseHandle(primaryToken);
	return result;
}


bool StartInteractiveProcess(LPTSTR cmd, LPCTSTR cmdDir) 
{
	STARTUPINFO si;
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	si.lpDesktop = TEXT("winsta0\\default");  // Use the default desktop for GUIs
	PROCESS_INFORMATION pi;
	ZeroMemory(&pi, sizeof(pi));
	HANDLE token;
	DWORD sessionId = GetCurrentActiveUserSessionId();
	if (sessionId == 0xffffffff)  // Noone is logged-in
		return false;
	// This only works if the current user is the system-account (we are probably a Windows-Service)
	HANDLE dummy;
	if (::WTSQueryUserToken(sessionId, &dummy))
	{
		if (!::DuplicateTokenEx(dummy, TOKEN_ALL_ACCESS, NULL, SecurityDelegation, TokenPrimary, &token)) 
		{
			::CloseHandle(dummy);
			return false;
		}
		::CloseHandle(dummy);
		// Create process for user with desktop
		if (!::CreateProcessAsUser(token, NULL, cmd, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, cmdDir, &si, &pi)) 
		{  // The "new console" is necessary. Otherwise the process can hang our main process
			::CloseHandle(token);
			return false;
		}
		::CloseHandle(token);
	}
	// Create process for current user
	else if (!::CreateProcess(NULL, cmd, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, cmdDir, &si, &pi))  // The "new console" is necessary. Otherwise the process can hang our main process
		return false;
	// The following commented lines can be used to wait for the process to exit and terminate it
	//::WaitForSingleObject(pi.hProcess, INFINITE);
	//::TerminateProcess(pi.hProcess, 0);
	::CloseHandle(pi.hProcess);
	::CloseHandle(pi.hThread);
	return true;
}

void ImpersonateActiveUserAndRun(WCHAR* path, WCHAR* args)
{
	DWORD session_id = -1;
	DWORD session_count = 0;

	WTS_SESSION_INFOW *pSession = NULL;

	if (WTSEnumerateSessions(WTS_CURRENT_SERVER_HANDLE, 0, 1, &pSession, &session_count))
	{
		//log success
	}
	else
	{
		//log error
		return;
	}

	for (int i = 0; i < session_count; i++)
	{
		session_id = pSession[i].SessionId;

		WTS_CONNECTSTATE_CLASS wts_connect_state = WTSDisconnected;
		WTS_CONNECTSTATE_CLASS* ptr_wts_connect_state = NULL;

		DWORD bytes_returned = 0;
		if (::WTSQuerySessionInformation(
			WTS_CURRENT_SERVER_HANDLE,
			session_id,
			WTSConnectState,
			reinterpret_cast<LPTSTR*>(&ptr_wts_connect_state),
			&bytes_returned))
		{
			wts_connect_state = *ptr_wts_connect_state;
			::WTSFreeMemory(ptr_wts_connect_state);
			if (wts_connect_state != WTSActive) continue;
		}
		else
		{
			//log error
			continue;
		}

		HANDLE hImpersonationToken;

		if (!WTSQueryUserToken(session_id, &hImpersonationToken))
		{
			//log error
			continue;
		}


		//Get real token from impersonation token
		DWORD neededSize1 = 0;
		HANDLE *realToken = new HANDLE;
		if (GetTokenInformation(hImpersonationToken, (::TOKEN_INFORMATION_CLASS) TokenLinkedToken, realToken, sizeof(HANDLE), &neededSize1))
		{
			CloseHandle(hImpersonationToken);
			hImpersonationToken = *realToken;
		}
		else
		{
			//log error
			continue;
		}


		HANDLE hUserToken;

		if (!DuplicateTokenEx(hImpersonationToken,
			//0,
			//MAXIMUM_ALLOWED,
			TOKEN_ASSIGN_PRIMARY | TOKEN_ALL_ACCESS | MAXIMUM_ALLOWED,
			NULL,
			SecurityImpersonation,
			TokenPrimary,
			&hUserToken))
		{
			//log error
			continue;
		}

		// Get user name of this process
		//LPTSTR pUserName = NULL;
		WCHAR* pUserName;
		DWORD user_name_len = 0;

		if (WTSQuerySessionInformationW(WTS_CURRENT_SERVER_HANDLE, session_id, WTSUserName, &pUserName, &user_name_len))
		{
			//log username contained in pUserName WCHAR string
		}

		//Free memory                         
		if (pUserName) WTSFreeMemory(pUserName);

		ImpersonateLoggedOnUser(hUserToken);

		STARTUPINFOW StartupInfo;
		GetStartupInfoW(&StartupInfo);
		StartupInfo.cb = sizeof(STARTUPINFOW);
		//StartupInfo.lpDesktop = "winsta0\\default";

		PROCESS_INFORMATION processInfo;

		SECURITY_ATTRIBUTES Security1;
		Security1.nLength = sizeof SECURITY_ATTRIBUTES;

		SECURITY_ATTRIBUTES Security2;
		Security2.nLength = sizeof SECURITY_ATTRIBUTES;

		void* lpEnvironment = NULL;

		// Get all necessary environment variables of logged in user
		// to pass them to the new process
		BOOL resultEnv = CreateEnvironmentBlock(&lpEnvironment, hUserToken, FALSE);
		if (!resultEnv)
		{
			//log error
			continue;
		}

		WCHAR PP[1024]; //path and parameters
		ZeroMemory(PP, 1024 * sizeof WCHAR);
		wcscpy(PP, path);
		wcscat(PP, L" ");
		wcscat(PP, args);

		// Start the process on behalf of the current user 
		BOOL result = CreateProcessAsUserW(hUserToken,
			NULL,
			PP,
			//&Security1,
			//&Security2,
			NULL,
			NULL,
			FALSE,
			NORMAL_PRIORITY_CLASS | CREATE_NEW_CONSOLE,
			//lpEnvironment,
			NULL,
			//"C:\\ProgramData\\some_dir",
			NULL,
			&StartupInfo,
			&processInfo);

		if (!result)
		{
			//log error
		}
		else
		{
			//log success
		}

		DestroyEnvironmentBlock(lpEnvironment);

		CloseHandle(hImpersonationToken);
		CloseHandle(hUserToken);
		CloseHandle(realToken);

		RevertToSelf();
	}

	WTSFreeMemory(pSession);
}

//return number of exlorer processes with pid
size_t GetAllExploreProcess(DWORD* ExploId)
{
	size_t id = 0;
	PROCESSENTRY32W pe32 = { sizeof(PROCESSENTRY32W) };
	HANDLE hProcSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcSnap == INVALID_HANDLE_VALUE)
	{
		return id;
	}

	if (Process32FirstW(hProcSnap, &pe32))
	{
		do
		{
			if (0 == _wcsicmp(pe32.szExeFile, L"explorer.exe"))
			{
				ExploId[id] = pe32.th32ProcessID;
				id++;
			}
		} while (Process32NextW(hProcSnap, &pe32));
	}
	CloseHandle(hProcSnap);
	return id;
}