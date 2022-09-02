#include "common.h"
#include "gxfunctions.h"
#include "egavsetgetdb.h"
#include "avmsgebox.h"
#include "HomeTrayWindow.h"
#include "avtheme.h"
#include "CheckNet.h"
#include "egavicon.h"
#include <wx\aboutdlg.h>
#include "SendMsgToService.h"
#include "winevent.h"
#include "datetime.h"

WinEventTestFrame* syswinevent = NULL;

extern wxString workingDir;
extern wxString AppDataEGAVPath;
extern wxString dbFile1;  //For antivirus settings
extern HomeTrayWin* gHomeTray;


bool gbUpdated = false;

BEGIN_EVENT_TABLE(HomeTrayWin, wxFrame)
EVT_CLOSE(HomeTrayWin::OnClose)
END_EVENT_TABLE()

/*
"EG ClamNet Antivirus \n"
"© 2022 EG1"
*/

/*
static const char * const title = "EG ClamNet Antivirus";
static const char * const message
= "EG ClamNet Antivirus is Active"
"\n"
"© 2022 EG1";
*/

const wxString IconStrU(
"Up To Date \n"
"SYSTEM IS SECURED \n"
"EG ClamNet AntiVirus\n"
"© 2022 EG1");

const wxString IconStrN(
	"Out Of Date  \n"
	"AV SIG. NOT UPDATED\n"
	"EG ClamNet AntiVirus \n"
	"© 2022 EG1");

const wxString IconStrC(
	"System At Risk \n"
	"REAL-TIME PROTECTON IS OFF\n"
	"EG ClamNet AntiVirus \n"
	"© 2022 EG1");

const wxString BaloonStrU(wxT("EG ClamNet AntiVirus is up-to-date\n"));
const wxString BaloonStrN(wxT("EG ClamNet AntiVirus is out-of-date\n"));
const wxString BaloonStrC(wxT("Real-Time Protection is off\n"));


