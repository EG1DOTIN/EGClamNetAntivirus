#include "common.h"
#include <ShlObj.h>
#include <TlHelp32.h>
#include <tchar.h>
#include <process.h>
#include <AclAPI.h>
#include <Psapi.h>
#include <UserEnv.h>
#include <algorithm>

void GetUsersProfileDir(std::wstring& strout)
{
	DWORD proDirSize = MAX_PATH;
	LPWSTR profileDir = new WCHAR[proDirSize];
	GetProfilesDirectoryW(profileDir, &proDirSize);
	strout = profileDir;
	if (profileDir)
	{
		DELETE_POINTER_ARRAY(WCHAR, profileDir);
	}
}

void GetAppDataFolderPath(std::wstring& AppDataFolderPath)
{
	LPWSTR AppDataPath;

#ifdef BUILD_4XP
	AppDataPath = new WCHAR[MAX_PATH];
	SHGetSpecialFolderPath(0, AppDataPath, CSIDL_COMMON_APPDATA, FALSE);
	AppDataFolderPath = AppDataPath;
	if (AppDataPath)
	{
		DELETE_POINTER_ARRAY(WCHAR, AppDataPath);
	}
#else	 
	HRESULT hr = SHGetKnownFolderPath(FOLDERID_ProgramData, 0, NULL, &AppDataPath);

	if (FAILED(hr))
	{
		// error handling
		ErrorFunctionLogsW(L"In GetAppDataFolderPath, SHGetKnownFolderPath failed.");
		return;
	}
	AppDataFolderPath = AppDataPath;
#endif
}

void GetHistoryFolderPath(std::wstring& strOut)
{
	LPWSTR path;
#	ifdef BUILD_4XP
	path = new WCHAR[MAX_PATH];
	SHGetSpecialFolderPath(0, path, CSIDL_HISTORY, FALSE);
	strOut = path;
	if (path)
	{
		delete[] path;
		path = (WCHAR*)(NULL);
	}
#	else	 
	HRESULT hr = SHGetKnownFolderPath(FOLDERID_History, 0, NULL, &path);
	if (FAILED(hr))
	{
		// error handling
		ErrorFunctionLogsW(L"In GetHistoryFolderPath, SHGetKnownFolderPath failed.");
		return;
	}
	strOut = path;
#	endif
}

void GetLocalAppDataFolderPath(std::wstring& strOut)
{
	LPWSTR path;
#	ifdef BUILD_4XP
	path = new WCHAR[MAX_PATH];
	SHGetSpecialFolderPath(0, path, CSIDL_LOCAL_APPDATA, FALSE);
	strOut = path;
	if (path)
	{
		delete[] path;
		path = (WCHAR*)(NULL);
	}
#	else	 
	HRESULT hr = SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, NULL, &path);
	if (FAILED(hr))
	{
		// error handling
		ErrorFunctionLogsW(L"In GetLocalAppDataFolderPath, SHGetKnownFolderPath failed.");
		return;
	}
	strOut = path;
#	endif
}

void GetRoamingAppDataFolderPath(std::wstring& strOut)
{
	LPWSTR path;
#	ifdef BUILD_4XP
	path = new WCHAR[MAX_PATH];
	SHGetSpecialFolderPath(0, path, CSIDL_APPDATA, FALSE);
	strOut = path;
	if (path)
	{
		delete[] path;
		path = (WCHAR*)(NULL);
	}
#	else	 
	HRESULT hr = SHGetKnownFolderPath(FOLDERID_RoamingAppData, 0, NULL, &path);
	if (FAILED(hr))
	{
		// error handling
		ErrorFunctionLogsW(L"In GetRoamingAppDataFolderPath, SHGetKnownFolderPath failed.");
		return;
	}
	strOut = path;
#	endif
}

void GetSystemFolderPath(std::wstring& strOut)
{
	LPWSTR path;
#	ifdef BUILD_4XP
	path = new WCHAR[MAX_PATH];
	SHGetSpecialFolderPath(0, path, CSIDL_SYSTEM, FALSE);
	strOut = path;
	if (path)
	{
		delete[] path;
		path = (WCHAR*)(NULL);
	}
#	else	 
	HRESULT hr = SHGetKnownFolderPath(FOLDERID_System, 0, NULL, &path);
	if (FAILED(hr))
	{
		// error handling
		ErrorFunctionLogsW(L"In GetSystemFolderPath, SHGetKnownFolderPath failed.");
		return;
	}
	strOut = path;
#	endif
}

