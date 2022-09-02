#include "selog.h"
#include "cleanupapi.h"
#include <string>     // Delete_DesktopRunHistory()
#include <wininet.h>  // DeleteUrlCacheEntry etc
#include <UrlHist.h>  // IUrlHistoryStg2 
#include <shlobj.h>   // CLSID_CUrlHistory, SHAddToRecentDocs
#include <shellapi.h> // SHEmptyRecycleBin
#include <tchar.h> 
#include <wx\dir.h>
#include <wx\filename.h>

//#define TEMP_FOLDER_INBUILT

void GetCookiesPath(wxString& strOut)
{
	LPWSTR path;
#	ifdef BUILD_4XP
	path = new WCHAR[MAX_PATH];
	SHGetSpecialFolderPath(0, path, CSIDL_COOKIES, FALSE);
	strOut = path;
	if (path)
	{
		delete[] path;
		path = (WCHAR*)(NULL);
	}
#	else	 
	HRESULT hr = SHGetKnownFolderPath(FOLDERID_Cookies, 0, NULL, &path);
	if (FAILED(hr))
	{
		// error handling
		//ErrorFunctionLogsW(L"In GetHistoryFolderPath, SHGetKnownFolderPath failed.");
		return;
	}
	strOut = path;
#	endif
}

void GetCachePath(wxString& strOut)
{
	LPWSTR path;
#	ifdef BUILD_4XP
	path = new WCHAR[MAX_PATH];
	SHGetSpecialFolderPath(0, path, CSIDL_INTERNET_CACHE, FALSE);
	strOut = path;
	if (path)
	{
		delete[] path;
		path = (WCHAR*)(NULL);
	}
#	else	 
	HRESULT hr = SHGetKnownFolderPath(FOLDERID_InternetCache, 0, NULL, &path);
	if (FAILED(hr))
	{
		// error handling
		//ErrorFunctionLogsW(L"In GetHistoryFolderPath, SHGetKnownFolderPath failed.");
		return;
	}
	strOut = path;
#	endif
}

void GetHistoryDirPath(wxString& strOut)
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
		//ErrorFunctionLogsW(L"In GetHistoryFolderPath, SHGetKnownFolderPath failed.");
		return;
	}
	strOut = path;
#	endif
}

void GetLocalAppDataDirPath(wxString& strOut)
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
		//ErrorFunctionLogsW(L"In GetLocalAppDataFolderPath, SHGetKnownFolderPath failed.");
		return;
	}
	strOut = path;
#	endif
}

void GetRoamingAppDataDirPath(wxString& strOut)
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
		//ErrorFunctionLogsW(L"In GetRoamingAppDataFolderPath, SHGetKnownFolderPath failed.");
		return;
	}
	strOut = path;
#	endif
}

void GetRecentItemPath(wxString& strOut)
{
	LPWSTR path;
#	ifdef BUILD_4XP
	path = new WCHAR[MAX_PATH];
	SHGetSpecialFolderPath(0, path, CSIDL_RECENT, FALSE);
	strOut = path;
	if (path)
	{
		delete[] path;
		path = (WCHAR*)(NULL);
	}
#	else	 
	HRESULT hr = SHGetKnownFolderPath(FOLDERID_Recent, 0, NULL, &path);
	if (FAILED(hr))
	{
		// error handling
		//ErrorFunctionLogsW(L"In GetRecentItemPath, SHGetKnownFolderPath failed.");
		return;
	}
	strOut = path;
#	endif
}

