#include "CleaningThreads.h"
#include "cleanupapi.h"

extern bool gbDeleteTemporaryFilesFolders;
extern bool gbDeleteIECache;
extern bool gbDeleteIECookies;
extern bool gbDeleteIEHistory;
extern bool gbDeleteOtherBrowserCacheNHistory;
extern bool gbDeleteRecycleBinContent;
extern bool gbDeleteIEAddressBarHistory;
extern bool gbDeleteRecentDocsHistory;
extern bool gbDeleteRunHistory;
extern bool gbDeleteAll;

bool gbIsCleaningFinished = false;
wxString gStrReportingForCleaning;

extern ULLONG gSizeVal[10];
ULLONG TotalSizeFreed = 0;

void CleaningMain()
{
	if (gbDeleteAll)
	{
		gStrReportingForCleaning = wxT("Cleaning Temporary Files/Folders...");
		Delete_TempFilesFolders();
		gStrReportingForCleaning = wxT("Cleaning IE Cache...");
		Delete_IECache();
		gStrReportingForCleaning = wxT("Cleaning IE Cookies...");
		Delete_IECookies();
		gStrReportingForCleaning = wxT("Cleaning IE History...");
		Delete_IEHistory();
		gStrReportingForCleaning = wxT("Cleaning Other Browser's Cache And History...");
		Delete_OtherBrowserCacheHistory();
		gStrReportingForCleaning = wxT("Cleaning Recycle Bin Contents...");
		Delete_DesktopRecycleBinContents();
		gStrReportingForCleaning = wxT("Cleaning IE Address Bar History...");
		Delete_IEAddressBarHistory();
		gStrReportingForCleaning = wxT("Cleaning Recent Documents History...");
		Delete_DesktopRecentDocsHistory();
		gStrReportingForCleaning = wxT("Cleaning Run History...");
		Delete_DesktopRunHistory();
		TotalSizeFreed = gSizeVal[9];
	}
	else
	{
		if (gbDeleteTemporaryFilesFolders)
		{
			gStrReportingForCleaning = wxT("Cleaning Temporary Files/Folders...");
			Delete_TempFilesFolders();
			TotalSizeFreed += gSizeVal[0];
		}
		
		if (gbDeleteIECache)
		{
			gStrReportingForCleaning = wxT("Cleaning IE Cache...");
			Delete_IECache();
			TotalSizeFreed += gSizeVal[1];

		}

		if (gbDeleteIECookies)
		{
			gStrReportingForCleaning = wxT("Cleaning IE Cookies...");
			Delete_IECookies();
			TotalSizeFreed += gSizeVal[2];
		}

		if (gbDeleteIEHistory)
		{
			gStrReportingForCleaning = wxT("Cleaning IE History...");
			Delete_IEHistory();
			TotalSizeFreed += gSizeVal[3];
		}

		if (gbDeleteOtherBrowserCacheNHistory)
		{
			gStrReportingForCleaning = wxT("Cleaning Other Browser's Cache And History...");
			Delete_OtherBrowserCacheHistory();
			TotalSizeFreed += gSizeVal[4];
		}

		if (gbDeleteRecycleBinContent)
		{
			gStrReportingForCleaning = wxT("Cleaning Recycle Bin Contents...");
			Delete_DesktopRecycleBinContents();
			TotalSizeFreed += gSizeVal[5];
		}

		if (gbDeleteIEAddressBarHistory)
		{
			gStrReportingForCleaning = wxT("Cleaning IE Address Bar History...");
			Delete_IEAddressBarHistory();
			TotalSizeFreed += gSizeVal[6];
		}

		if (gbDeleteRecentDocsHistory)
		{
			gStrReportingForCleaning = wxT("Cleaning Recent Documents History...");
			Delete_DesktopRecentDocsHistory();
			TotalSizeFreed += gSizeVal[7];
		}

		if (gbDeleteRunHistory)
		{
			gStrReportingForCleaning = wxT("Cleaning Run History...");
			Delete_DesktopRunHistory();
			TotalSizeFreed += gSizeVal[8];
		}
	}
	wxString totlsizemapped;
	gxDataSizeMap(TotalSizeFreed, totlsizemapped);
	gStrReportingForCleaning = wxT("Cleaning Process Has Been Finished. \n") + totlsizemapped + wxT(" Of Space Has Been Freed.");
}

DWORD WINAPI CleaningMainThreadFunction(LPVOID lparm)
{
	CleaningMain();
	gbIsCleaningFinished = true;
	return 0;
}