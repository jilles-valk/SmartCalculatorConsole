#pragma once

#include "Function.h"
#include "wx/wx.h"
#include <utility>

class Graph : public wxScrolledWindow
{
public:
	Graph(wxFrame* parent);
	void GenerateFunction(Tree t);
	void SetGraph(std::vector<std::pair<double, double>> graph);

private:
	void OnDraw(wxDC& dc);
	void OnPaint(wxPaintEvent& evt);
	void DrawGraph(wxDC& dc);

private:
	int numPoints = 1000;
	Function function;
	std::vector<std::pair<double, double>> graphPoints;

};

