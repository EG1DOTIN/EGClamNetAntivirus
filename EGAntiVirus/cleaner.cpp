#include "common.h"
#include "gxfunctions.h"
#include "cleaner.h"
#include <wx/wfstream.h>
#include <wx/filename.h>
#include "cleanupapi.h"
#include "avtheme.h"
#include "WaitForClean.h"
#include "avmsgebox.h"

/*
BEGIN_EVENT_TABLE(Cleaner, wxFrame)
EVT_CLOSE(Cleaner::OnClose)
END_EVENT_TABLE()
*/

bool gbDeleteTemporaryFilesFolders = false;
bool gbDeleteIECache = false;
bool gbDeleteIECookies = false;
bool gbDeleteIEHistory = false;
bool gbDeleteOtherBrowserCacheNHistory = false;
bool gbDeleteRecycleBinContent = false;
bool gbDeleteIEAddressBarHistory = false;
bool gbDeleteRecentDocsHistory = false;
bool gbDeleteRunHistory = false;
bool gbDeleteAll = false;

extern wxArrayString sizeOfAllFieldsInCleaner;

Cleaner::Cleaner(const wxString& title)
	: wxDialog(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(800, 600), EGAV_DIALOG_BORDER_STYLE)

{
	Connect(ID_CLEANER_BTN_CLEAN, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(Cleaner::OnBtnClean));
	Connect(ID_CLEANER_BTN_CANCEL, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(Cleaner::OnBtnCancel));

	for (size_t x = 0; x < 10; x++)
	{
		Connect(ID_CLEANER_CB_0 + x, wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(Cleaner::OnCheckBox));
	}

	//Load size values

	m_Panel_All = new wxPanel(this);
	m_Panel_Upper = new wxPanel(m_Panel_All);
	m_Panel_Lower = new wxPanel(m_Panel_All);
	m_Panel_Content = new wxPanel(m_Panel_Upper);
	m_Panel_ConSize = new wxPanel(m_Panel_Upper);
	m_Panel_CheckBox = new wxPanel(m_Panel_Upper);

	//panel upper
	m_SV_Content = new wxBoxSizer(wxVERTICAL);
	m_st_content[0] = new wxStaticText(m_Panel_Content, -1, wxT("Content"));
	m_st_content[0]->SetForegroundColour(EGAV_TEXT_COLOR_WHITE);
	m_st_content[0]->SetFont(EGAV_FONT_SMALL_ROMAN_NORMAL_BOLD);
	m_SV_Content->Add(m_st_content[0], 1, 0, 1);
	m_SV_Content->AddStretchSpacer();
	for (size_t i = 1; i < 10; i++)
	{
		m_st_content[i] = new wxStaticText(m_Panel_Content, -1, contentStr[i - 1]);
		m_st_content[i]->SetForegroundColour(EGAV_TEXT_COLOR_WHITE);
		m_st_content[i]->SetFont(EGAV_FONT_SMALL_ROMAN_NORMAL_NORMAL);
		m_SV_Content->Add(m_st_content[i], 1, 0, 1);
		m_SV_Content->AddStretchSpacer();
	}
	m_st_content[10] = new wxStaticText(m_Panel_Content, -1, wxT("Select All"));
	m_st_content[10]->SetForegroundColour(EGAV_TEXT_COLOR_WHITE);
	m_st_content[10]->SetFont(EGAV_FONT_SMALL_ROMAN_NORMAL_BOLD);
	m_SV_Content->Add(m_st_content[10], 1, 0, 1);
	m_Panel_Content->SetSizer(m_SV_Content);

	m_SV_ConSize = new wxBoxSizer(wxVERTICAL);
	m_st_contentsize[0] = new wxStaticText(m_Panel_ConSize, -1, wxT("Size "));
	m_st_contentsize[0]->SetForegroundColour(EGAV_TEXT_COLOR_WHITE);
	m_st_contentsize[0]->SetFont(EGAV_FONT_SMALL_ROMAN_NORMAL_BOLD);
	m_SV_ConSize->Add(m_st_contentsize[0], 1, 0, 1);
	m_SV_ConSize->AddStretchSpacer();
	for (size_t j = 1; j < 10; j++)
	{
		m_st_contentsize[j] = new wxStaticText(m_Panel_ConSize, -1, sizeOfAllFieldsInCleaner.Item(j - 1));
		m_st_contentsize[j]->SetForegroundColour(EGAV_TEXT_COLOR_WHITE);
		m_st_contentsize[j]->SetFont(EGAV_FONT_SMALL_ROMAN_NORMAL_NORMAL);
		m_SV_ConSize->Add(m_st_contentsize[j], 1, 0, 1);
		m_SV_ConSize->AddStretchSpacer();
	}
	m_st_contentsize[10] = new wxStaticText(m_Panel_ConSize, -1, sizeOfAllFieldsInCleaner.Item(9));
	m_st_contentsize[10]->SetForegroundColour(EGAV_TEXT_COLOR_WHITE);
	m_st_contentsize[10]->SetFont(EGAV_FONT_SMALL_ROMAN_NORMAL_BOLD);
	m_SV_ConSize->Add(m_st_contentsize[10], 1, 0, 1);
	m_Panel_ConSize->SetSizer(m_SV_ConSize);

	m_SV_CheckBox = new wxBoxSizer(wxVERTICAL);
	m_st_RemoveOrNot = new wxStaticText(m_Panel_CheckBox, -1, wxT("Remove"));
	m_st_RemoveOrNot->SetFont(EGAV_FONT_SMALL_ROMAN_NORMAL_BOLD);
	m_st_RemoveOrNot->SetForegroundColour(EGAV_TEXT_COLOR_WHITE);
	m_SV_CheckBox->Add(m_st_RemoveOrNot, 1, 0, 1);
	m_SV_CheckBox->AddStretchSpacer();
	for (size_t k = 0; k < 9; k++)
	{
		m_cb_RemoveOrNot[k] = new wxCheckBox(m_Panel_CheckBox, ID_CLEANER_CB_0 + k, wxT(""));
		m_cb_RemoveOrNot[k]->SetForegroundColour(EGAV_TEXT_COLOR_WHITE);
		m_SV_CheckBox->Add(m_cb_RemoveOrNot[k], 1, 0, 1);
		m_SV_CheckBox->AddStretchSpacer();
	}
	m_cb_RemoveOrNot[9] = new wxCheckBox(m_Panel_CheckBox, ID_CLEANER_CB_9, wxT(""));
	m_cb_RemoveOrNot[9]->SetForegroundColour(EGAV_TEXT_COLOR_WHITE);
	m_SV_CheckBox->Add(m_cb_RemoveOrNot[9], 1, 0, 1);
	m_Panel_CheckBox->SetSizer(m_SV_CheckBox);

	m_SH_Upper = new wxBoxSizer(wxHORIZONTAL);
	m_SH_Upper->Add(m_Panel_Content, 3, wxEXPAND | wxALL, 20);
	m_SH_Upper->Add(m_Panel_ConSize, 2, wxEXPAND | wxALL, 20);
	m_SH_Upper->Add(m_Panel_CheckBox, 1, wxEXPAND | wxALL, 20);
	m_Panel_Upper->SetSizer(m_SH_Upper);

	//panellower
	m_Btn_Clean = new MyWXRoundEdgeButton(m_Panel_Lower, ID_CLEANER_BTN_CLEAN, wxT("Clean"), wxDefaultPosition, EGAV_BUTTON_SIZE_MID, EGAV_BUTTON_STYLE_NOBORDER);
	m_Btn_Cancel = new MyWXRoundEdgeButton(m_Panel_Lower, ID_CLEANER_BTN_CANCEL, wxT("Cancel"), wxDefaultPosition, EGAV_BUTTON_SIZE_MID, EGAV_BUTTON_STYLE_NOBORDER);

	m_SH_Buttons = new wxBoxSizer(wxHORIZONTAL);
	m_SH_Buttons->AddStretchSpacer();
	m_SH_Buttons->Add(m_Btn_Clean);
	m_SH_Buttons->AddStretchSpacer();
	m_SH_Buttons->Add(m_Btn_Cancel);
	m_SH_Buttons->AddStretchSpacer();

	m_Panel_Lower->SetSizer(m_SH_Buttons);

	m_SV_Lower = new wxBoxSizer(wxVERTICAL);
	m_SV_Lower->AddStretchSpacer();
	m_SV_Lower->Add(m_Panel_Upper, 5, wxEXPAND | wxALL, 5);
	m_SV_Lower->AddStretchSpacer();
	m_SV_Lower->Add(m_Panel_Lower, 1, wxEXPAND | wxALL, 5);
	m_SV_Lower->AddStretchSpacer();
	m_Panel_All->SetSizer(m_SV_Lower);
	
	SetBackgroundColour(EGAV_WINDOW_BACK_COLOR1);

	Centre();
	sizeOfAllFieldsInCleaner.Clear();

	gbDeleteTemporaryFilesFolders = false;
	gbDeleteIECache = false;
	gbDeleteIECookies = false;
	gbDeleteIEHistory = false;
	gbDeleteOtherBrowserCacheNHistory = false;
	gbDeleteRecycleBinContent = false;
	gbDeleteIEAddressBarHistory = false;
	gbDeleteRecentDocsHistory = false;
	gbDeleteRunHistory = false;
	gbDeleteAll = false;

	ShowModal();
}

