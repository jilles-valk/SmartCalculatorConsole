#include "App.h"

wxIMPLEMENT_APP(App);

App::App() {}
App::~App() {}

bool App::OnInit()
{
	frameOne = new Main();
	frameOne->Show();

	return true;
}