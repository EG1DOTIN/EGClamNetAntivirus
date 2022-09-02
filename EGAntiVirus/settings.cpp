#include "common.h"
#include "gxDelPointer.h"
#include "gxfunctions.h"
#include "egavsetgetdb.h"
#include "avdb.h"
#include "settings.h"
#include "avtheme.h"
#include <wx\spinctrl.h>
#include <wx\textfile.h>

bool gbApplySettings = false;
bool gbIsDefaultBtnClicked = false;

extern wxString workingDir;
extern wxString dbFile1;  //For antivirus settings
extern wxString dbFile2;  //For antivirus settings exclude list 
extern wxString dbFile3;  //For antivirus settings include list

settingsEGAV::settingsEGAV(const wxString& title) :wxDialog(NULL, -1, title, wxDefaultPosition, wxSize(848, 580), EGAV_DIALOG_BORDER_STYLE)
{
	Connect(ID_SETTINGS_BTN1, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(settingsEGAV::OnButtonOk));
	Connect(ID_SETTINGS_BTN2, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(settingsEGAV::OnButtonApply));
	Connect(ID_SETTINGS_BTN3, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(settingsEGAV::OnButtonCancel));
	Connect(ID_SETTINGS_BTN4, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(settingsEGAV::OnButtonDefault));

	Connect(ID_SETTINGS_CB1, wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(settingsEGAV::OnCheckBox1));
	Connect(ID_SETTINGS_CB2, wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(settingsEGAV::OnCheckBox2));
	Connect(ID_SETTINGS_CB3, wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(settingsEGAV::OnCheckBox3));
	Connect(ID_SETTINGS_CB4, wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(settingsEGAV::OnCheckBox4));
	Connect(ID_SETTINGS_CB5, wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(settingsEGAV::OnCheckBox5));

	Connect(ID_SETTINGS_RB1, wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler(settingsEGAV::OnRadioButton1));
	Connect(ID_SETTINGS_RB2, wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler(settingsEGAV::OnRadioButton2));
	Connect(ID_SETTINGS_RB3, wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler(settingsEGAV::OnRadioButton3));

	Connect(ID_SETTINGS_SC1, wxEVT_COMMAND_SPINCTRL_UPDATED, wxCommandEventHandler(settingsEGAV::OnSpinCtrl1));
	Connect(ID_SETTINGS_SC2, wxEVT_COMMAND_SPINCTRL_UPDATED, wxCommandEventHandler(settingsEGAV::OnSpinCtrl2));
	Connect(ID_SETTINGS_SC3, wxEVT_COMMAND_SPINCTRL_UPDATED, wxCommandEventHandler(settingsEGAV::OnSpinCtrl3));
	Connect(ID_SETTINGS_SC4, wxEVT_COMMAND_SPINCTRL_UPDATED, wxCommandEventHandler(settingsEGAV::OnSpinCtrl4));
	//Connect(ID_SETTINGS_SC5, wxEVT_COMMAND_SPINCTRL_UPDATED, wxCommandEventHandler(settingsEGAV::OnSpinCtrl5));

	Connect(ID_SETTINGS_BTN_ADD1, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(settingsEGAV::OnButtonAdd1));
	Connect(ID_SETTINGS_BTN_ADD2, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(settingsEGAV::OnButtonAdd2));
	Connect(ID_SETTINGS_BTN_EDIT1, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(settingsEGAV::OnButtonEdit1));
	Connect(ID_SETTINGS_BTN_EDIT2, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(settingsEGAV::OnButtonEdit2));
	Connect(ID_SETTINGS_BTN_DELETE1, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(settingsEGAV::OnButtonDelete1));
	Connect(ID_SETTINGS_BTN_DELETE2, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(settingsEGAV::OnButtonDelete2));
	Connect(ID_SETTINGS_BTN_CLEAR1, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(settingsEGAV::OnButtonClear1));
	Connect(ID_SETTINGS_BTN_CLEAR2, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(settingsEGAV::OnButtonClear2));
	Connect(ID_SETTINGS_LB1, wxEVT_COMMAND_LISTBOX_DOUBLECLICKED, wxCommandEventHandler(settingsEGAV::OnDblClickList1));
	Connect(ID_SETTINGS_LB2, wxEVT_COMMAND_LISTBOX_DOUBLECLICKED, wxCommandEventHandler(settingsEGAV::OnDblClickList2));

	panel_ALL = new wxPanel(this, -1, wxPoint(5, 5), wxSize(795, 525));
	panel_1 = new wxPanel(panel_ALL, -1, wxPoint(8, 14), wxSize(386, 148));
	panel_2 = new wxPanel(panel_ALL, -1, wxPoint(8, 170), wxSize(386, 90));
	panel_3 = new wxPanel(panel_ALL, -1, wxPoint(8, 265), wxSize(386, 150));
	panel_4 = new wxPanel(panel_ALL, -1, wxPoint(398, 14), wxSize(420, 74));
	panel_5 = new wxPanel(panel_ALL, -1, wxPoint(398, 87), wxSize(420, 74));
	panel_6 = new wxPanel(panel_ALL, -1, wxPoint(398, 170), wxSize(420, 245));
	panel_7 = new wxPanel(panel_ALL, -1, wxPoint(8, 425), wxSize(772, 50));

	sb_InfectedFiles = new wxStaticBox(panel_1, -1, wxT("Infected Files"), wxPoint(6,6), wxSize(370, 134));
	sb_AllFiles = new wxStaticBox(panel_2, -1, wxT("All Files"), wxPoint(6, 7), wxSize(370, 80));
	sb_Archives = new wxStaticBox(panel_3, -1, wxT("Archive Files"), wxPoint(6, 8), wxSize(370, 137));
	sb_Shield = new wxStaticBox(panel_4, -1, wxT("Real-Time Protection"), wxPoint(6,6), wxSize(410, 60));
	sb_Update = new wxStaticBox(panel_5, -1, wxT("Updates"), wxPoint(6, 6), wxSize(410, 60));
	sb_Filters = new wxStaticBox(panel_6, -1, wxT("Filters"), wxPoint(6, 6), wxSize(410, 233));

	
	const wxColor ColorForStaticBox = *wxWHITE;

	sb_InfectedFiles->SetForegroundColour(ColorForStaticBox);
	sb_AllFiles->SetForegroundColour(ColorForStaticBox);
	sb_Archives->SetForegroundColour(ColorForStaticBox);
	sb_Shield->SetForegroundColour(ColorForStaticBox);
	sb_Update->SetForegroundColour(ColorForStaticBox);
	sb_Filters->SetForegroundColour(ColorForStaticBox);


	rb_Report = new wxRadioButton(panel_1, ID_SETTINGS_RB1, wxT(""), wxPoint(20, 35), wxDefaultSize, wxRB_GROUP);
	st_11 = new wxStaticText(panel_1, -1, wxT("Report Only"), wxPoint(40, 35), wxDefaultSize);
	rb_Remove = new wxRadioButton(panel_1, ID_SETTINGS_RB2, wxT(""), wxPoint(20, 60));
	st_12 = new wxStaticText(panel_1, -1, wxT("Remove (Use Carefully)"), wxPoint(40, 60), wxDefaultSize);
	rb_moveQuad = new wxRadioButton(panel_1, ID_SETTINGS_RB3, wxT(""), wxPoint(20, 85));
	st_13 = new wxStaticText(panel_1, -1, wxT("Move To Quarantine Folder"), wxPoint(40, 85), wxDefaultSize);

	cb_1 = new wxCheckBox(panel_1, ID_SETTINGS_CB1, wxT("Unload Infected Program from Computer Memory"), wxPoint(20, 115));

	st_1 = new wxStaticText(panel_2, -1, wxT("Do Not Scan Files Larger Than"), wxPoint(20, 30), wxDefaultSize);
	sc_1 = new wxSpinCtrl(panel_2, ID_SETTINGS_SC1, wxT("100"), wxPoint(225, 30), wxSize(60, 20), 16896L, 1, 5120);
	st_2 = new wxStaticText(panel_2, -1, wxT("MB"), wxPoint(295, 30), wxDefaultSize);

	cb_5 = new wxCheckBox(panel_2, ID_SETTINGS_CB5, wxT("Do Not Reload Virus Signature DB ( Fast Scan )"), wxPoint(20, 57), wxDefaultSize);


	cb_2 = new wxCheckBox(panel_3, ID_SETTINGS_CB2, wxT("Extract Files From Archives"), wxPoint(20, 35));

	st_3 = new wxStaticText(panel_3, -1, wxT("Do Not Extract More Than"), wxPoint(20, 65), wxDefaultSize);
	sc_2 = new wxSpinCtrl(panel_3, ID_SETTINGS_SC2, wxT("500"), wxPoint(200, 65), wxSize(60, 20), 16896L, 1, 5120);
	st_4 = new wxStaticText(panel_3, -1, wxT("MB"), wxPoint(270, 65), wxDefaultSize);

	st_5 = new wxStaticText(panel_3, -1, wxT("Do Not Extract More Than"), wxPoint(20, 90), wxDefaultSize);
	sc_3 = new wxSpinCtrl(panel_3, ID_SETTINGS_SC3, wxT("50"), wxPoint(200, 90), wxSize(60, 20), 16896L, 1, 500);
	st_6 = new wxStaticText(panel_3, -1, wxT("Files"), wxPoint(270, 90), wxDefaultSize);
	
	st_7 = new wxStaticText(panel_3, -1, wxT("Do Not Extract More Than"), wxPoint(20, 115), wxDefaultSize);
	sc_4 = new wxSpinCtrl(panel_3, ID_SETTINGS_SC4, wxT("50"), wxPoint(200, 115), wxSize(60, 20), 16896L, 1, 100);
	st_8 = new wxStaticText(panel_3, -1, wxT("Sub Archives"), wxPoint(270, 115), wxDefaultSize);

	//st_9 = new wxStaticText(panel_4, -1, wxT("Limit Log File Size To"), wxPoint(20, 30), wxDefaultSize);
	//sc_5 = new wxSpinCtrl(panel_4, ID_SETTINGS_SC5, wxT("1"), wxPoint(200, 30), wxSize(60, 20), 16896L, 1, 5);
	//st_10 = new wxStaticText(panel_4, -1, wxT("MB"), wxPoint(270, 30), wxDefaultSize);
	cb_4 = new wxCheckBox(panel_4, ID_SETTINGS_CB4, wxT("Real-Time Protection On/Off"), wxPoint(20, 30));

	cb_3 = new wxCheckBox(panel_5, ID_SETTINGS_CB3, wxT("Automatic Updates On/Off"), wxPoint(20, 30));

	st_14 = new wxStaticText(panel_6, -1, wxT("Add file extension to include and/or exclude in scanning"), wxPoint(15, 25), wxSize(400,20));
	st_15 = new wxStaticText(panel_6, -1, wxT("Exclude matching extension"), wxPoint(15, 55), wxSize(200, 20));
	st_16 = new wxStaticText(panel_6, -1, wxT("Scan only matching extension"), wxPoint(205, 55), wxSize(200, 20));

	panel_A = new wxPanel(panel_6, -1, wxPoint(15, 60), wxSize(165, 155));
	panel_B = new wxPanel(panel_6, -1, wxPoint(200,60), wxSize(165, 155));

	list1 = new wxListBox(panel_A, ID_SETTINGS_LB1, wxPoint(5, 17), wxSize(100, 145));
	list2 = new wxListBox(panel_B, ID_SETTINGS_LB2, wxPoint(5, 17), wxSize(100, 145));


	btn_Add1 = new MyWXRoundEdgeButton(panel_A, ID_SETTINGS_BTN_ADD1, wxT("Add"), wxPoint(110, 17), EGAV_BUTTON_SIZE_SMALL, EGAV_BUTTON_STYLE_NOBORDER, wxDefaultValidator, wxButtonNameStr, true);
	btn_Edit1 = new MyWXRoundEdgeButton(panel_A, ID_SETTINGS_BTN_EDIT1, wxT("Edit"), wxPoint(110, 54), EGAV_BUTTON_SIZE_SMALL, EGAV_BUTTON_STYLE_NOBORDER, wxDefaultValidator, wxButtonNameStr, true);
	btn_Delete1 = new MyWXRoundEdgeButton(panel_A, ID_SETTINGS_BTN_DELETE1, wxT("Delete"), wxPoint(110, 91), EGAV_BUTTON_SIZE_SMALL, EGAV_BUTTON_STYLE_NOBORDER, wxDefaultValidator, wxButtonNameStr, true);
	btn_Clear1 = new MyWXRoundEdgeButton(panel_A, ID_SETTINGS_BTN_CLEAR1, wxT("Clear"), wxPoint(110, 128), EGAV_BUTTON_SIZE_SMALL, EGAV_BUTTON_STYLE_NOBORDER, wxDefaultValidator, wxButtonNameStr, true);

	btn_Add2 = new MyWXRoundEdgeButton(panel_B, ID_SETTINGS_BTN_ADD2, wxT("Add"), wxPoint(110, 17), EGAV_BUTTON_SIZE_SMALL, EGAV_BUTTON_STYLE_NOBORDER, wxDefaultValidator, wxButtonNameStr, true);
	btn_Edit2 = new MyWXRoundEdgeButton(panel_B, ID_SETTINGS_BTN_EDIT2, wxT("Edit"), wxPoint(110, 54), EGAV_BUTTON_SIZE_SMALL, EGAV_BUTTON_STYLE_NOBORDER, wxDefaultValidator, wxButtonNameStr, true);
	btn_Delete2 = new MyWXRoundEdgeButton(panel_B, ID_SETTINGS_BTN_DELETE2, wxT("Delete"), wxPoint(110, 91), EGAV_BUTTON_SIZE_SMALL, EGAV_BUTTON_STYLE_NOBORDER, wxDefaultValidator, wxButtonNameStr, true);
	btn_Clear2 = new MyWXRoundEdgeButton(panel_B, ID_SETTINGS_BTN_CLEAR2, wxT("Clear"), wxPoint(110, 128), EGAV_BUTTON_SIZE_SMALL, EGAV_BUTTON_STYLE_NOBORDER, wxDefaultValidator, wxButtonNameStr, true);
	
	st_1->SetForegroundColour(EGAV_TEXT_COLOR_WHITE);
	st_2->SetForegroundColour(EGAV_TEXT_COLOR_WHITE);
	st_3->SetForegroundColour(EGAV_TEXT_COLOR_WHITE);
	st_4->SetForegroundColour(EGAV_TEXT_COLOR_WHITE);
	st_5->SetForegroundColour(EGAV_TEXT_COLOR_WHITE);
	st_6->SetForegroundColour(EGAV_TEXT_COLOR_WHITE);
	st_7->SetForegroundColour(EGAV_TEXT_COLOR_WHITE);
	st_8->SetForegroundColour(EGAV_TEXT_COLOR_WHITE);
//	st_9->SetForegroundColour(EGAV_TEXT_COLOR_WHITE);
//	st_10->SetForegroundColour(EGAV_TEXT_COLOR_WHITE);
	st_11->SetForegroundColour(EGAV_TEXT_COLOR_WHITE);
	st_12->SetForegroundColour(EGAV_TEXT_COLOR_WHITE);
	st_13->SetForegroundColour(EGAV_TEXT_COLOR_WHITE);
	st_14->SetForegroundColour(EGAV_TEXT_COLOR_WHITE);
	st_15->SetForegroundColour(EGAV_TEXT_COLOR_WHITE);
	st_16->SetForegroundColour(EGAV_TEXT_COLOR_WHITE);

	cb_1->SetForegroundColour(EGAV_TEXT_COLOR_WHITE);
	cb_2->SetForegroundColour(EGAV_TEXT_COLOR_WHITE);
	cb_3->SetForegroundColour(EGAV_TEXT_COLOR_WHITE);
	cb_4->SetForegroundColour(EGAV_TEXT_COLOR_WHITE);
	cb_5->SetForegroundColour(EGAV_TEXT_COLOR_WHITE);


	bs_V1 = new wxBoxSizer(wxVERTICAL);
	bs_V2 = new wxBoxSizer(wxVERTICAL);
	bs_V3 = new wxBoxSizer(wxVERTICAL);

	bs_V1->Add(panel_1, 3, wxEXPAND | wxALL, 0);
	bs_V1->Add(panel_2, 1, wxEXPAND | wxALL, 0);
	bs_V1->Add(panel_3, 4, wxEXPAND | wxALL, 0);

	bs_V2->Add(panel_4, 1, wxEXPAND | wxALL, 0);
	bs_V2->Add(panel_5, 1, wxEXPAND | wxALL, 0);
	bs_V2->Add(panel_6, 6, wxEXPAND | wxALL, 0);

	btn_ok = new MyWXRoundEdgeButton(panel_7, ID_SETTINGS_BTN1, wxT("OK"), wxPoint(48, 12), EGAV_BUTTON_SIZE_MID, EGAV_BUTTON_STYLE_NOBORDER);
	btn_apply = new MyWXRoundEdgeButton(panel_7, ID_SETTINGS_BTN2, wxT("Apply"), wxPoint(228, 12), EGAV_BUTTON_SIZE_MID, EGAV_BUTTON_STYLE_NOBORDER);
	btn_default = new MyWXRoundEdgeButton(panel_7, ID_SETTINGS_BTN4, wxT("Default"), wxPoint(408, 12), EGAV_BUTTON_SIZE_MID, EGAV_BUTTON_STYLE_NOBORDER);
	btn_cancel = new MyWXRoundEdgeButton(panel_7, ID_SETTINGS_BTN3, wxT("Cancel"), wxPoint(588, 12), EGAV_BUTTON_SIZE_MID, EGAV_BUTTON_STYLE_NOBORDER);

	bs_H1 = new wxBoxSizer(wxHORIZONTAL);
	bs_H2 = new wxBoxSizer(wxHORIZONTAL);

	bs_H1->Add(bs_V1, 1, wxEXPAND |wxALL , 0); 
	bs_H1->Add(bs_V2, 1, wxEXPAND | wxALL, 0);

	bs_H2->Add(btn_ok);
	bs_H2->Add(btn_apply);
	bs_H2->Add(btn_default);
	bs_H2->Add(btn_cancel);
	panel_7->SetSizer(bs_H2);

	bs_V3->Add(bs_H1, 8, wxEXPAND | wxALL, 0);
	bs_V3->Add(panel_7, 1, wxEXPAND | wxALL, 0);

	SetBackgroundColour(EGAV_WINDOW_BACK_COLOR1);

	//Loading database and initialising the data
	LoadDBAndInitData(dbFile1, dbFile2, dbFile3);

	btn_apply->Disable();

	Centre();

	ShowModal();

	Destroy();
}

