#ifndef CLEANER_HEADER
#define CLEANER_HEADER

#include <wx/wx.h>
#include "gxButton.h"
#include <wx/statline.h>

const wxString contentStr[9] =
{
	wxT("Temporary Files/Folders"), wxT("IE Cache"), wxT("IE Cookies"),
	wxT("IE History"), wxT("Other Browser's Cache And History"), wxT("Recycle Bin Contents"),
    wxT("IE AddressBar History"),wxT("Recent Docs History"), wxT("Run History"),
};

class Cleaner : public wxDialog
{

public:

	Cleaner(const wxString& title);
	void OnBtnClean(wxCommandEvent& event);
	void OnBtnCancel(wxCommandEvent& event);
	void OnCheckBox(wxCommandEvent& event);
	/*
private:
	void OnClose(wxCloseEvent& event);
	DECLARE_EVENT_TABLE()
	*/
	wxPanel* m_Panel_All;
	wxPanel* m_Panel_Upper;
	wxPanel* m_Panel_Lower;
	wxPanel* m_Panel_Content;
	wxPanel* m_Panel_ConSize;
	wxPanel* m_Panel_CheckBox;

	wxBoxSizer* m_SH_Upper;
	wxBoxSizer* m_SV_Content;
	wxBoxSizer* m_SV_ConSize;
	wxBoxSizer* m_SV_CheckBox;
	wxBoxSizer* m_SH_Buttons;
	wxBoxSizer* m_SV_Upper;
	wxBoxSizer* m_SV_Lower;

	wxStaticText* m_st_content[11];
	wxStaticText* m_st_contentsize[11];
	wxStaticText* m_st_RemoveOrNot;
	wxCheckBox* m_cb_RemoveOrNot[10];
	MyWXRoundEdgeButton* m_Btn_Clean;
	MyWXRoundEdgeButton* m_Btn_Cancel;	

};

enum eBTN_ID15
{
	//400 -429
	ID_CLEANER_BTN_CLEAN = 400,
	ID_CLEANER_BTN_CANCEL,
	ID_CLEANER_CB_0,
	ID_CLEANER_CB_1,
	ID_CLEANER_CB_2,
	ID_CLEANER_CB_3,
	ID_CLEANER_CB_4,
	ID_CLEANER_CB_5,
	ID_CLEANER_CB_6,
	ID_CLEANER_CB_7,
	ID_CLEANER_CB_8,
	ID_CLEANER_CB_9,
};

#endif