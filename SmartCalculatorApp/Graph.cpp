#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "Graph.h"
#include <wx/dcbuffer.h>


Graph::Graph(wxFrame* parent) : wxScrolledWindow(parent, wxID_ANY) 
{
	//Connect(wxEVT_PAINT, wxCommandEventHandler(Graph::OnPaint));
}
void Graph::SetGraph(std::vector<std::pair<double, double>> graph)
{
	graphPoints = graph;
}

void Graph::GenerateFunction(Tree t)
{
	function = Function(t);
	function.MakeGraph();
	this->Refresh(false);
}



void Graph::DrawGraph(wxDC& dc)
{
	wxPen pen = dc.GetPen();
	pen.SetStyle(wxPENSTYLE_SOLID);
	pen.SetColour(51, 51, 255);
	pen.SetWidth(1);
	dc.SetPen(pen);

	std::vector<Point>::const_iterator begin = std::cbegin(function.graph);
	std::vector<Point>::const_iterator end = std::cend(function.graph);

	double translateX = (canvasWidth / (function.upperRight.x - function.lowerLeft.x));
	double translateY = -(canvasHeight / (function.upperRight.y - function.lowerLeft.y));

	double shiftX = 0.5 * canvasWidth;
	double shiftY = 0.5 * canvasHeight;

	if (begin != end) {
		for (auto curr = begin, next = std::next(begin); next != end; curr++, next++) 
		{
			dc.DrawLine(translateX * (*curr).x + shiftX, translateY * (*curr).y+ shiftY, translateX * (*next).x + shiftX, translateY * (*next).y+ shiftY);
		}
	}
}

void Graph::DrawAxis(wxDC& dc)
{
	wxPen pen = dc.GetPen();

	pen.SetStyle(wxPENSTYLE_SOLID);
	dc.SetPen(pen);

	dc.DrawLine(0, 0.5 * canvasHeight, canvasWidth, 0.5 * canvasHeight);
	dc.DrawLine(0.5 * canvasWidth, 0, 0.5 * canvasWidth, canvasHeight);

	pen.SetStyle(wxPENSTYLE_VERTICAL_HATCH);
	pen.SetWidth(10);
	dc.SetPen(pen);

	dc.DrawLine(0, 0.5 * canvasHeight, canvasWidth, 0.5 * canvasHeight);

	pen.SetStyle(wxPENSTYLE_HORIZONTAL_HATCH);
	dc.SetPen(pen);

	dc.DrawLine(0.5 * canvasWidth, 0, 0.5 * canvasWidth, canvasHeight);
}

void Graph::OnDraw(wxDC& dc)
{
	dc.Clear();
	
	wxBrush brush = dc.GetBrush();
	wxPen pen = dc.GetPen();

	dc.SetPen(pen);
	dc.SetBrush(brush);

	DrawAxis(dc);
		
	DrawGraph(dc);
}

void Graph::OnPaint(wxPaintEvent& evt)
{
	wxBufferedPaintDC dc(this);
	this->PrepareDC(dc);
	this->OnDraw(dc);
}
