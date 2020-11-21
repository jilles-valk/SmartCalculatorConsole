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
	std::vector<Point>::const_iterator begin = std::cbegin(function.graph);
	std::vector<Point>::const_iterator end = std::cend(function.graph);

	if (begin != end) {
		for (auto curr = begin, next = std::next(begin); next != end; curr++, next++) 
		{
			dc.DrawLine((*curr).x, (*curr).y, (*next).x, (*next).y);
		}
	}
}

void Graph::OnDraw(wxDC& dc)
{
	dc.Clear();
	
	wxBrush brush = dc.GetBrush();
	wxPen pen = dc.GetPen();

	dc.SetPen(pen);
	dc.SetBrush(brush);

	dc.DrawRectangle(20, 20, 200, 200);

	wxPoint points[3] = { wxPoint(100, 100), wxPoint(200, 200), wxPoint(300, 250) };
	dc.DrawLines(3, points);

	DrawGraph(dc);
}

void Graph::OnPaint(wxPaintEvent& evt)
{
	wxBufferedPaintDC dc(this);
	this->PrepareDC(dc);
	this->OnDraw(dc);
}
