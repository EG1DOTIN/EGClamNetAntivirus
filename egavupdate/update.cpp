#include "common.h"
#include "update.h"
#include "avtheme.h"
#include <wx\process.h>
#include <wx\txtstrm.h>
#include <wx\textfile.h>
#include "egavicon.h"
#include "avmsgebox.h"

BEGIN_EVENT_TABLE(UpdateVirusDefinition, wxFrame)
EVT_CLOSE(UpdateVirusDefinition::OnClose)
END_EVENT_TABLE()

extern wxString workingDir;
extern wxString AppDataEGAVPath;
extern bool gbUpdateVirusDefinitionFrame;

wxString msgUpdate;
const wxString logFileUpdate = wxT("updatelog.txt");

void GetUpdateCmd(wxString& strOut)
{
	wxString freshClam = wxT("freshclam.exe");

	strOut = workingDir + wxT("\\") + freshClam + wxT(" --show-progress --check=2 --datadir=\"") + workingDir + wxT("\\db\" --log=\"") + AppDataEGAVPath + wxT("\\updatelog.txt\"");
}

UpdateVirusDefinition::UpdateVirusDefinition(const wxString& title) :wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(550, 350))
{
	gbUpdateVirusDefinitionFrame = true;
	SetEGAVFrameIcon(this);
	SetBackgroundColour(EGAV_WINDOW_BACK_COLOR1);

	Connect(ID_UPDATE_BUTTON_STOP, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(UpdateVirusDefinition::OnBtnstop));
	Connect(ID_UPDATE_TIMER, wxEVT_TIMER, wxTimerEventHandler(UpdateVirusDefinition::OnTimerUpdate));

	m_PanelAll = new wxPanel(this);
	m_PanelHeading = new wxPanel(m_PanelAll);
	m_PanelTextCtrl = new wxPanel(m_PanelAll);
	m_PanelButton = new wxPanel(m_PanelAll);

	m_TimerUpdate = new wxTimer(this, ID_UPDATE_TIMER);
	m_idleU = new wxIdleEvent;

	m_ST_Heading = new wxStaticText(m_PanelHeading, -1, wxT("Updating Virus Signature Database..."));
	m_ST_Heading->SetFont((wxFont(16, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL)));
	m_ST_Heading->SetForegroundColour(EGAV_TEXT_COLOR_WHITE);

	m_sH_Heading = new wxBoxSizer(wxHORIZONTAL);
	m_sH_Heading->Add(m_ST_Heading, 1, wxALIGN_CENTER, 10);
	m_PanelHeading->SetSizer(m_sH_Heading);

	m_TextCtrl_Update = new gxTextCtrlURL(m_PanelTextCtrl, -1, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_READONLY | wxTE_AUTO_URL);
	m_sH_TextCtrl = new wxBoxSizer(wxHORIZONTAL);
	m_sH_TextCtrl->Add(m_TextCtrl_Update, 1, wxEXPAND, 10);
	m_PanelTextCtrl->SetSizer(m_sH_TextCtrl);

	m_Button_Stop = new wxButton(m_PanelButton, ID_UPDATE_BUTTON_STOP, wxT("Stop"));
	m_sH_Button = new wxBoxSizer(wxHORIZONTAL);
	m_sH_Button->Add(m_Button_Stop, 1, wxALIGN_CENTER, 10);
	m_PanelButton->SetSizer(m_sH_Button);

	m_sV_All = new wxBoxSizer(wxVERTICAL);
	m_sV_All->Add(m_PanelHeading, 1, wxALIGN_CENTER, 10);
	m_sV_All->Add(m_PanelTextCtrl, 5, wxEXPAND | wxALL, 10);
	m_sV_All->Add(m_PanelButton, 1, wxALIGN_CENTER, 10);
	m_PanelAll->SetSizer(m_sV_All);

	wxString cmdtext;
	GetUpdateCmd(cmdtext);
	m_processUpdate = wxProcess::Open(cmdtext, wxEXEC_ASYNC | wxEXEC_HIDE_CONSOLE);
	m_processUpdate->Redirect();
	m_idleU->GetMode();
	m_ProcessUpdateId = m_processUpdate->GetPid();
	m_TimerUpdate->Start();

	Center();

}

void gxLoadFileInString(const wxString& textfilePath, wxString& linesStr)
{
	wxTextFile* tempfile = new wxTextFile(textfilePath);
	if (!tempfile->Exists())
		return;
	tempfile->Open();
	size_t nl = tempfile->GetLineCount();
	for (size_t i = 0; i < nl; i++)
	{
		linesStr += tempfile->GetLine(i) + wxT("\n");
	}
	linesStr += wxT("\n\n-------Completed---------");
	tempfile->Close();
	delete tempfile;
	tempfile = (wxTextFile*)(NULL);
}

