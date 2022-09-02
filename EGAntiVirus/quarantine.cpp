#include "avtheme.h"
#include "quarantine.h"
#include <wx\filename.h>
#include <wx\dir.h>
#include "avmsgebox.h"

extern wxString QuaranitineFolder;

Quarantine::Quarantine(const wxString& title) :wxDialog(NULL, -1, title, wxDefaultPosition, wxSize(680, 530), EGAV_DIALOG_BORDER_STYLE)
{
	Connect(ID_BTN_QUARANTINE_DELETE, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(Quarantine::OnButtonDelete));
	Connect(ID_BTN_QUARANTINE_RESTORE, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(Quarantine::OnButtonRestore));
	Connect(ID_BTN_QUARANTINE_DELETEALL, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(Quarantine::OnButtonDeleteAll));
	Connect(ID_BTN_QUARANTINE_CANCEL, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(Quarantine::OnButtonCancel));
	Connect(ID_BTN_QUARANTINE_CLEAR_LIST, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(Quarantine::OnButtonClearList));

	m_PanelAll = new wxPanel(this);
	m_PanelListCtrl = new wxPanel(m_PanelAll);
	m_PanelButton = new wxPanel(m_PanelAll);


	m_SV_ListCtrl = new wxBoxSizer(wxVERTICAL);

	m_listctrl = new wxListCtrl(m_PanelListCtrl, -1, wxDefaultPosition, wxDefaultSize, wxLC_REPORT | wxLC_HRULES | wxLC_VRULES);
	m_listctrl->SetBackgroundColour(EGAV_PANEL_BACK_COLOR);

	m_listctrl->InsertColumn(0, wxT("Date"));
	m_listctrl->InsertColumn(1, wxT("Detected Item"));
	m_listctrl->InsertColumn(2, wxT("Action Taken"));
	m_listctrl->InsertColumn(3, wxT("Location"));

	m_listctrl->Hide();
	wxArrayString virus;
	wxArrayString date;
	wxArrayString location;
	wxArrayString action;
	wxStopWatch sw;

	size_t count = GetAllFilesInQuarantine(QuaranitineFolder, &virus, &date, &location, &action);
	for (size_t i = 0; i < count; i++)
	{
		long tmp = m_listctrl->InsertItem(i, date[i]);
		m_listctrl->SetItem(tmp, 1, virus[i]);
		m_listctrl->SetItem(tmp, 2, action[i]);
		m_listctrl->SetItem(tmp, 3, location[i]);

		if (action[i] == wxT("Action Not Taken"))
		{
			m_listctrl->SetItemTextColour(i, wxColor(255, 128, 128));
		}
		else if (action[i] == wxT("Deleted"))
		{
			m_listctrl->SetItemTextColour(i, *wxGREEN);
		}
		else if (action[i] == wxT("Restored"))
		{
			m_listctrl->SetItemTextColour(i, *wxYELLOW);
		}
	}

	//m_listctrl->SetTextColour(wxColor(255, 128, 128));;

	m_listctrl->SetColumnWidth(0, wxLIST_AUTOSIZE_USEHEADER);
	m_listctrl->SetColumnWidth(1, wxLIST_AUTOSIZE_USEHEADER);
	m_listctrl->SetColumnWidth(2, wxLIST_AUTOSIZE_USEHEADER);
	m_listctrl->SetColumnWidth(3, wxLIST_AUTOSIZE_USEHEADER);
	m_listctrl->Show();

	m_SV_ListCtrl->Add(m_listctrl, 1, wxEXPAND | wxALL, 0);
	m_PanelListCtrl->SetSizer(m_SV_ListCtrl);

	m_Btn_Delete = new MyWXRoundEdgeButton(m_PanelButton, ID_BTN_QUARANTINE_DELETE, wxT("Delete"), wxDefaultPosition, wxSize(100, 25), EGAV_BUTTON_STYLE_NOBORDER);
	m_Btn_Restore = new MyWXRoundEdgeButton(m_PanelButton, ID_BTN_QUARANTINE_RESTORE, wxT("Restore"), wxDefaultPosition, wxSize(100, 25), EGAV_BUTTON_STYLE_NOBORDER);
	m_Btn_DeleteAll = new MyWXRoundEdgeButton(m_PanelButton, ID_BTN_QUARANTINE_DELETEALL, wxT("Delete All"), wxDefaultPosition, wxSize(100, 25), EGAV_BUTTON_STYLE_NOBORDER);
	m_Btn_ClearList = new MyWXRoundEdgeButton(m_PanelButton, ID_BTN_QUARANTINE_CLEAR_LIST, wxT("Clear List"), wxDefaultPosition, wxSize(100, 25), EGAV_BUTTON_STYLE_NOBORDER);
	m_Btn_Cancel = new MyWXRoundEdgeButton(m_PanelButton, ID_BTN_QUARANTINE_CANCEL, wxT("Exit"), wxDefaultPosition, wxSize(100, 25), EGAV_BUTTON_STYLE_NOBORDER);

	m_SH_Button = new wxBoxSizer(wxHORIZONTAL);
	m_SH_Button->AddStretchSpacer();
	m_SH_Button->Add(m_Btn_Delete);
	m_SH_Button->AddStretchSpacer();
	m_SH_Button->Add(m_Btn_Restore);
	m_SH_Button->AddStretchSpacer();
	m_SH_Button->Add(m_Btn_DeleteAll);
	m_SH_Button->AddStretchSpacer();
	m_SH_Button->Add(m_Btn_ClearList);
	m_SH_Button->AddStretchSpacer();
	m_SH_Button->Add(m_Btn_Cancel);
	m_SH_Button->AddStretchSpacer();
	m_PanelButton->SetSizer(m_SH_Button);

	m_SV_All = new wxBoxSizer(wxVERTICAL);
	m_SV_All->Add(m_PanelListCtrl, 5, wxEXPAND | wxALL, 30);
	m_SV_All->Add(m_PanelButton, 1, wxEXPAND | wxALL, 10);
	m_PanelAll->SetSizer(m_SV_All);

	SetBackgroundColour(EGAV_WINDOW_BACK_COLOR1);

	Centre();
	ShowModal();
	//	Destroy();
}

