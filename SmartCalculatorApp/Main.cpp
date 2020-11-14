#include "Main.h"
#include "Node.h"


wxBEGIN_EVENT_TABLE(Main, wxFrame)
	EVT_TEXT_ENTER(10001, OnPressedEnter)
wxEND_EVENT_TABLE()

Main::Main() : wxFrame(nullptr, wxID_ANY, "Calculator", wxPoint(50, 50), wxSize(1000, 1000))
{
	wxGridSizer* grid = new wxGridSizer(10, 2, 0, 0);
	wxFont myFont(30, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);

	input = new wxTextCtrl(this, 10001, "", wxPoint(10, 10), wxSize(500, 30), wxTE_PROCESS_ENTER);
	input->SetFont(myFont);
	grid->Add(input, 1, wxEXPAND | wxALL);

	output = new wxTextCtrl(this, 10002, "");
	output->SetFont(myFont);
	grid->AddSpacer(1);
	grid->AddSpacer(1);
	grid->Add(output, 1, wxEXPAND | wxALL);

	this->SetSizer(grid);
	grid->Layout();
}

Main::~Main()
{

}

void Main::OnPressedEnter(wxCommandEvent& evt)
{
	auto tree = BuildTree((string) input->GetValue());
	auto result = EvalTree(tree.get());

	output->SetValue(wxString::Format(wxT(" = %f"), result));
}