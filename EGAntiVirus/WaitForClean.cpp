#include "gxfunctions.h"
#include "WaitForClean.h"
#include "avtheme.h"
#include "CleaningThreads.h"

extern bool gbIsCleaningFinished;
extern wxString gStrReportingForCleaning;
extern wxString workingDir;

extern ULLONG gSizeVal[10];
extern ULLONG TotalSizeFreed;

WaitForCleaning::WaitForCleaning(const wxString& titleStr, const wxSize& winSize)
	: wxDialog(NULL, -1, titleStr, wxDefaultPosition, winSize, EGAV_DIALOG_BORDER_STYLE)
{
	Connect(ID_CLEANER_WAITING_FOR_CLEAN_TIMER, wxEVT_TIMER, wxTimerEventHandler(WaitForCleaning::OnWaitCleaningTimer));
	Connect(ID_CLEANER_WAITING_FOR_CLEAN_BTN_OK, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(WaitForCleaning::OnBtnOk));

	waitCleaningTimer = new wxTimer(this, ID_CLEANER_WAITING_FOR_CLEAN_TIMER);
	Panel_InnerTxt = new wxPanel(this);
	Panel_Buttons = new wxPanel(this);
	Panel_Animation = new wxPanel(this);

	BtnOk = new MyWXRoundEdgeButton(Panel_Buttons, ID_CLEANER_WAITING_FOR_CLEAN_BTN_OK, wxT("OK"), wxDefaultPosition, EGAV_BUTTON_SIZE_MID, EGAV_BUTTON_STYLE_NOBORDER);

	InnerTxt = new wxStaticText(Panel_InnerTxt, -1, wxT("Cleaning..."));
	InnerTxt->SetFont(wxFont(12, wxFONTFAMILY_DECORATIVE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
	InnerTxt->SetForegroundColour(EGAV_TEXT_COLOR_WHITE);

	SizerV_Upper = new wxBoxSizer(wxVERTICAL);
	SizerV_Upper->Add(InnerTxt, 1, wxALIGN_CENTER_VERTICAL | wxEXPAND | wxALL, 10);
	
	Panel_InnerTxt->SetSizer(SizerV_Upper);

	SizerH_Lower = new wxBoxSizer(wxHORIZONTAL);
	SizerH_Lower->Add(BtnOk, 1, wxALIGN_RIGHT, 10);
	Panel_Buttons->SetSizer(SizerH_Lower);

	Animation = new wxAnimationCtrl(Panel_Animation, -1);
	Animation->SetBackgroundColour(EGAV_WINDOW_BACK_COLOR1);

	if (Animation->LoadFile(workingDir + wxT("\\Resources\\egaval.gif")))
		Animation->Play();

	SizerH_Animation = new wxBoxSizer(wxHORIZONTAL);
	SizerH_Animation->Add(Animation, 1, wxALIGN_CENTER, 10);
	Panel_Animation->SetSizer(SizerH_Animation);


	SizerV_All = new wxBoxSizer(wxVERTICAL);
	SizerV_All->Add(Panel_InnerTxt, 3, wxEXPAND | wxALL, 5);
	SizerV_All->Add(Panel_Animation, 1, wxALIGN_CENTER, 5);
	SizerV_All->Add(Panel_Buttons, 1, wxALIGN_CENTER, 5);
	SetSizer(SizerV_All);

	SetBackgroundColour(EGAV_WINDOW_BACK_COLOR1);
	BtnOk->Disable();

	Center();
	CreateThread(0, 0, CleaningMainThreadFunction, 0, 0, 0); //

	waitCleaningTimer->Start();

	ShowModal();
	//	 Destroy();
}

void WaitForCleaning::OnWaitCleaningTimer(wxTimerEvent& tmrEvent)
{
	if (gbIsCleaningFinished)
	{
		gbIsCleaningFinished = false;
		waitCleaningTimer->Stop();
		BtnOk->Enable();
		Animation->Stop();
		Animation->Hide();
	}

	InnerTxt->SetLabelText(gStrReportingForCleaning);
}

void WaitForCleaning::OnBtnOk(wxCommandEvent& event)
{
	gSizeVal[10] = { 0 };
	TotalSizeFreed = 0;
	gStrReportingForCleaning.Clear();
	event.Skip();
	Destroy();
}