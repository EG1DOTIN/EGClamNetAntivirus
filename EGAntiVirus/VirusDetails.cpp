#include "VirusDetails.h"
#include "avtheme.h"


ViewVirusDetail::ViewVirusDetail(const wxString & title, const wxString & filefullpath, const wxString& heading1, const wxString& heading2)
	: wxDialog(NULL, -1, title, wxDefaultPosition, wxSize(480, 320), EGAV_DIALOG_BORDER_STYLE)
{
	Connect(ID_VIRUS_DETAILS_BTN_EXIT, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ViewVirusDetail::OnBtnExit));

	m_panelall = new wxPanel(this);

	m_heading1 = new wxStaticText(m_panelall, -1, heading1);

	m_heading2 = new wxStaticText(m_panelall, -1, heading2);

	wxFont fontheading1(18, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
	wxFont fontheading2(12, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);

	m_heading1->SetFont(fontheading1);
	m_heading2->SetFont(fontheading2);

	m_heading1->SetForegroundColour(EGAV_TEXT_COLOR_WHITE);
	m_heading2->SetForegroundColour(EGAV_TEXT_COLOR_WHITE);

	m_textctrl = new gxTextCtrlURL(m_panelall, -1, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_READONLY | wxTE_AUTO_URL);
	m_textctrl->SetBackgroundColour(wxColor(255, 255, 180));
	m_btn_exit = new MyWXRoundEdgeButton(m_panelall, ID_VIRUS_DETAILS_BTN_EXIT, wxT("Exit"), wxDefaultPosition, EGAV_BUTTON_SIZE_MID, EGAV_BUTTON_STYLE_NOBORDER);

	m_boxsizerVAll = new wxBoxSizer(wxVERTICAL);
	m_boxsizerVAll->Add(m_heading1, 2, wxALIGN_CENTER, 2);
	m_boxsizerVAll->Add(m_heading2, 1, wxALIGN_CENTER, 2);
	m_boxsizerVAll->Add(m_textctrl, 5, wxALIGN_CENTER | wxEXPAND | wxALL, 10);
	m_boxsizerVAll->Add(m_btn_exit, 0, wxALIGN_CENTER, 5);
	m_boxsizerVAll->AddSpacer(10);
	m_panelall->SetSizer(m_boxsizerVAll);

	m_textctrl->LoadFile(filefullpath);

	m_textctrl->SetStyle(0, 2147483647, wxTextAttr(wxColor(255, 0, 0)));

	SetBackgroundColour(EGAV_WINDOW_BACK_COLOR1);

	Center();

	ShowModal();
}

void ViewVirusDetail::OnBtnExit(wxCommandEvent& event)
{
	Close();
	Destroy();
}

void AVShowVirusDetail(const wxString & title, const wxString & filefullpath, const wxString& heading1, const wxString& heading2)
{
	ViewVirusDetail* vvd = new ViewVirusDetail(title, filefullpath, heading1, heading2);
	vvd->Show();
	delete vvd;
	vvd = (ViewVirusDetail*)(NULL);
}