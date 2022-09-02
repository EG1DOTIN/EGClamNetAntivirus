#include "AboutPanel.h"
#include "avtheme.h"
#include <wx/cshelp.h>

extern wxString workingDir;

AboutPanel::AboutPanel(const wxString& title, wxWindow* parent) :wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(550, 300))
{
	this->SetBackgroundColour(EGAV_PANEL_BACK_COLOR);
	Connect(ID_BITMAP_BUTTON_EGAV, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(AboutPanel::OnBitmapButtonEGAV));
	Connect(ID_BITMAP_BUTTON_CLAMAV, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(AboutPanel::OnBitmapButtonClamAV));
	Connect(ID_BITMAP_BUTTON_WXWIDJETS, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(AboutPanel::OnBitmapButtonWXwidjets));

	wxLogNull logNo;
	m_pHeadingText = new wxPanel(this);
	m_pTextCtrl = new wxPanel(this);
	m_pUpperBitmap = new wxPanel(this);
	m_pMid1Bitmap = new wxPanel(this);
	m_pMid2Bitmap = new wxPanel(this);

	m_st_heading = new wxStaticText(m_pHeadingText, -1, wxT("EG ClamNet AntiVirus"));
	m_st_heading->SetForegroundColour(EGAV_FONT_BACK_LIGHT_GOLD_COLOR);
	m_st_heading->SetFont(wxFont(18, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
	m_sHHeadingText = new wxBoxSizer(wxHORIZONTAL);
	m_sHHeadingText->Add(m_st_heading);
	m_pHeadingText->SetSizer(m_sHHeadingText);

	{
		wxString info[10];
		long infolenght[10];
		info[0] = wxT("Copyright:");
		info[1] = wxT(" © 2022 EG1, All Rights Reserved\n");
		info[2] = wxT("Website:");
		info[3] = wxT(" http://av.eg1.in/ \n");
		info[4] = wxT("Email:");
		info[5] = wxT(" < mailto:eg1dotin@gmail.com > \n");

		size_t infocount = _ARRAYSIZE(info);

		wxString CopyrightString;
		for (size_t i = 0; i < infocount; i++)
		{
			CopyrightString.append(info[i]);
			infolenght[i] = CopyrightString.length();
		}

		m_tc_Copyright = new gxTextCtrlURL(m_pTextCtrl, -1,
			CopyrightString,
			wxDefaultPosition,
			wxSize(400, 140),
			0 | wxTE_AUTO_URL | wxTE_MULTILINE | wxTE_NO_VSCROLL | wxNO_BORDER | wxTE_READONLY);
		m_tc_Copyright->SetEditable(false);
		m_tc_Copyright->SetBackgroundColour(EGAV_PANEL_BACK_COLOR);
		m_tc_Copyright->SetStyle(0, infolenght[0], *wxGREEN);
		m_tc_Copyright->SetStyle(infolenght[0], infolenght[1], *wxWHITE);
		m_tc_Copyright->SetStyle(infolenght[1], infolenght[2], *wxGREEN);
		m_tc_Copyright->SetStyle(infolenght[2], infolenght[3], *wxCYAN);
		m_tc_Copyright->SetStyle(infolenght[3], infolenght[4], *wxGREEN);
		m_tc_Copyright->SetStyle(infolenght[4], infolenght[5], *wxWHITE);
		m_tc_Copyright->SetStyle(infolenght[5], infolenght[6], *wxGREEN);
		m_tc_Copyright->SetStyle(infolenght[6], infolenght[7], *wxWHITE);
		m_tc_Copyright->SetStyle(infolenght[7], infolenght[8], *wxGREEN);
		m_tc_Copyright->SetStyle(infolenght[8], infolenght[9], *wxWHITE);
	}
	
	m_sHTextCtrl = new wxBoxSizer(wxHORIZONTAL);
	m_sHTextCtrl->Add(m_tc_Copyright);
	m_pTextCtrl->SetSizer(m_sHTextCtrl);

	wxImage::AddHandler(new wxPNGHandler);
	wxBitmap egavIcon(workingDir + wxT("\\Resources\\egav.png"), wxBITMAP_TYPE_PNG);
	wxBitmap clamIcon(workingDir + wxT("\\Resources\\ClamAV.png"), wxBITMAP_TYPE_PNG);
	wxBitmap wxwidjetIcon(workingDir + wxT("\\Resources\\wxwidjets.png"), wxBITMAP_TYPE_PNG);

	m_bb_eg1 = new wxBitmapButton(m_pUpperBitmap, ID_BITMAP_BUTTON_EGAV, egavIcon, wxDefaultPosition, wxDefaultSize,EGAV_BUTTON_STYLE_NOBORDER);
	m_bb_eg1->SetCursor(wxCursor(wxCURSOR_HAND));
	m_bb_eg1->SetBackgroundColour(EGAV_PANEL_BACK_COLOR);

	m_sH_UpperBitmap = new wxBoxSizer(wxHORIZONTAL);
	m_sH_UpperBitmap->Add(m_bb_eg1, 1, wxALIGN_CENTER, 10);
	m_pUpperBitmap->SetSizer(m_sH_UpperBitmap);

	wxBoxSizer* sV_upperTextPart = new wxBoxSizer(wxVERTICAL);
	sV_upperTextPart->AddSpacer(20);
	sV_upperTextPart->Add(m_pHeadingText, 1, 0, 10);
	sV_upperTextPart->Add(m_pTextCtrl, 4, wxALIGN_CENTER, 10);

	m_sH_Upper = new wxBoxSizer(wxHORIZONTAL);
	m_sH_Upper->AddStretchSpacer();
	m_sH_Upper->Add(sV_upperTextPart, 2, wxALIGN_CENTER, 10);
	//m_sH_Upper->AddStretchSpacer();
	m_sH_Upper->Add(m_pUpperBitmap, 1, wxALIGN_CENTER, 10);
	m_sH_Upper->AddStretchSpacer();


	m_st_SE_Text = new wxStaticText(m_pMid1Bitmap, -1, wxT("Powered by:"));
	m_st_SE_Text->SetForegroundColour(EGAV_FONT_DARKGRAY_COLOR);
	m_st_SE_Text->SetFont(wxFont(11, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
	m_bb_clam_av = new wxBitmapButton(m_pMid1Bitmap, ID_BITMAP_BUTTON_CLAMAV, clamIcon, wxDefaultPosition, wxDefaultSize, EGAV_BUTTON_STYLE_NOBORDER);
	m_bb_clam_av->SetCursor(wxCursor(wxCURSOR_HAND));
	m_bb_clam_av->SetBackgroundColour(EGAV_PANEL_BACK_COLOR);

	m_sH_Mid1Bitmap = new wxBoxSizer(wxHORIZONTAL);
	m_sH_Mid1Bitmap->AddStretchSpacer();
	m_sH_Mid1Bitmap->AddSpacer(50);
	m_sH_Mid1Bitmap->Add(m_st_SE_Text, 1, wxALIGN_CENTER, 10);
	m_sH_Mid1Bitmap->AddStretchSpacer();
	m_sH_Mid1Bitmap->Add(m_bb_clam_av, 1, wxALIGN_CENTER, 10);
	m_sH_Mid1Bitmap->AddStretchSpacer();
	m_pMid1Bitmap->SetSizer(m_sH_Mid1Bitmap);


	m_st_LU_Text = new wxStaticText(m_pMid2Bitmap, -1, wxT("GUI by:"));
	m_st_LU_Text->SetForegroundColour(EGAV_FONT_DARKGRAY_COLOR);
	m_st_LU_Text->SetFont(wxFont(11, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));

	m_bb_wxwidjets = new wxBitmapButton(m_pMid2Bitmap, ID_BITMAP_BUTTON_WXWIDJETS, wxwidjetIcon, wxDefaultPosition, wxDefaultSize, EGAV_BUTTON_STYLE_NOBORDER);
	m_bb_wxwidjets->SetCursor(wxCursor(wxCURSOR_HAND));
	m_bb_wxwidjets->SetBackgroundColour(EGAV_PANEL_BACK_COLOR);

	m_sH_Mid2Bitmap = new wxBoxSizer(wxHORIZONTAL);
	m_sH_Mid2Bitmap->AddStretchSpacer();
	m_sH_Mid2Bitmap->AddSpacer(130);
	m_sH_Mid2Bitmap->Add(m_st_LU_Text, 1, wxALIGN_CENTER, 10);
	m_sH_Mid2Bitmap->AddStretchSpacer();
	m_sH_Mid2Bitmap->Add(m_bb_wxwidjets, 1, wxALIGN_CENTER, 10);
	m_sH_Mid2Bitmap->AddStretchSpacer();
	m_pMid2Bitmap->SetSizer(m_sH_Mid2Bitmap);

	m_sV_All = new wxBoxSizer(wxVERTICAL);
	m_sV_All->Add(m_sH_Upper, 3, wxALIGN_CENTER, 10);
	m_sV_All->Add(m_pMid1Bitmap, 1, wxALIGN_CENTER, 10);
	m_sV_All->Add(m_pMid2Bitmap, 1, wxALIGN_CENTER, 10);

	this->SetSizer(m_sV_All);
}

void AboutPanel::OnBitmapButtonEGAV(wxCommandEvent& event)
{
	wxLaunchDefaultBrowser(wxT("http://av.eg1.in/"), wxBROWSER_NOBUSYCURSOR);
}

void AboutPanel::OnBitmapButtonClamAV(wxCommandEvent& event)
{
	wxLaunchDefaultBrowser(wxT("http://www.clamav.net"), wxBROWSER_NOBUSYCURSOR);
}

void AboutPanel::OnBitmapButtonWXwidjets(wxCommandEvent& event)
{
	wxLaunchDefaultBrowser(wxT("http://www.wxwidgets.org"), wxBROWSER_NOBUSYCURSOR);
}