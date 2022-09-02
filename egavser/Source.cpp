#include "StartProcess.h"
#include "common.h"

//CUSTOM MESSAGE FOR SERVICE TO LAUNCH AN APP INTO SESSION 1 
#define SERVICE_CONTROL_CUSTOM_MESSAGE			0x0085
#define SERVICE_CONTROL_CUSTOM_MSG_STOP_SERVICE 0x0088
#define SERVICE_CONTROL_CUSTOM_MSG_UPDATE_VSD   0x0089
#define SERVICE_CONTROL_CUSTOM_MSG_RESTART_IT   0x0090
#define SERVICE_CONTROL_CUSTOM_MSG_STOP_CLAMD   0x0091
#define SERVICE_CONTROL_CUSTOM_MSG_START_CLAMD  0x0092

//service name
#define SERVICE_NAME_EG_ANTI_VIRUS L"WinSEGAV AutoConfig"
BOOL LaunchAppIntoDifferentSession(LPCWSTR strExeWithDir);

SERVICE_STATUS m_ServiceStatus;
SERVICE_STATUS_HANDLE m_ServiceStatusHandle;
HANDLE g_ServiceStopEvent = INVALID_HANDLE_VALUE;

BOOL bRunning = true;
void WINAPI ServiceMain(DWORD argc, LPTSTR *argv);
void WINAPI ServiceCtrlHandler(DWORD Opcode);
DWORD WINAPI ServiceWorkerThread(LPVOID lpParam);
BOOL InstallService();
BOOL DeleteService();

using namespace std;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	if (argc>1)
	{
		if (wcscmp(argv[1], L"-i") == 0)
		{
			if (InstallService())
			{
				SuccessServiceLogW(L"Service Installed Sucessfully");
			}

			else
			{
				ErrorServiceLogW(L"Error Installing Service");
			}
		}
		else if (wcscmp(argv[1], L"-d") == 0)
		{
			if (DeleteService())
			{
				SuccessServiceLogW(L"Service UnInstalled Sucessfully");
			}
			else
			{
				ErrorServiceLogW(L"Error UnInstalling Service");
			}
		}
		else
		{
			ErrorServiceLogW(L"\n\nUnknown Switch Usage\n\nFor Install use egavser -i\n\nFor UnInstall use egavser -d\n");
		}
	}
	else
	{
		SERVICE_TABLE_ENTRY DispatchTable[] =
		{ { SERVICE_NAME_EG_ANTI_VIRUS, ServiceMain }, { NULL, NULL } };
		StartServiceCtrlDispatcher(DispatchTable);
	}
	return 0;
}

