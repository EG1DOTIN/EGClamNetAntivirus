#ifndef gxBUTTON_HEADER
#define gxBUTTON_HEADER

#include <wx\wx.h>
#include "avtheme.h"

class MyWXNoTabButton : public wxButton
{
public:
	MyWXNoTabButton(wxWindow *parent,
		wxWindowID id,
		const wxString& label = wxEmptyString,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = 0,
		const wxValidator& validator = wxDefaultValidator,
		const wxString& name = wxButtonNameStr)
		: wxButton(parent, id, label, pos, size, style, validator, name)
	{
		SetBackgroundColour(EGAV_BUTTON_BACK_COLOR2);
		SetForegroundColour(EGAV_BUTTON_TEXT_COLOR2);
		SetFont(EGAV_FONT_MID_DECORATIVE_NORMAL);
	}

	
	virtual bool AcceptsFocusFromKeyboard() const
	{
		return false;
	}
	
};

class MyWXTransParentButton : public wxButton
{
public:
	MyWXTransParentButton(wxWindow *parent,
		wxWindowID id,
		const wxString& label = wxEmptyString,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = 0,
		const wxValidator& validator = wxDefaultValidator,
		const wxString& name = wxButtonNameStr)
		: wxButton(parent, id, label, pos, size, style, validator, name)
	{
		SetCursor(wxCursor(wxCURSOR_HAND));
		SetFont(EGAV_FONT_MID_DECORATIVE_NORMAL);
		SetBackgroundColour(EGAV_BUTTON_BACK_COLOR1);
		SetForegroundColour(EGAV_BUTTON_TEXT_COLOR1);
	}

};

class MyWXColoredButton : public wxButton
{
public:
	MyWXColoredButton(wxWindow *parent,
		wxWindowID id,
		const wxString& label = wxEmptyString,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = 0,
		const wxValidator& validator = wxDefaultValidator,
		const wxString& name = wxButtonNameStr, bool bFontSmall = false)
		: wxButton(parent, id, label, pos, size, style, validator, name)
	{
		if (!bFontSmall)
		{
			SetFont(EGAV_FONT_MID_DECORATIVE_NORMAL);
		}

		SetBackgroundColour(EGAV_BUTTON_BACK_COLOR2);
		SetForegroundColour(EGAV_BUTTON_BACK_COLOR1);
	}

};

class MyWXRoundEdgeButton : public wxBitmapButton
{
public:
	MyWXRoundEdgeButton(wxWindow *parent,
		wxWindowID id,
		const wxString& label = wxEmptyString,
		const wxPoint& position = wxDefaultPosition,
		const wxSize& size = EGAV_BUTTON_SIZE_MID,
		long style = EGAV_BUTTON_STYLE_NOBORDER,
		const wxValidator& validator = wxDefaultValidator,
		const wxString& name = wxButtonNameStr, bool bFontSmall = false)
		:wxBitmapButton(parent, id, MyWXRoundButtonBitmap(size, label), position, size, style)
	{
		
	}

	wxBitmap MyWXRoundButtonBitmap(const wxSize& size, const wxString& label)
	{
		int labelsize = label.length();
		int labelPosX = size.x / 2 - 5 * (labelsize / 2) - 5;
		int labelPosY = 5;
		wxBitmap bitmap(size.x, size.y);
		wxMemoryDC dc;
		dc.SelectObject(bitmap);
		dc.SetBackground(EGAV_WINDOW_BACK_COLOR1);
		dc.SetBrush(EGAV_BUTTON_BACK_COLOR2);
		dc.Clear();
		dc.DrawRoundedRectangle(0, 0, size.x, size.y, 5);
		dc.SetTextForeground(EGAV_BUTTON_BACK_COLOR1);
		if (size == EGAV_BUTTON_SIZE_SMALL)
			dc.SetFont(EGAV_FONT_TINY_SWISS_NORMAL_NORMAL);
		else if (size == EGAV_BUTTON_SIZE_MID_SMALL)
			dc.SetFont(EGAV_FONT_TINY_SWISS_NORMAL_NORMAL1);
		else
			dc.SetFont(EGAV_FONT_SMALL_SWISS_NORMAL_NORMAL);
		
		dc.DrawText(label, labelPosX, labelPosY);
		dc.SelectObject(wxNullBitmap);
		return bitmap;
	}

};

#endif