void settingsEGAV::OnChangingSettings(void)
{
	wxTextFile* tempfile = new wxTextFile(dbFile1);
	tempfile->Open();
	tempfile->Clear();
	if (m_rb1)
	{
		tempfile->AddLine(AVDB_RB1);
	}
	
	if (m_rb2)
	{
		tempfile->AddLine(AVDB_RB2);
	}

	if (m_rb3)
	{
		tempfile->AddLine(AVDB_RB3);
	}

	if (m_cb1)
	{
		tempfile->AddLine(AVDB_TR);
	}
	else
	{
		tempfile->AddLine(AVDB_FL);
	}
	
	wxString temp;
	std::string tempstdstr;

	temp.Printf("%d", m_sc1);
	Base64EncodeN(temp.wc_str(), tempstdstr);
	tempfile->AddLine(wxString(tempstdstr));
	temp.Clear();
	tempstdstr.clear();

	if (m_cb2)
	{
		tempfile->AddLine(AVDB_TR);
	}
	else
	{
		tempfile->AddLine(AVDB_FL);
	}

	temp.Printf("%d", m_sc2);
	Base64EncodeN(temp.wc_str(), tempstdstr);
	tempfile->AddLine(wxString(tempstdstr));
	temp.Clear();
	tempstdstr.clear();

	temp.Printf("%d", m_sc3);
	Base64EncodeN(temp.wc_str(), tempstdstr);
	tempfile->AddLine(wxString(tempstdstr));
	temp.Clear();
	tempstdstr.clear();

	temp.Printf("%d", m_sc4);
	Base64EncodeN(temp.wc_str(), tempstdstr);
	tempfile->AddLine(wxString(tempstdstr));
	temp.Clear();
	tempstdstr.clear();

	if (m_cb4)
	{
		tempfile->AddLine(AVDB_TR);
		//SetEGAVRegForLiveShieldOnOff(TRUE);
		wxExecute(wxT("sc config ClamD start= auto"), wxEXEC_ASYNC | wxEXEC_HIDE_CONSOLE);
		//ExecuteCmdUACSilentW(L"sc.exe", L"stop \"WinSEGAV AutoConfig\"", L"");
		//ExecuteCmdUACSilentW(L"sc.exe", L"start \"WinSEGAV AutoConfig\"", L"");
		//ExecuteCmdUACSilentW(L"sc.exe", L"config ClamD start= auto", L"");
		//ExecuteCmdUACSilentW(L"sc.exe", L"start \"ClamD\"", L"");
	}
	else
	{
		tempfile->AddLine(AVDB_FL);
		//SetEGAVRegForLiveShieldOnOff(FALSE);
		wxExecute(wxT("sc config ClamD start= demand"), wxEXEC_ASYNC | wxEXEC_HIDE_CONSOLE);
		//ExecuteCmdUACSilentW(L"sc.exe", L"config ClamD start= demand", L"");
		//ExecuteCmdUACSilentW(L"sc.exe", L"stop \"ClamD\"", L"");
		//ExecuteCmdUACSilentW(L"sc.exe", L"stop \"WinSEGAV AutoConfig\"", L"");
		//ExecuteCmdUACSilentW(L"sc.exe", L"start \"WinSEGAV AutoConfig\"", L"");
	}

	if (m_cb3)
	{
		tempfile->AddLine(AVDB_TR);
		//SetEGAVRegForAutoUpdateOnOff(TRUE);
		wxExecute(wxT("sc config FreshClam start= auto"), wxEXEC_ASYNC | wxEXEC_HIDE_CONSOLE);
		//ExecuteCmdUACSilentW(L"sc.exe", L"stop \"WinSEGAV AutoConfig\"", L"");
		//ExecuteCmdUACSilentW(L"sc.exe", L"start \"WinSEGAV AutoConfig\"", L"");
		//ExecuteCmdUACSilentW(L"sc.exe", L"config FreshClam start= auto", L"");
		//ExecuteCmdUACSilentW(L"sc.exe", L"start \"FreshClam\"", L"");
	}
	else
	{
		tempfile->AddLine(AVDB_FL);
		//SetEGAVRegForAutoUpdateOnOff(FALSE);
		wxExecute(wxT("sc config FreshClam start= demand"), wxEXEC_ASYNC | wxEXEC_HIDE_CONSOLE);
		//ExecuteCmdUACSilentW(L"sc.exe", L"config FreshClam start= demand", L"");
		//ExecuteCmdUACSilentW(L"sc.exe", L"stop \"FreshClam\"", L"");
		//ExecuteCmdUACSilentW(L"sc.exe", L"stop \"WinSEGAV AutoConfig\"", L"");
		//ExecuteCmdUACSilentW(L"sc.exe", L"start \"WinSEGAV AutoConfig\"", L"");
	}
	wxExecute(wxT("egavservicerestart.exe"), wxEXEC_ASYNC | wxEXEC_HIDE_CONSOLE);
	
	if (m_cb5)
	{
		tempfile->AddLine(AVDB_TR);
	}
	else
	{
		tempfile->AddLine(AVDB_FL);
	}

	tempfile->Write();
	tempfile->Close();
	DELETE_POINTER_WXTEXTFILE(tempfile);

	if (gbIsDefaultBtnClicked)
		MakeDefaultExcludeListFile(dbFile2);
}

