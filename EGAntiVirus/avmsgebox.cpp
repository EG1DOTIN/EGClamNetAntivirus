#include "common.h"
#include "avmsgebox.h"
#include "avtheme.h"

MyMsgBox::MyMsgBox(const wxString& title, const wxString& innerText, const wxSize& winSize)
	: wxDialog(NULL, -1, title, wxDefaultPosition, winSize, EGAV_DIALOG_BORDER_STYLE)
{
	SetBackgroundColour(EGAV_WINDOW_BACK_COLOR1);
	Connect(ID_MSGBOX_BTN_OK, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MyMsgBox::OnBtnOk));
	Connect(wxEVT_CHAR_HOOK, wxKeyEventHandler(MyMsgBox::OnExit));

	Panel_InnerTxt = new wxPanel(this);
	Panel_Buttons = new wxPanel(this);

	BtnOk = new MyWXRoundEdgeButton(Panel_Buttons, ID_MSGBOX_BTN_OK, wxT("OK"));

	InnerTxt = new wxStaticText(Panel_InnerTxt, -1, innerText);
	InnerTxt->SetFont(wxFont(12, wxFONTFAMILY_DECORATIVE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
	InnerTxt->SetForegroundColour(EGAV_TEXT_COLOR_WHITE);

	SizerV_Upper = new wxBoxSizer(wxVERTICAL);
	SizerV_Upper->Add(InnerTxt, 1, wxALIGN_CENTER_VERTICAL | wxEXPAND | wxALL, 10);

	Panel_InnerTxt->SetSizer(SizerV_Upper);

	SizerH_Lower = new wxBoxSizer(wxHORIZONTAL);
	SizerH_Lower->Add(BtnOk, 1, wxALIGN_RIGHT, 10);
	Panel_Buttons->SetSizer(SizerH_Lower);

	SizerV_All = new wxBoxSizer(wxVERTICAL);
	SizerV_All->Add(Panel_InnerTxt, 3, wxEXPAND | wxALL, 10);
	SizerV_All->Add(Panel_Buttons, 1, wxALIGN_CENTER, 10);
	SetSizer(SizerV_All);

	Center();
	BtnOk->SetFocusFromKbd();
	ShowModal();
	//	 Destroy();

}

void MyMsgBox::OnBtnOk(wxCommandEvent& event)
{
	Close(true);
	Destroy();
}

void MyMsgBox::OnExit(wxKeyEvent& event)
{
	if (event.GetKeyCode() == WXK_ESCAPE)
	{
		EndModal(0);
	}
	else if (event.GetKeyCode() == 87 && event.ControlDown())
	{
		EndModal(0);
	}
	else
	{
		event.Skip();
	}
}


MyMsgBox2::MyMsgBox2(const wxString& title, const wxString& innerText, const wxSize& winSize)
	: wxDialog(NULL, -1, title, wxDefaultPosition, winSize, EGAV_DIALOG_BORDER_STYLE)
{
	Connect(ID_MSGBOX2_BTN_YES, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MyMsgBox2::OnBtnYes));
	Connect(ID_MSGBOX2_BTN_NO, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MyMsgBox2::OnBtnNo));
	Connect(ID_MSGBOX2_BTN_CANCEL, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MyMsgBox2::OnBtnCancel));

	Panel_InnerTxt = new wxPanel(this);
	Panel_Buttons = new wxPanel(this);

	BtnYes = new MyWXRoundEdgeButton(Panel_Buttons, ID_MSGBOX2_BTN_YES, wxT("Yes"), wxDefaultPosition, EGAV_BUTTON_SIZE_MID, EGAV_BUTTON_STYLE_NOBORDER);
	BtnNo = new MyWXRoundEdgeButton(Panel_Buttons, ID_MSGBOX2_BTN_NO, wxT("No"), wxDefaultPosition, EGAV_BUTTON_SIZE_MID, EGAV_BUTTON_STYLE_NOBORDER);
	BtnCancel = new MyWXRoundEdgeButton(Panel_Buttons, ID_MSGBOX2_BTN_CANCEL, wxT("Cancel"), wxDefaultPosition, EGAV_BUTTON_SIZE_MID, EGAV_BUTTON_STYLE_NOBORDER);

	InnerTxt = new wxStaticText(Panel_InnerTxt, -1, innerText);
	InnerTxt->SetFont(wxFont(12, wxFONTFAMILY_DECORATIVE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
	InnerTxt->SetForegroundColour(EGAV_TEXT_COLOR_WHITE);

	SizerV_Upper = new wxBoxSizer(wxVERTICAL);
	SizerV_Upper->Add(InnerTxt, 1, wxALIGN_CENTER_VERTICAL | wxEXPAND | wxALL, 10);

	Panel_InnerTxt->SetSizer(SizerV_Upper);

	SizerH_Lower = new wxBoxSizer(wxHORIZONTAL);
	SizerH_Lower->AddStretchSpacer();
	SizerH_Lower->Add(BtnYes, 1, 0, 5);
	SizerH_Lower->AddStretchSpacer();
	SizerH_Lower->Add(BtnNo, 1, 0, 5);
	SizerH_Lower->AddStretchSpacer();
	SizerH_Lower->Add(BtnCancel, 1, 0, 5);
	SizerH_Lower->AddStretchSpacer();

	Panel_Buttons->SetSizer(SizerH_Lower);

	SizerV_All = new wxBoxSizer(wxVERTICAL);
	SizerV_All->Add(Panel_InnerTxt, 3);
	SizerV_All->Add(Panel_Buttons, 1);
	SetSizer(SizerV_All);

	SetBackgroundColour(EGAV_WINDOW_BACK_COLOR1);

	Center();
	BtnCancel->SetFocusFromKbd();
	ShowModal();
	//	 Destroy();
}

bool gbMsgResponse = false;

void MyMsgBox2::OnBtnYes(wxCommandEvent& event)
{
	gbMsgResponse = true;
	Destroy();
}

void MyMsgBox2::OnBtnNo(wxCommandEvent& event)
{
	gbMsgResponse = false;
	Destroy();
}

void MyMsgBox2::OnBtnCancel(wxCommandEvent& event)
{
	Destroy();
}

void AVShowMsg(const wxString& titleStr, const wxString& MsgStr, const wxSize& winSize)
{
	//MyMsgBox(EGAV_ADMIN_PANEL_TITLE, MsgStr);
	MyMsgBox* tempMsg = new MyMsgBox(titleStr, MsgStr, winSize);
	tempMsg->Show();
	DELETE_POINTER(MyMsgBox, tempMsg);
}


void AVShowMsgYN(const wxString& titleStr, const wxString& MsgStr, const wxSize& winSize)
{
	MyMsgBox2* tempMsg = new MyMsgBox2(titleStr, MsgStr, winSize);
	tempMsg->Show();
	DELETE_POINTER(MyMsgBox2, tempMsg);
}

MyWaitingMsg::MyWaitingMsg(const wxString& intialStr, unsigned int timeInSec, bool bDestroyWindow, const wxString& finalStr, const wxSize& winSize)
	: wxDialog(NULL, -1, wxT("Processing..."), wxDefaultPosition, winSize, EGAV_DIALOG_BORDER_STYLE)
{
	FinalStr = finalStr;
	Connect(ID_WAIT_MSGBOX_BTN_OK, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MyWaitingMsg::OnBtnOk));
	Connect(ID_TIMER_WAIT_MSG, wxEVT_TIMER, wxTimerEventHandler(MyWaitingMsg::OnMsgTimer));
	msgtimer = new wxTimer(this, ID_TIMER_WAIT_MSG);

	Panel_InnerTxt = new wxPanel(this);
	Panel_Buttons = new wxPanel(this);

	BtnOk = new MyWXRoundEdgeButton(Panel_Buttons, ID_WAIT_MSGBOX_BTN_OK, wxT("OK"), wxDefaultPosition, EGAV_BUTTON_SIZE_MID, EGAV_BUTTON_STYLE_NOBORDER);

	InnerTxt = new wxStaticText(Panel_InnerTxt, -1, intialStr);
	InnerTxt->SetFont(wxFont(12, wxFONTFAMILY_DECORATIVE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
	InnerTxt->SetForegroundColour(EGAV_TEXT_COLOR_WHITE);

	SizerV_Upper = new wxBoxSizer(wxVERTICAL);
	SizerV_Upper->Add(InnerTxt, 1, wxALIGN_CENTER_VERTICAL | wxEXPAND | wxALL, 10);

	Panel_InnerTxt->SetSizer(SizerV_Upper);

	SizerH_Lower = new wxBoxSizer(wxHORIZONTAL);
	SizerH_Lower->Add(BtnOk, 1, wxALIGN_RIGHT, 10);
	Panel_Buttons->SetSizer(SizerH_Lower);

	SizerV_All = new wxBoxSizer(wxVERTICAL);
	SizerV_All->Add(Panel_InnerTxt, 3, wxEXPAND | wxALL, 10);
	SizerV_All->Add(Panel_Buttons, 1, wxALIGN_CENTER, 10);
	SetSizer(SizerV_All);

	SetBackgroundColour(EGAV_WINDOW_BACK_COLOR1);

	Center();
	WaitingtimeInMillsec = timeInSec * 1000;
	WaitingtimeInsec = timeInSec;
	m_bDestroyWindow = bDestroyWindow;
	if (m_bDestroyWindow)
	{
		BtnOk->Disable();
		BtnOk->Hide();
	}
	else
		BtnOk->Disable();
	StaringTime = clock();
	msgtimer->Start();


	ShowModal();
	//	 Destroy();

}

void MyWaitingMsg::OnBtnOk(wxCommandEvent& event)
{
	Close(true);
	Destroy();
}

void MyWaitingMsg::OnMsgTimer(wxTimerEvent& tmrEvent)
{
	clock_t now = clock();
	if ((now - StaringTime) > WaitingtimeInMillsec)
	{
		if (m_bDestroyWindow)
		{
			msgtimer->Stop();
			Close(true);
			Destroy();
		}
		else
		{
			this->SetTitle(EGAV_ADMIN_PANEL_TITLE);
			InnerTxt->SetLabelText(FinalStr);
			BtnOk->Enable();
			msgtimer->Stop();
		}
	}
	else
	{

		float k = (now - StaringTime) / 1000.0;
		int j = (int)(k);
		wxString kstr;
		kstr.Printf("%d", WaitingtimeInsec - j);
		this->SetTitle(kstr);
	}
}

void AVShowMsgWait(const wxString& intialStr, unsigned int timeInSec, bool bDestroyWindow, const wxString& finalStr, const wxSize& winSize)
{
	MyWaitingMsg* tempWMsg = new MyWaitingMsg(intialStr, timeInSec, bDestroyWindow, finalStr, winSize);
	tempWMsg->Show();
	DELETE_POINTER(MyWaitingMsg, tempWMsg);
}