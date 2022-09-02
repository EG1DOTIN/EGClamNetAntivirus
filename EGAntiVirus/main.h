#ifndef EVAG_MAIN_H
#define EVAG_MAIN_H

#include <wx/wx.h>
#include <wx/snglinst.h>

class MyApp : public wxApp
{
public:
	virtual bool OnInit();
	int OnExit();

	wxSingleInstanceChecker* m_checker;

};


void ConfigClamD(const wxString& WorkingDir, const wxString& AppDataDir);

void checkUpdate();
void setMKeyToServer();

void makeDefaultMonitoringFolderList(const wxString& dbFileFullPath);
#endif