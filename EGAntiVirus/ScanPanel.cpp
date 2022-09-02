#include "common.h"
#include "gxfunctions.h"
#include "ScanPanel.h"
#include "avtheme.h"
#include <wx/cshelp.h>
#include "avmsgebox.h"
#include "avdb.h"
#include "scanner.h"

extern wxString workingDir;
extern wxArrayString gArrStrDirCtrlSelectedPaths;

ScanPanel::ScanPanel(const wxString& title, wxWindow* parent) :wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(500, 100))
{
	this->SetBackgroundColour(EGAV_PANEL_BACK_COLOR);
	Connect(ID_BITMAP_BUTTON_CUSTOM_SCAN, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ScanPanel::OnBitmapButtonCstmScan));
	Connect(ID_BITMAP_BUTTON_QUICK_SCAN, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ScanPanel::OnBitmapButtonQuikScan));
	Connect(ID_BITMAP_BUTTON_FULL_SCAN, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ScanPanel::OnBitmapButtonFullScan));

	wxLogNull logNo;

	m_p1 = new wxPanel(this);
	m_p2 = new wxPanel(this);
	m_p3 = new wxPanel(this);

	m_st_cstm_scan = new wxStaticText(m_p1, -1, wxT("Custom Scan"));
	m_st_quik_scan = new wxStaticText(m_p2, -1, wxT("Quick Scan"));
	m_st_full_scan = new wxStaticText(m_p3, -1, wxT("Full Scan"));

	m_st_cstm_scan->SetFont(EGAV_FONT_MID_DECORATIVE_NORMAL);
	m_st_cstm_scan->SetForegroundColour(EGAV_TEXT_COLOR_WHITE);
	m_st_quik_scan->SetFont(EGAV_FONT_MID_DECORATIVE_NORMAL);
	m_st_quik_scan->SetForegroundColour(EGAV_TEXT_COLOR_WHITE);
	m_st_full_scan->SetFont(EGAV_FONT_MID_DECORATIVE_NORMAL);
	m_st_full_scan->SetForegroundColour(EGAV_TEXT_COLOR_WHITE);


	wxImage::AddHandler(new wxPNGHandler);
	wxBitmap cscan(workingDir + wxT("\\Resources\\btnimg\\cscan.png"), wxBITMAP_TYPE_PNG);
	wxBitmap qscan(workingDir + wxT("\\Resources\\btnimg\\qscan.png"), wxBITMAP_TYPE_PNG);
	wxBitmap fscan(workingDir + wxT("\\Resources\\btnimg\\fscan.png"), wxBITMAP_TYPE_PNG);

	m_bb_cscan = new wxBitmapButton(m_p1, ID_BITMAP_BUTTON_CUSTOM_SCAN, cscan, wxDefaultPosition, wxDefaultSize, EGAV_BUTTON_STYLE_NOBORDER);
	m_bb_qscan = new wxBitmapButton(m_p2, ID_BITMAP_BUTTON_QUICK_SCAN, qscan, wxDefaultPosition, wxDefaultSize, EGAV_BUTTON_STYLE_NOBORDER);
	m_bb_fscan = new wxBitmapButton(m_p3, ID_BITMAP_BUTTON_FULL_SCAN, fscan, wxDefaultPosition, wxDefaultSize, EGAV_BUTTON_STYLE_NOBORDER);

	m_bb_cscan->SetBackgroundColour(EGAV_PANEL_BACK_COLOR);
	m_bb_cscan->SetCursor(wxCursor(wxCURSOR_HAND));
	m_bb_qscan->SetBackgroundColour(EGAV_PANEL_BACK_COLOR);
	m_bb_qscan->SetCursor(wxCursor(wxCURSOR_HAND));
	m_bb_fscan->SetBackgroundColour(EGAV_PANEL_BACK_COLOR);
	m_bb_fscan->SetCursor(wxCursor(wxCURSOR_HAND));


	m_sv1 = new wxBoxSizer(wxVERTICAL);
	m_sv1->AddSpacer(20);
	m_sv1->Add(m_bb_cscan, 5, wxALIGN_CENTER, 5);
	m_sv1->Add(m_st_cstm_scan, 5, wxALIGN_CENTER, 5);
	m_sv1->AddSpacer(20);
	m_p1->SetSizer(m_sv1);

	m_sv2 = new wxBoxSizer(wxVERTICAL);
	m_sv2->AddSpacer(20);
	m_sv2->Add(m_bb_qscan, 5, wxALIGN_CENTER, 5);
	m_sv2->Add(m_st_quik_scan, 5, wxALIGN_CENTER, 5);
	m_sv2->AddSpacer(20);
	m_p2->SetSizer(m_sv2);

	m_sv3 = new wxBoxSizer(wxVERTICAL);
	m_sv3->AddSpacer(20);
	m_sv3->Add(m_bb_fscan, 5, wxALIGN_CENTER, 5);
	m_sv3->Add(m_st_full_scan, 5, wxALIGN_CENTER, 5);
	m_sv3->AddSpacer(20);
	m_p3->SetSizer(m_sv3);

	m_sh = new wxBoxSizer(wxHORIZONTAL);
	m_sh->AddStretchSpacer();
	m_sh->Add(m_p1);
	m_sh->AddStretchSpacer();
	m_sh->Add(m_p2);
	m_sh->AddStretchSpacer();
	m_sh->Add(m_p3);
	m_sh->AddStretchSpacer();

	this->SetSizer(m_sh);
}

