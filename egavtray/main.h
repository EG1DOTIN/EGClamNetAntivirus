#ifndef EGAVTRAY_MAIN_HEADER
#define EGAVTRAY_MAIN_HEADER

#include <wx\wx.h>
#include <wx\snglinst.h>

class MyApp : public wxApp
{
public:
	virtual bool OnInit();
	int OnExit();

	wxSingleInstanceChecker* m_checker;
};

#endif // !EGAVTRAY_MAIN_HEADER