BOOL Delete_IECache()
{
	HANDLE hCacheEnumHandle = NULL;
	LPINTERNET_CACHE_ENTRY_INFO lpCacheEntry = NULL;
	DWORD  dwSize = 4096; // initial buffer size

	// Enable initial buffer size for cache entry structure.
	lpCacheEntry = (LPINTERNET_CACHE_ENTRY_INFO) new char[dwSize];
	lpCacheEntry->dwStructSize = dwSize;

	// URL search pattern (1st parameter) options are:  NULL ("*.*"), "cookie:", 
	// or "visited:".
	hCacheEnumHandle = FindFirstUrlCacheEntry(NULL /* in */,
		lpCacheEntry /* out */, &dwSize /* in, out */);

	// First, obtain handle to internet cache with FindFirstUrlCacheEntry
	// for later use with FindNextUrlCacheEntry.
	if (hCacheEnumHandle != NULL)
	{
		// When cache entry is not a cookie, delete entry. 
		if (!(lpCacheEntry->CacheEntryType & COOKIE_CACHE_ENTRY))
		{
			DeleteUrlCacheEntry(lpCacheEntry->lpszSourceUrlName);
		}
	}
	else
	{
		switch (GetLastError())
		{
		case ERROR_INSUFFICIENT_BUFFER:
			lpCacheEntry = (LPINTERNET_CACHE_ENTRY_INFO) new char[dwSize];
			lpCacheEntry->dwStructSize = dwSize;

			// Repeat first step search with adjusted buffer, exit if not
			// found again (in practice one buffer's size adustment is  
			// always OK).
			hCacheEnumHandle = FindFirstUrlCacheEntry(NULL, lpCacheEntry,
				&dwSize);
			if (hCacheEnumHandle != NULL)
			{
				// When cache entry is not a cookie, delete entry. 
				if (!(lpCacheEntry->CacheEntryType & COOKIE_CACHE_ENTRY))
				{
					DeleteUrlCacheEntry(lpCacheEntry->lpszSourceUrlName);
				}
				break;
			}
			else
			{
				// FindFirstUrlCacheEntry fails again, return.
				return FALSE;
			}
		default:
			FindCloseUrlCache(hCacheEnumHandle);
			return FALSE;
		}
	}

	// Next, use hCacheEnumHandle obtained from the previous step to delete 
	// subsequent items of the cache.
	do
	{
		// Notice that return values of FindNextUrlCacheEntry (BOOL) and 
		// FindFirstUrlCacheEntry (HANDLE) are different.
		if (FindNextUrlCacheEntry(hCacheEnumHandle, lpCacheEntry, &dwSize))
		{
			// When cache entry is not a cookie, delete entry. 
			if (!(lpCacheEntry->CacheEntryType & COOKIE_CACHE_ENTRY))
			{
				DeleteUrlCacheEntry(lpCacheEntry->lpszSourceUrlName);
			}
		}
		else
		{
			switch (GetLastError())
			{
			case ERROR_INSUFFICIENT_BUFFER:
				lpCacheEntry = (LPINTERNET_CACHE_ENTRY_INFO)
					new char[dwSize];
				lpCacheEntry->dwStructSize = dwSize;

				// Repeat next step search with adjusted buffer, exit if 
				// error comes up again ((in practice one buffer's size 
				// adustment is always OK).
				if (FindNextUrlCacheEntry(hCacheEnumHandle, lpCacheEntry,
					&dwSize))
				{
					// When cache entry is not a cookie, delete entry. 
					if (!(lpCacheEntry->CacheEntryType & COOKIE_CACHE_ENTRY))
					{
						DeleteUrlCacheEntry(lpCacheEntry->lpszSourceUrlName);
					}
					break;
				}
				else
				{
					// FindFirstUrlCacheEntry fails again, return.
					FindCloseUrlCache(hCacheEnumHandle);
					return FALSE;
				}
				break;
			case ERROR_NO_MORE_ITEMS:
				FindCloseUrlCache(hCacheEnumHandle);
				return TRUE;
			default:
				FindCloseUrlCache(hCacheEnumHandle);
				return FALSE;
			}
		}
	} while (TRUE);

	return FALSE; // never here
}

