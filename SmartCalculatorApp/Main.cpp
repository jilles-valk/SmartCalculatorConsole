#include "Main.h"
#include "Tree.h"
//#include "Graph.h"


//wxBEGIN_EVENT_TABLE(Main, wxFrame)
//	EVT_TEXT_ENTER(10001, OnPressedEnter)
//wxEND_EVENT_TABLE()

Main::Main() : wxFrame(nullptr, wxID_ANY, "Calculator", wxPoint(50, 50), wxSize(1000, 1000))
{
	wxFlexGridSizer* grid = new wxFlexGridSizer(3, 1, 0, 0);
	//wxGridSizer* topGrid = new wxGridSizer(3, 1, 0, 0);
	//grid->Add(topGrid, 100, wxEXPAND | wxALL);
	

	wxFont myFont(30, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);

	input = new wxTextCtrl(this, 10001, "", wxDefaultPosition, wxDefaultSize, wxTE_RICH);
	//Connect(input->GetId(), wxEVT_TEXT_ENTER, wxCommandEventHandler(Main::OnPressedEnter));
	Connect(input->GetId(), wxEVT_TEXT, wxCommandEventHandler(Main::OnPressedEnter));
	input->SetFont(myFont);
	grid->Add(input, 1, wxEXPAND | wxALL);

	output = new wxTextCtrl(this, 10002, "", wxDefaultPosition, wxDefaultSize, wxTE_READONLY | wxTE_RIGHT | wxTE_RICH);
	output->SetFont(myFont);
	//grid->AddSpacer(1);
	//grid->AddSpacer(1);
	grid->Add(output, 1, wxEXPAND | wxALL);

	graph = new Graph(this);
	grid->Add(graph, 1, wxEXPAND);
	//grid->GetItem(2)->SetInitSize(1000, 1000);
	//grid->SetFlexibleDirection(wxBOTH);
	//grid->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_ALL);
	grid->AddGrowableCol(0);
	grid->AddGrowableRow(2);

	grid->FitInside(this);
	this->SetSizer(grid);
	grid->Layout();
}
	
Main::~Main()
{

}

void Main::OnPressedEnter(wxCommandEvent& evt)
{
	Tree tree;

	tree = Tree((std::string)input->GetValue());
	tree.Build();

	if (!tree.BuildSuccesfully())
	{
		input->SetStyle(tree.GetStartIndexException(), tree.GetEndIndexException(), wxTextAttr(*wxRED));
		input->SetDefaultStyle(wxTextAttr(*wxBLACK));
		output->SetStyle(0, output->GetLastPosition(), wxTextAttr(*wxLIGHT_GREY));
	}
	else
	{
		//(*graph).GenerateFunction(tree);

		input->SetStyle(0, input->GetLastPosition(), wxTextAttr(*wxBLACK));
		output->SetDefaultStyle(wxTextAttr(*wxBLACK));

		auto result = tree.Eval();

		output->SetLabel(wxString::Format(wxT(" = %f"), result));
	}

	//evt.Skip();
}