HomeTrayWin::HomeTrayWin(const wxString& title) : wxFrame(NULL, -1, title, wxDefaultPosition, wxSize(600, 450), EGAV_HOME_WINDOW_BORDER_STYLE)
{
	SetEGAVFrameIcon(this);

	AppIconU = wxIcon(wxIconLocation(workingDir + wxT("\\Resources\\egavu.ico")));
	AppIconU.RealizeResource();

	AppIconN = wxIcon(wxIconLocation(workingDir + wxT("\\Resources\\egavn.ico")));
	AppIconN.RealizeResource();

	AppIconC = wxIcon(wxIconLocation(workingDir + wxT("\\Resources\\egavc.ico")));
	AppIconC.RealizeResource();

	Connect(wxEVT_PAINT, wxPaintEventHandler(HomeTrayWin::OnPaint));
	Connect(ID_TRAY_MENU_UPDATE_VSD, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(HomeTrayWin::OnMenuUpdateVSD));
	Connect(ID_TRAY_MENU_UPDATE_SOFTWARE, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(HomeTrayWin::OnMenuUpdateSoftware));
	//Connect(ID_TRAY_MENU_DOC_HELP, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(HomeTrayWin::OnMenuDocHelp));
	Connect(ID_TRAY_MENU_ANY_FEEDBACK, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(HomeTrayWin::OnMenuAnyFeedback));
	Connect(ID_TRAY_MENU_ABOUT_US, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(HomeTrayWin::OnMenuAboutUs));
	Connect(ID_TRAY_BUTTON_OAPW, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(HomeTrayWin::OnButtonOAPW));
	Connect(ID_TRAY_TIMER_SCB_MSG, wxEVT_TIMER, wxTimerEventHandler(HomeTrayWin::OnTimerScbMsg));


	TimerScbMsg = new wxTimer(this, ID_TRAY_TIMER_SCB_MSG);

	// MENU BAR
	menubar = new wxMenuBar;

	// MENU
	m_update = new wxMenu;
	m_help = new wxMenu;

	// APPEND ITEM IN MENU

	//m_update->Append(ID_TRAY_MENU_UPDATE_SOFTWARE, wxT("&Download Software Update"));
	m_update->Append(ID_TRAY_MENU_UPDATE_VSD, wxT("&Update Virus Signature Database"));
	//m_help->Append(ID_TRAY_MENU_DOC_HELP, wxT("Documentation"));
	m_help->Append(ID_TRAY_MENU_ANY_FEEDBACK, wxT("Any &Feedback?"));
	m_help->Append(ID_TRAY_MENU_ABOUT_US, wxT("&About Us"));

	// APPEND MENU IN MENUBAR
	menubar->Append(m_update, wxT("&Update"));
	menubar->Append(m_help, wxT("&Help"));
	// SET THE MENU BAR
	SetMenuBar(menubar);

	m_PanelAll = new wxPanel(this);
	m_PanelBuyNow = new wxPanel(m_PanelAll, -1, wxPoint(10, 20), wxSize(500, 50));
	m_PanelRemainingDays = new wxPanel(m_PanelAll, -1, wxPoint(10, 70), wxSize(500, 50));
	m_PanelUpper = new wxPanel(m_PanelAll);
	m_PanelLower = new wxPanel(m_PanelAll);
	m_PanelUpper1 = new wxPanel(m_PanelUpper);
	m_PanelUpper2 = new wxPanel(m_PanelUpper);
	m_PanelUpper3 = new wxPanel(m_PanelUpper);


	wxImage::AddHandler(new wxICOHandler);
	m_BM_EGAVU = wxBitmap(workingDir + wxT("\\Resources\\egavu.ico"), wxBITMAP_TYPE_ICO);
	m_BM_EGAVN = wxBitmap(workingDir + wxT("\\Resources\\egavn.ico"), wxBITMAP_TYPE_ICO);
	m_BM_EGAVC = wxBitmap(workingDir + wxT("\\Resources\\egavc.ico"), wxBITMAP_TYPE_ICO);


	m_SB_IMAGE_STATUS = new wxStaticBitmap(m_PanelUpper1, -1, m_BM_EGAVN, wxPoint(0, 0), wxDefaultSize);
	m_SH_IMAGE = new wxBoxSizer(wxHORIZONTAL);
	m_SH_IMAGE->Add(m_SB_IMAGE_STATUS);
	m_PanelUpper1->SetSizer(m_SH_IMAGE);
	
	m_ST_PC_STATUS_RESULT = new wxStaticText(m_PanelUpper3, -1, wxT("..."));
	m_ST_PC_STATUS_RESULT->SetFont(EGAV_FONT_MID4_SWISS_ITALIC_BOLD);

	m_SV_STATUS_RESULT = new wxBoxSizer(wxVERTICAL);
	m_SV_STATUS_RESULT->AddStretchSpacer();
	m_SV_STATUS_RESULT->Add(m_ST_PC_STATUS_RESULT);
	m_SV_STATUS_RESULT->AddStretchSpacer();
	m_PanelUpper3->SetSizer(m_SV_STATUS_RESULT);

	m_SH_UPPER = new wxBoxSizer(wxHORIZONTAL);
	m_SH_UPPER->Add(m_PanelUpper1, 1, wxEXPAND | wxALL, 20);
	m_SH_UPPER->Add(m_PanelUpper3, 1, wxEXPAND | wxALL, 50);
	m_PanelUpper->SetSizer(m_SH_UPPER);

	m_BTN_OAPW = new wxButton(m_PanelLower, ID_TRAY_BUTTON_OAPW, wxT("Run as administrator"), wxDefaultPosition, wxDefaultSize, EGAV_BUTTON_STYLE_NOBORDER);
	m_BTN_OAPW->SetAuthNeeded(true);
	m_BTN_OAPW->SetFont(EGAV_FONT_MID_DECORATIVE_NORMAL);
	m_SH_BUTTON = new wxBoxSizer(wxHORIZONTAL);
	m_SH_BUTTON->Add(m_BTN_OAPW);
	m_PanelLower->SetSizer(m_SH_BUTTON);

	m_SV_ALL = new wxBoxSizer(wxVERTICAL);
	m_SV_ALL->Add(m_PanelBuyNow, 1, 0, 0);
	m_SV_ALL->Add(m_PanelRemainingDays, 1, 0, 0);
	m_SV_ALL->Add(m_PanelUpper, 3, wxEXPAND | wxALL, 0);
	m_SV_ALL->Add(m_PanelLower, 1, wxALIGN_CENTER, 0);
	m_SV_ALL->AddSpacer(10);
	m_PanelAll->SetSizer(m_SV_ALL);

	SetBackgroundColour(EGAV_WINDOW_BACK_COLOR1);

	Center();

	//Refresh this window
	Refresh();

	m_taskBarIcon = new MyTaskBarIcon();

	// we should be able to show up to 128 characters on recent Windows versions
	// (and 64 on Win9x)

	if (wxFile::Exists(dbFile1))
		AutoUpdateVSD();
	else
		AutoUpdateVSD1();
	syswinevent = new WinEventTestFrame(wxEmptyString, wxDefaultPosition, wxDefaultSize);
	syswinevent->Show(false);
	TimerScbMsg->Start();
}

