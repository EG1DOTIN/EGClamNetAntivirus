#include "main.h"
#include "common.h"
#include "gxDelPointer.h"
#include "gxfunctions.h"
#include "avmsgebox.h"
#include "HomeWindow.h"
#include <wx\filename.h>
#include <wx\stdpaths.h>
#include <wx\textfile.h>
#include "avdb.h"
#include "egavsetgetdb.h"

#include "CheckNet.h"
#include "sendrecv.h"

IMPLEMENT_APP(MyApp)

wxString workingDir;
wxString AppDataEGAVPath;
wxString DatabaseFolder;
wxString QuaranitineFolder;
wxString TempFileForENote;


wxString dbFile1;  //For antivirus settings
wxString dbFile2;  //For antivirus settings exclude list 
wxString dbFile3;  //For antivirus settings include list
wxString dbFile4;  //For Update days to upddate 
wxString dbFile5;  //For machineID
wxString dbFile6;  //For monitor a Folder

bool gbValidateProduct;

bool MyApp::OnInit()
{
	m_checker = new wxSingleInstanceChecker;

	if (m_checker->IsAnotherRunning())
	{
		//wxLogError(wxT("Another program instance is already running."));
		AVShowMsg(EGAV_ADMIN_PANEL_TITLE, wxT("Another program instance is already running"));

		DELETE_POINTER(wxSingleInstanceChecker, m_checker); // OnExit() won't be called if we return false
		return false;
	}
	else
	{
		workingDir = wxPathOnly(wxStandardPaths::Get().GetExecutablePath());
		AppDataEGAVPath = wxStandardPaths::Get().GetConfigDir();

		ProtectProcess();

		DatabaseFolder = AppDataEGAVPath + wxT("\\db");
		QuaranitineFolder = AppDataEGAVPath + wxT("\\Vault");
		TempFileForENote = DatabaseFolder + wxT("\\Temp");

		dbFile1 = DatabaseFolder + wxT("\\db1");
		dbFile2 = DatabaseFolder + wxT("\\db2");
		dbFile3 = DatabaseFolder + wxT("\\db3");
		dbFile4 = DatabaseFolder + wxT("\\db4");
		dbFile5 = DatabaseFolder + wxT("\\db5");

		dbFile6 = DatabaseFolder + wxT("\\db6");

		//make files or folders if not exist
		/////////////////////////////////////////////////////////////////////////////////

		MakeFolderIfNotExist(AppDataEGAVPath);
		MakeFolderIfNotExist(DatabaseFolder);
		MakeFolderIfNotExist(QuaranitineFolder);


		{
			bool bIsDBFile1Exist = wxFile::Exists(dbFile1);
			bool bIsDBFile2Exist = wxFile::Exists(dbFile2);
			bool bIsDBFile3Exist = wxFile::Exists(dbFile3);
			bool bIsDBFile6Exist = wxFile::Exists(dbFile6);

			if ((!bIsDBFile1Exist) || (!bIsDBFile2Exist) || (!bIsDBFile3Exist))
			{
				MakeDefaultAVDBIfNotExist();
			}

			if (!bIsDBFile6Exist)
			{
				makeDefaultMonitoringFolderList(dbFile6);
			}
		}

		gxMakeTextFile(dbFile4);
		gxMakeTextFile(dbFile5);



		if (!gxGetTotalLineInTextFile(dbFile5))
			SetEGAVMachineIdInDB();

#ifdef NDEBUG
		//if service config is not running make it run now
		bool bCheckService = IsServiceRunningW(L"WinSEGAV AutoConfig");
		if (!bCheckService)
		{
			ExecuteCmdUACSilentW(L"sc.exe", L"start \"WinSEGAV AutoConfig\"", L"");
			wxMilliSleep(10);
		}

		//if (GetEGAVRegForAutoUpdateOnOff())
		if(GetEGAVDBForAutoUpdateOnOff())
		{
			//check if EGAV Updater Service running
			bool bIsSerUpdater = IsServiceRunningW(L"FreshClam");
			if (!bIsSerUpdater)
			{
				ConfigClamD(workingDir, AppDataEGAVPath);
				ExecuteCmdUACSilentW(L"sc.exe", L"config FreshClam start= auto", L"");
				wxMilliSleep(10);
				ExecuteCmdUACSilentW(L"sc.exe", L"start FreshClam", L"");
				wxMilliSleep(10);
			}
		}

		//if (GetEGAVRegForLiveShieldOnOff())
		if (GetEGAVDBForLiveShieldOnOff())
		{
			//check if EGAV Scanner Service running
			bool bIsSerScanner = IsServiceRunningW(L"ClamD");
			if (!bIsSerScanner)
			{
				ConfigClamD(workingDir, AppDataEGAVPath);
				ExecuteCmdUACSilentW(L"sc.exe", L"config ClamD start= auto", L"");
				wxMilliSleep(10);
				wxExecute(wxT("sc start ClamD"), wxEXEC_ASYNC | wxEXEC_HIDE_CONSOLE);
				wxMilliSleep(10);
			}
		}
#endif
		setMKeyToServer();
		checkUpdate();

		AVHomeWindow *avhw = new AVHomeWindow(EGAV_ADMIN_PANEL_TITLE wxT(": Home"));
		avhw->Show(true);
	}
	return true;
}

