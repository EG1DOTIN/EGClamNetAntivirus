#ifndef EGAV_HOMEWINDOW_H
#define EGAV_HOMEWINDOW_H

#include <wx/wx.h>
#include <wx/dirctrl.h>
#include "gxClickText.h"
#include "scanner.h"


class AVHomeWindow : public wxDialog
{
public:
	//Constructor AVHomeWindow
	AVHomeWindow(const wxString& winTitle);
	void OnBtnScan(wxCommandEvent& btnEvent);
	void OnBtnSettings(wxCommandEvent& WXUNUSED(btnEvent));
	void OnBtnCleaner(wxCommandEvent& WXUNUSED(btnEvent));

	void OnBtnHelp(wxCommandEvent& WXUNUSED(btnEvent));

	void OnCollapseDirCtrl(wxTreeEvent& event);
	void OnTimerHome(wxTimerEvent& event);

	wxClickText* m_avClogo;                  							//Heading Image EG logo
	wxClickText* m_avQuarantineText;
	wxStaticText *m_avHeadingText1, *m_avHeadingText2;          	   // Heading "Anti Virus"
	wxGenericDirCtrl* m_avDirCtrl;            							// Dir/File Selection Window
	wxStaticText* dirCtrlTxt0;
	wxStaticText* dirCtrlTxt1;

	wxBitmap m_bmpScan, m_bmpSettings, m_bmpCleaner, m_bmpMisc, m_bmpRegister, m_bmpHelp;
	wxBitmapButton *m_bb_Scan, *m_bb_Settings, *m_bb_Cleaner, *m_bb_Help;
	wxStaticText *m_st_scan, *m_st_settings, *m_st_cleaner, *m_st_misc;

	//Panels
	wxPanel* m_avPanel0;
	wxPanel* m_avPanel1;
	wxPanel* m_avPanel2;
	wxPanel* m_avPanel3;
	wxPanel* m_avPanel4;
	wxPanel* m_avPanel5;
	wxPanel* m_avPanel6;
	wxPanel* m_avPanel7;

	wxPanel* m_avPanel34;
	wxPanel* m_avPanel56;

	//Sizers
	wxBoxSizer* m_avSizer1H;
	wxBoxSizer* m_avSizer2H;
	wxBoxSizer* m_avSizer3H;
	wxBoxSizer* m_avSizer4V;
	wxBoxSizer* m_avSizer5H;
	wxBoxSizer* m_avSizer6V;
	wxBoxSizer* m_avSizer7V;
	wxBoxSizer* m_avSizer8H;
	wxBoxSizer* m_avSizer9V;

	ScannerWin* m_QuickScan;
	ScannerWin* m_FullScan;
	
	/////////////////////////////////////////////////////////////////////
	wxPanel* Panel_FTG;
	wxBitmap bmp_facebook, bmp_twitter, bmp_google, bmp_instagram;
	wxBitmapButton *Button_FACEBOOK, *Button_TWITTER, *Button_GOOGLE, *Button_INSTAGRAM;
	wxBoxSizer *SizerH_FTDPanel;
	void OnBunttonFacebook(wxCommandEvent& event);
	void OnBunttonTwitter(wxCommandEvent& event);
	void OnBunttonGoogle(wxCommandEvent& event);
	void OnBunttonInstagram(wxCommandEvent& event);
	/////////////////////////////////////////////////////////////////////

	//Timer
	wxTimer* m_TimerHome;

private:
	void OnClose(wxCloseEvent& event);
	DECLARE_EVENT_TABLE()
	
};

enum eBTN_ID1
{
	ID_BTN_SCAN = 120, 
	ID_BTN_SETTINGS,
	ID_BTN_UPDATE,
	ID_BTN_CLEANER,
	ID_BTN_HELP,
	ID_TIMER_HOME,
	ID_EG_LOGO,
	ID_QUARANTINETEXT,
	ID_BUTTON_FACEBOOK,
	ID_BUTTON_TWITTER,
	ID_BUTTON_GOOGLE,
	ID_BUTTON_INSTAGRAM,
};
#endif