wxString gxGetFileLastEditTimeTime(const wxString& filename)
{
	wxString res;
	if (!wxFileExists(filename))
		return res;

	wxStructStat structStat;
	wxStat(filename, &structStat);

	wxDateTime fileMtime = structStat.st_mtime;

	res = fileMtime.Format(wxT("%d-%b-%Y at %H:%M:S"));

	return res;
}

size_t GetAllFilesInQuarantine(
	const wxString& dirName,					// Full Path of Quarantine Folder
	wxArrayString* resultFile,					// String Array of virus names 
	wxArrayString* resultFileTime,				// String Array of Time when the virus found
	wxArrayString* resultFilePath,				// String Array of Path where the virus found
	wxArrayString* resultAction					// String Array of Action Taken on the virus
	)
{
	wxString fileSpec = wxT("*.txt");
	wxArrayString files;
	wxBusyCursor busyCursor;

	if (wxDir::GetAllFiles(dirName, &files, fileSpec) > 0)
	{
		wxString name, ext;
		wxString allFileNames;
		for (size_t i = 0; i < files.GetCount(); i++)
		{

			wxFileName::SplitPath(files[i], NULL, &name, &ext);

			allFileNames = name + wxT(".") + ext;

			wxString filedata;
			wxFile ftxt(files[i]);
			ftxt.ReadAll(&filedata);
			ftxt.Close();
			filedata = filedata.BeforeFirst('\t');
			filedata = filedata.substr(4);
			resultFilePath->Add(filedata);

			resultFile->Add(name.BeforeLast('.'));
			resultFileTime->Add(gxGetFileLastEditTimeTime(files[i]));

			wxFileName tempfn(files[i]);
			tempfn.ClearExt();
			wxString filenameV = tempfn.GetFullPath();

			if (wxFile::Exists(filenameV))
			{
				resultAction->Add(wxT("Action Not Taken"));
			}
			else if (wxFile::Exists(filedata))
			{
				resultAction->Add(wxT("Restored"));
			}
			else
			{
				resultAction->Add(wxT("Deleted"));
			}
		}
	}
	return files.GetCount();
}

void Quarantine::OnButtonDelete(wxCommandEvent& event)
{
	// if no files do nothing
	if (m_listctrl->GetItemCount() == 0)
	{
		return;
	}

	// if 1 file delete it even if not selected!
	if (m_listctrl->GetItemCount() == 1)
	{
		if (m_listctrl->GetItemText(0, 2) == wxT("Action Not Taken"))
		{
			//wxMessageBox(_T("one"),_T("one"));
			wxString itemname = m_listctrl->GetItemText(0, 1);

			if (wxRemoveFile(QuaranitineFolder + wxT("\\") + itemname + wxT(".infected")))
			{
				m_listctrl->SetItem(0, 2, wxT("Deleted"));
				m_listctrl->SetItemTextColour(0, *wxGREEN);
			}
		}
		return;
	}

	// listctrl delete all selected ----------------
#if 1 //if windows
	int total_items = m_listctrl->GetItemCount();
	int i = 0;

	while (total_items > i)
	{
		int index = (total_items - 1) - i;

		if (m_listctrl->GetItemState(index, wxLIST_STATE_SELECTED) == wxLIST_STATE_SELECTED)
		{
			if (m_listctrl->GetItemText(index, 2) == wxT("Action Not Taken"))
			{
				wxString itemname = m_listctrl->GetItemText(index, 1);

				if (wxRemoveFile(QuaranitineFolder + wxT("\\") + itemname + wxT(".infected")))
				{
					m_listctrl->SetItem(index, 2, wxT("Deleted"));
					m_listctrl->SetItemTextColour(index, *wxGREEN);
				}
			}
		}
		i++;
	}
#endif
	// -----------------------------------------
}

