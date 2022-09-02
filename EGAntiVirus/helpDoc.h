#ifndef HELP_DOC_HEADER
#define HELP_DOC_HEADER

#include <wx/wx.h>
#include <wx/html/htmlwin.h>

class HelpDoc : public wxFrame
{
public:
	HelpDoc(const wxString& title, const wxString& htmlFileLoc);

	wxHtmlWindow* htmlhelp;
};

void ExecuteCHMDoc(void);

#endif