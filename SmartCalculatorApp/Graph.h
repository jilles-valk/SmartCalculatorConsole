#pragma once

#include "Function.h"
#include "wx/wx.h"
#include <utility>

class Graph : public wxScrolledCanvas
{
public:
	Graph(wxFrame* parent);
	void GenerateFunction(Tree t);
	void SetGraph(std::vector<std::pair<double, double>> graph);

private:
	void OnDraw(wxDC& dc);
	void OnLeftDown(wxMouseEvent& evt);
	void OnMove(wxMouseEvent& evt);
	void OnPaint(wxPaintEvent& evt);
	void OnResize(wxSizeEvent& evt);
	void OnZoom(wxMouseEvent& evt);
	Point ScreenToFunctionCoord(wxPoint p);
	wxPoint FunctionToScreenCoord(Point p);
	void DrawGraph(wxDC& dc);
	void DrawAxis(wxDC& dc);

private:
	int numPoints = 1000;
	wxPoint clickPos;
	Function function;
	std::vector<std::pair<double, double>> graphPoints;
	size_t canvasHeight = 1000;
	size_t canvasWidth = 1000;

};