extern bool gbMsgResponse;
void Quarantine::OnButtonRestore(wxCommandEvent& event)
{
	// if no files do nothing
	if (m_listctrl->GetItemCount() == 0)
	{
		return;
	}

	AVShowMsgYN(EGAV_ADMIN_PANEL_TITLE, wxT("It may be a risk to restore infected files. \nDo you really want to restore ?"));
	if (gbMsgResponse)
	{
		gbMsgResponse = false;

		// if 1 file delete it even if not selected!
		if (m_listctrl->GetItemCount() == 1)
		{
			if (m_listctrl->GetItemText(0, 2) == wxT("Action Not Taken"))
			{
				wxString itemname = m_listctrl->GetItemText(0, 1);

				if (wxRenameFile(QuaranitineFolder + wxT("\\") + itemname + wxT(".infected"), m_listctrl->GetItemText(0, 3)))
				{
					m_listctrl->SetItem(0, 2, wxT("Restored"));
					m_listctrl->SetItemTextColour(0, *wxYELLOW);
				}
			}
			return;
		}

		// listctrl delete all selected ----------------
#if 1 //if windows
		int total_items = m_listctrl->GetItemCount();
		int i = 0;

		while (total_items > i)
		{
			int index = (total_items - 1) - i;
			if (m_listctrl->GetItemState(index, wxLIST_STATE_SELECTED) == wxLIST_STATE_SELECTED)
			{
				if (m_listctrl->GetItemText(index, 2) == wxT("Action Not Taken"))
				{
					wxString itemname = m_listctrl->GetItemText(index, 1);

					if (wxRenameFile(QuaranitineFolder + wxT("\\") + itemname + wxT(".infected"), m_listctrl->GetItemText(index, 3)))
					{
						m_listctrl->SetItem(index, 2, wxT("Restored"));
						m_listctrl->SetItemTextColour(index, *wxYELLOW);
					}
				}
			}
			i++;
		}
#endif
		// -----------------------------------------
	}
}

void Quarantine::OnButtonDeleteAll(wxCommandEvent& event)
{
	// if no files do nothing
	if (m_listctrl->GetItemCount() == 0)
	{
		return;
	}

	// if 1 file delete it even if not selected!
	if (m_listctrl->GetItemCount() == 1)
	{
		if (m_listctrl->GetItemText(0, 2) == wxT("Action Not Taken"))
		{
			wxString itemname = m_listctrl->GetItemText(0, 1);

			if (wxRemoveFile(QuaranitineFolder + wxT("\\") + itemname + wxT(".infected")))
			{
				m_listctrl->SetItem(0, 2, wxT("Deleted"));
				m_listctrl->SetItemTextColour(0, *wxGREEN);
			}
		}
		return;
	}

	// listctrl delete all selected ----------------
#if 1 //if windows
	int total_items = m_listctrl->GetItemCount();
	int i = 0;

	while (total_items > i)
	{
		int index = (total_items - 1) - i;
		if (m_listctrl->GetItemText(index, 2) == wxT("Action Not Taken"))
		{
			wxString itemname = m_listctrl->GetItemText(index, 1);

			if (wxRemoveFile(QuaranitineFolder + wxT("\\") + itemname + wxT(".infected")))
			{
				m_listctrl->SetItem(index, 2, wxT("Deleted"));
				m_listctrl->SetItemTextColour(index, *wxGREEN);
			}
		}
		i++;
	}
#endif
	// -----------------------------------------
}

void Quarantine::OnButtonCancel(wxCommandEvent& event)
{
	Close(true);
	Destroy();
}

void Quarantine::OnButtonClearList(wxCommandEvent& event)
{
	// if no files do nothing
	if (m_listctrl->GetItemCount() == 0)
	{
		return;
	}

	// if 1 file delete it even if not selected!
	if (m_listctrl->GetItemCount() == 1)
	{
		if (m_listctrl->GetItemText(0, 2) != wxT("Action Not Taken"))
		{
			wxString itemname = m_listctrl->GetItemText(0, 1);

			if (wxRemoveFile(QuaranitineFolder + wxT("\\") + itemname + wxT(".infected.txt")))
			{
				m_listctrl->DeleteItem(0);
			}
		}
		return;
	}

	// listctrl delete all selected ----------------
#if 1 //if windows
	int total_items = m_listctrl->GetItemCount();
	int i = 0;

	while (total_items > i)
	{
		int index = (total_items - 1) - i;

		if (m_listctrl->GetItemText(index, 2) != wxT("Action Not Taken"))
		{
			wxString itemname = m_listctrl->GetItemText(index, 1);

			if (wxRemoveFile(QuaranitineFolder + wxT("\\") + itemname + wxT(".infected.txt")))
			{
				m_listctrl->DeleteItem(index);
			}
		}
		i++;
	}
#endif
	// -----------------------------------------
}