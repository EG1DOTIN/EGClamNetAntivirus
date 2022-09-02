#ifndef HOME_TRAY_WINDOW_HEADER
#define HOME_TRAY_WINDOW_HEADER

#include <wx\wx.h>
#include <wx\taskbar.h>
#include "gxClickText.h"


class MyTaskBarIcon : public wxTaskBarIcon
{
public:
#if defined(__WXOSX__) && wxOSX_USE_COCOA
	MyTaskBarIcon(wxTaskBarIconType iconType = wxTBI_DEFAULT_TYPE)
		: wxTaskBarIcon(iconType)
	{

	}
#else
	MyTaskBarIcon()
	{

	}
#endif

	void OnLeftButtonDClick(wxTaskBarIconEvent&);
	void OnMenuOpenWindow(wxCommandEvent&);
	void OnMenuOpenAdminWindow(wxCommandEvent&);
	void OnMenuDownloadUpdates(wxCommandEvent&);
	void OnMenuVisitWebSite(wxCommandEvent&);
	void OnMenuExitAll(wxCommandEvent&);

	virtual wxMenu *CreatePopupMenu();

	wxDECLARE_EVENT_TABLE();
};

enum eTrayIDs
{
	PU_OPEN_WINDOW = 10001, 
	PU_DOWNLOAD_UPDATES,
	PU_VISIT_WEBSITE,
	PU_EXIT_ALL,
};

class HomeTrayWin : public wxFrame
{
public:
	HomeTrayWin(const wxString& title);
	virtual ~HomeTrayWin();

	void OnPaint(wxPaintEvent& event);

	void OnMenuUpdateVSD(wxCommandEvent& event);
	void OnMenuUpdateSoftware(wxCommandEvent& event);
	void OnMenuDocHelp(wxCommandEvent& event);
	void OnMenuAnyFeedback(wxCommandEvent& event);
	void OnMenuAboutUs(wxCommandEvent& event);

	void OnButtonOAPW(wxCommandEvent& event);

	void OnTimerScbMsg(wxTimerEvent& event);
	void UpdatedTrayIcon(const wxIcon& icon, const wxString& IconString, const wxString& BaloonString);
	void SetUpdateStatus(const wxString& Status, BOOL bUpdate);
	bool GetUpdateStatus();
	void AutoUpdateVSD();
	void AutoUpdateVSD1();
	void CleanExit();

	//Panels
	wxPanel* m_PanelAll;
	wxPanel* m_PanelBuyNow;
	wxPanel* m_PanelRemainingDays;
	wxPanel* m_PanelUpper;
	wxPanel* m_PanelLower;
	wxPanel* m_PanelUpper1;
	wxPanel* m_PanelUpper2;
	wxPanel* m_PanelUpper3;

	//BoxSizers
	wxBoxSizer* m_SV_ALL;
	wxBoxSizer* m_SH_BuyNow;
	wxBoxSizer* m_SH_IMAGE;
	wxBoxSizer* m_SV_STATUS_TEXT;
	wxBoxSizer* m_SV_STATUS_RESULT;
	wxBoxSizer* m_SH_UPPER;
	wxBoxSizer* m_SH_BUTTON;

	//Static Text
	wxStaticText* m_ST_PC_STATUS_TEXT;
	wxStaticText* m_ST_PC_STATUS_RESULT;


	//BitMap
	wxBitmap m_BM_EGAVU;
	wxBitmap m_BM_EGAVN;
	wxBitmap m_BM_EGAVC;

	//Static BitMap
	wxStaticBitmap* m_SB_IMAGE_STATUS;

	//Button
	wxButton* m_BTN_OAPW; //OPAM -> open admin panel window


	// MENU AND MENUBAR
	wxMenuBar* menubar;
	wxMenu* m_update;
	wxMenu* m_view;
	wxMenu* m_help;

	//app Icon
	wxIcon AppIconU;
	wxIcon AppIconN;
	wxIcon AppIconC;


	//Timer for Showing SysComBlocker Messages
	wxTimer* TimerScbMsg; 

	MyTaskBarIcon   *m_taskBarIcon;
#if defined(__WXOSX__) && wxOSX_USE_COCOA
	MyTaskBarIcon   *m_dockIcon;
#endif

private:
	void OnClose(wxCloseEvent& event);
	DECLARE_EVENT_TABLE()

};

int CheckVirusSigDatabase();
void DownloadSoftwareUpdate();

const int DAYS_LIMIT_TO_CHECK_VSD = 3;

enum eWinIDs0
{
	//100 -119
	ID_TRAY_MENU_UPDATE_VSD = 100,
	ID_TRAY_MENU_UPDATE_SOFTWARE,
	ID_TRAY_MENU_DOC_HELP,
	ID_TRAY_MENU_ANY_FEEDBACK,
	ID_TRAY_MENU_ABOUT_US,
	ID_TRAY_BUTTON_OAPW,
	ID_TRAY_TIMER_SCB_MSG,
};

#endif // !HOME_TRAY_WINDOW_HEADER
