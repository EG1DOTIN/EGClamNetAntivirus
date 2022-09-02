#include "common.h"
#include "main.h"
#include "avtheme.h"
#include "update.h"
#include "avmsgebox.h"
#include "wx\stdpaths.h"

bool gbUpdateVirusDefinitionFrame = false;
wxString workingDir;
wxString AppDataEGAVPath;

IMPLEMENT_APP(MyApp)

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
		std::wstring AppDataFolderPath;
		GetAppDataFolderPath(AppDataFolderPath);
		AppDataEGAVPath = wxString(AppDataFolderPath) + wxT("\\EGAntiVirus");

		ProtectProcess();
		//if service is not running make it run now
#ifdef NDEBUG
		if (!IsServiceRunningW(L"WinSEGAV AutoConfig"))
		{
			ErrorAppUpdateLogsW(L"Service is not running");
			return false;
			//ExecuteCmdUACSilentW(L"sc.exe", L"start \"WinSEGAV AutoConfig\"", L"");
			//wxMilliSleep(100);
		}
#endif

		UpdateVirusDefinition* uvd = new UpdateVirusDefinition(EGAV_TITLE_NAME);
		uvd->Show(true);
	}
	return true;
}

int MyApp::OnExit()
{
	DELETE_POINTER(wxSingleInstanceChecker, m_checker);
	return 0;
}