BOOL Delete_IECookies()
{
	HANDLE hCacheEnumHandle = NULL;
	LPINTERNET_CACHE_ENTRY_INFO lpCacheEntry = NULL;
	DWORD  dwSize = 4096; // initial buffer size

	// Enable initial buffer size for cache entry structure.
	lpCacheEntry = (LPINTERNET_CACHE_ENTRY_INFO) new char[dwSize];
	lpCacheEntry->dwStructSize = dwSize;

	// URL search pattern (1st parameter) options are:  "cookie:", "visited:", 
	// or NULL ("*.*").
	hCacheEnumHandle = FindFirstUrlCacheEntry(_T("cookie:") /* in */,
		lpCacheEntry /* out */, &dwSize /* in, out */);

	// First, obtain handle to internet cache with FindFirstUrlCacheEntry
	// for late use with FindNextUrlCacheEntry.

	if (hCacheEnumHandle != NULL)
	{
		DeleteUrlCacheEntry(lpCacheEntry->lpszSourceUrlName);
	}
	else
	{
		switch (GetLastError())
		{
		case ERROR_INSUFFICIENT_BUFFER:
			lpCacheEntry = (LPINTERNET_CACHE_ENTRY_INFO) new char[dwSize];
			lpCacheEntry->dwStructSize = dwSize;

			// Repeat first step search with adjusted buffer, exit if not
			// found again (in practice one buffer's size adustment is  
			// always OK).
			hCacheEnumHandle = FindFirstUrlCacheEntry(NULL, lpCacheEntry,
				&dwSize);
			if (hCacheEnumHandle != NULL)
			{
				DeleteUrlCacheEntry(lpCacheEntry->lpszSourceUrlName);
				break;
			}
			else
			{
				// FindFirstUrlCacheEntry fails again, return.
				return FALSE;
			}
		default:
			FindCloseUrlCache(hCacheEnumHandle);
			return FALSE;
		}
	}

	// Next, use hCacheEnumHandle obtained from the previous step to delete 
	// subsequent items of cache.

	do
	{
		// Notice that return values of FindNextUrlCacheEntry (BOOL) and 
		// FindFirstUrlCacheEntry (HANDLE) are different.

		if (FindNextUrlCacheEntry(hCacheEnumHandle, lpCacheEntry, &dwSize))
		{
			DeleteUrlCacheEntry(lpCacheEntry->lpszSourceUrlName);
		}
		else
		{
			switch (GetLastError())
			{
			case ERROR_INSUFFICIENT_BUFFER:
				lpCacheEntry = (LPINTERNET_CACHE_ENTRY_INFO)
					new char[dwSize];
				lpCacheEntry->dwStructSize = dwSize;

				// Repeat next step search with adjusted buffer, exit if 
				// error comes up again ((in practice one buffer's size 
				// adustment is always OK).

				if (FindNextUrlCacheEntry(hCacheEnumHandle, lpCacheEntry,
					&dwSize))
				{
					DeleteUrlCacheEntry(lpCacheEntry->lpszSourceUrlName);
					break;
				}
				else
				{
					// FindFirstUrlCacheEntry fails again, return.
					FindCloseUrlCache(hCacheEnumHandle);
					return FALSE;
				}
				break;
			case ERROR_NO_MORE_ITEMS:
				FindCloseUrlCache(hCacheEnumHandle);
				return TRUE;
			default:
				FindCloseUrlCache(hCacheEnumHandle);
				return FALSE;
			}
		}
	} while (TRUE);

	return FALSE; // never here
}

HRESULT Delete_IEHistory()
{
	HRESULT hr;
	CoInitialize(NULL);

	IUrlHistoryStg2* pUrlHistoryStg2 = NULL;
	hr = CoCreateInstance(CLSID_CUrlHistory, NULL, CLSCTX_INPROC,
		IID_IUrlHistoryStg2, (void**)&pUrlHistoryStg2);
	if (SUCCEEDED(hr))
	{
		hr = pUrlHistoryStg2->ClearHistory();
		pUrlHistoryStg2->Release();
	}
	CoUninitialize();

	return hr;
}

void Delete_IEAddressBarUrlHistoryInReg()
{
	HKEY hKey;
	DWORD dwResult;
	TCHAR szValueName[10];

	// Open TypedURLs key.
	dwResult = RegOpenKey(HKEY_CURRENT_USER,
		_T("SOFTWARE\\Microsoft\\Internet Explorer\\TypedURLs"), &hKey);

	if (dwResult != ERROR_SUCCESS)
	{
		ErrorCommonLogsW(L"Delete_IEAddressBarUrlHistoryInReg: Error in RegOpenKey: %s", wxSysErrorMsg(dwResult));
		return;
	}

	int i = 1; wsprintf(szValueName, _T("url%d"), i);

	while (RegDeleteValue(hKey, szValueName) == ERROR_SUCCESS)
	{
		i++; wsprintf(szValueName, _T("url%d"), i);
	}
	RegCloseKey(hKey);
}

