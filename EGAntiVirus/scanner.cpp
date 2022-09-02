#include "common.h"
#include "lastline.h"
#include "gxfunctions.h"
#include "gxDelPointer.h"
#include "scanner.h"
#include "avtheme.h"
#include "scancmds.h"
#include <wx\process.h>
#include "avmsgebox.h"
#include <wx/txtstrm.h>
#include "egavicon.h"
#include "VirusDetails.h"
#include "SendMsgToService.h"

#include "avdb.h"


extern bool gbScanSwitch;

extern wxString workingDir;
extern wxString AppDataEGAVPath;
extern wxString dbFile1;

const wxString QuickScanBATFileName = wxT("tempquickscan");
const wxString FullScanBATFileName = wxT("tempfullscan");

BEGIN_EVENT_TABLE(ScannerWin, wxFrame)
EVT_CLOSE(ScannerWin::OnClose)
END_EVENT_TABLE()

ScannerWin::ScannerWin(const wxString& title, const wxString& scanningType, const wxString& filesToScan) :wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(600, 400))
{
	gxGetLineFromTextFile(dbFile1, 9, m_isFastScan);

	SetEGAVFrameIcon(this);
	Connect(ID_SCANNER_BUTTON, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ScannerWin::OnBtnstop));
	Connect(ID_SCANNER_TIMER, wxEVT_TIMER, wxTimerEventHandler(ScannerWin::OnTimerScanner));
	Connect(ID_FSCANNER_TIMER, wxEVT_TIMER, wxTimerEventHandler(ScannerWin::OnTimerFastScanner));
	

	m_PanelAll = new wxPanel(this);
	m_PanelTextHeading = new wxPanel(m_PanelAll);
	m_PanelTextProgress = new wxPanel(m_PanelAll);
	m_PanelTextScanning = new wxPanel(m_PanelAll);
	m_PanelTextInfectedFiles = new wxPanel(m_PanelAll);
	m_PanelButton = new wxPanel(m_PanelAll);

	m_TimerScanner = new wxTimer(this, ID_SCANNER_TIMER);
	m_TimerFScanner = new wxTimer(this, ID_FSCANNER_TIMER);
	m_idleS = new wxIdleEvent;

	m_TextHeading = new wxStaticText(m_PanelTextHeading, -1, scanningType);
	m_TextHeading->SetFont(wxFont(24, wxFONTFAMILY_DECORATIVE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
	m_TextHeading->SetForegroundColour(*wxWHITE);

	m_SHTextHeading = new wxBoxSizer(wxHORIZONTAL);
	m_SHTextHeading->Add(m_TextHeading, 1, wxALIGN_CENTER, 10);
	m_PanelTextHeading->SetSizer(m_SHTextHeading);

	m_animationCtrl = new wxAnimationCtrl(m_PanelTextProgress, -1);
	m_animationCtrl->SetBackgroundColour(EGAV_WINDOW_BACK_COLOR1);

	if (m_animationCtrl->LoadFile(workingDir + wxT("\\Resources\\egav.gif"), wxANIMATION_TYPE_GIF))
		m_animationCtrl->Play();


	m_SHTextProgress = new wxBoxSizer(wxHORIZONTAL);
	m_SHTextProgress->Add(m_animationCtrl, 1, wxALIGN_CENTER, 10);
	m_PanelTextProgress->SetSizer(m_SHTextProgress);

	wxString textScanning = (m_isFastScan == AVDB_TR) ? wxT("scanning...") : wxT("Please wait, loading virus signature database...");
	m_TextScanning = new wxStaticText(m_PanelTextScanning, -1, textScanning, wxDefaultPosition, wxSize(550, 30));
	m_TextScanning->SetFont(wxFont(10, wxFONTFAMILY_DECORATIVE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
	m_TextScanning->SetForegroundColour(*wxWHITE);

	m_SHTextScanning = new wxBoxSizer(wxHORIZONTAL);
	m_SHTextScanning->Add(m_TextScanning, 1, wxEXPAND, 10);
	m_PanelTextScanning->SetSizer(m_SHTextScanning);


	m_TextInfectedFiles = new wxStaticText(m_PanelTextInfectedFiles, -1, wxT("Infected Files: "));
	m_TextInfectedFiles->SetFont(wxFont(12, wxFONTFAMILY_DECORATIVE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
	m_TextInfectedFiles->SetForegroundColour(*wxWHITE);

	m_TextVirusCount = new wxStaticText(m_PanelTextInfectedFiles, -1, wxT(""));
	m_TextVirusCount->SetFont(wxFont(12, wxFONTFAMILY_DECORATIVE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
	m_TextVirusCount->SetForegroundColour(*wxRED);

	m_SHTextInfectedFiles = new wxBoxSizer(wxHORIZONTAL);
	m_SHTextInfectedFiles->AddStretchSpacer();
	m_SHTextInfectedFiles->Add(m_TextInfectedFiles, 1, wxALIGN_CENTER, 1);
	m_SHTextInfectedFiles->Add(m_TextVirusCount, 1, wxALIGN_CENTER, 1);
	m_SHTextInfectedFiles->AddStretchSpacer();
	m_PanelTextInfectedFiles->SetSizer(m_SHTextInfectedFiles);

	m_ButtonStop = new MyWXRoundEdgeButton(m_PanelButton, ID_SCANNER_BUTTON, wxT("Stop"));

	m_SHButton = new wxBoxSizer(wxHORIZONTAL);
	m_SHButton->Add(m_ButtonStop, 0, wxALIGN_CENTER, 10);
	m_PanelButton->SetSizer(m_SHButton);

	m_SVAll = new wxBoxSizer(wxVERTICAL);
	m_SVAll->AddStretchSpacer();
	m_SVAll->Add(m_PanelTextHeading, 2, wxALIGN_CENTER, 1);
	m_SVAll->AddStretchSpacer();
	m_SVAll->Add(m_PanelTextProgress, 4, wxALIGN_CENTER, 1);
	m_SVAll->AddStretchSpacer();
	m_SVAll->Add(m_PanelTextScanning, 1, wxALIGN_CENTER, 1);
	m_SVAll->AddStretchSpacer();
	m_SVAll->Add(m_PanelTextInfectedFiles, 1, wxALIGN_CENTER, 1);
	m_SVAll->AddStretchSpacer();
	m_SVAll->Add(m_PanelButton, 2, wxALIGN_CENTER, 1);
	m_SVAll->AddStretchSpacer();

	m_PanelAll->SetSizer(m_SVAll);


	SetBackgroundColour(EGAV_WINDOW_BACK_COLOR1);

	m_FileForScan = filesToScan;
	m_ScanType = scanningType;
	m_VirusCount = 0;

	m_LogFileScan = gxMakeScanLogTextFileWithNameNow(AppDataEGAVPath);

	if (m_ScanType == wxT("Custom Scan"))
	{	
		if (m_isFastScan == AVDB_TR)
		{
			wxString customFastscan;
			GetFastScanCmd(customFastscan);
			m_CmdToScan = workingDir + wxT("\\") + customFastscan + m_FileForScan;
		}
		else
		{
			wxString customscan;
			GetScanCmd(customscan);
			m_CmdToScan = workingDir + wxT("\\") + customscan + m_FileForScan;
		}
	}
	else if (m_ScanType == wxT("Quick Scan"))
	{
		wxString memoryscancmd;
		GetMemoryScanCmd(memoryscancmd);
		wxString quickscancmd = wxEmptyString;
		//GetQuickScanCmd(quickscancmd);
		//m_CmdToScan = memoryscancmd + wxT("&& ") + quickscancmd + m_FileForScan;
		gxMakeCmdBATFileForQuickNFullScan(QuickScanBATFileName, memoryscancmd, quickscancmd + m_FileForScan);
		m_CmdToScan = AppDataEGAVPath + wxT("\\") + QuickScanBATFileName + wxT(".bat");
	}
	else if (m_ScanType == wxT("Full Scan"))
	{
		wxString memoryscancmd;
		GetMemoryScanCmd(memoryscancmd);
		wxString fullscancmd;
		GetFullScanCmd(fullscancmd);
		//m_CmdToScan = memoryscancmd + wxT("&& ") + fullscancmd + m_FileForScan;
		gxMakeCmdBATFileForQuickNFullScan(FullScanBATFileName, memoryscancmd, fullscancmd + m_FileForScan);
		m_CmdToScan = AppDataEGAVPath + wxT("\\") + FullScanBATFileName + wxT(".bat");
	}
	else
	{
		//
	}

	//SuccessAppWinLogW(L"Working Dir: %s", workingDir.wc_str());
	//SuccessAppWinLogW(L"Scan command : %s", m_CmdToScan.wc_str());
	nFiles1 = getTotalInfectedFiles();
	if (!((m_ScanType == wxT("Custom Scan")) && (m_isFastScan == AVDB_TR)))
	{
		m_processScanner = wxProcess::Open(m_CmdToScan, wxEXEC_ASYNC | wxEXEC_MAKE_GROUP_LEADER | wxEXEC_HIDE_CONSOLE);
		m_processScanner->Redirect();
		m_idleS->GetMode();
		SendMsgToService(SERVICE_CONTROL_CUSTOM_MSG_STOP_CLAMD);
		m_TimerScanner->Start(250,false);
	}
	else
	{
		m_ClamdLogsFile = AppDataEGAVPath + wxT("\\ClamdLog.txt");
		m_processScanner = wxProcess::Open(m_CmdToScan, wxEXEC_ASYNC | wxEXEC_HIDE_CONSOLE);
		m_TimerFScanner->Start(250, false);
	}

	gbScanSwitch = true;
	Center();
	Restore();
	Raise();
}

void ScannerWin::OnBtnstop(wxCommandEvent& btnEvent)
{
	if (!((m_ScanType == wxT("Custom Scan")) && (m_isFastScan == AVDB_TR)))
	{
		gbScanSwitch = false;
		if (m_TimerScanner->IsRunning())
			m_TimerScanner->Stop();
		wxProcess::Kill(m_processScanner->GetPid(), wxSIGINT, wxKILL_NOCHILDREN);
		m_animationCtrl->Stop();

		Hide();

		AVShowMsgWait(wxT("Please wait, terminating ") + m_ScanType + wxT("..."), 5, true, wxT(""));
		nFiles2 = getTotalInfectedFiles();
		AVShowVirusDetail(EGAV_TITLE_NAME, m_LogFileScan, m_ScanType + wxT(" cancelled"), wxT("Infected Files: ") + VirusCountStr(nFiles2 - nFiles1));

		wxProcess::Kill(m_processScanner->GetPid(), wxSIGKILL, wxKILL_CHILDREN);
		m_VirusCount = 0;

		SendMsgToService(SERVICE_CONTROL_CUSTOM_MSG_START_CLAMD);

		//wxRemoveFile(m_LogFileScan);
		if (m_ScanType == wxT("Quick Scan"))
		{
			gxRemoveCmdBATScanFile(QuickScanBATFileName);
		}
		if (m_ScanType == wxT("Full Scan"))
		{
			gxRemoveCmdBATScanFile(FullScanBATFileName);
		}
	}
	else
	{
		gbScanSwitch = false;
		if (m_TimerFScanner->IsRunning())
			m_TimerFScanner->Stop();

		wxProcess::Kill(m_processScanner->GetPid(), wxSIGINT, wxKILL_NOCHILDREN);
		SendMsgToService(SERVICE_CONTROL_CUSTOM_MSG_STOP_CLAMD);
		m_animationCtrl->Stop();
		Hide();

		AVShowMsgWait(wxT("Please wait, terminating ") + m_ScanType + wxT("..."), 5, true, wxT(""));
		SendMsgToService(SERVICE_CONTROL_CUSTOM_MSG_START_CLAMD);
		nFiles2 = getTotalInfectedFiles();
		AVShowVirusDetail(EGAV_TITLE_NAME, m_LogFileScan, m_ScanType + wxT(" cancelled"), wxT("Infected Files: ") + VirusCountStr(nFiles2 - nFiles1));
		// kill scanning process if not stopped
		m_VirusCount = 0;
	}

	gbScanSwitch = false;
	Close(true);
}

wxString ScannerWin::VirusCountStr(int viruscount)
{
	wxString res;
	if (viruscount == 0)
	{
		res = wxT("Not Found.");
	}
	else
	{
		res.Printf("%d", viruscount);
	}

	return res;
}

void ScannerWin::OnTimerScanner(wxTimerEvent& tmrEvent)
{
	long ScanId = m_processScanner->GetPid();
	bool IsProcessExist = wxProcess::Exists(ScanId);
	if (IsProcessExist)
	{
		if (m_processScanner)
		{
			if (m_processScanner->IsInputAvailable())
			{
				wxString msg;
				wxInputStream *pSout = m_processScanner->GetInputStream();
				wxBufferedInputStream tbS(*pSout);
				int checkCharS = tbS.GetC();
				//if ((checkCharS >= 0) && (checkCharS < 255))
				if (checkCharS != wxEOF)
				{
					wxTextInputStream tsStream(*pSout);
					msg = tsStream.ReadLine();
					if (msg.Contains(wxT("FOUND")) || msg.Contains(wxT("'FOUND',")))
					{
						gxAddLineInTextFile(m_LogFileScan, msg);
						m_VirusCount++;
						m_TextVirusCount->SetLabelText(VirusCountStr(m_VirusCount));
						m_TextVirusCount->Refresh();
					}

				}
				else
					return;
				//-----------------------------------------------------------//
				m_TextScanning->SetLabelText(msg);
				//-----------------------------------------------------------//
			}
			m_idleS->RequestMore();
		}
		else
		{
			m_TimerScanner->Stop();
			m_animationCtrl->Stop();
			
			/*
			AVShowMsg(
				EGAV_ADMIN_PANEL_TITLE,
				m_ScanType + wxT(" has been completed.\n\nInfected Files: ") + VirusCountStr(m_VirusCount)
				);	*/
			Hide();

			SendMsgToService(SERVICE_CONTROL_CUSTOM_MSG_START_CLAMD);
			nFiles2 = getTotalInfectedFiles();
			AVShowVirusDetail(EGAV_TITLE_NAME, m_LogFileScan, m_ScanType + wxT(" has been completed"), wxT("Infected Files : ") + VirusCountStr(nFiles2 - nFiles1));
			
			m_VirusCount = 0;
			//wxRemoveFile(m_LogFileScan);
			if (m_ScanType == wxT("Quick Scan"))
			{
				gxRemoveCmdBATScanFile(QuickScanBATFileName);
			}
			if (m_ScanType == wxT("Full Scan"))
			{
				gxRemoveCmdBATScanFile(FullScanBATFileName);
			}

			Close(true);
		}
	}
	else
	{
		m_TimerScanner->Stop();
		m_animationCtrl->Stop();

		Hide();

		SendMsgToService(SERVICE_CONTROL_CUSTOM_MSG_START_CLAMD);

		/*
		AVShowMsg(
		EGAV_ADMIN_PANEL_TITLE,
		m_ScanType + wxT(" has been completed.\n\nInfected Files: ") + VirusCountStr(m_VirusCount)
		);
		*/
		nFiles2 = getTotalInfectedFiles();
		AVShowVirusDetail(EGAV_TITLE_NAME, m_LogFileScan, m_ScanType + wxT(" has been completed"), wxT("Infected Files : ") + VirusCountStr(nFiles2 - nFiles1));

		m_VirusCount = 0;
		//wxRemoveFile(m_LogFileScan);
		if (m_ScanType == wxT("Quick Scan"))
		{
			gxRemoveCmdBATScanFile(QuickScanBATFileName);
		}
		if (m_ScanType == wxT("Full Scan"))
		{
			gxRemoveCmdBATScanFile(FullScanBATFileName);
		}

		Close(true);
	}

}

#if 1
void ScannerWin::OnTimerFastScanner(wxTimerEvent & tmrEvent)
{
	long ScanId = m_processScanner->GetPid();
	bool IsProcessExist = wxProcess::Exists(ScanId);
	if (IsProcessExist)
	{
		if (m_processScanner)
		{
			if (m_processScanner->IsInputAvailable())
			{
				wxString msg;
				wxInputStream* pSout = m_processScanner->GetInputStream();
				wxBufferedInputStream tbS(*pSout);
				int checkCharS = tbS.GetC();
				//if ((checkCharS >= 0) && (checkCharS < 255))
				if (checkCharS != wxEOF)
				{
					wxTextInputStream tsStream(*pSout);
					msg = tsStream.ReadLine();
					if (msg.Contains(wxT("FOUND")) || msg.Contains(wxT("'FOUND',")))
					{
						gxAddLineInTextFile(m_LogFileScan, msg);
						m_VirusCount++;
						m_TextVirusCount->SetLabelText(VirusCountStr(m_VirusCount));
						m_TextVirusCount->Refresh();
					}

				}
				else
					return;
				//-----------------------------------------------------------//
				m_TextScanning->SetLabelText(msg);
				//-----------------------------------------------------------//
			}
			m_idleS->RequestMore();
		}
		else
		{
			m_TimerFScanner->Stop();
			m_animationCtrl->Stop();

			/*
			AVShowMsg(
				EGAV_ADMIN_PANEL_TITLE,
				m_ScanType + wxT(" has been completed.\n\nInfected Files: ") + VirusCountStr(m_VirusCount)
				);	*/
			Hide();

			nFiles2 = getTotalInfectedFiles();
			AVShowVirusDetail(EGAV_TITLE_NAME, m_LogFileScan, m_ScanType + wxT(" has been completed"), wxT("Infected Files : ") + VirusCountStr(nFiles2 - nFiles1));

			m_VirusCount = 0;
			
			Close(true);
}
	}
	else
	{
		m_TimerFScanner->Stop();

		m_animationCtrl->Stop();

		Hide();

		/*
		AVShowMsg(
		EGAV_ADMIN_PANEL_TITLE,
		m_ScanType + wxT(" has been completed.\n\nInfected Files: ") + VirusCountStr(m_VirusCount)
		);
		*/
		nFiles2 = getTotalInfectedFiles();
		AVShowVirusDetail(EGAV_TITLE_NAME, m_LogFileScan, m_ScanType + wxT(" has been completed"), wxT("Infected Files : ") + VirusCountStr(nFiles2 - nFiles1));

		m_VirusCount = 0;
		//wxRemoveFile(m_LogFileScan);
		
		Close(true);
	}
}
#else
void ScannerWin::OnTimerFastScanner(wxTimerEvent& tmrEvent)
{
	long ScanId = m_processScanner->GetPid();
	bool IsProcessExist = wxProcess::Exists(ScanId);

	if (IsProcessExist)
	{
		if (m_processScanner)
		{
			string scannedlastfile;
			getlastlineFast(m_ClamdLogsFile.ToStdString(), scannedlastfile);
			wxString msg(scannedlastfile);
			if (msg.EndsWith(wxT("FOUND")))
			{
				if (m_LastVirusDetected != msg)
				{
					//if (!msg.Contains(wxT('ProgramData\\EGAntiVirus\\Vault')))
					{
						m_LastVirusDetected = msg;
						gxAddLineInTextFile(m_LogFileScan, msg);
						m_VirusCount++;
						m_TextVirusCount->SetLabelText(VirusCountStr(m_VirusCount));
						m_TextVirusCount->Refresh();
					}
				}
			}
			m_TextScanning->SetLabelText(msg);
			m_TextScanning->Refresh();
		}
		else
		{
			m_TimerFScanner->Stop();
			m_animationCtrl->Stop();
			Hide();
			nFiles2 = getTotalInfectedFiles();
			AVShowVirusDetail(EGAV_TITLE_NAME, m_LogFileScan, m_ScanType + wxT(" has been completed"), wxT("Infected Files : ") + VirusCountStr(nFiles2 - nFiles1));
			m_VirusCount = 0;
			Close(true);
		}
	}
	else
	{
		m_TimerFScanner->Stop();
		m_animationCtrl->Stop();
		Hide();
		nFiles2 = getTotalInfectedFiles();
		AVShowVirusDetail(EGAV_TITLE_NAME, m_LogFileScan, m_ScanType + wxT(" has been completed"), wxT("Infected Files : ") + VirusCountStr(nFiles2 - nFiles1));
		m_VirusCount = 0;
		Close(true);
	}
}
#endif


void ScannerWin::OnClose(wxCloseEvent& event)
{
	if (m_TimerScanner->IsRunning())
	{
		//stop scanning
		m_TimerScanner->Stop();
		m_animationCtrl->Stop();
		wxProcess::Kill(m_processScanner->GetPid(), wxSIGINT, wxKILL_NOCHILDREN);

		Hide();
		AVShowMsgWait(wxT("Please wait, terminating ") + m_ScanType + wxT("..."), 5, true, wxT(""));
		nFiles2 = getTotalInfectedFiles();
		AVShowVirusDetail(EGAV_TITLE_NAME, m_LogFileScan, m_ScanType + wxT(" cancelled"), wxT("Infected Files: ") + VirusCountStr(nFiles2 - nFiles1));

		wxProcess::Kill(m_processScanner->GetPid(), wxSIGKILL, wxKILL_CHILDREN);
		m_VirusCount = 0;

		SendMsgToService(SERVICE_CONTROL_CUSTOM_MSG_START_CLAMD);

		//wxRemoveFile(m_LogFileScan);
		if (m_ScanType == wxT("Quick Scan"))
		{
			gxRemoveCmdBATScanFile(QuickScanBATFileName);
		}
		if (m_ScanType == wxT("Full Scan"))
		{
			gxRemoveCmdBATScanFile(FullScanBATFileName);
		}
	}
	else if (m_TimerFScanner->IsRunning())
	{
		m_TimerFScanner->Stop();
		m_animationCtrl->Stop();
		wxProcess::Kill(m_processScanner->GetPid(), wxSIGINT, wxKILL_NOCHILDREN);
		SendMsgToService(SERVICE_CONTROL_CUSTOM_MSG_STOP_CLAMD);
		Hide();
		AVShowMsgWait(wxT("Please wait, terminating ") + m_ScanType + wxT("..."), 5, true, wxT(""));
		SendMsgToService(SERVICE_CONTROL_CUSTOM_MSG_START_CLAMD);
		nFiles2 = getTotalInfectedFiles();
		AVShowVirusDetail(EGAV_TITLE_NAME, m_LogFileScan, m_ScanType + wxT(" cancelled"), wxT("Infected Files: ") + VirusCountStr(nFiles2 - nFiles1));

		//wxProcess::Kill(m_processScanner->GetPid(), wxSIGKILL, wxKILL_CHILDREN);
		m_VirusCount = 0;
	}
	gbScanSwitch = false;
	event.Skip();
	Destroy();
}

wxString gxMakeScanLogTextFileWithNameNow(const wxString& dirPath)
{
	if (!wxDir::Exists(dirPath))
	{
		return wxEmptyString;
	}
	wxLongLong loc_time_ll = wxGetLocalTimeMillis();
	wxDateTime loc_dt(loc_time_ll);
	wxString date = loc_dt.Format(wxT("SCAN LOG %d-%b-%Y AT %H-%M-%S"));
	wxString fileName = dirPath + wxT("\\") + date + wxT(".txt");
	gxMakeTextFile(fileName);
	return fileName;
}

void gxMakeCmdBATFileForQuickNFullScan(const wxString& FileNameWithoutExt, const wxString& cmd1, const wxString& cmd2)
{
	wxString temp;
	if (cmd2 != wxT(""))
	{
		temp = cmd1 + wxT(" && ") + cmd2;
	}
	else
	{
		temp = cmd1 + cmd2;
	}

	gxRemoveCmdBATScanFile(FileNameWithoutExt);
	wxTextFile* quicscanbat = new wxTextFile(AppDataEGAVPath + wxT("\\") + FileNameWithoutExt + wxT(".bat"));
	quicscanbat->Create();
	quicscanbat->Open();
	quicscanbat->AddLine(temp);
	quicscanbat->Write();
	quicscanbat->Close();
	DELETE_POINTER_WXTEXTFILE(quicscanbat);
}

void gxRemoveCmdBATScanFile(const wxString& FileNameWithoutExt)
{
	wxString batfile = AppDataEGAVPath + wxT("\\") + FileNameWithoutExt + wxT(".bat");
	if (wxFileExists(batfile))
	{
		if (!wxRemoveFile(batfile))
			DeleteFileW(batfile.wc_str());
	}
}

size_t getTotalInfectedFiles()
{
	wxArrayString files;
	size_t n = wxDir::GetAllFiles(AppDataEGAVPath + wxT("\\Vault"), &files, wxT("*.infected"));
	files.clear();
	return n;
}

