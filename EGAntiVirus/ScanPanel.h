#ifndef SCAN_PANEL_HEADER
#define SCAN_PANEL_HEADER

#include <wx\wx.h>

class ScanPanel :public wxPanel
{
public:
	ScanPanel(const wxString& title, wxWindow* parent);
	void OnBitmapButtonCstmScan(wxCommandEvent& event);
	void OnBitmapButtonQuikScan(wxCommandEvent& event);
	void OnBitmapButtonFullScan(wxCommandEvent& event);

	wxPanel *m_p1, *m_p2, *m_p3;

	wxBoxSizer* m_sh;
	wxBoxSizer *m_sv1, *m_sv2, *m_sv3;

	wxStaticText* m_st_cstm_scan;
	wxStaticText* m_st_quik_scan; 
	wxStaticText* m_st_full_scan;

	wxBitmapButton* m_bb_cscan;
	wxBitmapButton* m_bb_qscan;
	wxBitmapButton* m_bb_fscan;
};

enum eBTN_ID50
{
	//3000
	ID_BUTTON_SP_OK = 3000,
	ID_BITMAP_BUTTON_CUSTOM_SCAN,
	ID_BITMAP_BUTTON_QUICK_SCAN,
	ID_BITMAP_BUTTON_FULL_SCAN,
};

#endif // !SCAN_PANEL_HEADER