void settingsEGAV::OnChangingAnyControl()
{
	if (!gbApplySettings)
	{
		btn_apply->Enable();
		gbApplySettings = true;
	}
}

void settingsEGAV::OnButtonOk(wxCommandEvent& event)
{
	if (gbApplySettings)
	{
		gbApplySettings = false;
		OnChangingSettings();
	}
	Destroy();
}

void settingsEGAV::OnButtonApply(wxCommandEvent& event)
{
	OnChangingSettings();
	btn_apply->Disable();
	gbApplySettings = false;
}

void settingsEGAV::OnButtonCancel(wxCommandEvent& event)
{
	gbApplySettings = false;
	gbIsDefaultBtnClicked = false;
	event.Skip();
	Destroy();
}

void settingsEGAV::OnButtonDefault(wxCommandEvent& event)
{
	gbIsDefaultBtnClicked = true;
	OnChangingAnyControl();
	rb_moveQuad->SetValue(true);
	rb_Remove->SetValue(false);
	rb_Report->SetValue(false);
	m_rb3 = true;
	m_rb2 = false;
	m_rb1 = false;
	
	cb_1->SetValue(true);
	m_cb1 = true;
	
	sc_1->SetValue(30);
	m_sc1 = 30;
	
	cb_2->SetValue(true);
	m_cb2 = true;

	sc_2->SetValue(50);
	m_sc2 = 50;

	sc_3->SetValue(100);
	m_sc3 = 100;

	sc_4->SetValue(10);
	m_sc4 = 10;

	cb_4->SetValue(true);
	m_cb4 = true;

	cb_3->SetValue(true);
	m_cb3 = true;

	wxString excludefiles[] =
	{
		wxT("jpg"), wxT("jpeg"), wxT("gif"), wxT("png"), wxT("tif"),
		wxT("tiff"), wxT("txt"), wxT("docx"), wxT("pdf"), wxT("mp3"),
		wxT("mp4"), wxT("wav"), wxT("avi"), wxT("mpg"), wxT("mpeg"),
		wxT("wmv"), wxT("vob"), wxT("3gp"), 
	};
	size_t tnos = sizeof(excludefiles) / sizeof(excludefiles[0]);

	for (size_t j = 0; j < tnos; j++)
	{
		list1->Append(excludefiles[j]);
	}

}

