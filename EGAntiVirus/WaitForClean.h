#ifndef WAIT_FOR_CLEANING_HEADER
#define WAIT_FOR_CLEANING_HEADER

#include <wx\wx.h>
#include "gxButton.h"
#include <wx\animate.h>

class  WaitForCleaning :public wxDialog
{
public:
	WaitForCleaning(const wxString& titleStr, const wxSize& winSize = wxSize(300, 200));
	void OnWaitCleaningTimer(wxTimerEvent& tmrEvent);
	void OnBtnOk(wxCommandEvent& event);

	wxPanel* Panel_InnerTxt;
	wxPanel* Panel_Buttons;
	wxPanel* Panel_Animation;

	MyWXRoundEdgeButton* BtnOk;

	wxStaticText* InnerTxt;

	wxBoxSizer* SizerV_All;
	wxBoxSizer* SizerV_Upper;
	wxBoxSizer* SizerH_Lower;
	wxBoxSizer* SizerH_Animation;

	wxAnimationCtrl* Animation;

	wxTimer* waitCleaningTimer;
};

enum eBTN_ID17
{
	//450 -469
	ID_CLEANER_WAITING_FOR_CLEAN_TIMER = 450,
	ID_CLEANER_WAITING_FOR_CLEAN_BTN_OK,
};

#endif