HomeTrayWin::~HomeTrayWin()
{
	delete m_taskBarIcon;
	m_taskBarIcon = (MyTaskBarIcon*)(NULL);
#if defined(__WXCOCOA__)
	delete m_dockIcon;
	m_taskBarIcon = (MyTaskBarIcon*)(NULL);
#endif
}

void HomeTrayWin::OnMenuUpdateVSD(wxCommandEvent& event)
{
	int chqnet = CheckInternet();
	if (chqnet)
	{
		AVShowMsg(EGAV_TITLE_NAME, wxT("Please, check your internet\nconnection and try again."));
	}
	else
	{
		//check if virus defination updated
		//if no
		// update  virus defination sig
		// run egavupdate.exe for update

		ExecuteExeUACSilentW(L"egavupdate.exe", L"", workingDir);
		SetUpdateStatus(wxT("UP-TO-DATE"), 1);
	}
}

void HomeTrayWin::OnMenuUpdateSoftware(wxCommandEvent& event)
{
	DownloadSoftwareUpdate();
}

bool HomeTrayWin::GetUpdateStatus()
{
	return 0;
}

void HomeTrayWin::SetUpdateStatus(const wxString& Status, BOOL bUpdate)
{
	switch (bUpdate)
	{
	case TRUE:
		UpdatedTrayIcon(AppIconU, IconStrU, BaloonStrU);
		m_SB_IMAGE_STATUS->SetBitmap(m_BM_EGAVU);
		m_ST_PC_STATUS_RESULT->SetLabelText(Status);
		m_ST_PC_STATUS_RESULT->SetForegroundColour(*wxGREEN);
		break;
	case FALSE:
		UpdatedTrayIcon(AppIconN, IconStrN, BaloonStrN);
		m_SB_IMAGE_STATUS->SetBitmap(m_BM_EGAVN);
		m_ST_PC_STATUS_RESULT->SetLabelText(Status);
		m_ST_PC_STATUS_RESULT->SetForegroundColour(*wxYELLOW);
		break;
	default:
		UpdatedTrayIcon(AppIconC, IconStrC, BaloonStrC);
		m_SB_IMAGE_STATUS->SetBitmap(m_BM_EGAVC);
		m_ST_PC_STATUS_RESULT->SetLabelText(Status);
		m_ST_PC_STATUS_RESULT->SetForegroundColour(*wxRED);
		break;
	}
}

void HomeTrayWin::OnMenuDocHelp(wxCommandEvent& event)
{
	/*
	wxString urlstr = wxT("http://av.eg1.in/egcnav.aspx");
	wxExecute(wxT("explorer ") + urlstr, wxEXEC_HIDE_CONSOLE);
	//wxLaunchDefaultBrowser(urlstr, wxBROWSER_NOBUSYCURSOR);
	*/
}

void HomeTrayWin::OnMenuAnyFeedback(wxCommandEvent& event)
{
	wxString urlstr = wxT("http://av.eg1.in/contact.html");
	wxExecute(wxT("explorer ") + urlstr, wxEXEC_HIDE_CONSOLE);
	//wxLaunchDefaultBrowser(urlstr, wxBROWSER_NOBUSYCURSOR);
}

static void InitAboutInfoMinimal(wxAboutDialogInfo& info)
{
	wxIcon AppIcon(wxIconLocation(workingDir + wxT("\\Resources\\egav.ico")));
	AppIcon.RealizeResource();
	info.SetWebSite(wxT("http://av.eg1.in"));
	info.SetIcon(AppIcon);
	info.SetName(EGAV_TITLE_NAME);
	info.SetVersion(wxT("0.105"));
	info.SetCopyright(wxT("(C) 2022 EG1, All Rights Reserved"));
}


void HomeTrayWin::OnMenuAboutUs(wxCommandEvent& event)
{
	wxAboutDialogInfo info;

	InitAboutInfoMinimal(info);

	wxAboutBox(info, this);
}

void HomeTrayWin::OnClose(wxCloseEvent& event)
{
	if (gHomeTray)
		gHomeTray->Show(false);
}

