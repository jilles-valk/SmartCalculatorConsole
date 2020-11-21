#pragma once

#include "wx/wx.h"
#include "Graph.h"

class Main : public wxFrame
{
public:
	wxTextCtrl* input;
	wxTextCtrl* output;
	Graph* graph;

	Main();
	~Main();

private:
	void OnPressedEnter(wxCommandEvent& evt);
};

