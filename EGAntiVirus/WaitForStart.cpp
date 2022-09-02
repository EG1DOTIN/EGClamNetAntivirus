#include "WaitForStart.h"
#include "avtheme.h"
#include "cleanupapi.h"

wxArrayString sizeOfAllFieldsInCleaner;

extern bool gbIsCleanerCancelled;
extern wxString workingDir;

DWORD WINAPI WaitStartScanningForClean(LPVOID lparm)
{
	Size_GetAll(&sizeOfAllFieldsInCleaner);
	return 0;
}

WaitForStart::WaitForStart(const wxString& titleStr, const wxString& InnerStr, const wxSize& winSize)
	: wxDialog(NULL, -1, titleStr, wxDefaultPosition, winSize, EGAV_DIALOG_BORDER_STYLE)
{
	Connect(ID_CLEANER_WAITING_FOR_START_TIMER, wxEVT_TIMER, wxTimerEventHandler(WaitForStart::OnWaitForStartTimer));
	Connect(ID_CLEANER_WAITING_FOR_START_BTN_CANCEL, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(WaitForStart::OnBtnCancel));

	CreateThread(0, 0, WaitStartScanningForClean, 0, 0, 0); //thread for scanning field for cleaning
	waitForStartTimer = new wxTimer(this, ID_CLEANER_WAITING_FOR_START_TIMER);
	waitForStartTimer->Start();
	Panel_InnerTxt = new wxPanel(this);
	Panel_Buttons = new wxPanel(this);
	Panel_Animation = new wxPanel(this);

	BtnCancel = new MyWXRoundEdgeButton(Panel_Buttons, ID_CLEANER_WAITING_FOR_START_BTN_CANCEL, wxT("Cancel"), wxDefaultPosition, EGAV_BUTTON_SIZE_MID, EGAV_BUTTON_STYLE_NOBORDER);

	InnerTxt = new wxStaticText(Panel_InnerTxt, -1, InnerStr);
	InnerTxt->SetFont(wxFont(12, wxFONTFAMILY_DECORATIVE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
	InnerTxt->SetForegroundColour(EGAV_TEXT_COLOR_WHITE);

	SizerV_Upper = new wxBoxSizer(wxVERTICAL);
	SizerV_Upper->Add(InnerTxt, 1, wxALIGN_CENTER_VERTICAL | wxEXPAND | wxALL, 10);
	Panel_InnerTxt->SetSizer(SizerV_Upper);

	SizerH_Lower = new wxBoxSizer(wxHORIZONTAL);
	SizerH_Lower->Add(BtnCancel, 1, wxALIGN_RIGHT, 10);
	Panel_Buttons->SetSizer(SizerH_Lower);

	Animation = new wxAnimationCtrl(Panel_Animation, -1);
	Animation->SetBackgroundColour(EGAV_WINDOW_BACK_COLOR1);

	if (Animation->LoadFile(workingDir + wxT("\\Resources\\egavhg.gif")))
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

	Center();
	ShowModal();
	//	 Destroy();
}

void WaitForStart::OnWaitForStartTimer(wxTimerEvent& tmrEvent)
{
	if (sizeOfAllFieldsInCleaner.GetCount() == 10)
	{
		waitForStartTimer->Stop();
		Animation->Stop();
		Close(true);
		Destroy();
	}
}

void WaitForStart::OnBtnCancel(wxCommandEvent& event)
{
	gbIsCleanerCancelled = true;
	sizeOfAllFieldsInCleaner.Clear();
	waitForStartTimer->Stop();
	Close(true);
	Destroy();
}