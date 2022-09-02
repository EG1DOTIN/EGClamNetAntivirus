#ifndef __WIN__EVENT__HEADER__
#define __WIN__EVENT__HEADER__

#include <wx/wx.h>

class WinEventTestFrame : public wxFrame
{
public:
	WinEventTestFrame(const wxString& title, const wxPoint& pos, const wxSize& size);
	virtual ~WinEventTestFrame();
	virtual WXLRESULT MSWWindowProc(WXUINT, WXWPARAM, WXLPARAM);

	bool m_bShutdownOrLogoffEvent;

private:
	DECLARE_CLASS(WinEventTestFrame)

};
#endif //__DEVTESTFRAME