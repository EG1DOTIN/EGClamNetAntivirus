#ifndef gxTEXT_CONTROL_URL_HEADER
#define gxTEXT_CONTROL_URL_HEADER

#include <wx\wx.h>

class gxTextCtrlURL : public wxTextCtrl
{
public:
	gxTextCtrlURL(wxWindow *parent, wxWindowID id, const wxString &value,
		const wxPoint &pos, const wxSize &size, int style = 0)
		: wxTextCtrl(parent, id, value, pos, size, style)
	{
	
	}

	void OnTextURL(wxTextUrlEvent& event);
	
private:
	wxDECLARE_EVENT_TABLE();
};

#endif