void Delete_IEAddressBarPathHistoryInReg()
{
	HKEY hKey;
	DWORD dwResult;
	TCHAR szValueName[10];

	// Open TypedURLs key.
	dwResult = RegOpenKey(HKEY_CURRENT_USER,
		_T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\TypedPaths"), &hKey);

	if (dwResult != ERROR_SUCCESS)
	{
		ErrorCommonLogsW(L"Delete_IEAddressBarPathHistoryInReg: Error in RegOpenKey: %s", wxSysErrorMsg(dwResult));
		return;
	}

	int i = 1; wsprintf(szValueName, _T("url%d"), i);

	while (RegDeleteValue(hKey, szValueName) == ERROR_SUCCESS)
	{
		i++; wsprintf(szValueName, _T("url%d"), i);
	}

	RegCloseKey(hKey);
}

void Delete_IEAddressBarHistory()
{
	Delete_IEAddressBarUrlHistoryInReg();
	Delete_IEAddressBarPathHistoryInReg();
}

void Delete_DesktopRecentDocsHistory()
{
	SHAddToRecentDocs(SHARD_PATH, NULL /* NULL clears history */);
	wxString recentFolderPath;
	GetRecentItemPath(recentFolderPath);
	gxMakeDirEmpty(recentFolderPath);
}

// Note: actually, effect of running Delete_DesktopRunHistory is 
// visible after reboot. 

void Delete_DesktopRunHistory()
{
	HKEY hKey;
	DWORD dwResult;
	TCHAR szValueName[10];
	std::string c, s;
	s = "abcdefghijklmnopqrstuvwxyz";
	int i;

	// Open RunMRU key.
	dwResult = RegOpenKey(HKEY_CURRENT_USER,
		_T("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\RunMRU"),
		&hKey);
	// Traverse all possible values and delete. This guarantees deletion
	// even if the sequence in broken.
	if (dwResult != ERROR_SUCCESS)
	{
		ErrorCommonLogsW(L"Error in RegOpenKey: %s", wxSysErrorMsg(dwResult));
		return;
	}

	for (i = 0; i < 26 /* z */; i++)
	{
		c = s.at(i); wsprintfW(szValueName, _T("%S"), c.c_str());
		//SuccessCommonLogsW(L" Run List: szValueName: %s",szValueName);
		RegDeleteValue(hKey, szValueName);
	}

	RegDeleteValue(hKey, _T("MRUList"));

	RegCloseKey(hKey);
}

void Delete_DesktopRecycleBinContents()
{
	SHEmptyRecycleBin(NULL, NULL,
		SHERB_NOCONFIRMATION | SHERB_NOPROGRESSUI | SHERB_NOSOUND);
}

void Delete_OtherBrowserCacheHistory()
{
	wxString appdatapath;
	GetLocalAppDataDirPath(appdatapath);
	wxString appdatarompath;
	GetRoamingAppDataDirPath(appdatarompath);

	wxString ChromeDir = appdatapath + wxT("\\Google\\Chrome\\User Data");
	wxString MozilaDir = appdatapath + wxT("\\Mozilla\\Firefox\\Profiles");
	wxString OperaDir1 = appdatapath + wxT("\\Opera\\Opera");
	wxString OperaDir2 = appdatarompath + wxT("\\Opera\\Opera");
	wxString SafariDir1 = appdatapath + wxT("\\Apple Computer\\Safari");
	wxString SafariDir2 = appdatarompath + wxT("\\Apple Computer\\Safari");

	if (wxDir::Exists(ChromeDir))
	{
		gxDeleteDir(ChromeDir, true);
	}

	if (wxDir::Exists(MozilaDir))
	{
		gxDeleteDir(MozilaDir, true);
	}

	if (wxDir::Exists(OperaDir1))
	{
		gxDeleteDir(OperaDir1, true);
	}

	if (wxDir::Exists(OperaDir2))
	{
		gxDeleteDir(OperaDir2, true);
	}

	if (wxDir::Exists(SafariDir1))
	{
		gxDeleteDir(SafariDir1, true);
	}

	if (wxDir::Exists(SafariDir2))
	{
		gxDeleteDir(SafariDir2, true);
	}
}

/*
void Delete_TempFilesFolders()
{
	wxString TempPath;
#ifndef TEMP_FOLDER_INBUILT
	wxString LocalAppDataPath;
	GetLocalAppDataDirPath(LocalAppDataPath);
#ifdef BUILD_4XP
	for (;;)
	{
		if (LocalAppDataPath.EndsWith("\\"))
			break;
		else
			LocalAppDataPath.RemoveLast();
}
	TempPath = LocalAppDataPath + wxT("Temp");
#else
	TempPath = LocalAppDataPath + wxT("\\Temp");
#endif
#else
	WCHAR temppath[MAX_PATH];
	GetTempPathW(MAX_PATH, temppath);
	TempPath = wxString(temppath);
#endif
	gxMakeDirEmpty(TempPath);
	return;
}
*/

void Delete_TempFilesFolders()
{
	WCHAR propath[MAX_PATH];
	DWORD propathsize = MAX_PATH;
	GetProfilesDirectoryW(propath, &propathsize);
	wxString ProfilePath(propath);
	wxDir dir(ProfilePath);
	wxString UserFolders, temppath;
	bool cont = dir.GetFirst(&UserFolders, wxEmptyString, wxDIR_DIRS | wxDIR_HIDDEN);
	while (cont)
	{
#ifndef BUILD_4XP
		temppath = ProfilePath + wxT("\\") + UserFolders + wxT("\\AppData\\Local\\Temp");
#else
		temppath = ProfilePath + wxT("\\") + UserFolders + wxT("\\Local Settings\\Temp");
#endif
		if (wxDir::Exists(temppath))
		{
			gxMakeDirEmpty(temppath);
		}
		cont = dir.GetNext(&UserFolders);
	}
	return;
}

ULLONG Size_IECache()
{
	//wxString cachepath;
	//GetCachePath(cachepath);
	//SuccessCommonLogsW(L"IE_Cache Folder: %s",cachepath.wc_str());

	ULLONG res = 0;

	HANDLE hCacheEnumHandle = NULL;
	LPINTERNET_CACHE_ENTRY_INFO lpCacheEntry = NULL;
	DWORD  dwSize = 4096; // initial buffer size

	// Enable initial buffer size for cache entry structure.
	lpCacheEntry = (LPINTERNET_CACHE_ENTRY_INFO) new char[dwSize];
	lpCacheEntry->dwStructSize = dwSize;

	// URL search pattern (1st parameter) options are:  NULL ("*.*"), "cookie:", 
	// or "visited:".
	hCacheEnumHandle = FindFirstUrlCacheEntry(NULL /* in */,
		lpCacheEntry /* out */, &dwSize /* in, out */);

	// First, obtain handle to internet cache with FindFirstUrlCacheEntry
	// for later use with FindNextUrlCacheEntry.
	if (hCacheEnumHandle != NULL)
	{
		// When cache entry is not a cookie, delete entry. 
		if (!(lpCacheEntry->CacheEntryType & COOKIE_CACHE_ENTRY))
		{
			//DeleteUrlCacheEntry(lpCacheEntry->lpszSourceUrlName);
			res += gxGetFileSize(lpCacheEntry->lpszLocalFileName);
		}
	}
	else
	{
		DWORD err = GetLastError();
		ErrorCommonLogsW(L"FindFirstUrlCacheEntry failed: %s", wxSysErrorMsg(err));
		switch (err)
		{
		case ERROR_INSUFFICIENT_BUFFER:
			lpCacheEntry = (LPINTERNET_CACHE_ENTRY_INFO) new char[dwSize];
			lpCacheEntry->dwStructSize = dwSize;

			// Repeat first step search with adjusted buffer, exit if not
			// found again (in practice one buffer's size adustment is  
			// always OK).
			hCacheEnumHandle = FindFirstUrlCacheEntry(NULL, lpCacheEntry,
				&dwSize);
			if (hCacheEnumHandle != NULL)
			{
				// When cache entry is not a cookie, delete entry. 
				if (!(lpCacheEntry->CacheEntryType & COOKIE_CACHE_ENTRY))
				{
					//DeleteUrlCacheEntry(lpCacheEntry->lpszSourceUrlName);
					res += gxGetFileSize(lpCacheEntry->lpszLocalFileName);

				}
				break;
			}
			else
			{
				// FindFirstUrlCacheEntry fails again, return.
				ErrorCommonLogsW(L"FindFirstUrlCacheEntry failed again: %s", wxSysErrorMsg(GetLastError()));
				return res;
			}
		default:
			FindCloseUrlCache(hCacheEnumHandle);
			return res;
		}
	}

	// Next, use hCacheEnumHandle obtained from the previous step to delete 
	// subsequent items of the cache.
	do
	{
		// Notice that return values of FindNextUrlCacheEntry (BOOL) and 
		// FindFirstUrlCacheEntry (HANDLE) are different.
		if (FindNextUrlCacheEntry(hCacheEnumHandle, lpCacheEntry, &dwSize))
		{
			// When cache entry is not a cookie, delete entry. 
			if (!(lpCacheEntry->CacheEntryType & COOKIE_CACHE_ENTRY))
			{
				//DeleteUrlCacheEntry(lpCacheEntry->lpszSourceUrlName);
				res += gxGetFileSize(lpCacheEntry->lpszLocalFileName);
			}
		}
		else
		{
			DWORD Error = GetLastError();
			ErrorCommonLogsW(L"FindNextUrlCacheEntry failed : %s", wxSysErrorMsg(Error));

			switch (Error)
			{
			case ERROR_INSUFFICIENT_BUFFER:
				lpCacheEntry = (LPINTERNET_CACHE_ENTRY_INFO)
					new char[dwSize];
				lpCacheEntry->dwStructSize = dwSize;

				// Repeat next step search with adjusted buffer, exit if 
				// error comes up again ((in practice one buffer's size 
				// adustment is always OK).
				if (FindNextUrlCacheEntry(hCacheEnumHandle, lpCacheEntry,
					&dwSize))
				{
					// When cache entry is not a cookie, delete entry. 
					if (!(lpCacheEntry->CacheEntryType & COOKIE_CACHE_ENTRY))
					{
						//DeleteUrlCacheEntry(lpCacheEntry->lpszSourceUrlName);
						res += gxGetFileSize(lpCacheEntry->lpszLocalFileName);
					}
					break;
				}
				else
				{
					// FindFirstUrlCacheEntry fails again, return.
					ErrorCommonLogsW(L"FindNextUrlCacheEntry failed again : %s", wxSysErrorMsg(GetLastError()));
					FindCloseUrlCache(hCacheEnumHandle);
					return res;
				}
				break;
			case ERROR_NO_MORE_ITEMS:
				FindCloseUrlCache(hCacheEnumHandle);
				return res;
			default:
				FindCloseUrlCache(hCacheEnumHandle);
				return res;
			}
		}
	} while (TRUE);

	return res; // never here
}


ULLONG Size_IECookies()
{
	//wxString cookiespath;
	//GetCookiesPath(cookiespath);
	//SuccessCommonLogsW(L"IE_Cookies Folder: %s",cookiespath.wc_str());

	ULLONG res = 0;

	HANDLE hCacheEnumHandle = NULL;
	LPINTERNET_CACHE_ENTRY_INFO lpCacheEntry = NULL;
	DWORD  dwSize = 4096; // initial buffer size

	// Enable initial buffer size for cache entry structure.
	lpCacheEntry = (LPINTERNET_CACHE_ENTRY_INFO) new char[dwSize];
	lpCacheEntry->dwStructSize = dwSize;

	// URL search pattern (1st parameter) options are:  "cookie:", "visited:", 
	// or NULL ("*.*").
	hCacheEnumHandle = FindFirstUrlCacheEntry(_T("cookie:") /* in */,
		lpCacheEntry /* out */, &dwSize /* in, out */);

	// First, obtain handle to internet cache with FindFirstUrlCacheEntry
	// for late use with FindNextUrlCacheEntry.

	if (hCacheEnumHandle != NULL)
	{
		//DeleteUrlCacheEntry(lpCacheEntry->lpszSourceUrlName);
		res += gxGetFileSize(lpCacheEntry->lpszLocalFileName);
	}
	else
	{
		DWORD err = GetLastError();
		ErrorCommonLogsW(L"FindFirstUrlCacheEntry failed: %s", wxSysErrorMsg(err));

		switch (err)
		{
		case ERROR_INSUFFICIENT_BUFFER:
			lpCacheEntry = (LPINTERNET_CACHE_ENTRY_INFO) new char[dwSize];
			lpCacheEntry->dwStructSize = dwSize;

			// Repeat first step search with adjusted buffer, exit if not
			// found again (in practice one buffer's size adustment is  
			// always OK).
			hCacheEnumHandle = FindFirstUrlCacheEntry(NULL, lpCacheEntry,
				&dwSize);
			if (hCacheEnumHandle != NULL)
			{
				//DeleteUrlCacheEntry(lpCacheEntry->lpszSourceUrlName);
				res += gxGetFileSize(lpCacheEntry->lpszLocalFileName);
				break;
			}
			else
			{
				// FindFirstUrlCacheEntry fails again, return.
				ErrorCommonLogsW(L"FindFirstUrlCacheEntry failed again: %s", wxSysErrorMsg(GetLastError()));
				return res;
			}
		default:
			FindCloseUrlCache(hCacheEnumHandle);
			return res;
		}
	}

	// Next, use hCacheEnumHandle obtained from the previous step to delete 
	// subsequent items of cache.

	do
	{
		// Notice that return values of FindNextUrlCacheEntry (BOOL) and 
		// FindFirstUrlCacheEntry (HANDLE) are different.

		if (FindNextUrlCacheEntry(hCacheEnumHandle, lpCacheEntry, &dwSize))
		{
			//DeleteUrlCacheEntry(lpCacheEntry->lpszSourceUrlName);
			res += gxGetFileSize(lpCacheEntry->lpszLocalFileName);
		}
		else
		{
			DWORD error = GetLastError();
			ErrorCommonLogsW(L"FindNextUrlCacheEntry failed : %s", wxSysErrorMsg(error));

			switch (error)
			{
			case ERROR_INSUFFICIENT_BUFFER:

				lpCacheEntry = (LPINTERNET_CACHE_ENTRY_INFO)
					new char[dwSize];
				lpCacheEntry->dwStructSize = dwSize;

				// Repeat next step search with adjusted buffer, exit if 
				// error comes up again ((in practice one buffer's size 
				// adustment is always OK).

				if (FindNextUrlCacheEntry(hCacheEnumHandle, lpCacheEntry, &dwSize))
				{
					//DeleteUrlCacheEntry(lpCacheEntry->lpszSourceUrlName);
					res += gxGetFileSize(lpCacheEntry->lpszLocalFileName);

					break;
				}
				else
				{
					// FindNextUrlCacheEntry fails again, return.
					ErrorCommonLogsW(L"FindNextUrlCacheEntry failed again: %u", wxSysErrorMsg(GetLastError()));
					FindCloseUrlCache(hCacheEnumHandle);
					return res;
				}
				break;
			case ERROR_NO_MORE_ITEMS:
				FindCloseUrlCache(hCacheEnumHandle);
				return res;
			default:
				FindCloseUrlCache(hCacheEnumHandle);
				return res;
			}
		}
	} while (TRUE);

	return res; // never here
}



ULLONG Size_IEHistory()
{
	ULLONG res = 0;

	wxString szFilePath;
	GetHistoryDirPath(szFilePath);

	res = gxGetDirSize(szFilePath);
	return res;
}

ULLONG Size_DesktopRecycleBinContents()
{
	ULLONG res = 0;
	SHQUERYRBINFO shq;
	shq.cbSize = sizeof(SHQUERYRBINFO);
	HRESULT hr = SHQueryRecycleBin(NULL, &shq);
	if (hr == S_OK)
	{
		res = (ULLONG)(shq.i64Size);
		return res;
	}
	else
	{
		return res;
	}
}

ULLONG Size_OtherBrowserCacheHistory()
{
	ULLONG res = 0;
	wxString appdatapath;
	GetLocalAppDataDirPath(appdatapath);
	wxString appdatarompath;
	GetRoamingAppDataDirPath(appdatarompath);

	wxString ChromeDir = appdatapath + wxT("\\Google\\Chrome\\User Data");
	wxString MozilaDir = appdatapath + wxT("\\Mozilla\\Firefox\\Profiles");
	wxString OperaDir1 = appdatapath + wxT("\\Opera\\Opera");
	wxString OperaDir2 = appdatarompath + wxT("\\Opera\\Opera");
	wxString SafariDir1 = appdatapath + wxT("\\Apple Computer\\Safari");
	wxString SafariDir2 = appdatarompath + wxT("\\Apple Computer\\Safari");

	if (wxDir::Exists(ChromeDir))
	{
		res += gxGetDirSize(ChromeDir);
	}

	if (wxDir::Exists(MozilaDir))
	{
		res += gxGetDirSize(MozilaDir);
	}

	if (wxDir::Exists(OperaDir1))
	{
		res += gxGetDirSize(OperaDir1);
	}

	if (wxDir::Exists(OperaDir2))
	{
		res += gxGetDirSize(OperaDir2);
	}

	if (wxDir::Exists(SafariDir1))
	{
		res += gxGetDirSize(SafariDir1);
	}

	if (wxDir::Exists(SafariDir2))
	{
		res += gxGetDirSize(SafariDir2);
	}

	return res;
}


/*
ULLONG Size_TempFilesFolders()
{
	wxString TempPath;
#ifndef TEMP_FOLDER_INBUILT
	wxString LocalAppDataPath;
	GetLocalAppDataDirPath(LocalAppDataPath);
#ifdef BUILD_4XP
	for (;;)
	{
		if (LocalAppDataPath.EndsWith("\\"))
			break;
		else
			LocalAppDataPath.RemoveLast();
}
	TempPath = LocalAppDataPath + wxT("Temp");
#else
	TempPath = LocalAppDataPath + wxT("\\Temp");
#endif
#else
	WCHAR temppath[MAX_PATH];
	GetTempPathW(MAX_PATH, temppath);
	TempPath = wxString(temppath);
#endif
	SuccessCommonLogsW(L"Temp Path : %s", TempPath.wc_str());
	ULLONG res = gxGetDirSize(TempPath);
	return res;
}
*/

ULONGLONG Size_TempFilesFolders()
{
	ULONGLONG res = 0;
	WCHAR propath[MAX_PATH];
	DWORD propathsize = MAX_PATH;
	GetProfilesDirectoryW(propath, &propathsize);
	wxString ProfilePath(propath);
	wxDir dir(ProfilePath);
	wxString UserFolders, temppath;
	bool cont = dir.GetFirst(&UserFolders, wxEmptyString, wxDIR_DIRS | wxDIR_HIDDEN);
	while (cont)
	{
#ifndef BUILD_4XP
		temppath = ProfilePath + wxT("\\") + UserFolders + wxT("\\AppData\\Local\\Temp");
#else
		temppath = ProfilePath + wxT("\\") + UserFolders + wxT("\\Local Settings\\Temp");
#endif

			if (wxDir::Exists(temppath))
			{
				res += gxGetDirSize(temppath);
			}
		cont = dir.GetNext(&UserFolders);
	}
	return res;
}


ULLONG Size_DesktopRecentDocsHistory()
{
	ULLONG res = 0;
	wxString recentFolderPath;
	GetRecentItemPath(recentFolderPath);
	res = gxGetDirSize(recentFolderPath);
	return res;
}

ULLONG gSizeVal[10];

void Size_GetAll(wxArrayString* sizeStr)
{
	wxBeginBusyCursor();
	gSizeVal[0] = Size_TempFilesFolders();
	gSizeVal[1] = Size_IECache();
	gSizeVal[2] = Size_IECookies();
	gSizeVal[3] = Size_IEHistory();
	gSizeVal[4] = Size_OtherBrowserCacheHistory();
	gSizeVal[5] = Size_DesktopRecycleBinContents();
	gSizeVal[6] = 0;
	gSizeVal[7] = Size_DesktopRecentDocsHistory();
	gSizeVal[8] = 0;
	gSizeVal[9] = gSizeVal[0] + gSizeVal[1] + gSizeVal[2] + gSizeVal[3] + gSizeVal[4] + gSizeVal[5] + gSizeVal[6] + gSizeVal[7] + gSizeVal[8];
	
	for (size_t i = 0; i < 10; i++)
	{
		wxString sizemapped;
		gxDataSizeMap(gSizeVal[i], sizemapped);
		sizeStr->Add(sizemapped);
	}
	wxEndBusyCursor();
}