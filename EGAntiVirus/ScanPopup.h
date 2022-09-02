#ifndef SCAN_POPUP_HEADER
#define SCAN_POPUP_HEADER

#include <wx\wx.h>
#include <wx\popupwin.h>
#include <wx\scrolbar.h>
#include "ScanPanel.h"

class SimpleScanTransientPopup : public wxPopupTransientWindow
{
public:
	SimpleScanTransientPopup(wxWindow *parent, bool scrolled);
	virtual ~SimpleScanTransientPopup();

	// wxPopupTransientWindow virtual methods are all overridden to log them
	virtual void Popup(wxWindow *focus = NULL);
	virtual void OnDismiss();
	virtual bool Show(bool show = true);

	wxScrolledWindow *m_panel;

	ScanPanel* m_ScanPanel;

private:
	wxDECLARE_ABSTRACT_CLASS(SimpleTransientPopup);
	wxDECLARE_EVENT_TABLE();
};

#endif