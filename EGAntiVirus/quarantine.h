#ifndef QUARANTINE_HEADER
#define QUARANTINE_HEADER

#include <wx\wx.h>
#include <wx\listctrl.h>
#include "gxButton.h"


class Quarantine :public wxDialog
{
public:
	Quarantine(const wxString& title);
	void OnButtonDelete(wxCommandEvent& event);
	void OnButtonRestore(wxCommandEvent& event);
	void OnButtonDeleteAll(wxCommandEvent& event);
	void OnButtonCancel(wxCommandEvent& event);
	void OnButtonClearList(wxCommandEvent& event);

	wxPanel* m_PanelAll;
	wxPanel* m_PanelListCtrl;
	wxPanel* m_PanelButton;

	wxBoxSizer* m_SV_All;
	wxBoxSizer* m_SV_ListCtrl;
	wxBoxSizer* m_SH_Button;

	MyWXRoundEdgeButton* m_Btn_Delete;
	MyWXRoundEdgeButton* m_Btn_Restore;
	MyWXRoundEdgeButton* m_Btn_DeleteAll;
	MyWXRoundEdgeButton* m_Btn_Cancel;
	MyWXRoundEdgeButton* m_Btn_ClearList;

	wxListCtrl* m_listctrl;
};

wxString gxGetFileLastEditTimeTime(const wxString& filename);
size_t GetAllFilesInQuarantine(
	const wxString& dirName,					// Full Path of Quarantine Folder
	wxArrayString* resultFile,					// String Array of virus names 
	wxArrayString* resultFileTime,				// String Array of Time when the virus found
	wxArrayString* resultFilePath,				// String Array of Path where the virus found
	wxArrayString* resultAction					// String Array of Action Taken on the virus
	);

enum eBTN_ID18
{
	//470 - 489
	ID_BTN_QUARANTINE_DELETE = 470,
	ID_BTN_QUARANTINE_RESTORE,
	ID_BTN_QUARANTINE_DELETEALL,
	ID_BTN_QUARANTINE_CANCEL,
	ID_BTN_QUARANTINE_CLEAR_LIST,
};

#endif // !1
