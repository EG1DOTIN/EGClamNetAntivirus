#ifndef EGAV_SCANNER_HEADER
#define EGAV_SCANNER_HEADER

#include <wx\wx.h>
#include <wx\snglinst.h>
#include <wx\cmdline.h>
	
class MyApp : public wxApp
{
public:
	virtual bool OnInit();
	int OnExit();
	wxCmdLineParser* parser;
	ScannerWin* m_scanwin;

	wxSingleInstanceChecker* m_checker;
};

#endif // !EGAV_SCANNER_HEADER