void HomeTrayWin::OnButtonOAPW(wxCommandEvent& event)
{
	if (IsProcessRunningW(L"EGAntiVirus.exe"))
	{
		AVShowMsg(EGAV_TITLE_NAME, wxT("An instance is already running..."));
	}
	else
	{
		gHomeTray->Show(false);
		ExecuteExeUACSilentW(L"EGAntiVirus.exe", L"", workingDir);
	}
}

void HomeTrayWin::CleanExit()
{
	if (syswinevent)
	{
		syswinevent->Close(true);
		syswinevent = NULL;
	}
	TimerScbMsg->Stop();
	
	gHomeTray->Destroy();
}

void HomeTrayWin::OnTimerScbMsg(wxTimerEvent& event)
{
#ifdef NDEBUG
	if ((!IsServiceRunningW(L"WinSEGAV AutoConfig")) || (syswinevent->m_bShutdownOrLogoffEvent))
	{
		CleanExit();
	}
	else
	{
#endif

#ifdef NDEBUG
	}
#endif
}

void HomeTrayWin::UpdatedTrayIcon(const wxIcon& icon, const wxString& IconString, const wxString& BaloonString)
{
	if (!m_taskBarIcon->SetIcon(icon, IconString))
	{
		//wxLogError(wxT("Could not set icon."));
		ErrorAppTrayLogW(L"Could not set tray icon.");
	}

#if defined(__WXOSX__) && wxOSX_USE_COCOA
	m_dockIcon = new MyTaskBarIcon(wxTBI_DOCK);
	if (!m_dockIcon->SetIcon(wxICON(sample)))
	{
		wxLogError(wxT("Could not set icon."));
	}
#endif

	//Showing ballon
	////////////////////////////////////
	 {
#if defined(__WXMSW__) && wxUSE_TASKBARICON_BALLOONS

		 m_taskBarIcon->ShowBalloon(EGAV_TITLE_NAME, BaloonString, 15000, wxICON_INFORMATION);
#else // !__WXMSW__
		 wxMessageBox(message, title, wxICON_INFORMATION | wxOK, this);
#endif // __WXMSW__/!__WXMSW__
	 }
	 ////////////////////////////////////
}

wxBEGIN_EVENT_TABLE(MyTaskBarIcon, wxTaskBarIcon)
EVT_MENU(PU_OPEN_WINDOW, MyTaskBarIcon::OnMenuOpenWindow)
EVT_MENU(PU_DOWNLOAD_UPDATES, MyTaskBarIcon::OnMenuDownloadUpdates)
EVT_MENU(PU_VISIT_WEBSITE, MyTaskBarIcon::OnMenuVisitWebSite)
EVT_MENU(PU_EXIT_ALL, MyTaskBarIcon::OnMenuExitAll)
EVT_TASKBAR_LEFT_DCLICK(MyTaskBarIcon::OnLeftButtonDClick)
wxEND_EVENT_TABLE()


void MyTaskBarIcon::OnMenuOpenWindow(wxCommandEvent&)
{
	gHomeTray->Show(true);
	gHomeTray->SetFocus();
}

void MyTaskBarIcon::OnMenuDownloadUpdates(wxCommandEvent&)
{
	DownloadSoftwareUpdate();
}

void MyTaskBarIcon::OnMenuVisitWebSite(wxCommandEvent&)
{
	wxString urlstr = wxT("http://av.eg1.in");
	wxExecute(wxT("explorer ") + urlstr, wxEXEC_HIDE_CONSOLE);
	//wxLaunchDefaultBrowser(urlstr, wxBROWSER_NOBUSYCURSOR);
}

void MyTaskBarIcon::OnMenuExitAll(wxCommandEvent& event)
{
#ifdef NDEBUG
		SendMsgToService(SERVICE_CONTROL_CUSTOM_MSG_STOP_SERVICE);
		wxMilliSleep(100);
#else
		gHomeTray->CleanExit();
#endif
}


// Overridables
wxMenu *MyTaskBarIcon::CreatePopupMenu()
{
	wxMenu *menu = new wxMenu;
	menu->Append(PU_OPEN_WINDOW, wxT("&Open Window"));
	//menu->Append(PU_DOWNLOAD_UPDATES, wxT("&Download Updates"));
	menu->Append(PU_VISIT_WEBSITE, wxT("&Visit Website"));
	menu->Append(PU_EXIT_ALL, wxT("&Exit All"));

	return menu;
}

void MyTaskBarIcon::OnLeftButtonDClick(wxTaskBarIconEvent&)
{
	gHomeTray->Show(true);
	gHomeTray->SetFocus();
}

void HomeTrayWin::OnPaint(wxPaintEvent& event)
{

}