void GetSystemx86FolderPath(std::wstring& strOut)
{
	LPWSTR path;
#	ifdef BUILD_4XP
	path = new WCHAR[MAX_PATH];
	SHGetSpecialFolderPath(0, path, CSIDL_SYSTEMX86, FALSE);
	strOut = path;
	if (path)
	{
		delete[] path;
		path = (WCHAR*)(NULL);
	}
#	else	 
	HRESULT hr = SHGetKnownFolderPath(FOLDERID_SystemX86, 0, NULL, &path);
	if (FAILED(hr))
	{
		// error handling
		ErrorFunctionLogsW(L"In GetSystemFolderPath, SHGetKnownFolderPath failed.");
		return;
	}
	strOut = path;
#	endif
}

void WriteLogsW(const WCHAR *filename, const WCHAR *fmt, ...)
{
	WCHAR str[1024];
	std::wstring AppFolderPath;
	GetAppDataFolderPath(AppFolderPath);
	
	wcscpy(str, AppFolderPath.c_str());
	wcscat(str, L"\\EGAntiVirus\\");
	wcscat(str, filename);

	FILE *log_file = _wfopen(str, L"at");

	if (log_file)
	{
		va_list arg;
		va_start(arg, fmt);
		fwprintf(log_file, L"\r\n---------------------------------------------------------------------------\r\n");
		vfwprintf(log_file, fmt, arg);
		va_end(arg);
		fclose(log_file);
	}
}

void GetLastErrorAsString(DWORD errorMessageID, std::string& ErrorInStdStr)
{
	//Get the error message, if any.
	//	DWORD errorMessageID = ::GetLastError();
	if (errorMessageID == 0)
		return; //No error message has been recorded

	LPSTR messageBuffer = (LPSTR)(NULL);
	size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);

	std::string message(messageBuffer, size);
	
	//Free the buffer.
	LocalFree(messageBuffer);
	ErrorInStdStr = message;
}

static const std::string base64_chars("bcBdefgoh4ijUV87kSlmn-FX2EpAQGOCDIKqrYMN9Wstu3aJ6LTRvZwxyzH+05P1");

static bool Is_Base64(unsigned char c)
{
	return (isalnum(c) || (c == '+') || (c == '-'));
}

void Base64_Encode(unsigned char const* bytes_to_encode, unsigned int in_len, std::string& EncodedBytes)
{
	std::string ret;
	int i = 0;
	int j = 0;
	unsigned char char_array_3[3];
	unsigned char char_array_4[4];

	while (in_len--)
	{
		char_array_3[i++] = *(bytes_to_encode++);
		if (i == 3)
		{
			char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
			char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
			char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
			char_array_4[3] = char_array_3[2] & 0x3f;

			for (i = 0; (i < 4); i++)
				ret += base64_chars[char_array_4[i]];
			i = 0;
		}
	}

	if (i)
	{
		for (j = i; j < 3; j++)
			char_array_3[j] = '\0';

		char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
		char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
		char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
		char_array_4[3] = char_array_3[2] & 0x3f;

		for (j = 0; (j < i + 1); j++)
			ret += base64_chars[char_array_4[j]];

		while ((i++ < 3))
			ret += '=';
	}

	EncodedBytes = ret;
}

void Base64_Decode(std::string const& encoded_string, std::string& DecodedBytes)
{
	size_t in_len = encoded_string.size();
	int i = 0;
	int j = 0;
	int in_ = 0;
	unsigned char char_array_4[4], char_array_3[3];
	std::string ret;

	while (in_len-- && (encoded_string[in_] != '=') && Is_Base64(encoded_string[in_]))
	{
		char_array_4[i++] = encoded_string[in_]; in_++;
		if (i == 4)
		{
			for (i = 0; i < 4; i++)
				char_array_4[i] = (unsigned char)(base64_chars.find(char_array_4[i]));

			char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
			char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
			char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

			for (i = 0; (i < 3); i++)
				ret += char_array_3[i];
			i = 0;
		}
	}

	if (i)
	{
		for (j = i; j < 4; j++)
			char_array_4[j] = 0;

		for (j = 0; j < 4; j++)
			char_array_4[j] = (unsigned char)(base64_chars.find(char_array_4[j]));

		char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
		char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
		char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

		for (j = 0; (j < i - 1); j++) ret += char_array_3[j];
	}

	DecodedBytes = ret;
}

