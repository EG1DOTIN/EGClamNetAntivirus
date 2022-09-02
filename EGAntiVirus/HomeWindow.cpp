#include "HomeWindow.h"
#include "common.h"
#include "avmsgebox.h"
#include "gxfunctions.h"
#include <wx/process.h>
#include <wx/utils.h> 
#include "settings.h"
#include <wx/textfile.h>
#include "avdb.h"
#include "cleaner.h"
#include "WaitForStart.h"
#include "egavicon.h"
#include "helpDoc.h"

extern wxString workingDir;
extern bool gbMsgResponse;

wxString password;
wxArrayString gArrStrDirCtrlSelectedPaths;


bool gbIsPasswordRight = false;
bool gbIsCleanerCancelled = false;
bool gbScanSwitch = false;

BEGIN_EVENT_TABLE(AVHomeWindow, wxDialog)
EVT_CLOSE(AVHomeWindow::OnClose)
END_EVENT_TABLE()

AVHomeWindow::AVHomeWindow(const wxString& winTitle)
: wxDialog(NULL, -1, winTitle, wxDefaultPosition, wxSize(820, 600), EGAV_HOME_WINDOW_BORDER_STYLE)
{
	SetEGAVDialogIcon(this);
	// Connecting Events On Buttons
	Connect(ID_BTN_SCAN, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(AVHomeWindow::OnBtnScan));
	Connect(ID_BTN_SETTINGS, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(AVHomeWindow::OnBtnSettings));
	Connect(ID_BTN_CLEANER, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(AVHomeWindow::OnBtnCleaner));
	Connect(ID_BTN_HELP, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(AVHomeWindow::OnBtnHelp));
	Connect(ID_TIMER_HOME, wxEVT_TIMER, wxTimerEventHandler(AVHomeWindow::OnTimerHome));

	Connect(ID_BUTTON_FACEBOOK, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(AVHomeWindow::OnBunttonFacebook));
	Connect(ID_BUTTON_TWITTER, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(AVHomeWindow::OnBunttonTwitter));
	//Connect(ID_BUTTON_GOOGLE, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(AVHomeWindow::OnBunttonGoogle));
	Connect(ID_BUTTON_INSTAGRAM, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(AVHomeWindow::OnBunttonInstagram));


	// Defining Panels
	m_avPanel0 = new wxPanel(this);
	m_avPanel1 = new wxPanel(m_avPanel0);

	m_avPanel34 = new wxPanel(m_avPanel1);
	m_avPanel56 = new wxPanel(m_avPanel0);

	m_avPanel2 = new wxPanel(m_avPanel1);
	m_avPanel3 = new wxPanel(m_avPanel34);
	m_avPanel4 = new wxPanel(m_avPanel34);
	m_avPanel5 = new wxPanel(m_avPanel56);
	m_avPanel6 = new wxPanel(m_avPanel56);

	m_TimerHome = new wxTimer(this, ID_TIMER_HOME);
	
	wxImage::AddHandler(new wxPNGHandler);
	m_bmpScan= wxBitmap(workingDir + wxT("\\Resources\\btnimg\\scan.png"), wxBITMAP_TYPE_PNG);
	m_bmpSettings = wxBitmap(workingDir + wxT("\\Resources\\btnimg\\settings.png"), wxBITMAP_TYPE_PNG);
	m_bmpCleaner = wxBitmap(workingDir + wxT("\\Resources\\btnimg\\cleaner.png"), wxBITMAP_TYPE_PNG);

	m_bmpHelp = wxBitmap(workingDir + wxT("\\Resources\\btnimg\\help.png"), wxBITMAP_TYPE_PNG);

	m_bb_Scan = new wxBitmapButton(m_avPanel5, ID_BTN_SCAN, m_bmpScan, wxDefaultPosition, wxDefaultSize, EGAV_BUTTON_STYLE_NOBORDER);
	m_bb_Scan->SetCursor(wxCursor(wxCURSOR_HAND));
	m_bb_Scan->SetBackgroundColour(EGAV_WINDOW_BACK_COLOR1);

	m_bb_Settings = new wxBitmapButton(m_avPanel5, ID_BTN_SETTINGS, m_bmpSettings, wxDefaultPosition, wxDefaultSize, EGAV_BUTTON_STYLE_NOBORDER);
	m_bb_Settings->SetCursor(wxCursor(wxCURSOR_HAND));
	m_bb_Settings->SetBackgroundColour(EGAV_WINDOW_BACK_COLOR1);

	m_bb_Cleaner = new wxBitmapButton(m_avPanel5, ID_BTN_CLEANER, m_bmpCleaner, wxDefaultPosition, wxDefaultSize, EGAV_BUTTON_STYLE_NOBORDER);
	m_bb_Cleaner->SetCursor(wxCursor(wxCURSOR_HAND));
	m_bb_Cleaner->SetBackgroundColour(EGAV_WINDOW_BACK_COLOR1);

	m_bb_Help = new wxBitmapButton(m_avPanel4, ID_BTN_HELP, m_bmpHelp, wxDefaultPosition, wxDefaultSize, EGAV_BUTTON_STYLE_NOBORDER);
	m_bb_Help->SetCursor(wxCursor(wxCURSOR_HAND));
	m_bb_Help->SetBackgroundColour(EGAV_WINDOW_BACK_COLOR1);

	m_st_scan = new wxStaticText(m_avPanel5, -1, wxT("Scan"));
	m_st_settings = new wxStaticText(m_avPanel5, -1, wxT("Settings"));
	m_st_cleaner = new wxStaticText(m_avPanel5, -1, wxT("Cleaner"));

	m_st_scan->SetFont(EGAV_FONT_MID_DECORATIVE_NORMAL);
	m_st_scan->SetForegroundColour(EGAV_TEXT_COLOR_WHITE);

	m_st_settings->SetFont(EGAV_FONT_MID_DECORATIVE_NORMAL);
	m_st_settings->SetForegroundColour(EGAV_TEXT_COLOR_WHITE);

	m_st_cleaner->SetFont(EGAV_FONT_MID_DECORATIVE_NORMAL);
	m_st_cleaner->SetForegroundColour(EGAV_TEXT_COLOR_WHITE);

	// Defining Sizer and adding buttons
	//Upper Panel

	m_avClogo = new wxClickText(m_avPanel2, ID_EG_LOGO, wxT("EG"));
	m_avClogo->SetCursor(wxCursor(wxCURSOR_HAND));
	m_avQuarantineText = new wxClickText(m_avPanel3, ID_QUARANTINETEXT, wxT("Quarantine Folder"), wxPoint(187, 0));
	m_avQuarantineText->SetCursor(wxCursor(wxCURSOR_HAND));
	m_avHeadingText1 = new wxStaticText(m_avPanel2, -1, wxT(" ClamNet"));
	m_avHeadingText2 = new wxStaticText(m_avPanel2, -1, wxT(" AntiVirus"));
	m_avClogo->SetFont(wxFont(21, wxFONTFAMILY_SCRIPT, wxFONTSTYLE_ITALIC, wxFONTWEIGHT_BOLD));
	m_avQuarantineText->SetFont(wxFont(12, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, true));

	m_avClogo->SetForegroundColour(EGAV_FONT_BACK_LIGHT_GOLD_COLOR);
	m_avQuarantineText->SetForegroundColour(EGAV_FONT_BACK_LIGHT_GOLD_COLOR);
	m_avHeadingText1->SetFont(wxFont(20, wxFONTFAMILY_SCRIPT, wxFONTSTYLE_ITALIC, wxFONTWEIGHT_NORMAL));
	m_avHeadingText1->SetForegroundColour(wxColor(119, 175, 250));
	m_avHeadingText2->SetFont(wxFont(20, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
	m_avHeadingText2->SetForegroundColour(wxColor(27, 146, 48));

	m_avSizer1H = new wxBoxSizer(wxHORIZONTAL);

	m_avSizer1H->AddSpacer(20);
	m_avSizer1H->Add(m_avClogo, 1, 0, 0);
	m_avSizer1H->Add(m_avHeadingText1, 0, 0, 0);
	m_avSizer1H->Add(m_avHeadingText2, 0, 0, 0);

	m_avPanel2->SetSizer(m_avSizer1H);

	m_avSizer3H = new wxBoxSizer(wxHORIZONTAL);
	m_avSizer3H->AddSpacer(10);
	m_avSizer3H->AddSpacer(10);
	m_avSizer3H->Add(m_bb_Help, 1, 0, 10);
	m_avSizer3H->AddSpacer(22);
	m_avPanel4->SetSizer(m_avSizer3H);

	m_avSizer4V = new wxBoxSizer(wxVERTICAL);
	m_avSizer4V->Add(m_avPanel3, 1, wxALL, 10);
	m_avSizer4V->Add(m_avPanel4, 1, wxALIGN_RIGHT, 10);

	m_avPanel34->SetSizer(m_avSizer4V);

	m_avSizer5H = new wxBoxSizer(wxHORIZONTAL);
	m_avSizer5H->Add(m_avPanel2, 1, wxEXPAND, 10);
	m_avSizer5H->AddStretchSpacer();
	m_avSizer5H->Add(m_avPanel34, 1, wxEXPAND, 10);

	m_avPanel1->SetSizer(m_avSizer5H);

	m_avSizer6V = new wxBoxSizer(wxVERTICAL);
	m_avSizer6V->AddSpacer(10);
	m_avSizer6V->Add(m_bb_Scan, 1, wxALIGN_CENTER, 1);
	m_avSizer6V->Add(m_st_scan, 1, wxALIGN_CENTER, 1);
	m_avSizer6V->AddSpacer(10);
	m_avSizer6V->Add(m_bb_Settings, 1, wxALIGN_CENTER, 1);
	m_avSizer6V->Add(m_st_settings, 1, wxALIGN_CENTER, 1);
	m_avSizer6V->AddSpacer(10);
	m_avSizer6V->Add(m_bb_Cleaner, 1, wxALIGN_CENTER, 1);
	m_avSizer6V->Add(m_st_cleaner, 1, wxALIGN_CENTER, 1);
	m_avSizer6V->AddSpacer(10);
	m_avSizer6V->AddSpacer(10);
	m_avPanel5->SetSizer(m_avSizer6V);

	m_avDirCtrl = new wxGenericDirCtrl(m_avPanel6, -1, wxDirDialogDefaultFolderStr, wxPoint(-1, -1), wxSize(-1, -1), wxDIRCTRL_3D_INTERNAL | wxDIRCTRL_SHOW_FILTERS | wxDIRCTRL_MULTIPLE);
	dirCtrlTxt0 = new wxStaticText(m_avPanel6, -1, wxT("  Custom Scan"));
	dirCtrlTxt1 = new wxStaticText(m_avPanel6, -1, wxT("    Select A File Or Folder To Scan"));
	dirCtrlTxt0->SetFont(wxFont(22, wxFONTFAMILY_SCRIPT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
	dirCtrlTxt1->SetFont(wxFont(12, wxFONTFAMILY_ROMAN, wxFONTSTYLE_ITALIC, wxFONTWEIGHT_NORMAL));
	dirCtrlTxt0->SetForegroundColour(EGAV_WINDOW_BACK_COLOR2);
	dirCtrlTxt1->SetForegroundColour(EGAV_TEXT_COLOR_WHITE);

	m_avDirCtrl->ShowHidden(true);
	m_avDirCtrl->Connect(wxID_ANY, wxEVT_TREE_ITEM_COLLAPSED, wxTreeEventHandler(AVHomeWindow::OnCollapseDirCtrl), NULL, this);

	m_avSizer7V = new wxBoxSizer(wxVERTICAL);
	m_avSizer7V->AddSpacer(10);
	m_avSizer7V->Add(dirCtrlTxt0, 1, 0, 5);
	m_avSizer7V->Add(dirCtrlTxt1, 0, 0, 5);
	m_avSizer7V->Add(m_avDirCtrl, 8, wxEXPAND, 10);

	m_avPanel6->SetSizer(m_avSizer7V);

	//////////////////////////////////////////////////
	//FTD Panel
	Panel_FTG = new wxPanel(m_avPanel0);
	bmp_facebook = wxBitmap(workingDir + wxT("\\Resources\\facebook.png"), wxBITMAP_TYPE_PNG);
	bmp_twitter = wxBitmap(workingDir + wxT("\\Resources\\twitter.png"), wxBITMAP_TYPE_PNG);
	//bmp_google = wxBitmap(workingDir + wxT("\\Resources\\utube.png"), wxBITMAP_TYPE_PNG);
	bmp_instagram = wxBitmap(workingDir + wxT("\\Resources\\instalogo.png"), wxBITMAP_TYPE_PNG);

	Button_FACEBOOK = new wxBitmapButton(Panel_FTG, ID_BUTTON_FACEBOOK, bmp_facebook, wxDefaultPosition, wxSize(22, 22), EGAV_BUTTON_STYLE_NOBORDER);
	Button_TWITTER = new wxBitmapButton(Panel_FTG, ID_BUTTON_TWITTER, bmp_twitter, wxDefaultPosition, wxSize(22, 22), EGAV_BUTTON_STYLE_NOBORDER);
	//Button_GOOGLE = new wxBitmapButton(Panel_FTG, ID_BUTTON_GOOGLE, bmp_google, wxDefaultPosition, wxSize(22, 22), EGAV_BUTTON_STYLE_NOBORDER);
	Button_INSTAGRAM = new wxBitmapButton(Panel_FTG, ID_BUTTON_INSTAGRAM, bmp_instagram, wxDefaultPosition, wxSize(22, 22), EGAV_BUTTON_STYLE_NOBORDER);


	Button_FACEBOOK->SetCursor(wxCursor(wxCURSOR_HAND));
	Button_TWITTER->SetCursor(wxCursor(wxCURSOR_HAND));
	//Button_GOOGLE->SetCursor(wxCursor(wxCURSOR_HAND));
	Button_INSTAGRAM->SetCursor(wxCursor(wxCURSOR_HAND));

	Button_FACEBOOK->SetBackgroundColour(EGAV_WINDOW_BACK_COLOR1);
	Button_TWITTER->SetBackgroundColour(EGAV_WINDOW_BACK_COLOR1);
	//Button_GOOGLE->SetBackgroundColour(EGAV_WINDOW_BACK_COLOR1);
	Button_INSTAGRAM->SetBackgroundColour(EGAV_WINDOW_BACK_COLOR1);

	SizerH_FTDPanel = new wxBoxSizer(wxHORIZONTAL);
	SizerH_FTDPanel->Add(Button_FACEBOOK, 1, 0, 30);
	SizerH_FTDPanel->AddSpacer(5);
	SizerH_FTDPanel->Add(Button_TWITTER, 1, 0, 30);
	SizerH_FTDPanel->AddSpacer(5);
	//SizerH_FTDPanel->Add(Button_GOOGLE, 1, 0, 30);
	SizerH_FTDPanel->Add(Button_INSTAGRAM, 1, 0, 30);
	SizerH_FTDPanel->AddSpacer(5);
	Panel_FTG->SetSizer(SizerH_FTDPanel);
	//////////////////////////////////////////////////


	// ALL Panel
	m_avSizer8H = new wxBoxSizer(wxHORIZONTAL);
	m_avSizer8H->AddSpacer(20);
	m_avSizer8H->Add(m_avPanel5, 1, wxEXPAND, 1);
	m_avSizer8H->AddSpacer(20);
	m_avSizer8H->Add(m_avPanel6, 8, wxEXPAND, 1);
	m_avSizer8H->AddSpacer(22);

	m_avPanel56->SetSizer(m_avSizer8H);

	m_avSizer9V = new wxBoxSizer(wxVERTICAL);
	m_avSizer9V->AddSpacer(20);
	m_avSizer9V->Add(m_avPanel1);
	m_avSizer9V->AddStretchSpacer();
	m_avSizer9V->Add(m_avPanel56);
	m_avSizer9V->AddSpacer(2);
	m_avSizer9V->Add(Panel_FTG, 0, wxALIGN_RIGHT);
	m_avSizer9V->AddSpacer(2);

	m_avPanel0->SetSizer(m_avSizer9V);

	m_avPanel0->SetBackgroundColour(EGAV_WINDOW_BACK_COLOR1);
	m_avPanel1->SetBackgroundColour(EGAV_WINDOW_BACK_COLOR1);
	m_avPanel5->SetBackgroundColour(EGAV_WINDOW_BACK_COLOR1);
	m_avPanel6->SetBackgroundColour(EGAV_PANEL_BACK_COLOR);

	Center();
	m_TimerHome->Start();
}

void AVHomeWindow::OnBtnSettings(wxCommandEvent& WXUNUSED(btnEvent))
{
	settingsEGAV* sett = new settingsEGAV(EGAV_ADMIN_PANEL_TITLE wxT(": Settings"));
	sett->Show();
	DELETE_POINTER(settingsEGAV, sett);
}

void AVHomeWindow::OnBtnCleaner(wxCommandEvent& WXUNUSED(btnEvent))
{
	WaitForStart* pleaseWait = new WaitForStart(EGAV_ADMIN_PANEL_TITLE wxT(": Cleaner"), wxT("please wait, while scanning fields for cleanning..."));
	pleaseWait->Show();
	DELETE_POINTER(WaitForStart, pleaseWait);

	if (!gbIsCleanerCancelled)
	{
		Cleaner* cleaner = new Cleaner(EGAV_ADMIN_PANEL_TITLE wxT(": Cleaner"));
		cleaner->Show();
		DELETE_POINTER(Cleaner, cleaner);
	}
	else
	{
		gbIsCleanerCancelled = false;
	}
}

void AVHomeWindow::OnBtnHelp(wxCommandEvent& WXUNUSED(btnEvent))
{
	ExecuteCHMDoc();
}

void AVHomeWindow::OnClose(wxCloseEvent& event)
{
	if (m_TimerHome->IsRunning())
		m_TimerHome->Stop();
	event.Skip();
	Destroy();
}

void AVHomeWindow::OnCollapseDirCtrl(wxTreeEvent& event)
{
	wxTreeItemId trid = event.GetItem();
	wxString temp = m_avDirCtrl->GetPath(trid);
	
	m_avDirCtrl->UnselectAll();
	m_avDirCtrl->SelectPath(temp);
}

void AVHomeWindow::OnTimerHome(wxTimerEvent& event)
{
	
}

#include "ScanPopup.h"
void AVHomeWindow::OnBtnScan(wxCommandEvent& btnEvent)
{
	if (gbScanSwitch == false)
	{
		m_avDirCtrl->GetPaths(gArrStrDirCtrlSelectedPaths);
		m_avDirCtrl->UnselectAll();//  Refresh();
		SimpleScanTransientPopup* m_ScanPopup = new SimpleScanTransientPopup(this, false);
		wxWindow *egtext = (wxWindow*)btnEvent.GetEventObject();
		wxPoint pos = egtext->ClientToScreen(wxPoint(0, 0));
		wxSize sz = egtext->GetSize();
		m_ScanPopup->Position(pos, sz);
		m_ScanPopup->Popup();
	}
	else
	{
		AVShowMsg(EGAV_TITLE_NAME, wxT("scan already running..."));
	}
}

void AVHomeWindow::OnBunttonFacebook(wxCommandEvent& event)
{
	wxLaunchDefaultBrowser(wxT("https://www.facebook.com/eg1dotin"), wxBROWSER_NOBUSYCURSOR);
}

void AVHomeWindow::OnBunttonTwitter(wxCommandEvent& event)
{
	wxLaunchDefaultBrowser(wxT("https://twitter.com/eg1dotin"), wxBROWSER_NOBUSYCURSOR);
}

void AVHomeWindow::OnBunttonGoogle(wxCommandEvent& event)
{
	//wxLaunchDefaultBrowser(wxT("https://www.youtube.com/channel/UCozNmrss2AVE7naw4neqHGg"), wxBROWSER_NOBUSYCURSOR);
}

void AVHomeWindow::OnBunttonInstagram(wxCommandEvent& event)
{
	wxLaunchDefaultBrowser(wxT("https://www.instagram.com/eg1dotin"), wxBROWSER_NOBUSYCURSOR);
}