void settingsEGAV::OnCheckBox1(wxCommandEvent& event)
{
	OnChangingAnyControl();

	if (cb_1->GetValue())
	{
		m_cb1 = true;
	}
	else
	{
		m_cb1 = false;
	}
}

void settingsEGAV::OnCheckBox2(wxCommandEvent& event)
{
	OnChangingAnyControl();

	if (cb_2->GetValue())
	{
		m_cb2 = true;
		sc_2->Enable();
		sc_3->Enable();
		sc_4->Enable();
	}
	else
	{
		m_cb2 = false;
		sc_2->Disable();
		sc_3->Disable();
		sc_4->Disable();
	}
}

void settingsEGAV::OnCheckBox3(wxCommandEvent& event)
{
	OnChangingAnyControl();

	if (cb_3->GetValue())
	{
		m_cb3 = true;
	}
	else
	{
		m_cb3 = false;
	}
}

void settingsEGAV::OnCheckBox4(wxCommandEvent& event)
{
	OnChangingAnyControl();

	if (cb_4->GetValue())
	{
		m_cb4 = true;
	}
	else
	{
		m_cb4 = false;
	}
}

void settingsEGAV::OnCheckBox5(wxCommandEvent& event)
{
	OnChangingAnyControl();

	if (cb_5->GetValue())
	{
		m_cb5 = true;
	}
	else
	{
		m_cb5 = false;
	}
}