int MyApp::OnExit()
{
	DELETE_POINTER(wxSingleInstanceChecker, m_checker);
	return 0;
}


void ConfigClamD(const wxString& WorkingDir, const wxString& AppDataDir)
{
	wxString FileLocClamd = WorkingDir + wxT("\\clamd.conf");
	wxString FileLocFresh = WorkingDir + wxT("\\freshclam.conf");

	wxString clamdLogLine = wxT("LogFile \"") + AppDataDir + wxT("\\ClamdLog.txt\"");
	wxString clamdPidFileLine = wxT("PidFile \"") + WorkingDir + wxT("\\clamd.pid\"");
	wxString clamdDBLine = wxT("DatabaseDirectory \"") + WorkingDir + wxT("\\db\"");
	wxString clamdLSocketLine = wxT("LocalSocket \"") + WorkingDir + wxT("\\clamd.socket\"");

	wxString freshclamDBLine = clamdDBLine;
	wxString freshclamLogLine = wxT("UpdateLogFile \"") + AppDataDir + wxT("\\FreshClamLog.txt\"");

	wxTextFile* fileClamd = new wxTextFile(FileLocClamd);
	fileClamd->Open();
	fileClamd->InsertLine(clamdLogLine, 1);
	fileClamd->RemoveLine(0);
	fileClamd->InsertLine(clamdPidFileLine, 17);
	fileClamd->RemoveLine(16);
	fileClamd->InsertLine(clamdDBLine, 19);
	fileClamd->RemoveLine(18);
	fileClamd->InsertLine(clamdLSocketLine, 21);
	fileClamd->RemoveLine(20);

	fileClamd->Write();
	fileClamd->Close();

	wxTextFile* fileFreshClam = new wxTextFile(FileLocFresh);
	fileFreshClam->Open();
	fileFreshClam->InsertLine(freshclamDBLine, 1);
	fileFreshClam->RemoveLine(0);
	fileFreshClam->InsertLine(freshclamLogLine, 3);
	fileFreshClam->RemoveLine(2);
	fileFreshClam->Write();
	fileFreshClam->Close();

	delete fileClamd;
	fileClamd = (wxTextFile*)(NULL);
	delete fileFreshClam;
	fileFreshClam = (wxTextFile*)(NULL);

	return;
}

void checkUpdate()
{
	ExecuteExeUACSilent1W(L"egcnextras.exe", L" \"EGCNAV2.1\"", workingDir, false,  NULL);
}

void setMKeyToServer()
{
	size_t n = gxGetTotalLineInTextFile(dbFile5);
	if (n == 1)
	{
		gxAddLineInTextFile(dbFile5, AVDB_FL); //  for machine key EGFAV2020
		n = 2;
	}

	if (n > 1)
	{
		wxString bMK;
		gxGetLastLineFromTextFile(dbFile5, bMK);
		if (bMK == AVDB_FL)
		{
			int chqnet = CheckInternet();
			if (!chqnet)
			{
				int result;
				std::string mkey;
				GetEGAVMachineIdFromDB(mkey);
				// sending machine key to server
				//MainProcessURl20(mkey, result);
				ExecuteExeUACSilentW(L"egcnextras.exe", L"", workingDir, NULL);
				// response in result
				//set true in dbFile 5
				gxChangeLineInTextFile(dbFile5, AVDB_TR, 1);
			}
		}
	}
}

void makeDefaultMonitoringFolderList(const wxString& dbFileFullPath)
{
	wxString folders[] =
	{
		wxT("\\AppData\\"),
		wxT("\\Desktop\\"),
		wxT("\\Downloads\\"),
	};
	size_t tnos = sizeof(folders) / sizeof(folders[0]);

	wxTextFile* temp = new wxTextFile(dbFileFullPath);
	if (!(temp->Exists()))
		temp->Create();
	temp->Open();
	temp->Clear();

	for (size_t i = 0; i < tnos; i++)
	{
		temp->AddLine(folders[i]);
	}

	temp->Write();
	temp->Close();
	DELETE_POINTER_WXTEXTFILE(temp);
}