void WINAPI ServiceMain(DWORD argc, LPTSTR *argv)
{
	m_ServiceStatus.dwServiceType = SERVICE_WIN32;
	m_ServiceStatus.dwCurrentState = SERVICE_START_PENDING;
	m_ServiceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP;
	m_ServiceStatus.dwWin32ExitCode = 0;
	m_ServiceStatus.dwServiceSpecificExitCode = 0;
	m_ServiceStatus.dwCheckPoint = 0;
	m_ServiceStatus.dwWaitHint = 0;

	m_ServiceStatusHandle = RegisterServiceCtrlHandler(SERVICE_NAME_EG_ANTI_VIRUS,
		ServiceCtrlHandler);
	if (m_ServiceStatusHandle == (SERVICE_STATUS_HANDLE)0)
	{
		return;
	}
	m_ServiceStatus.dwCurrentState = SERVICE_RUNNING;
	m_ServiceStatus.dwCheckPoint = 0;
	m_ServiceStatus.dwWaitHint = 0;
	if (!SetServiceStatus(m_ServiceStatusHandle, &m_ServiceStatus))
	{
	}

	// Create stop event to wait on later.
	g_ServiceStopEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	if (g_ServiceStopEvent == NULL)
	{
		ErrorServiceLogW(_T("ServiceMain: CreateEvent(g_ServiceStopEvent) returned error"));

		m_ServiceStatus.dwControlsAccepted = 0;
		m_ServiceStatus.dwCurrentState = SERVICE_STOPPED;
		m_ServiceStatus.dwWin32ExitCode = GetLastError();
		m_ServiceStatus.dwCheckPoint = 1;

		if (SetServiceStatus(m_ServiceStatusHandle, &m_ServiceStatus) == FALSE)
		{
			ErrorServiceLogW(_T("ServiceMain: SetServiceStatus returned error"));
		}
		return;
	}

	// Tell the service controller we are started
	m_ServiceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP;
	m_ServiceStatus.dwCurrentState = SERVICE_RUNNING;
	m_ServiceStatus.dwWin32ExitCode = 0;
	m_ServiceStatus.dwCheckPoint = 0;

	if (SetServiceStatus(m_ServiceStatusHandle, &m_ServiceStatus) == FALSE)
	{
		ErrorServiceLogW(_T("ServiceMain: SetServiceStatus returned error"));
	}

	// Start the thread that will perform the main task of the service
	HANDLE hThread = CreateThread(NULL, 0, ServiceWorkerThread, NULL, 0, NULL);

	//OutputDebugString(_T("WinSEGAV AutoConfig: ServiceMain: Waiting for Worker Thread to complete"));

	// Wait until our worker thread exits effectively signaling that the service needs to stop
	WaitForSingleObject(hThread, INFINITE);

	//OutputDebugString(_T("WinSEGAV AutoConfig: ServiceMain: Worker Thread Stop Event signaled"));
	/*
	* Perform any cleanup tasks
	*/
	//OutputDebugString(_T("WinSEGAV AutoConfig: ServiceMain: Performing Cleanup Operations"));

	CloseHandle(g_ServiceStopEvent);

	m_ServiceStatus.dwControlsAccepted = 0;
	m_ServiceStatus.dwCurrentState = SERVICE_STOPPED;
	m_ServiceStatus.dwWin32ExitCode = 0;
	m_ServiceStatus.dwCheckPoint = 3;

	if (SetServiceStatus(m_ServiceStatusHandle, &m_ServiceStatus) == FALSE)
	{
		ErrorServiceLogW(_T("ServiceMain: SetServiceStatus returned error"));
	}

	/*
	bRunning = true;
	while (bRunning)
	{
	Sleep(3000);
	//Place Your Code for processing here....
	}
	*/
	return;
}

