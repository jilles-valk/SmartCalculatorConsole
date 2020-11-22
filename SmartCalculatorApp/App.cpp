
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "App.h"


wxIMPLEMENT_APP(App);

App::App() {}
App::~App() {}

bool App::OnInit()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	frameOne = new Main();
	frameOne->Show();

	return true;
}