void MyEncode64(const std::string& strInput, std::string& strOutput)
{
	int i, a, x, b;
	std::string result = "";
	a = 0;
	b = 0;
	int LengthOfS = (int)(strInput.length());
	for (i = 0; i < LengthOfS; i++)
	{
		x = BYTE(strInput[i]);
		b = b * 256 + x;
		a = a + 8;
		while (a >= 6)
		{
			a = a - 6;
			x = b / (1 << a);
			b = b % (1 << a);
			result += base64_chars[x];
		}
	}
	if (a > 0)
	{
		x = b << (6 - a);
		result += base64_chars[x];
	}
	a = result.length() % 4;
	if (a == 2)
	{
		result += "@";
	}
	else if (a == 3)
	{
		result += "=";
	}
	strOutput = result;
}

int Pos(const std::string& str, char chr)
{
	int i;
	int length = (int)(str.length());
	for (i = 0; i < length; i++)
	{
		if (str[i] == chr)
			break;
	}
	if (i >= 64)
		return -1;
	return i;
}

void MyDecode64(const std::string& strIn, std::string& strOut)
{
	int i, a, x, b;
	std::string result = "";

	a = 0;
	b = 0;

	int lengthOfs = (int)(strIn.length());

	for (i = 0; i < lengthOfs; i++)
	{
		x = Pos(base64_chars, strIn[i]);
		if (x >= 0)
		{
			b = b * 64 + x;
			a = a + 6;
			if (a >= 8)
			{
				a = a - 8;
				x = b >> a;
				b = b % (1 << a);
				x = x % 256;
				char res = char(x);
				result += res;
			}
		}
		else
			strOut = result;
	}
	strOut = result;
}

void Base64EncodeN(std::wstring const& wstr, std::string& EncodedStrN)
{
	int size_wstr = (int)(wstr.size());
	int &size_needed = size_wstr;
	size_needed = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, NULL, 0, NULL, NULL);
	CHAR *utf8_string = new CHAR[size_needed];
	WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, utf8_string, size_needed, NULL, NULL);
	std::string str(utf8_string);
	std::string encoded_str;
	Base64_Encode((const unsigned char*)str.c_str(), (unsigned int)(str.size()), encoded_str);
	DELETE_POINTER_ARRAY(CHAR, utf8_string);
	EncodedStrN = encoded_str;
}

void Base64DecodeN(std::string const& str, std::wstring& DecodedStrN)
{
	std::string decodedSTR;
	Base64_Decode(str, decodedSTR);
	int size_decodedSTR = (int)(decodedSTR.size());

	int &size_needed = size_decodedSTR;

	size_needed = MultiByteToWideChar(CP_UTF8, 0, decodedSTR.c_str(), -1, NULL, 0);
	WCHAR *wstrTo = new WCHAR[size_needed];
	MultiByteToWideChar(CP_UTF8, 0, decodedSTR.c_str(), -1, wstrTo, size_needed);

	std::wstring decodedWstr(wstrTo);
	DELETE_POINTER_ARRAY(WCHAR, wstrTo);
	DecodedStrN = decodedWstr;
}

DWORD ExecuteCmdUACSilentW(const WCHAR* lpCommand, const WCHAR* lpCommandParm, const WCHAR* lpVerb)
{
	SHELLEXECUTEINFOW shExInfo = { 0 };
	shExInfo.cbSize = sizeof(shExInfo);
	shExInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	shExInfo.hwnd = 0;
	shExInfo.lpVerb = lpVerb;                 //_T("runas");  // Operation to perform
	shExInfo.lpFile = lpCommand;              // Application to start    

	shExInfo.lpParameters = lpCommandParm;    // Additional parameters
	shExInfo.lpDirectory = 0;                 //L"C:\\Windows\\System32";
	shExInfo.nShow = SW_HIDE;
	shExInfo.hInstApp = 0;

	if (ShellExecuteExW(&shExInfo))
	{
		WaitForSingleObject(shExInfo.hProcess, INFINITE);
		CloseHandle(shExInfo.hProcess);
		return 0;
	}
	else
	{
		DWORD err = GetLastError();
		std::string errstr;
		GetLastErrorAsString(err, errstr);
		ErrorShellExecLogsW(L"Error -> %u :%S", err, errstr.c_str());
		return err;
	}
}