void settingsEGAV::OnSpinCtrl1(wxCommandEvent& event)
{
	OnChangingAnyControl();
	m_sc1 = sc_1->GetValue();
}

void settingsEGAV::OnSpinCtrl2(wxCommandEvent& event)
{
	OnChangingAnyControl();
	m_sc2 = sc_2->GetValue();
}

void settingsEGAV::OnSpinCtrl3(wxCommandEvent& event)
{
	OnChangingAnyControl();
	m_sc3 = sc_3->GetValue();
}

void settingsEGAV::OnSpinCtrl4(wxCommandEvent& event)
{
	OnChangingAnyControl();
	m_sc4 = sc_4->GetValue();
}

/*
void settingsEGAV::OnSpinCtrl5(wxCommandEvent& event)
{
	OnChangingAnyControl();
	m_sc5 = sc_5->GetValue();
}
*/

void settingsEGAV::OnRadioButton1(wxCommandEvent& event)
{
	OnChangingAnyControl();

	m_rb1 = true;
	m_rb2 = false;
	m_rb3 = false;
}

void settingsEGAV::OnRadioButton2(wxCommandEvent& event)
{
	OnChangingAnyControl();

	m_rb1 = false;
	m_rb2 = true;
	m_rb3 = false;
}

void settingsEGAV::OnRadioButton3(wxCommandEvent& event)
{
	OnChangingAnyControl();

	m_rb1 = false;
	m_rb2 = false;
	m_rb3 = true;
}