//return the number of days between now and from it is updated
int CheckVirusSigDatabase()
{
	wxString lmtime, lmtime1;
	gxGetFileLastModifiedTime(workingDir + wxT("\\db\\main.cld"), lmtime, false);
	if (lmtime == wxT(""))
	{
		gxGetFileLastModifiedTime(workingDir + wxT("\\db\\main.cvd"), lmtime, false);
		if (lmtime == wxT(""))
			return -1;
	}

	gxGetFileLastModifiedTime(workingDir + wxT("\\db\\daily.cld"), lmtime1, false);
	if (lmtime1 == wxT(""))
	{
		gxGetFileLastModifiedTime(workingDir + wxT("\\db\\daily.cvd"), lmtime1, false);
		if (lmtime1 == wxT(""))
			return -1;
	}

	OnlyDateStruct lmtimeods, lmtimeods1;
	ParseDateInOnlyDateStruct(lmtime, lmtimeods);
	ParseDateInOnlyDateStruct(lmtime1, lmtimeods1);

	int days1 = GetNumberOfDaysFromToday(lmtimeods.m_day, lmtimeods.m_month, lmtimeods.m_year);
	int days2 = GetNumberOfDaysFromToday(lmtimeods1.m_day, lmtimeods1.m_month, lmtimeods1.m_year);

	int ret;
	if (days1 <= days2)
	{
		ret = days1;
	}
	else
	{
		ret = days2;
	}

	return ret;
}

void DownloadSoftwareUpdate()
{
	/*
	int chqnet = CheckInternet();
	if (chqnet)
	{
		AVShowMsg(EGAV_TITLE_NAME, wxT("Please, check your internet\nconnection and try again."));
	}
	else
	{
#ifdef __WIN64__
		wxString urlstr = wxT("https://drive.google.com/uc?export=download&id=1NeHpJEoNwPN9B6O0J-ZI7eF2G2B2QDrE");
#else
		wxString urlstr = wxT("https://drive.google.com/uc?export=download&id=1fgyOIp0thaxLkFsRThjvbNEgAHnIYreY");
#endif

		//wxLaunchDefaultBrowser(urlstr);
		wxExecute(wxT("explorer ") + urlstr, wxEXEC_HIDE_CONSOLE);
	}
	*/
}

void HomeTrayWin::AutoUpdateVSD()
{
	BOOL bServiceOnOff = GetEGAVDBForLiveShieldOnOff();
	if (bServiceOnOff == FALSE)
	{
		SetUpdateStatus(wxT("REAL-TIME PROTECTION IS OFF"), -1);
		return;
	}

	int vsdolddays = CheckVirusSigDatabase();

	bool bIsVSDFileUpdated = ((vsdolddays >= 0) && (vsdolddays <= DAYS_LIMIT_TO_CHECK_VSD));
	BOOL bAutoUpdate = GetEGAVDBForAutoUpdateOnOff();
	int chknet = CheckInternet();

	if (!bAutoUpdate)
	{
		if (!bIsVSDFileUpdated)
		{
			SetUpdateStatus(wxT("ANTIVIRUS IS NOT UPDATED"), 0);
			return;
		}
		else
		{
			SetUpdateStatus(wxT("UP-TO-DATE"), 1);
			return;
		}
	}
	else
	{
		if (chknet)
		{
			if (!bIsVSDFileUpdated)
			{
				SetUpdateStatus(wxT("ANTIVIRUS IS NOT UPDATED"), 0);
				return;
			}
			else
			{
				SetUpdateStatus(wxT("UP-TO-DATE"), 1);
				return;
			}
		}
		else
		{
			SetUpdateStatus(wxT("UP-TO-DATE"), 1);
			return;
		}
	}
}

void HomeTrayWin::AutoUpdateVSD1()
{
	int vsdolddays = CheckVirusSigDatabase();

	bool bIsVSDFileUpdated = ((vsdolddays >= 0) && (vsdolddays <= DAYS_LIMIT_TO_CHECK_VSD));
	int chknet = CheckInternet();

	if (chknet)
	{
		if (!bIsVSDFileUpdated)
		{
			SetUpdateStatus(wxT("ANTIVIRUS IS NOT UPDATED"), 0);
			return;
		}
		else
		{
			SetUpdateStatus(wxT("UP-TO-DATE"), 1);
			return;
		}
	}
	else
	{
		SetUpdateStatus(wxT("UP-TO-DATE"), 1);
		return;
	}
}
