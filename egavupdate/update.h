#ifndef UPDATE_HEADER
#define UPDATE_HEADER

#include <wx\wx.h>
#include "gxTextCtrlURL.h"

class UpdateVirusDefinition :public wxFrame
{
public:
	UpdateVirusDefinition(const wxString& title);
	void OnBtnstop(wxCommandEvent& WXUNUSED(btnEvent));
	void OnTimerUpdate(wxTimerEvent& tmrEvent);
	void OnExitUpdateWindow();

	wxPanel* m_PanelAll;
	wxPanel* m_PanelHeading;
	wxPanel* m_PanelTextCtrl;
	wxPanel* m_PanelButton;

	wxBoxSizer* m_sV_All;
	wxBoxSizer* m_sH_Heading;
	wxBoxSizer* m_sH_TextCtrl;
	wxBoxSizer* m_sH_Button;

	wxButton* m_Button_Stop;
	gxTextCtrlURL* m_TextCtrl_Update;

	wxStaticText* m_ST_Heading;

	wxTimer* m_TimerUpdate;

	wxProcess* m_processUpdate;
	long m_ProcessUpdateId;
	wxIdleEvent *m_idleU;

private:
	void OnClose(wxCloseEvent& event);
	DECLARE_EVENT_TABLE()
};

enum eWinIDs1
{
	//121 -140
	ID_UPDATE_BUTTON_STOP = 121,
	ID_UPDATE_TIMER,
};

void GetUpdateCmd(wxString& strOut);
void ChangeInUpdateDBFile();

#endif // !SCANNER_HEADER