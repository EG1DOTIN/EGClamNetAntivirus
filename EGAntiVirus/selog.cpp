#include "selog.h"

void GetAppDataFolderPath2(std::wstring& strOut)
{
	LPWSTR AppDataPath;
#ifdef BUILD_4XP
	AppDataPath = new WCHAR[MAX_PATH];
	SHGetSpecialFolderPath(0, AppDataPath, CSIDL_COMMON_APPDATA, FALSE);
	strOut = AppDataPath;
	if (AppDataPath)
	{
		DELETE_POINTER_ARRAY(WCHAR, AppDataPath);
	}
#else	 
	HRESULT hr = SHGetKnownFolderPath(FOLDERID_ProgramData, 0, NULL, &AppDataPath);

	if (FAILED(hr))
	{
		// error handling
		return;
	}
	strOut = AppDataPath;
#endif
}

void WriteLogsW2(const WCHAR *filename, const WCHAR *fmt, ...)
{
	WCHAR str[1024];
	std::wstring AppFolderPath;
	GetAppDataFolderPath2(AppFolderPath);

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