void UpdateVirusDefinition::OnExitUpdateWindow()
{
	if (m_TimerUpdate->IsRunning())
		m_TimerUpdate->Stop();
	if (wxProcess::Exists(m_ProcessUpdateId))
	{
		wxProcess::Kill(m_processUpdate->GetPid(), wxSIGINT, wxKILL_NOCHILDREN);
		m_Button_Stop->SetLabelText(wxT("Stopping"));
		m_Button_Stop->Refresh();
		wxProcess::Kill(m_processUpdate->GetPid(), wxSIGKILL, wxKILL_CHILDREN);
		//wxRemoveFile(m_LogFileScan);
		msgUpdate.Clear();
	}
}

void UpdateVirusDefinition::OnBtnstop(wxCommandEvent& WXUNUSED(btnEvent))
{
	wxString btnLabel = m_Button_Stop->GetLabelText();
	if (btnLabel == wxT("Stop"))
	{
		OnExitUpdateWindow();
		m_Button_Stop->SetLabelText(wxT("Close"));
	}
	else if (btnLabel == wxT("Close"))
		Close(true);
}

void UpdateVirusDefinition::OnTimerUpdate(wxTimerEvent& tmrEvent)
{
	bool IsProcessExist = wxProcess::Exists(m_ProcessUpdateId);
	if (IsProcessExist)
	{
		if (m_processUpdate)
		{
			if (m_processUpdate->IsInputAvailable())
			{
				wxInputStream *pUout = m_processUpdate->GetInputStream();
				wxBufferedInputStream tbU(*pUout);
				int checkCharU = tbU.GetC();
				if ((checkCharU >= 0) && (checkCharU < 255))
				{
					wxTextInputStream tuStream(*pUout);
					msgUpdate += tuStream.ReadLine() + wxT("\n");
				}
				else
					return;
				//-----------------------------------------------------------//
				m_TextCtrl_Update->SetValue(msgUpdate);
				m_TextCtrl_Update->SetSelection(m_TextCtrl_Update->GetLastPosition(), -1);
				//-----------------------------------------------------------//
			}
			m_idleU->RequestMore();
		}
		else
		{
			m_TimerUpdate->Stop();
			wxMilliSleep(100);
			wxString tempstr;
			gxLoadFileInString(AppDataEGAVPath + wxT("\\") + logFileUpdate, tempstr);
			m_TextCtrl_Update->SetValue(tempstr);
			m_TextCtrl_Update->SetSelection(m_TextCtrl_Update->GetLastPosition(), -1);
			tempstr.Clear();
			msgUpdate.Clear();
			m_Button_Stop->SetLabelText(wxT("Close"));
			ChangeInUpdateDBFile();
			SuccessAppUpdateLogsW(L"An Attempt Of Virus Signature Database Manual Update Is Completed.");
			//wxRemoveFile(m_LogFileScan);			
		}
	}
	else
	{
		m_TimerUpdate->Stop();
		wxMilliSleep(100);
		wxString tempstr;
		gxLoadFileInString(AppDataEGAVPath + wxT("\\") + logFileUpdate, tempstr);
		m_TextCtrl_Update->SetValue(tempstr);
		m_TextCtrl_Update->SetSelection(m_TextCtrl_Update->GetLastPosition(), -1);
		tempstr.Clear();
		msgUpdate.Clear();
		m_Button_Stop->SetLabelText(wxT("Close"));
		ChangeInUpdateDBFile();
		SuccessAppUpdateLogsW(L"An Attempt Of Virus Signature Database Manual Update Is completed.");
		//wxRemoveFile(m_LogFileScan);
	}

}

void UpdateVirusDefinition::OnClose(wxCloseEvent& event)
{
	OnExitUpdateWindow();
	gbUpdateVirusDefinitionFrame = false;
	event.Skip();
	Destroy();
}

void ChangeInUpdateDBFile()
{
	//save current time status in str
	time_t s = time(0);
	wxString TimeNowStrFrom1970;
	TimeNowStrFrom1970.Printf("%u", s);

	//save current time status in file
	wxTextFile* tempfile = new wxTextFile(AppDataEGAVPath + wxT("\\db\\db4"));
	if (!(tempfile->Exists()))
		tempfile->Create();
	tempfile->Open();
	tempfile->Clear();
	tempfile->AddLine(TimeNowStrFrom1970);
	tempfile->Write();
	tempfile->Close();
	DELETE_POINTER(wxTextFile, tempfile);
}