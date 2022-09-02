#ifndef EGAV_UPDATE_MAIN_HEADER
#define EGAV_UPDATE_MAIN_HEADER

#include <wx/wx.h>
#include <wx\snglinst.h>

class MyApp : public wxApp
{

public:

	virtual bool OnInit();
	int OnExit();

	wxSingleInstanceChecker* m_checker;

};

#endif