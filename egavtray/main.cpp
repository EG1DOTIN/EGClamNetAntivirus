#include "main.h"
#include "common.h"
#include "gxfunctions.h"
#include "avmsgebox.h"
#include "avtheme.h"
#include "HomeTrayWindow.h"
#include <wx\stdpaths.h>


IMPLEMENT_APP(MyApp)

HomeTrayWin* gHomeTray = NULL;

wxString workingDir;
wxString AppDataEGAVPath;
wxString DatabaseFolder;
wxString dbFile1;  //For antivirus settings
wxString dbFile5; // for machine id now 

bool MyApp::OnInit()
{
	m_checker = new wxSingleInstanceChecker;
	if (m_checker->IsAnotherRunning())
	{
		AVShowMsg(EGAV_TITLE_NAME, wxT("Another program instance is already running."));
		delete m_checker; // OnExit() won't be called if we return false
		m_checker = (wxSingleInstanceChecker*)(NULL);
		return false;
	}
	else
	{	
		workingDir = wxPathOnly(wxStandardPaths::Get().GetExecutablePath());
		std::wstring AppDataDir;
		GetAppDataFolderPath(AppDataDir);
		AppDataEGAVPath = wxString(AppDataDir) + wxT("\\EGAntiVirus");
		DatabaseFolder = AppDataEGAVPath + wxT("\\db");

		dbFile1 = DatabaseFolder + wxT("\\db1");
		dbFile5 = DatabaseFolder + wxT("\\db5");
		
		
		ProtectProcess();

		//if service is not running make it run now
#ifdef NDEBUG
		if (!IsServiceRunningW(L"WinSEGAV AutoConfig"))
		{
			ErrorAppTrayLogW(L"Service is not running");
			return false;
			//ExecuteCmdUACSilentW(L"sc.exe", L"start \"WinSEGAV AutoConfig\"", L"");
			//wxMilliSleep(100);
		}
#endif
		
		gHomeTray = new HomeTrayWin(EGAV_TITLE_NAME);
		gHomeTray->Show(false);
		return true;
	}
}

int MyApp::OnExit()
{
	DELETE_POINTER(wxSingleInstanceChecker, m_checker);
	return 0;
}