void Cleaner::OnBtnClean(wxCommandEvent& event)
{
	WaitForCleaning* waitfclean = new WaitForCleaning(EGAV_ADMIN_PANEL_TITLE wxT(": Cleaning"), wxSize(400, 200));
	waitfclean->Show();
	DELETE_POINTER(WaitForCleaning, waitfclean);
	event.Skip();
	Destroy();
}

void Cleaner::OnBtnCancel(wxCommandEvent& event)
{
	event.Skip();
	Destroy();
}

void Cleaner::OnCheckBox(wxCommandEvent& event)
{
	int id = event.GetId();
	switch (id)
	{
	case ID_CLEANER_CB_0:
		(m_cb_RemoveOrNot[0]->GetValue()) ? (gbDeleteTemporaryFilesFolders = true) : (gbDeleteTemporaryFilesFolders = false, m_cb_RemoveOrNot[9]->SetValue(false), gbDeleteAll = false);
		break;
	case ID_CLEANER_CB_1:
		(m_cb_RemoveOrNot[1]->GetValue()) ? (gbDeleteIECache = true) : (gbDeleteIECache = false, m_cb_RemoveOrNot[9]->SetValue(false), gbDeleteAll = false);
		break;
	case ID_CLEANER_CB_2:
		(m_cb_RemoveOrNot[2]->GetValue()) ? (gbDeleteIECookies = true) : (gbDeleteIECookies = false, m_cb_RemoveOrNot[9]->SetValue(false), gbDeleteAll = false);
		break;
	case ID_CLEANER_CB_3:
		(m_cb_RemoveOrNot[3]->GetValue()) ? (gbDeleteIEHistory = true) : (gbDeleteIEHistory = false, m_cb_RemoveOrNot[9]->SetValue(false), gbDeleteAll = false);
		break;
	case ID_CLEANER_CB_4:
		(m_cb_RemoveOrNot[4]->GetValue()) ? (gbDeleteOtherBrowserCacheNHistory = true) : (gbDeleteOtherBrowserCacheNHistory = false, m_cb_RemoveOrNot[9]->SetValue(false), gbDeleteAll = false);
		break;
	case ID_CLEANER_CB_5:
		(m_cb_RemoveOrNot[5]->GetValue()) ? (gbDeleteRecycleBinContent = true) : (gbDeleteRecycleBinContent = false, m_cb_RemoveOrNot[9]->SetValue(false), gbDeleteAll = false);
		break;
	case ID_CLEANER_CB_6:
		(m_cb_RemoveOrNot[6]->GetValue()) ? (gbDeleteIEAddressBarHistory = true) : (gbDeleteIEAddressBarHistory = false, m_cb_RemoveOrNot[9]->SetValue(false), gbDeleteAll = false);
		break;
	case ID_CLEANER_CB_7:
		(m_cb_RemoveOrNot[7]->GetValue()) ? (gbDeleteRecentDocsHistory = true) : (gbDeleteRecentDocsHistory = false, m_cb_RemoveOrNot[9]->SetValue(false), gbDeleteAll = false);
		break;
	case ID_CLEANER_CB_8:
		(m_cb_RemoveOrNot[8]->GetValue()) ? (gbDeleteRunHistory = true) : (gbDeleteRunHistory = false, m_cb_RemoveOrNot[9]->SetValue(false), gbDeleteAll = false);
		break;
	case ID_CLEANER_CB_9:
	{
		if (m_cb_RemoveOrNot[9]->GetValue())
		{
			for (size_t i = 0; i < 9; i++)
			{
				m_cb_RemoveOrNot[i]->SetValue(true);
			}
			gbDeleteTemporaryFilesFolders = true;
			gbDeleteIECache = true;
			gbDeleteIECookies = true;
			gbDeleteIEHistory = true;
			gbDeleteOtherBrowserCacheNHistory = true;
			gbDeleteRecycleBinContent = true;
			gbDeleteIEAddressBarHistory = true;
			gbDeleteRecentDocsHistory = true;
			gbDeleteRunHistory = true;
			gbDeleteAll = true;
		}
		else
		{
			for (size_t i = 0; i < 9; i++)
			{
				m_cb_RemoveOrNot[i]->SetValue(false);
			}
			gbDeleteTemporaryFilesFolders = false;
			gbDeleteIECache = false;
			gbDeleteIECookies = false;
			gbDeleteIEHistory = false;
			gbDeleteOtherBrowserCacheNHistory = false;
			gbDeleteRecycleBinContent = false;
			gbDeleteIEAddressBarHistory = false;
			gbDeleteRecentDocsHistory = false;
			gbDeleteRunHistory = false;
			gbDeleteAll = false;
		}
	}

	default:
		break;
	}
}