void ScanPanel::OnBitmapButtonCstmScan(wxCommandEvent& event)
{
	wxString filesForScanning = wxT("");
	size_t countItems = gArrStrDirCtrlSelectedPaths.GetCount();
	if (countItems == 0)
	{
		AVShowMsg(EGAV_ADMIN_PANEL_TITLE, wxT("Any file/folder is not selected."));
		return;
	}

	for (size_t i = 0; i < countItems; i++)
	{
		if (gArrStrDirCtrlSelectedPaths[i].EndsWith("\\"))
			gArrStrDirCtrlSelectedPaths[i] += wxT("\\");
		filesForScanning += wxT("\"") + gArrStrDirCtrlSelectedPaths[i] + wxT("\" ");
	}
	gArrStrDirCtrlSelectedPaths.Clear();
	MakeDefaultAVDBIfNotExist();
	ScannerWin* customScan = new ScannerWin(EGAV_ADMIN_PANEL_TITLE, wxT("Custom Scan"), filesForScanning);
	customScan->Show();
}

void ScanPanel::OnBitmapButtonQuikScan(wxCommandEvent& event)
{
	MakeDefaultAVDBIfNotExist();
	wxString fileforscan = wxEmptyString;
	/*std::wstring SysPath, Sysx86Path, RoamingAppData, LocalAppData, AppDataPath;
	GetSystemFolderPath(SysPath);
	GetSystemx86FolderPath(Sysx86Path);
	GetRoamingAppDataFolderPath(RoamingAppData);
	GetLocalAppDataFolderPath(LocalAppData);
	GetAppDataFolderPath(AppDataPath);
	
	if (SysPath != Sysx86Path)
	{
		fileforscan = wxT("\"") +
			wxString(AppDataPath) + wxT("\" \"") +
			wxString(LocalAppData) + wxT("\" \"") +
			wxString(RoamingAppData) + wxT("\" \"") +
			wxString(SysPath) + wxT("\" \"") +
			wxString(Sysx86Path) + wxT("\"");
	}
	else
	{
		fileforscan = wxT("\"") +
			wxString(AppDataPath) + wxT("\" \"") +
			wxString(LocalAppData) + wxT("\" \"") +
			wxString(RoamingAppData) + wxT("\" \"") +
			wxString(SysPath) + wxT("\"");
	}*/

	ScannerWin* quickScan = new ScannerWin(EGAV_ADMIN_PANEL_TITLE, wxT("Quick Scan"), fileforscan);
	quickScan->Show();
}

void ScanPanel::OnBitmapButtonFullScan(wxCommandEvent& event)
{
	MakeDefaultAVDBIfNotExist();
	wxString fileforscan;
	GetListOfAllDrivesString(fileforscan);
	ScannerWin* fullScan = new ScannerWin(EGAV_ADMIN_PANEL_TITLE, wxT("Full Scan"), fileforscan);
	fullScan->Show();
}