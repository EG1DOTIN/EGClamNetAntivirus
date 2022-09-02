#include <wx\wx.h>

class wxClickText : public wxStaticText
{
public:
	wxClickText(
		wxWindow *parent, wxWindowID id,
		const wxString &label,
		const wxPoint &pos = wxDefaultPosition,
		const wxSize &size = wxDefaultSize,
		int style = 0,
		const wxString& name = "staticText");
	
	virtual ~wxClickText();
	void OnMouseEvent(wxMouseEvent& event);
	DECLARE_EVENT_TABLE()
};

