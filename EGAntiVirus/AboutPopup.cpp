#include "AboutPopup.h"
#include "avtheme.h"

IMPLEMENT_CLASS(SimpleTransientPopup, wxPopupTransientWindow)

wxBEGIN_EVENT_TABLE(SimpleTransientPopup, wxPopupTransientWindow)
//EVT_BUTTON(Minimal_PopupButton, SimpleTransientPopup::OnButton)
wxEND_EVENT_TABLE()

extern wxString workingDir;

SimpleTransientPopup::SimpleTransientPopup(wxWindow *parent, bool scrolled)
:wxPopupTransientWindow(parent)
{
	m_panel = new wxScrolledWindow(this, wxID_ANY);
	m_panel->SetBackgroundColour(EGAV_FONT_BACK_LIGHT_GOLD_COLOR);

	// Keep this code to verify if mouse events work, they're required if
	// you're making a control like a combobox where the items are highlighted
	// under the cursor, the m_panel is set focus in the Popup() function

	m_AboutPanel = new AboutPanel(wxT(""), m_panel);

	wxBoxSizer *topSizer = new wxBoxSizer(wxVERTICAL);
	topSizer->Add(m_AboutPanel, 0, wxALL, 1);

	//topSizer->Add(new wxPanel(m_panel, wxID_ANY, wxDefaultPosition, wxSize(450, 80)));

	if (scrolled)
	{
		// Add a big window to ensure that scrollbars are shown when we set the
		// panel size to a lesser size below.
		topSizer->Add(new wxPanel(m_panel, wxID_ANY, wxDefaultPosition,
			wxSize(600, 300)));
	}

	m_panel->SetSizer(topSizer);
	if (scrolled)
	{
		// Set the fixed size to ensure that the scrollbars are shown.
		m_panel->SetSize(300, 300);

		// And also actually enable them.
		m_panel->SetScrollRate(10, 10);
	}
	else
	{
		// Use the fitting size for the panel if we don't need scrollbars.
		topSizer->Fit(m_panel);
	}

	SetClientSize(m_panel->GetSize());
}

SimpleTransientPopup::~SimpleTransientPopup()
{
}

void SimpleTransientPopup::Popup(wxWindow* WXUNUSED(focus))
{
	wxPopupTransientWindow::Popup();
}

void SimpleTransientPopup::OnDismiss()
{
	wxPopupTransientWindow::OnDismiss();
}

bool SimpleTransientPopup::Show(bool show)
{
	return wxPopupTransientWindow::Show(show);
}
