#ifndef EGAV_MESSAGE_BOX_H
#define EGAV_MESSAGE_BOX_H

#include <wx\wx.h>
#include "gxButton.h"

class  MyMsgBox :public wxDialog
{
public:
	MyMsgBox(const wxString& title, const wxString& innerText, const wxSize& winSize);
	
	void OnBtnOk(wxCommandEvent& event);
	void OnExit(wxKeyEvent& event);

	MyWXRoundEdgeButton* BtnOk;

	wxPanel* Panel_InnerTxt;
	wxPanel* Panel_Buttons;

	wxStaticText* InnerTxt;

	wxBoxSizer* SizerV_All;
	wxBoxSizer* SizerV_Upper;
	wxBoxSizer* SizerH_Lower;

};


class MyMsgBox2 :public wxDialog
{
public:
	MyMsgBox2(const wxString& title, const wxString& innerText, const wxSize& winSize = wxSize(410, 200));
	void OnBtnYes(wxCommandEvent& event);
	void OnBtnNo(wxCommandEvent& event);
	void OnBtnCancel(wxCommandEvent& event);

	MyWXRoundEdgeButton* BtnYes;
	MyWXRoundEdgeButton* BtnNo;
	MyWXRoundEdgeButton* BtnCancel;

	wxPanel* Panel_InnerTxt;
	wxPanel* Panel_Buttons;

	wxStaticText* InnerTxt;

	wxBoxSizer* SizerV_All;
	wxBoxSizer* SizerV_Upper;
	wxBoxSizer* SizerH_Lower;

};

class  MyWaitingMsg :public wxDialog
{
public:
	MyWaitingMsg(const wxString& intialStr, unsigned int timeInSec, bool bDestroyWindow, const wxString& finalStr, const wxSize& winSize = wxSize(300, 200));
	void OnBtnOk(wxCommandEvent& event);
	void OnMsgTimer(wxTimerEvent& tmrEvent);

	MyWXRoundEdgeButton* BtnOk;

	wxPanel* Panel_InnerTxt;
	wxPanel* Panel_Buttons;

	wxStaticText* InnerTxt;

	wxBoxSizer* SizerV_All;
	wxBoxSizer* SizerV_Upper;
	wxBoxSizer* SizerH_Lower;

	wxTimer* msgtimer;

	wxString FinalStr;
	unsigned int WaitingtimeInMillsec;
	unsigned int WaitingtimeInsec;
	bool m_bDestroyWindow;

	clock_t StaringTime;


};

enum eBTN_ID8
{
	ID_MSGBOX_BTN_OK = 260,
	ID_MSGBOX2_BTN_YES, 
	ID_MSGBOX2_BTN_NO,
	ID_MSGBOX2_BTN_CANCEL, 
	ID_WAIT_MSGBOX_BTN_OK, 
	ID_TIMER_WAIT_MSG,
};

void AVShowMsg(const wxString& titleStr, const wxString& MsgStr, const wxSize& winSize = wxSize(300, 200));
void AVShowMsgYN(const wxString& titleStr, const wxString& MsgStr, const wxSize& winSize = wxSize(450, 200));
void AVShowMsgWait(const wxString& intialStr, unsigned int timeInSec, bool bDestroyWindow, const wxString& finalStr, const wxSize& winSize = wxSize(300, 200));

#endif