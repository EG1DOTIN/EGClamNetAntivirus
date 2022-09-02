#include "common.h"
#include "egavicon.h"
#include "helpDoc.h"

extern wxString workingDir;

void ExecuteCHMDoc(void)
{
	ExecuteExeUACSilentW(L"EG ClamNet Antivirus.chm", L"", workingDir + wxT("\\Docs"), NULL);
}

HelpDoc::HelpDoc(const wxString& title, const wxString& htmlFileLoc) : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(795, 530))
{
	SetEGAVFrameIcon(this);
	htmlhelp = new wxHtmlWindow(this);
	htmlhelp->LoadPage(htmlFileLoc);

	htmlhelp->Show();

	Center();
}

