#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "Graph.h"
#include <wx/dcbuffer.h>


Graph::Graph(wxFrame* parent) : wxScrolledCanvas(parent, wxID_ANY) 
{
	Bind(wxEVT_PAINT, &Graph::OnPaint, this);
	Bind(wxEVT_SIZE, &Graph::OnResize, this);
	Bind(wxEVT_MOUSEWHEEL, &Graph::OnZoom, this);
	Bind(wxEVT_MOTION, &Graph::OnMove, this);
	Bind(wxEVT_LEFT_DOWN, &Graph::OnLeftDown, this);

	SetBackgroundStyle(wxBG_STYLE_PAINT);
}
void Graph::SetGraph(std::vector<std::pair<double, double>> graph)
{
	graphPoints = graph;
}

void Graph::GenerateFunction(Tree t)
{
	function = Function(t, t.GetVaryingVariableName(), this->GetSize().x);
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

	if (begin != end) {
		for (auto curr = begin, next = std::next(begin); next != end; curr++, next++) 
		{
			dc.DrawLine(FunctionToScreenCoord(*curr), FunctionToScreenCoord(*next));
		}
	}
}

void Graph::DrawAxis(wxDC& dc)
{
	wxPen pen = dc.GetPen();

	pen.SetStyle(wxPENSTYLE_SOLID);
	dc.SetPen(pen);

	dc.DrawLine(FunctionToScreenCoord(Point(0, function.upperLeft.y)),
		FunctionToScreenCoord(Point(0, function.lowerRight.y)));
	dc.DrawLine(FunctionToScreenCoord(Point(function.upperLeft.x, 0)),
		FunctionToScreenCoord(Point(function.lowerRight.x, 0)));
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

void Graph::OnLeftDown(wxMouseEvent& evt)
{
	clickPos = evt.GetPosition();
}

void Graph::OnMove(wxMouseEvent& evt)
{
	if (evt.Dragging() && evt.LeftIsDown())
	{
		Point moveV = ScreenToFunctionCoord(clickPos) - ScreenToFunctionCoord(evt.GetPosition());

		function.SetBounds(function.upperLeft + moveV, function.lowerRight + moveV);

		clickPos = evt.GetPosition();

		Refresh(false);
	}

	evt.Skip();
}

void Graph::OnPaint(wxPaintEvent& evt)
{
	wxAutoBufferedPaintDC dc(this);
	this->PrepareDC(dc);
	this->OnDraw(dc);
}

void Graph::OnResize(wxSizeEvent& evt)
{
	canvasWidth = evt.GetSize().x;
	canvasHeight = evt.GetSize().y;
	function.SetWidth(canvasWidth);

	function.MakeGraph();
	Refresh(false);
	evt.Skip();
}

void Graph::OnZoom(wxMouseEvent& evt)
{
	double zoomAmount = 0.1 * (evt.GetWheelRotation() / 120);
	Point mousePos = ScreenToFunctionCoord(evt.GetPosition());

	if (zoomAmount < 0)
	{
		zoomAmount = zoomAmount / (1 + zoomAmount);
	}

	function.SetBounds(function.upperLeft + (mousePos - function.upperLeft) * zoomAmount, 
		function.lowerRight + (mousePos - function.lowerRight) * zoomAmount);

	Refresh(false);
	evt.Skip();
}

Point Graph::ScreenToFunctionCoord(wxPoint p)
{
	double translateX = ((function.lowerRight.x - function.upperLeft.x) / canvasWidth);
	double translateY = ((function.lowerRight.y - function.upperLeft.y) / canvasHeight);

	return Point(p.x * translateX + function.upperLeft.x, p.y * translateY + function.upperLeft.y);
}

wxPoint Graph::FunctionToScreenCoord(Point p)
{
	double translateX = (canvasWidth / (function.lowerRight.x - function.upperLeft.x));
	double translateY = (canvasHeight / (function.lowerRight.y - function.upperLeft.y));

	return wxPoint(translateX * (p.x - function.upperLeft.x), translateY * (p.y - function.upperLeft.y));
}
