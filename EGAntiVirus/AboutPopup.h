#ifndef ABOUT_POPUP_HEADER
#define ABOUT_POPUP_HEADER

#include <wx\wx.h>
#include <wx\popupwin.h>
#include <wx\scrolbar.h>
#include "AboutPanel.h"

class SimpleTransientPopup : public wxPopupTransientWindow
{
public:
	SimpleTransientPopup(wxWindow *parent, bool scrolled);
	virtual ~SimpleTransientPopup();

	// wxPopupTransientWindow virtual methods are all overridden to log them
	virtual void Popup(wxWindow *focus = NULL);
	virtual void OnDismiss();
	virtual bool Show(bool show = true);

	wxScrolledWindow *m_panel;

	AboutPanel* m_AboutPanel;

private:
	wxDECLARE_ABSTRACT_CLASS(SimpleTransientPopup);
	wxDECLARE_EVENT_TABLE();
};

enum
{
	Minimal_PopupButton,
};
#endif