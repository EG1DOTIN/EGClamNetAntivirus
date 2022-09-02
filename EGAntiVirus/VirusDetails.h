#ifndef VIRUS_DETAILS_HEADER
#define VIRUS_DETAILS_HEADER
#include <wx\wx.h>
#include "gxTextCtrlURL.h"
#include "gxButton.h"

class ViewVirusDetail : public wxDialog
{
public:
	ViewVirusDetail(const wxString& title, const wxString & filefullpath, const wxString& heading1, const wxString& heading2);

	void OnBtnExit(wxCommandEvent& event);

	wxPanel* m_panelall;

	wxBoxSizer* m_boxsizerVAll;

	wxStaticText* m_heading1;
	wxStaticText* m_heading2;

	gxTextCtrlURL* m_textctrl;

	MyWXRoundEdgeButton* m_btn_exit;

};

enum eBTN_ID21
{
	//600-619
	ID_VIRUS_DETAILS_BTN_EXIT = 600, 
};

void AVShowVirusDetail(const wxString & title, const wxString & filefullpath, const wxString& heading1, const wxString& heading2);
#endif