DWORD WINAPI ServiceWorkerThread(LPVOID lpParam)
{
	//bool bEGAVSysRunning;
	BOOL bEGAVTrayRunning;
	bool bEGAVASRunning;

	//OutputDebugString(_T("WinSEGAV AutoConfig: ServiceWorkerThread: Entry"));

	//  Periodically check if the service has been requested to stop

	///////////////////////////////////////////////////////////////////////////////
	std::wstring prgPath;
	GetProgramFilesFolderPath(prgPath);
	prgPath.append(L"\\EG1\\EGClamNetAV");
	//std::wstring SysExe = prgPath;
	//SysExe.append(L"\\egavsys.exe");
	std::wstring TrayExe = prgPath;
	TrayExe.append(L"\\egavtray.exe");
	std::wstring ASExe = prgPath;
	ASExe.append(L"\\egavas.exe");
	//ErrorServiceLogW(L"prgPath in source file->%ws", prgPath.c_str());
	//////////////////////////////////////////////////////////////////////////////
	DWORD explorerPids[256];
	while (WaitForSingleObject(g_ServiceStopEvent, 0) != WAIT_OBJECT_0)
	{
		/*
		* Perform main service function here
		*/
		//bEGAVSysRunning = IsProcessRunningW(L"egavsys.exe");

		size_t CountExplorerExe = GetAllExploreProcess(explorerPids);
		for (size_t i = 0; i < CountExplorerExe; i++)
		{
			bEGAVTrayRunning = IsProcessRunningFromUserExploreId(L"egavtray.exe", explorerPids[i]);
			if (!bEGAVTrayRunning)
				StartProcessAndBypassUAC((WCHAR*)(TrayExe.c_str()), explorerPids[i]);
		}

		/*
		if (!bEGAVTrayRunning)
		{
			//StartProcessAndBypassUAC((WCHAR*)(TrayExe.c_str()), exepid);
			ImpersonateActiveUserAndRun((WCHAR*)TrayExe.c_str());
		}
		*/

		/*if (!bEGAVSysRunning)
		{
			PROCESS_INFORMATION ProcessInfo;
			STARTUPINFO StartupInfo;
			memset(&StartupInfo, 0, sizeof(StartupInfo));
			StartupInfo.cb = INT32(sizeof(StartupInfo));

			StartupInfo.lpDesktop = L"winsta0\\default";
			StartupInfo.dwFlags = STARTF_USESHOWWINDOW;
			StartupInfo.wShowWindow = SW_HIDE;

			WCHAR strcmd[128];
			wsprintf(strcmd, L"--hidden");

			if (!CreateProcessW(SysExe.c_str(), strcmd, NULL, NULL, FALSE,
				NORMAL_PRIORITY_CLASS | CREATE_UNICODE_ENVIRONMENT,
				NULL,
				prgPath.c_str(),
				&StartupInfo,
				&ProcessInfo))

			{
				std::string errstr;
				GetLastErrorAsString(GetLastError(), errstr);
				ErrorServiceLogW(L"Error:CreateProcess(egavsys.exe): %S", errstr.c_str());
				return false;
			}

			CloseHandle(ProcessInfo.hThread);
			CloseHandle(ProcessInfo.hProcess);
		}*/

		if (IsServiceRunningW(L"ClamD"))
		{
			bEGAVASRunning = IsProcessRunningW(L"egavas.exe");

			if (!bEGAVASRunning)
			{
				PROCESS_INFORMATION ProcessInfo;
				STARTUPINFO StartupInfo;
				memset(&StartupInfo, 0, sizeof(StartupInfo));
				StartupInfo.cb = INT32(sizeof(StartupInfo));

				StartupInfo.lpDesktop = L"winsta0\\default";
				StartupInfo.dwFlags = STARTF_USESHOWWINDOW;
				StartupInfo.wShowWindow = SW_HIDE;

				WCHAR strcmd[128];
				wsprintf(strcmd, L"--hidden");

				if (!CreateProcessW(ASExe.c_str(), strcmd, NULL, NULL, FALSE,
					NORMAL_PRIORITY_CLASS | CREATE_UNICODE_ENVIRONMENT,
					NULL,
					prgPath.c_str(),
					&StartupInfo,
					&ProcessInfo))

				{
					std::string errstr;
					GetLastErrorAsString(GetLastError(), errstr);
					ErrorServiceLogW(L"Error:CreateProcess(egavas.exe): %S", errstr.c_str());
					return false;
				}

				CloseHandle(ProcessInfo.hThread);
				CloseHandle(ProcessInfo.hProcess);
			}
		}
		
		Sleep(20000);
	}

	//OutputDebugString(_T("WinSEGAV AutoConfig: ServiceWorkerThread: Exit"));

	return ERROR_SUCCESS;
}