DWORD ExecuteExeUACSilentW(const WCHAR* lpCommand, const WCHAR* lpCommandParm, const WCHAR* lpDirectory, const WCHAR* lpVerb)
{
	SHELLEXECUTEINFOW shExInfo = { 0 };
	shExInfo.cbSize = sizeof(shExInfo);
	shExInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	shExInfo.hwnd = 0;
	shExInfo.lpVerb = lpVerb;                 //_T("runas");  // Operation to perform
	shExInfo.lpFile = lpCommand;              // Application to start    

	shExInfo.lpParameters = lpCommandParm;    // Additional parameters
	shExInfo.lpDirectory = lpDirectory;                 //L"C:\\Windows\\System32";
	shExInfo.nShow = SW_SHOW;
	shExInfo.hInstApp = 0;

	if (ShellExecuteExW(&shExInfo))
	{
		//WaitForSingleObject(shExInfo.hProcess, INFINITE);
		//CloseHandle(shExInfo.hProcess);
		return 0;
	}
	else
	{
		DWORD err = GetLastError();
		std::string errstr;
		GetLastErrorAsString(err,errstr);
		ErrorShellExecLogsW(L"Error -> %u :%S", err, errstr.c_str());
		return err;
	}
}

DWORD ExecuteExeUACSilent1W(const WCHAR* lpCommand, const WCHAR* lpCommandParm, const WCHAR* lpDirectory, bool showConsole, const WCHAR* lpVerb)
{
	SHELLEXECUTEINFOW shExInfo = { 0 };
	shExInfo.cbSize = sizeof(shExInfo);
	shExInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	shExInfo.hwnd = 0;
	shExInfo.lpVerb = lpVerb;                 //_T("runas");  // Operation to perform
	shExInfo.lpFile = lpCommand;              // Application to start    

	shExInfo.lpParameters = lpCommandParm;    // Additional parameters
	shExInfo.lpDirectory = lpDirectory;                 //L"C:\\Windows\\System32";
	if (showConsole)
	{
		shExInfo.nShow = SW_SHOW;
	}
	else
	{
		shExInfo.nShow = SW_HIDE;
	}
	shExInfo.hInstApp = 0;

	if (ShellExecuteExW(&shExInfo))
	{
		//WaitForSingleObject(shExInfo.hProcess, INFINITE);
		//CloseHandle(shExInfo.hProcess);
		return 0;
	}
	else
	{
		DWORD err = GetLastError();
		std::string errstr;
		GetLastErrorAsString(err, errstr);
		ErrorShellExecLogsW(L"Error -> %u :%S", err, errstr.c_str());
		return err;
	}
}


DWORD GetServiceStatusW(WCHAR* name, int &status)
{
	DWORD err = 0;
	std::string errstr;
	SC_HANDLE theService, scm;
	SERVICE_STATUS_PROCESS ssStatus;
	DWORD dwBytesNeeded;

	scm = OpenSCManagerW(0, 0, SC_MANAGER_CONNECT);
	if (!scm)
	{
		err = GetLastError();
		GetLastErrorAsString(err, errstr);
		ErrorFunctionLogsW(L"In GetServiceStatusW: OpenSCManagerW failed with: %S", errstr.c_str());
		return err;
	}

	theService = OpenServiceW(scm, name, SERVICE_QUERY_STATUS);

	if (!theService)
	{
		CloseServiceHandle(scm);
		err = GetLastError();
		GetLastErrorAsString(err,errstr);
		ErrorFunctionLogsW(L"In GetServiceStatusW: OpenServiceW failed with: %S", errstr.c_str());
		return err;
	}

	int result = QueryServiceStatusEx(theService, SC_STATUS_PROCESS_INFO, (LPBYTE)
		&ssStatus, sizeof(SERVICE_STATUS_PROCESS), &dwBytesNeeded);

	CloseServiceHandle(theService);
	CloseServiceHandle(scm);

	if (result == 0)
	{
		err = GetLastError();
		GetLastErrorAsString(err,errstr);
		ErrorFunctionLogsW(L"In GetServiceStatusW: QueryServiceStatusEx failed with: %S", errstr.c_str());
		return err; // fail query status
	}

	status = ssStatus.dwCurrentState;
	return err;
}

