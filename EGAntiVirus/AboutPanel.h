#ifndef ABOUT_PANEL_HEADER
#define ABOUT_PANEL_HEADER

#include <wx\wx.h>
#include "gxTextCtrlURL.h"

class AboutPanel :public wxPanel
{
public:
	AboutPanel(const wxString& title, wxWindow* parent);
	void OnBitmapButtonEGAV(wxCommandEvent& event);
	void OnBitmapButtonClamAV(wxCommandEvent& event);
	void OnBitmapButtonWXwidjets(wxCommandEvent& event);

	wxPanel* m_pHeadingText;
	wxPanel* m_pTextCtrl;
	wxPanel* m_pUpperBitmap;
	wxPanel* m_pMid1Bitmap;
	wxPanel* m_pMid2Bitmap;

	wxBoxSizer* m_sV_All;
	wxBoxSizer* m_sHHeadingText;
	wxBoxSizer* m_sHTextCtrl;
	wxBoxSizer* m_sH_UpperBitmap;
	wxBoxSizer* m_sH_Upper;
	wxBoxSizer* m_sH_Mid1Bitmap;
	wxBoxSizer* m_sH_Mid2Bitmap;

	wxStaticText* m_st_heading;
	gxTextCtrlURL* m_tc_Copyright;

	wxStaticText* m_st_SE_Text;
	wxStaticText* m_st_LU_Text;

	wxBitmapButton* m_bb_eg1;
	wxBitmapButton* m_bb_clam_av;
	wxBitmapButton* m_bb_wxwidjets;
};

enum e_BTN_ID20
{
	//550 -570
	ID_BUTTON_OK = 550,
	ID_BITMAP_BUTTON_EGAV,
	ID_BITMAP_BUTTON_CLAMAV,
	ID_BITMAP_BUTTON_WXWIDJETS,
};

#endif // !ABOUT_PANEL_HEADER
