#ifndef SCANNER_HEADER
#define SCANNER_HEADER

#include <wx\wx.h>
#include <wx\animate.h>
#include "gxButton.h"
#include <wx\textfile.h>

class ScannerWin :public wxFrame
{
public:
	ScannerWin(const wxString& title, const wxString& scanningType, const wxString& filesToScan);
	void OnBtnstop(wxCommandEvent& btnEvent);
	void OnTimerScanner(wxTimerEvent& tmrEvent);
	void OnTimerFastScanner(wxTimerEvent& tmrEvent);
	wxString VirusCountStr(int viruscount);

	wxPanel* m_PanelAll;
	wxPanel* m_PanelTextHeading;
	wxPanel* m_PanelTextProgress;
	wxPanel* m_PanelTextScanning;
	wxPanel* m_PanelTextInfectedFiles;
	wxPanel* m_PanelButton;

	wxBoxSizer* m_SVAll;
	wxBoxSizer* m_SHTextHeading;
	wxBoxSizer* m_SHTextProgress;
	wxBoxSizer* m_SHTextScanning;
	wxBoxSizer* m_SHTextInfectedFiles;
	wxBoxSizer* m_SHButton;


	wxStaticText* m_TextHeading;
	wxStaticText* m_TextScanning;
	wxStaticText* m_TextInfectedFiles;
	wxStaticText* m_TextVirusCount;

	MyWXRoundEdgeButton* m_ButtonStop;

	wxString m_FileForScan;
	wxArrayString m_FilesForScanInArray;
	wxString m_CmdToScan;
	wxString m_ScanType;
	wxString m_LogFileScan;
	wxString m_isFastScan;
	wxString m_ClamdLogsFile;
	wxString m_LastVirusDetected;

	wxAnimationCtrl*    m_animationCtrl;

	wxTimer* m_TimerScanner;
	wxTimer* m_TimerFScanner;

	int m_VirusCount;
	size_t nFiles1, nFiles2;

	wxProcess* m_processScanner;
	wxIdleEvent *m_idleS;

	

private:
	void OnClose(wxCloseEvent& event);
	DECLARE_EVENT_TABLE()
};

wxString gxMakeScanLogTextFileWithNameNow(const wxString& dirPath);

void gxMakeCmdBATFileForQuickNFullScan(const wxString& FileNameWithoutExt, const wxString& cmd1, const wxString& cmd2);
void gxRemoveCmdBATScanFile(const wxString& FileNameWithoutExt);

enum eBTN_ID19
{
	//500-519
	ID_SCANNER_BUTTON = 500, 
	ID_SCANNER_TIMER,
	ID_FSCANNER_TIMER,
};

size_t getTotalInfectedFiles();

#endif // !SCANNER_HEADER
