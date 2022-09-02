#include "common.h"
#include "gxfunctions.h"
#include "avdb.h"
#include "scancmds.h"
#include "scanner.h"
#include "egavscanner.h"
#include "avmsgebox.h"
#include <wx\stdpaths.h>

#ifdef SCANNER_HEADER
bool gbScanSwitch = false;
#endif


#ifdef DBCMD_HEADER
wxString QuaranitineFolder;
#endif


IMPLEMENT_APP(MyApp)

wxString workingDir;
wxString AppDataEGAVPath;
wxString DatabaseFolder;
wxString dbFile1;  //For antivirus settings
wxString dbFile2;  //For antivirus settings exclude list 
wxString dbFile3;  //For antivirus settings include list
wxString dbFile5;  //For machineID	

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
		if (gbScanSwitch)
		{
			AVShowMsg(EGAV_TITLE_NAME, wxT("scan already running..."));
			return true;
		}

		ProtectProcess();

		workingDir = wxPathOnly(wxStandardPaths::Get().GetExecutablePath());
		std::wstring AppDataDirPath;
		GetAppDataFolderPath(AppDataDirPath);
		AppDataEGAVPath = wxString(AppDataDirPath) + wxT("\\EGAntiVirus");
		DatabaseFolder = AppDataEGAVPath + wxT("\\db");
		QuaranitineFolder = AppDataEGAVPath + wxT("\\Vault");

		dbFile1 = DatabaseFolder + wxT("\\db1");
		dbFile2 = DatabaseFolder + wxT("\\db2");
		dbFile3 = DatabaseFolder + wxT("\\db3");
		dbFile5 = DatabaseFolder + wxT("\\db5");

		MakeFolderIfNotExist(AppDataEGAVPath);
		MakeFolderIfNotExist(DatabaseFolder);
		MakeFolderIfNotExist(QuaranitineFolder);

		if (!(wxFileName::FileExists(dbFile1)))
		{
			MakeDefaultAVDBIfNotExist();
		}

		gxMakeTextFile(dbFile2);
		gxMakeTextFile(dbFile3);
		gxMakeTextFile(dbFile5);


		//parser = new wxCmdLineParser;
		//parser->Found(wxT("s"));
		wxArrayString x = wxApp::argv.GetArguments();
		wxString param = wxEmptyString;
		for (size_t i = 1; i < x.GetCount(); i++)
		{
			param += wxT("\"") + x[i] + wxT("\" ");
		}

		m_scanwin = new ScannerWin(wxT("EG ClamNet AntiVirus"), wxT("Custom Scan"), param);
		m_scanwin->Show();
		return true;
	}
}

int MyApp::OnExit()
{
	DELETE_POINTER(wxSingleInstanceChecker, m_checker);
	return 0;
}