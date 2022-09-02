#ifndef SETTINGS_HEADER
#define SETTINGS_HEADER

#include <wx\wx.h>
#include "gxButton.h"

class settingsEGAV : public wxDialog
{
public:
	settingsEGAV(const wxString& title);
	void OnButtonOk(wxCommandEvent& event);
	void OnButtonApply(wxCommandEvent& event);
	void OnButtonCancel(wxCommandEvent& event);
	void OnButtonDefault(wxCommandEvent& event);
	
	void OnButtonAdd1(wxCommandEvent& event);
	void OnButtonAdd2(wxCommandEvent& event);
	void OnButtonEdit1(wxCommandEvent& event);
	void OnButtonEdit2(wxCommandEvent& event);
	void OnButtonClear1(wxCommandEvent& event);
	void OnButtonClear2(wxCommandEvent& event);
	void OnButtonDelete1(wxCommandEvent& event);
	void OnButtonDelete2(wxCommandEvent& event);
	void OnDblClickList1(wxCommandEvent& event);
	void OnDblClickList2(wxCommandEvent& event);


	void OnCheckBox1(wxCommandEvent& event);	//Event function for Option "Unload Infected Program from Computer Memory" 
	void OnCheckBox2(wxCommandEvent& event);	//Event function for Option "Extract Files From Archives"
	void OnCheckBox3(wxCommandEvent& event);	//Event function for Option "Automatic Updates On/Off"
	void OnCheckBox4(wxCommandEvent& event);	//Event function for Option "Real-Time Protection On/Off"
	void OnCheckBox5(wxCommandEvent& event);    //Event function for Option "Do Not Reload Virus Signature DB (Fast Scan)

	void OnSpinCtrl1(wxCommandEvent& event);	//Event function for Option "Do Not Scan Files Larger Than"MB
	void OnSpinCtrl2(wxCommandEvent& event);	//Event function for Option	"Do Not Extract More Than"MB
	void OnSpinCtrl3(wxCommandEvent& event);	//Event function for Option "Do Not Extract More Than"Files
	void OnSpinCtrl4(wxCommandEvent& event);	//Event function for Option "Do Not Extract More Than"Sub Archives
	//void OnSpinCtrl5(wxCommandEvent& event);	//Event function for Option "Limit Log File Size To"MB

	void OnRadioButton1(wxCommandEvent& event);	//Event function for Option radio button "Report Only" 
	void OnRadioButton2(wxCommandEvent& event);	//Event function for Option	radio button "Remove (Use Carefully)"
	void OnRadioButton3(wxCommandEvent& event);	//Event function for Option	radio button "Move To Quarantine Folder"

	void OnChangingAnyControl(void);
	void OnChangingSettings(void);

	void LoadDBAndInitData(const wxString& filedb1, const wxString& filedb2, const wxString& filedb3);

	wxPanel* panel_ALL;
	wxPanel* panel_1;	//Consist Infected Files options
	wxPanel* panel_2;	//Consist All Files options
	wxPanel* panel_3;	//Consist Archive Files options
	wxPanel* panel_4;	//Consist Real-Time Protection On/Off
	wxPanel* panel_5;	//Consist Updates options
	wxPanel* panel_6;	//Consist Filters options
	wxPanel* panel_7;	//Consist Buttons OK Apply Cancel 

	wxPanel* panel_A;	//Consist Filters options Exclude list in scan
	wxPanel* panel_B;	//Consist Filters options include list in scan

	MyWXRoundEdgeButton* btn_ok;
	MyWXRoundEdgeButton* btn_apply;
	MyWXRoundEdgeButton* btn_cancel;
	MyWXRoundEdgeButton* btn_default;

	MyWXRoundEdgeButton* btn_Add1;	//exclude options Buttons 
	MyWXRoundEdgeButton* btn_Edit1;
	MyWXRoundEdgeButton* btn_Delete1;
	MyWXRoundEdgeButton* btn_Clear1;
	MyWXRoundEdgeButton* btn_Add2;	//include options Buttons 
	MyWXRoundEdgeButton* btn_Edit2;
	MyWXRoundEdgeButton* btn_Delete2;
	MyWXRoundEdgeButton* btn_Clear2;


	wxStaticBox *sb_InfectedFiles;
	wxStaticBox *sb_AllFiles;
	wxStaticBox *sb_Archives;
	wxStaticBox *sb_Shield;
	wxStaticBox *sb_Update;
	wxStaticBox *sb_Filters;

	wxRadioButton *rb_Report;
	wxRadioButton *rb_Remove;
	wxRadioButton *rb_moveQuad;

	wxCheckBox* cb_1;
	wxCheckBox* cb_2;
	wxCheckBox* cb_3;
	wxCheckBox* cb_4;
	
	wxCheckBox* cb_5;


	wxSpinCtrl* sc_1;
	wxSpinCtrl* sc_2;
	wxSpinCtrl* sc_3;
	wxSpinCtrl* sc_4;
//	wxSpinCtrl* sc_5;

	wxBoxSizer* bs_H1;
	wxBoxSizer* bs_H2;
	wxBoxSizer* bs_V1;
	wxBoxSizer* bs_V2;
	wxBoxSizer* bs_V3;

	wxStaticText* st_1;
	wxStaticText* st_2;
	wxStaticText* st_3;
	wxStaticText* st_4;
	wxStaticText* st_5;
	wxStaticText* st_6;
	wxStaticText* st_7;
	wxStaticText* st_8;
//	wxStaticText* st_9;
//	wxStaticText* st_10;
	wxStaticText* st_11;
	wxStaticText* st_12;
	wxStaticText* st_13;
	wxStaticText* st_14;
	wxStaticText* st_15;
	wxStaticText* st_16;

	wxListBox* list1;	//Exclude list box
	wxListBox* list2;	//Include list box

	bool m_rb1, m_rb2, m_rb3, m_cb1, m_cb2, m_cb3, m_cb4, m_cb5;
	unsigned long m_sc1, m_sc2, m_sc3, m_sc4;// m_sc5;

};

enum eBTN_ID9
{
	//280-339

	ID_SETTINGS_BTN1 = 280, 
	ID_SETTINGS_BTN2,
	ID_SETTINGS_BTN3,
	ID_SETTINGS_BTN4,

	ID_SETTINGS_RB1,
	ID_SETTINGS_RB2,
	ID_SETTINGS_RB3,

	ID_SETTINGS_CB1,
	ID_SETTINGS_CB2,
	ID_SETTINGS_CB3,
	ID_SETTINGS_CB4,
	ID_SETTINGS_CB5,

	ID_SETTINGS_SC1,
	ID_SETTINGS_SC2,
	ID_SETTINGS_SC3,
	ID_SETTINGS_SC4,
	//ID_SETTINGS_SC5,

	ID_SETTINGS_LB1,
	ID_SETTINGS_LB2,

	ID_SETTINGS_BTN_ADD1,
	ID_SETTINGS_BTN_EDIT1,
	ID_SETTINGS_BTN_DELETE1,
	ID_SETTINGS_BTN_CLEAR1,
	ID_SETTINGS_BTN_ADD2,
	ID_SETTINGS_BTN_EDIT2,
	ID_SETTINGS_BTN_DELETE2,
	ID_SETTINGS_BTN_CLEAR2,
};

//void RestartEGAVServices(bool bStartClamd, bool bStartFreshClam);
#endif