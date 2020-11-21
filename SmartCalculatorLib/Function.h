#pragma once

#include "Tree.h"

struct Point
{
	double x, y;

	Point() : x{ 0 }, y{ 0 }{};
	Point(double x, double y) : x{ x }, y{ y }{};
};

class Function
{
public:
	Tree tree;
	std::vector<Point> graph;
	Point lowerLeft;
	Point upperRight;
	unsigned int numPoints;
	std::string varName;

	Function() = default;
	Function(Tree tree) : tree{ tree }, lowerLeft{ -10, -10 }, upperRight{ 10, 10 }, numPoints{ 1000 }, varName{ "x" } {};
	
	
	bool GetMinMax();
	bool SetAutoView();
	bool MakeGraph();

};