void settingsEGAV::OnButtonAdd1(wxCommandEvent& event)
{
	wxString str = wxGetTextFromUser(wxT("Add new item"));
	if (str.Len() > 0)
	{
		list1->Append(str);
		gxAddLineInTextFile(dbFile2, str);	
	}
}

void settingsEGAV::OnButtonAdd2(wxCommandEvent& event)
{
	wxString str = wxGetTextFromUser(wxT("Add new item"));
	if (str.Len() > 0)
	{
		list2->Append(str);
		gxAddLineInTextFile(dbFile3, str);
	}
}

void settingsEGAV::OnButtonEdit1(wxCommandEvent& event)
{
	wxString text;
	wxString edited;
	int sel = list1->GetSelection();
	if (sel != -1)
	{
		text = list1->GetString(sel);
		edited = wxGetTextFromUser(wxT("Edit item"), wxT("Edit dialog"), text);
	}
	if (!edited.IsEmpty())
	{
		list1->Delete(sel);
		list1->Insert(edited, sel);
		gxChangeLineInTextFile(dbFile2, edited, sel);
	}
}

void settingsEGAV::OnButtonEdit2(wxCommandEvent& event)
{
	wxString text;
	wxString edited;
	int sel = list2->GetSelection();
	if (sel != -1)
	{
		text = list2->GetString(sel);
		edited = wxGetTextFromUser(wxT("Edit item"), wxT("Edit dialog"), text);
	}
	if (!edited.IsEmpty())
	{
		list2->Delete(sel);
		list2->Insert(edited, sel);
		gxChangeLineInTextFile(dbFile3, edited, sel);
	}
}

