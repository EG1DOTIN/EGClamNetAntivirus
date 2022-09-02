#include "common.h"
#include "egavsetgetdb.h"
#include <wx\string.h>

wxString DatabaseFolder;
wxString AppDataEGAVPath;
wxString dbFile1;  //For antivirus settings
wxString dbFile5;  //For machineID	

int main()
{
	std::wstring AppDataDir;
	GetAppDataFolderPath(AppDataDir);
	AppDataEGAVPath = wxString(AppDataDir) + wxT("\\EGAntiVirus");
	DatabaseFolder = AppDataEGAVPath + wxT("\\db");
	dbFile1 = DatabaseFolder + wxT("\\db1");
	dbFile5 = DatabaseFolder + wxT("\\db5");

	ExecuteCmdUACSilentW(L"sc.exe", L"stop \"WinSEGAV AutoConfig\"", L"");
	Sleep(100);
	ExecuteCmdUACSilentW(L"sc.exe", L"start \"WinSEGAV AutoConfig\"", L"");
	Sleep(100);

	//if (GetEGAVRegForAutoUpdateOnOff())
	if (GetEGAVDBForAutoUpdateOnOff())
	{
		ExecuteCmdUACSilentW(L"sc.exe", L"start \"FreshClam\"", L"");
		Sleep(100);
	}
	//if (GetEGAVRegForLiveShieldOnOff())
	if (GetEGAVDBForLiveShieldOnOff())
	{
		ExecuteCmdUACSilentW(L"sc.exe", L"start \"ClamD\"", L"");
		Sleep(100);
	}

	return 0;
}