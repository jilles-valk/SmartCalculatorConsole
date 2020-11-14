#pragma once

#include <wx/wxprec.h>

#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif

#include "Main.h"

class App : public wxApp
{
public:
	App();
	~App();
	virtual bool OnInit();

private:
	Main* frameOne = nullptr;
};

