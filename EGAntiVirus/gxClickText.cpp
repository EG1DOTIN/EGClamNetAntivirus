#include "common.h"
#include "gxClickText.h"
#include "avtheme.h"
#include "quarantine.h"

#ifndef _EGAV_TRAY_
#include "AboutPopup.h"
#endif

BEGIN_EVENT_TABLE(wxClickText, wxStaticText)
EVT_LEFT_DOWN(wxClickText::OnMouseEvent)
END_EVENT_TABLE()

wxClickText::wxClickText(
wxWindow *parent,
wxWindowID id,
const wxString &label,
const wxPoint &pos,
const wxSize &size,
int style,
const wxString& name
) : wxStaticText(parent, id, label, pos, size, style | wxPOPUP_WINDOW, name)
{}

wxClickText::~wxClickText(){}

void wxClickText::OnMouseEvent(wxMouseEvent& event)
{
//	wxCommandEvent myevent(wxEVT_COMMAND_BUTTON_CLICKED, GetId());
//	wxPostEvent(this, myevent);
	if (wxT("EG") == this->GetLabel())
	{
#ifndef _EGAV_TRAY_
		SimpleTransientPopup* m_AboutPopup = new SimpleTransientPopup(this, false);
		wxWindow *egtext = (wxWindow*)event.GetEventObject();
		wxPoint pos = egtext->ClientToScreen(wxPoint(0, 0));
		wxSize sz = egtext->GetSize();
		m_AboutPopup->Position(pos, sz);
		m_AboutPopup->Popup();
#endif
	}
	else if (wxT("Quarantine Folder") == this->GetLabel())
	{
#ifndef _EGAV_TRAY_
		Quarantine* quarant = new Quarantine(EGAV_ADMIN_PANEL_TITLE wxT(": Quarantine Folder"));
		quarant->Show();
		DELETE_POINTER(Quarantine, quarant);
#endif
	}

}