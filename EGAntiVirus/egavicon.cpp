#include "egavicon.h"

extern wxString workingDir;


void SetEGAVFrameIcon(wxFrame* frame)
{
	wxIcon AppIconA(wxIconLocation(workingDir + wxT("\\Resources\\EGAV.ico")));
	AppIconA.RealizeResource();
	frame->SetIcon(AppIconA);
}

void SetEGAVDialogIcon(wxDialog* dialog)
{

	wxIcon AppIconA(wxIconLocation(workingDir + wxT("\\Resources\\EGAV.ico")));
	AppIconA.RealizeResource();
	dialog->SetIcon(AppIconA);
}