bool IsServiceRunningW(WCHAR* servicename)
{
	int status = 0;
	if (!GetServiceStatusW(servicename, status))
	{
		return (status == SERVICE_RUNNING);
	}
	return false;
}

bool IsProcessRunningW(const WCHAR *procesName)
{
	PROCESSENTRY32W pe32 = { sizeof(PROCESSENTRY32W) };
	HANDLE hProcSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcSnap == INVALID_HANDLE_VALUE) return false;
	if (Process32FirstW(hProcSnap, &pe32))
	{
		do
		{
			if (0 == _wcsicmp(pe32.szExeFile, procesName))
			{
				CloseHandle(hProcSnap);
				return true;
			}
		} while (Process32NextW(hProcSnap, &pe32));
	}
	CloseHandle(hProcSnap);
	return false;
}


bool IsApplicationRunning(const WCHAR *procesName, DWORD &expid)
{
	PROCESSENTRY32W pe32 = { sizeof(PROCESSENTRY32W) };
	HANDLE hProcSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcSnap == INVALID_HANDLE_VALUE) return false;
	bool bprocessfound = false;
	bool bexpfound = false;
	if (Process32FirstW(hProcSnap, &pe32))
	{
		do
		{
			if (0 == _wcsicmp(pe32.szExeFile, procesName))
			{
				bprocessfound = true;
				if (bexpfound&&bprocessfound)break;
				//	CloseHandle(hProcSnap);
				//return true;
			}
			if (0 == _wcsicmp(pe32.szExeFile, L"explorer.exe"))
			{
				expid = pe32.th32ProcessID;
				bexpfound = true;
				if (bexpfound&&bprocessfound)break;
				//CloseHandle(hProcSnap);
				//return true;
			}
		} while (Process32NextW(hProcSnap, &pe32));
	}
	CloseHandle(hProcSnap);
	return (bprocessfound);
}

void GetProgramFilesFolderPath(std::wstring& ProgramFilesDirPath)
{
	LPWSTR ProgramFiles_Path;
#ifdef BUILD_4XP
	ProgramFiles_Path = new WCHAR[MAX_PATH];
	SHGetSpecialFolderPath(0, ProgramFiles_Path, CSIDL_PROGRAM_FILES, FALSE);
	ProgramFilesDirPath = ProgramFiles_Path;
	if (ProgramFiles_Path)
	{
		DELETE_POINTER_ARRAY(WCHAR, ProgramFiles_Path);
	}
#else	
	HRESULT hr = SHGetKnownFolderPath(FOLDERID_ProgramFiles, 0, NULL, &ProgramFiles_Path);

	if (FAILED(hr))
	{
		ErrorFunctionLogsW(_T("ProgramFilesFolderPath: Error Getting folder path:%d\n"), hr);
		if (hr == E_FAIL)
		{
			ErrorFunctionLogsW(_T("The rfid parameter references a KNOWNFOLDERID which does not have\n")
				_T("a path (such as a folder marked as  KF_CATEGORY_VIRTUAL)...\n"));
			return;
		}
		if (hr == E_INVALIDARG)
		{
			ErrorFunctionLogsW(_T("The rfid parameter references a KNOWNFOLDERID that is not present\n")
				_T("on the system Not all KNOWNFOLDERID values are present on all system\n")
				_T("Use IKnownFolderManager::GetFolderIds to retrieve the set of KNOWNFOLDERID\n")
				_T("values for the current system."));
			return;
		}
		return;
		// error handling
	}
	ProgramFilesDirPath = ProgramFiles_Path;
#endif
	SuccessFunctionLogsW(_T("Program Files Folder Path:%s"), ProgramFilesDirPath.c_str());
}

BOOL SetPrivilege(
	HANDLE hToken,          // access token handle
	LPCTSTR lpszPrivilege,  // name of privilege to enable/disable
	BOOL bEnablePrivilege   // to enable or disable privilege
	)
{
	TOKEN_PRIVILEGES tp;
	LUID luid;

	if (!LookupPrivilegeValueW(
		NULL,            // lookup privilege on local system
		lpszPrivilege,   // privilege to lookup 
		&luid))        // receives LUID of privilege
	{
		ErrorFunctionLogsW(L"LookupPrivilegeValue error: %u\n", GetLastError());
		return FALSE;
	}

	tp.PrivilegeCount = 1;
	tp.Privileges[0].Luid = luid;
	if (bEnablePrivilege)
		tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	else
		tp.Privileges[0].Attributes = 0;

	// Enable the privilege or disable all privileges.

	if (!AdjustTokenPrivileges(
		hToken,
		FALSE,
		&tp,
		sizeof(TOKEN_PRIVILEGES),
		(PTOKEN_PRIVILEGES)NULL,
		(PDWORD)NULL))
	{
		ErrorFunctionLogsW(L"AdjustTokenPrivileges error: %u\n", GetLastError());
		return FALSE;
	}

	if (GetLastError() == ERROR_NOT_ALL_ASSIGNED)
	{
		//ErrorFunctionLogsW(L"The token does not have the specified privilege. \n");
		return FALSE;
	}

	return TRUE;
}

