#ifndef CLEANUP_API_HEADER
#define CLEANUP_API_HEADER

#include <Windows.h>
#include "gxfunctions.h"
#include <wx\wx.h>
#include <UserEnv.h>

#pragma comment(lib,"userenv.lib")
#pragma comment(lib, "wininet.lib")

void GetCookiesPath(wxString& strOut);
void GetCachePath(wxString& strOut);
void GetHistoryDirPath(wxString& strOut);
void GetLocalAppDataDirPath(wxString& strOut);
void GetRoamingAppDataDirPath(wxString& strOut);
void GetRecentItemPath(wxString& strOut);

BOOL Delete_IECache();
BOOL Delete_IECookies();
HRESULT Delete_IEHistory();
void Delete_IEAddressBarHistory();
void Delete_DesktopRecentDocsHistory();
void Delete_DesktopRunHistory();
void Delete_DesktopRecycleBinContents();
void Delete_TempFilesFolders();
void Delete_OtherBrowserCacheHistory();
void Delete_IEAddressBarUrlHistoryInReg();
void Delete_IEAddressBarPathHistoryInReg();

ULLONG Size_IECache();
ULLONG Size_IECookies();
ULLONG Size_IEHistory();
ULLONG Size_DesktopRecycleBinContents();
ULLONG Size_OtherBrowserCacheHistory();
ULLONG Size_TempFilesFolders();
ULLONG Size_DesktopRecentDocsHistory();

void Size_GetAll(wxArrayString* sizeStr);

#endif