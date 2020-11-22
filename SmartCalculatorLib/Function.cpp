#include "Function.h"

bool Function::GetMinMax()
{
	return false;
}

bool Function::SetAutoView()
{
	return false;
}

bool Function::MakeGraph()
{
	double dx = (lowerRight.x - upperLeft.x)/(numPoints - 1);
	double x = upperLeft.x;

	graph.clear();
	graph.reserve(numPoints);

	for (int i = 0; i < numPoints; i++)
	{
		tree.SetVariable(std::pair<std::string, double>{varName, x});
		graph.push_back(Point(x, tree.Eval()));

		x += dx;
	}
	return true;
}