BOOL StartProcessAndBypassUAC(WCHAR* applicationName, int id)
{
#if 1
	//SuccessFunctionLogsW(L"Function StartProcessAndBypassUAC called.");
	BOOL result = false;

	UINT32 winlogonPid = 0;
	HANDLE hUserTokenDup;/*= (HANDLE)malloc(sizeof(HANDLE));*/
	HANDLE hPToken;
	HANDLE hProcess;

	PROCESS_INFORMATION procInfo;

	// obtain the currently active session id; every logged on user in the system has a unique session id
	//UINT32 dwSessionId = WTSGetActiveConsoleSessionId();

	winlogonPid = UINT32(id);
	// obtain a handle to the winlogon process
	hProcess = OpenProcess(MAXIMUM_ALLOWED, false, winlogonPid);

	if (hProcess == NULL)
	{
		ErrorFunctionLogsW(L"Error in OpenProcess:%d", GetLastError());
	}



	// obtain a handle to the access token of the winlogon process
	if (!OpenProcessToken(hProcess, TOKEN_DUPLICATE, &hPToken))
	{
		ErrorFunctionLogsW(L"Error in OpenProcessToken:%d", GetLastError());
		CloseHandle(hProcess);
		return false;
	}


	SECURITY_ATTRIBUTES sa;
	memset(&sa, 0, sizeof(sa));
	sa.nLength = sizeof(sa);

	// copy the access token of the winlogon process; the newly created token will be a primary token
	if (!DuplicateTokenEx(hPToken, TOKEN_ASSIGN_PRIMARY | TOKEN_ALL_ACCESS, &sa, SECURITY_IMPERSONATION_LEVEL::SecurityIdentification, TOKEN_TYPE::TokenPrimary, &hUserTokenDup))
	{
		ErrorFunctionLogsW(L"Error in DuplicateTokenEx:%d", GetLastError());
		CloseHandle(hProcess);
		CloseHandle(hPToken);
		return false;
	}

	SetPrivilege(hUserTokenDup,/* access token handle*/SE_TCB_NAME,/* name of privilege to enable/disable*/1 /* to enable or disable privilege*/);


	STARTUPINFO si;
	memset(&si, 0, sizeof(si));
	si.cb = INT32(sizeof(si));

	si.lpDesktop = L"winsta0\\default"; // interactive window station parameter; basically this indicates that the process created can display a GUI on the desktop

	// flags that specify the priority and creation method of the process
	int dwCreationFlags = NORMAL_PRIORITY_CLASS | CREATE_UNICODE_ENVIRONMENT | CREATE_NEW_CONSOLE;

	// create a new process in the current user's logon session
	//		result = CreateProcessAsUserW(hUserTokenDup, NULL, applicationName, &sa, &sa, false, dwCreationFlags, NULL, NULL, &si, &procInfo);

	std::wstring progFP;
	GetProgramFilesFolderPath(progFP);
	progFP = progFP.append(L"\\EG1\\EGClamNetAV");

	//SuccessServiceLogW(L"progFP in winFunction file->%ws", progFP.c_str());

	result = CreateProcessAsUserW(hUserTokenDup, applicationName, NULL, NULL, NULL, false, dwCreationFlags, NULL, (WCHAR*)progFP.c_str(), &si, &procInfo);

	if (result == NULL)
	{
		std::string errstr;
		GetLastErrorAsString(GetLastError(), errstr);
		ErrorFunctionLogsW(L"Function CreateProcessAsUserW Failed.->%S",errstr.c_str());
		ErrorFunctionLogsW(L"appname %s", applicationName);
		ErrorFunctionLogsW(L"progFP %s", progFP.c_str());

#if 0
		ErrorServiceLogW(L"Using ShellExecute Function");

		if (!ImpersonateLoggedOnUser(hUserTokenDup))
			ErrorServiceLogW(L"ImpersonateLoggedOnUser failed : %u", GetLastError());
		CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
		HINSTANCE S = ShellExecuteW(NULL, L"runas", applicationName, NULL, NULL, SW_SHOWNORMAL);
		if ((int)(S) <= 32)
		{
			ErrorServiceLogW(L"ShellExecuteW failed: %d", (int)(S));
		}
		if (!RevertToSelf())
		{
			ErrorServiceLogW(L"RevertToSelf failed: %u", GetLastError());
		}
		CoUninitialize();
#endif
	}

	CloseHandle(hProcess);
	CloseHandle(hPToken);
	CloseHandle(hUserTokenDup);

	return result;
#endif
}