void settingsEGAV::OnButtonClear1(wxCommandEvent& event)
{
	list1->Clear();
	gxRemoveAllLinesInTextFile(dbFile2);
}

void settingsEGAV::OnButtonClear2(wxCommandEvent& event)
{
	list2->Clear();
	gxRemoveAllLinesInTextFile(dbFile3);
}

void settingsEGAV::OnButtonDelete1(wxCommandEvent& event)
{
	int sel = list1->GetSelection();
	if (sel != -1)
	{
		list1->Delete(sel);
		gxRemoveLineInTextFile(dbFile2, sel);
	}
}

void settingsEGAV::OnButtonDelete2(wxCommandEvent& event)
{
	int sel = list2->GetSelection();
	if (sel != -1)
	{
		list2->Delete(sel);
		gxRemoveLineInTextFile(dbFile3, sel);
	}
}

void settingsEGAV::OnDblClickList1(wxCommandEvent& event)
{
	wxString text;
	wxString edited;
	int sel = list1->GetSelection();
	if (sel != -1)
	{
		text = list1->GetString(sel);
		edited = wxGetTextFromUser(wxT("Edit item"), wxT("Edit dialog"), text);
	}
	if (!edited.IsEmpty())
	{
		list1->Delete(sel);
		list1->Insert(edited, sel);
		gxChangeLineInTextFile(dbFile2, edited, sel);
	}
}

void settingsEGAV::OnDblClickList2(wxCommandEvent& event)
{
	wxString text;
	wxString edited;
	int sel = list2->GetSelection();
	if (sel != -1)
	{
		text = list2->GetString(sel);
		edited = wxGetTextFromUser(wxT("Edit item"), wxT("Edit dialog"), text);
	}
	if (!edited.IsEmpty())
	{
		list2->Delete(sel);
		list2->Insert(edited, sel);
		gxChangeLineInTextFile(dbFile3, edited, sel);
	}
}

