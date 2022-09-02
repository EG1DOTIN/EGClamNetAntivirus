#ifndef WAIT_FOR_START_CLEANER_HEADER
#define WAIT_FOR_START_CLEANER_HEADER

#include <wx\wx.h>
#include "gxButton.h"
#include <wx\animate.h>

class  WaitForStart :public wxDialog
{
public:
	WaitForStart(const wxString& titleStr, const wxString& InnerStr, const wxSize& winSize = wxSize(300, 200));
	void OnWaitForStartTimer(wxTimerEvent& tmrEvent);
	void OnBtnCancel(wxCommandEvent& event);

	wxPanel* Panel_InnerTxt;
	wxPanel* Panel_Buttons;
	wxPanel* Panel_Animation;


	MyWXRoundEdgeButton* BtnCancel;

	wxStaticText* InnerTxt;

	wxBoxSizer* SizerV_All;
	wxBoxSizer* SizerV_Upper;
	wxBoxSizer* SizerH_Lower;
	wxBoxSizer* SizerH_Animation;

	wxAnimationCtrl* Animation;

	wxTimer* waitForStartTimer;
};

enum eBTN_ID16
{
	//430 -449
	ID_CLEANER_WAITING_FOR_START_TIMER = 430,
	ID_CLEANER_WAITING_FOR_START_BTN_CANCEL,
};

DWORD WINAPI WaitStartScanningForClean(LPVOID lparm);

#endif