DWORD ProtectProcess(void)
{
	HANDLE hProcess = GetCurrentProcess();
	PACL pEmptyDacl;
	DWORD dwErr;

	// using malloc guarantees proper alignment
	pEmptyDacl = (PACL)malloc(sizeof(ACL));

	if (!InitializeAcl(pEmptyDacl, sizeof(ACL), ACL_REVISION))
	{
		dwErr = GetLastError();
	}
	else
	{
		dwErr = SetSecurityInfo(hProcess, SE_KERNEL_OBJECT,
			DACL_SECURITY_INFORMATION, NULL, NULL, pEmptyDacl, NULL);
	}

	free(pEmptyDacl);
	return dwErr;
}

bool IsProcessRunningFromUser(const WCHAR *procesName, int sessionid)
{
	PROCESSENTRY32W pe32 = { sizeof(PROCESSENTRY32W) };
	HANDLE hProcSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcSnap == INVALID_HANDLE_VALUE) return false;
	if (Process32FirstW(hProcSnap, &pe32))
	{
		do
		{
			if (0 == _wcsicmp(pe32.szExeFile, procesName))
			{
				DWORD sessionidprocess = 0;
				if (0 == ProcessIdToSessionId(pe32.th32ProcessID, &sessionidprocess))
				{
					ErrorFunctionLogsW(L"IsProcessRunningFromUser: Error in ProcessIdToSessionId: %u", GetLastError());
					continue;
				}
				if (sessionidprocess == sessionid)
				{
					CloseHandle(hProcSnap);
					return true;
				}
			}
		} while (Process32NextW(hProcSnap, &pe32));
	}
	CloseHandle(hProcSnap);
	return false;
}

WCHAR* GetFileNameFromFullPathW(WCHAR * path)
{
	WCHAR * ret = path;
	bool isFullPath = false;
	for (size_t i = 0; i < wcslen(path); i++)
	{
		if (ret[i] == '\\')
		{
			isFullPath = true;
		}
	}
	if (isFullPath)
	{
		ret = (WCHAR *)((DWORD)path + wcslen(path) - 1);
		while (*ret != '\\')
			ret--;
		ret++;
	}
	return ret;
}

void GetFolderPathFromFileFullPathW(WCHAR* fileFullpath, std::wstring& folderPath)
{
	size_t endChar;
	for (size_t i = wcslen(fileFullpath); i >0; i--)
	{
		if (fileFullpath[i] == '\\')
		{
			endChar = i;
			break;
		}
	}

	for (size_t j = 0; j < endChar; j++)
	{
		folderPath += fileFullpath[j];
	}
}

bool GetProcessFileName(HANDLE hProc, std::wstring& ProcessFileName)
{
	WCHAR processFileName[MAX_PATH];
	DWORD sizefileName = MAX_PATH;
	if (!GetModuleFileNameEx(hProc, NULL, processFileName, MAX_PATH))
	{
		if (!QueryFullProcessImageNameW(hProc, 0, processFileName, &sizefileName))
		{
			ErrorFunctionLogsW(L"GetProcessFileName failed:");
			return false;
		}
		ProcessFileName = processFileName;
		return true;
	}
	ProcessFileName = processFileName;
	return true;
}

void RemoveWhiteSpace(std::string& stdstr)
{
	stdstr.erase(std::remove(stdstr.begin(), stdstr.end(), ' '), stdstr.end());
	stdstr.erase(std::remove(stdstr.begin(), stdstr.end(), '\t'), stdstr.end());
}