void settingsEGAV::LoadDBAndInitData(const wxString& filedb1, const wxString& filedb2, const wxString& filedb3)
{
	wxTextFile* tempfile = new wxTextFile(filedb1);
	tempfile->Open();
	wxString temp;
	temp = tempfile->GetLine(0);
	if (temp == AVDB_RB1)
	{
		rb_Report->SetValue(true);
		rb_Remove->SetValue(false);
		rb_moveQuad->SetValue(false);
		m_rb1 = true;
		m_rb2 = false;
		m_rb3 = false;
	}
	else if (temp == AVDB_RB2)
	{
		rb_Report->SetValue(false);
		rb_Remove->SetValue(true);
		rb_moveQuad->SetValue(false);
		m_rb1 = false;
		m_rb2 = true;
		m_rb3 = false;
	}
	else
	{
		rb_Report->SetValue(false);
		rb_Remove->SetValue(false);
		rb_moveQuad->SetValue(true);
		m_rb1 = false;
		m_rb2 = false;
		m_rb3 = true;
	}
	temp = tempfile->GetLine(1);
	if (temp == AVDB_TR)
	{
		cb_1->SetValue(true);
		m_cb1 = true;
	}
	else
	{
		cb_1->SetValue(false);
		m_cb1 = false;
	}
	temp = tempfile->GetLine(2);
	std::wstring tempstdwstr;
	Base64DecodeN(temp.c_str().AsChar(), tempstdwstr);
	wxString(tempstdwstr).ToCULong(&m_sc1);
	tempstdwstr.clear();
	sc_1->SetValue(m_sc1);
	temp = tempfile->GetLine(3);
	if (temp == AVDB_TR)
	{
		cb_2->SetValue(true);
		m_cb2 = true;
		sc_2->Enable();
		sc_3->Enable();
		sc_4->Enable();
	}
	else
	{
		cb_2->SetValue(false);
		m_cb2 = false;
		sc_2->Disable();
		sc_3->Disable();
		sc_4->Disable();
	}
	temp = tempfile->GetLine(4);
	Base64DecodeN(temp.c_str().AsChar(), tempstdwstr);
	wxString(tempstdwstr).ToCULong(&m_sc2);
	tempstdwstr.clear();
	sc_2->SetValue(m_sc2);
	temp = tempfile->GetLine(5);
	Base64DecodeN(temp.c_str().AsChar(), tempstdwstr);
	wxString(tempstdwstr).ToCULong(&m_sc3);
	tempstdwstr.clear();
	sc_3->SetValue(m_sc3);
	temp = tempfile->GetLine(6);
	Base64DecodeN(temp.c_str().AsChar(), tempstdwstr);
	wxString(tempstdwstr).ToCULong(&m_sc4);
	tempstdwstr.clear();
	sc_4->SetValue(m_sc4);
	temp = tempfile->GetLine(7);
	if (temp == AVDB_TR)
	{
		cb_4->SetValue(true);
		m_cb4 = true;
	}
	else
	{
		cb_4->SetValue(false);
		m_cb4 = false;
	}
	temp = tempfile->GetLine(8);
	if (temp == AVDB_TR)
	{
		cb_3->SetValue(true);
		m_cb3 = true;
	}
	else
	{
		cb_3->SetValue(false);
		m_cb3 = false;
	}

	temp = tempfile->GetLine(9);
	if (temp == AVDB_TR)
	{
		cb_5->SetValue(true);
		m_cb5 = true;
	}
	else
	{
		cb_5->SetValue(false);
		m_cb5 = false;
	}

	tempfile->Close();
	temp.Clear();
	DELETE_POINTER_WXTEXTFILE(tempfile);

	//------------------------------------------------------------//
	//exclude file extension list 1
	wxTextFile* tempfile2 = new wxTextFile(filedb2);
	tempfile2->Open();
	size_t tnol;
	tnol = tempfile2->GetLineCount();
	if (tnol != 0)
	{
		for (size_t i = 0; i < tnol; i++)
		{
			temp = tempfile2->GetLine(i);
			if (temp.Len() > 0)
				list1->Append(temp);
			temp.Clear();
		}
	}
	tempfile2->Close();
	DELETE_POINTER_WXTEXTFILE(tempfile2);

	//------------------------------------------------------------//
	//include file extension list 2
	wxTextFile* tempfile3 = new wxTextFile(filedb3);
	tempfile3->Open();
	tnol = tempfile3->GetLineCount();
	if (tnol != 0)
	{
		for (size_t j = 0; j < tnol; j++)
		{
			temp = tempfile3->GetLine(j);
			if (temp.Len() > 0)
				list2->Append(temp);
			temp.Clear();
		}
	}
	tempfile3->Close();
	DELETE_POINTER_WXTEXTFILE(tempfile3);
	//------------------------------------------------------------//
}

/*
void RestartEGAVServices(bool bStartClamd, bool bStartFreshClam)
{
wxExecute(wxT("sc stop \"WinSEGAV AutoConfig\""), wxEXEC_ASYNC | wxEXEC_HIDE_CONSOLE);
wxMilliSleep(50);
wxExecute(wxT("sc start \"WinSEGAV AutoConfig\""), wxEXEC_ASYNC | wxEXEC_HIDE_CONSOLE);
wxMilliSleep(50);
if (bStartClamd)
wxExecute(wxT("sc start ClamD"), wxEXEC_ASYNC | wxEXEC_HIDE_CONSOLE);
wxMilliSleep(50);
if (bStartFreshClam)
wxExecute(wxT("sc start FreshClam"), wxEXEC_ASYNC | wxEXEC_HIDE_CONSOLE);
}*/
