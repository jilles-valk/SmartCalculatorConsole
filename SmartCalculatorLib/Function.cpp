#include "Function.h"

Function& Function::operator=(Function f)
{
	std::swap(minX, f.minX);
	std::swap(maxX, f.maxX);
	std::swap(numPoints, f.numPoints);
	std::swap(tree, f.tree);
	std::swap(graph, f.graph);

	lowerRight = f.lowerRight;
	upperLeft = f.upperLeft;
	varName = f.varName;

	return *this;
}

bool Function::GetMinMax()
{
	return false;
}

bool Function::SetAutoView()
{
	return false;
}

void Function::SetBounds(Point uLeft, Point lRight)
{
	upperLeft = uLeft;
	lowerRight = lRight;
	
	double width = lowerRight.x - upperLeft.x;
	bool remakeGraph = false;

	// keep minx and maxx outside of the plot to reduce calling makegraph()
	// zooming out: make graph wider
	if (upperLeft.x < minX)
	{
		minX = upperLeft.x - increaseScale * width;
		remakeGraph = true;
	}
	else if (upperLeft.x - minX > reduceScale * width) // zooming in: make graph less wide
	{
		minX = upperLeft.x - 0.5 * reduceScale * width;
		remakeGraph = true;
	}

	if (lowerRight.x > maxX)
	{
		maxX = lowerRight.x + increaseScale * width;
		remakeGraph = true;
	}
	else if (maxX - lowerRight.x > reduceScale * width)
	{
		maxX = lowerRight.x + 0.5 * reduceScale * width;
		remakeGraph = true;
	}

	if (remakeGraph)
	{
		MakeGraph();
	}
}

void Function::SetWidth(int numPixels)
{
	numPoints = (1 + 2 * reduceScale) * numPixels;
}

bool Function::MakeGraph()
{
	double dx = (maxX - minX)/(numPoints - 1);
	double x = minX;

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
