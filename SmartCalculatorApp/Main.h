#pragma once

#include "wx/wx.h"

class Main : public wxFrame
{
public:
	wxTextCtrl* input;
	wxTextCtrl* output;

	Main();
	~Main();

	void OnPressedEnter(wxCommandEvent& evt);

	wxDECLARE_EVENT_TABLE();
};