void WINAPI ServiceCtrlHandler(DWORD Opcode)
{
	/*WCHAR strBlockerExe[MAX_PATH];
	GetSystemDirectoryW(strBlockerExe, MAX_PATH);
	wcscat(strBlockerExe, L"\\scbegav.exe");*/
	WCHAR strUpdateExe[MAX_PATH];
	GetModuleFileNameW(NULL, strUpdateExe, MAX_PATH);
	PathRemoveFileSpecW(strUpdateExe);
	wcscat(strUpdateExe, L"\\egavupdate.exe");
	WCHAR strRestartApp[MAX_PATH];
	GetModuleFileNameW(NULL, strRestartApp, MAX_PATH);
	PathRemoveFileSpecW(strRestartApp);
	wcscat(strRestartApp, L"\\egavservicerestart.exe");

	switch (Opcode)
	{
	case SERVICE_CONTROL_CUSTOM_MSG_START_CLAMD:
		ExecuteCmdUACSilentW(L"sc.exe", L"start \"ClamD\"", L"");
		break;
	case SERVICE_CONTROL_CUSTOM_MSG_STOP_CLAMD:
		ExecuteCmdUACSilentW(L"sc.exe", L"stop \"ClamD\"", L"");
		break;
	case SERVICE_CONTROL_CUSTOM_MSG_RESTART_IT:
		SuccessServiceLogW(L"SERVICE_CONTROL_CUSTOM_MSG SERVICE RESTART RECEIVED");
		LaunchAppIntoDifferentSession(strRestartApp);
		break;
	case SERVICE_CONTROL_CUSTOM_MSG_UPDATE_VSD:
		SuccessServiceLogW(L"SERVICE_CONTROL_CUSTOM_MSG UPDATE VSD RECEIVED");
		LaunchAppIntoDifferentSession(strUpdateExe);
		break;
	case SERVICE_CONTROL_PAUSE:
		m_ServiceStatus.dwCurrentState = SERVICE_PAUSED;
		break;
	case SERVICE_CONTROL_CONTINUE:
		m_ServiceStatus.dwCurrentState = SERVICE_RUNNING;
		break;
	case SERVICE_CONTROL_STOP:
	case SERVICE_CONTROL_CUSTOM_MSG_STOP_SERVICE:
		SuccessServiceLogW(L"SERVICE_CONTROL_STOP MSG RECEIVED");
		bRunning = false;
		ExecuteCmdUACSilentW(L"sc.exe", L"stop \"ClamD\"", L"");
		ExecuteCmdUACSilentW(L"sc.exe", L"stop \"FreshClam\"", L"");
		m_ServiceStatus.dwWin32ExitCode = 0;
		m_ServiceStatus.dwCurrentState = SERVICE_STOPPED;
		m_ServiceStatus.dwCheckPoint = 0;
		m_ServiceStatus.dwWaitHint = 0;

		SetServiceStatus(m_ServiceStatusHandle, &m_ServiceStatus);
		break;
	}
	return;
}

BOOL InstallService()
{
	WCHAR strDir[1024];
	SC_HANDLE schSCManager, schService;
	GetCurrentDirectory(1024, strDir);
	wcscat(strDir, L"\\egavser.exe");
	schSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

	if (schSCManager == NULL)
		return false;
	LPCTSTR lpszBinaryPathName = strDir;

	schService = CreateService(schSCManager, SERVICE_NAME_EG_ANTI_VIRUS,
		L"Windows Service For EG ClamNet AntiVirus", // service name to display
		SERVICE_ALL_ACCESS, // desired access 
		SERVICE_WIN32_OWN_PROCESS, // service type 
		SERVICE_AUTO_START, // start type 
		SERVICE_ERROR_NORMAL, // error control type 
		lpszBinaryPathName, // service's binary 
		NULL, // no load ordering group 
		NULL, // no tag identifier 
		NULL, // no dependencies
		NULL, // LocalSystem account
		NULL); // no password

	if (schService == NULL)
		return false;

	::CloseServiceHandle(schService);
	return true;
}

BOOL DeleteService()
{
	SC_HANDLE schSCManager;
	SC_HANDLE hService;
	schSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

	if (schSCManager == NULL)
		return false;
	hService = OpenService(schSCManager, SERVICE_NAME_EG_ANTI_VIRUS, SERVICE_ALL_ACCESS);
	if (hService == NULL)
		return false;
	if (DeleteService(hService) == 0)
		return false;
	if (CloseServiceHandle(hService) == 0)
		return false;

	return true;
}

////////////////////////////////////////////////////////////////////////////////////
//Added By Jaisvar on 04/11/07 to Launch the process into a different session

