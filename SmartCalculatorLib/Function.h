#pragma once

#include "Tree.h"

struct Point
{
	double x, y;

	Point() : x{ 0 }, y{ 0 }{};
	Point(double x, double y) : x{ x }, y{ y }{};

	Point operator + (Point const& p2) { return Point{ x + p2.x, y + p2.y }; };
	Point operator - (Point const& p2) { return Point{ x - p2.x, y - p2.y }; };
	Point operator * (double const& scalar) { return Point{ scalar * x, scalar * y }; };
};

class Function
{
public:
	Tree tree;
	std::vector<Point> graph;
	Point upperLeft;
	Point lowerRight;
	std::string varName;

	Function() = default;
	Function(Tree tree) : tree{ tree }, upperLeft{ -10, 10 }, lowerRight{ 10, -10 }, minX{ -10 }, maxX{ 10 }, varName{ "x" } {};
	Function(Tree tree, std::string varName, size_t numPoints) : tree{ tree }, upperLeft{ -10, 10 }, lowerRight{ 10, -10 }, minX{ -10 }, maxX{ 10 }, varName{ varName } { SetWidth(numPoints); };
	Function& operator=(Function f);

	bool GetMinMax();
	bool SetAutoView();
	bool MakeGraph();
	void SetBounds(Point uLeft, Point lRight);
	void SetWidth(int numPixels);

private:
	double minX;
	double maxX;
	unsigned int numPoints = 1000;
	double const reduceScale = 0.3;
	double const increaseScale = reduceScale / (1 - reduceScale);

};

