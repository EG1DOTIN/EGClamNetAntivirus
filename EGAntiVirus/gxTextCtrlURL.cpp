#include "gxTextCtrlURL.h"


wxBEGIN_EVENT_TABLE(gxTextCtrlURL, wxTextCtrl)
EVT_TEXT_URL(wxID_ANY, gxTextCtrlURL::OnTextURL)
wxEND_EVENT_TABLE()


void gxTextCtrlURL::OnTextURL(wxTextUrlEvent& event)
{
	const wxMouseEvent& ev = event.GetMouseEvent();

	// filter out mouse moves, too many of them
	if (ev.Moving())
		return;

	long start = event.GetURLStart(),
		end = event.GetURLEnd();

	wxString TextURL = GetValue().Mid(start, end - start);

	wxLaunchDefaultBrowser(TextURL, wxBROWSER_NOBUSYCURSOR);
}