/////////////////////////////////////////////////////////////////////////  
BOOL LaunchAppIntoDifferentSession(LPCWSTR strExeWithDir)
{
	PROCESS_INFORMATION pi;
	STARTUPINFO si;
	BOOL bResult = FALSE;
	DWORD dwSessionId, winlogonPid;
	HANDLE hUserToken, hUserTokenDup, hPToken, hProcess;
	DWORD dwCreationFlags;

	// Log the client on to the local computer.

	dwSessionId = WTSGetActiveConsoleSessionId();

	//////////////////////////////////////////
	// Find the winlogon process
	////////////////////////////////////////

	PROCESSENTRY32 procEntry;

	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnap == INVALID_HANDLE_VALUE)
	{
		ErrorServiceLogW(L"CreateToolhelp32Snapshot failed");
		return 1;
	}

	procEntry.dwSize = sizeof(PROCESSENTRY32);

	if (!Process32First(hSnap, &procEntry))
	{
		return 1;
	}

	do
	{
		if (_wcsicmp(procEntry.szExeFile, L"winlogon.exe") == 0)
		{
			// We found a winlogon process...make sure it's running in the console session
			DWORD winlogonSessId = 0;
			if (ProcessIdToSessionId(procEntry.th32ProcessID, &winlogonSessId) && winlogonSessId == dwSessionId)
			{
				winlogonPid = procEntry.th32ProcessID;
				break;
			}
		}

	} while (Process32Next(hSnap, &procEntry));

	////////////////////////////////////////////////////////////////////////

	WTSQueryUserToken(dwSessionId, &hUserToken);
	dwCreationFlags = NORMAL_PRIORITY_CLASS | CREATE_NO_WINDOW;
	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);
	si.lpDesktop = L"winsta0\\default";
	ZeroMemory(&pi, sizeof(pi));
	TOKEN_PRIVILEGES tp;
	LUID luid;
	hProcess = OpenProcess(MAXIMUM_ALLOWED, FALSE, winlogonPid);

	if (!::OpenProcessToken(hProcess, TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY
		| TOKEN_DUPLICATE | TOKEN_ASSIGN_PRIMARY | TOKEN_ADJUST_SESSIONID
		| TOKEN_READ | TOKEN_WRITE, &hPToken))
	{
		int abcd = GetLastError();
		ErrorServiceLogW(L"Process token open Error: %u\n", GetLastError());
	}

	if (!LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &luid))
	{
		ErrorServiceLogW(L"Lookup Privilege value Error: %u\n", GetLastError());
	}
	tp.PrivilegeCount = 1;
	tp.Privileges[0].Luid = luid;
	tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	DuplicateTokenEx(hPToken, MAXIMUM_ALLOWED, NULL, SecurityIdentification, TokenPrimary, &hUserTokenDup);
	int dup = GetLastError();

	//Adjust Token privilege
	SetTokenInformation(hUserTokenDup, TokenSessionId, (void*)dwSessionId, sizeof(DWORD));

	if (!AdjustTokenPrivileges(hUserTokenDup, FALSE, &tp, sizeof(TOKEN_PRIVILEGES), (PTOKEN_PRIVILEGES)NULL, NULL))
	{
		int abc = GetLastError();
		ErrorServiceLogW(L"Adjust Privilege value Error: %u\n", GetLastError());
	}

	if (GetLastError() == ERROR_NOT_ALL_ASSIGNED)
	{
		ErrorServiceLogW(L"Token does not have the provilege\n");
	}

	LPVOID pEnv = NULL;

	if (CreateEnvironmentBlock(&pEnv, hUserTokenDup, TRUE))
	{
		dwCreationFlags |= CREATE_UNICODE_ENVIRONMENT;
	}
	else
		pEnv = NULL;

	// Launch the process in the client's logon session.
	//WCHAR strExeWithDir[MAX_PATH];
	//GetSystemDirectoryW(strExeWithDir, MAX_PATH);
	//wcscat(strExeWithDir, L"\\scbegav.exe");
	//SuccessServiceLogW(L"LaunchAppIntoDifferentSession dir =%s", strDir);
	
	bResult = CreateProcessAsUserW(
		hUserTokenDup,            // client's access token
		strExeWithDir,                  // file to execute
		NULL,     // command line
		NULL,              // pointer to process SECURITY_ATTRIBUTES
		NULL,              // pointer to thread SECURITY_ATTRIBUTES
		FALSE,             // handles are not inheritable
		dwCreationFlags,  // creation flags
		pEnv,              // pointer to new environment block 
		NULL,              // name of current directory 
		&si,               // pointer to STARTUPINFO structure
		&pi                // receives information about new process
		);
	// End impersonation of client.

	//GetLastError Shud be 0

	int iResultOfCreateProcessAsUser = GetLastError();

	//Perform All the Close Handles task

	CloseHandle(hProcess);
	CloseHandle(hUserToken);
	CloseHandle(hUserTokenDup);
	CloseHandle(hPToken);

	return 0;
}