#pragma once

#include "Tree.h"

struct Point
{
	double x, y;

	Point(double x, double y) : x{ x }, y{ y }{};
};

class Function
{
public:
	std::unordered_map<std::string, std::vector<Node*>> variables;
	std::vector<Point> graph;



};

