#include "Function.h"
#include <algorithm>
#include <execution>

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

	graph.clear();
	graph.resize(numPoints);

	std::for_each(std::execution::seq, begin(graph), end(graph), [tree = tree, varName = varName, x = minX, &dx](Point& p) mutable {
		tree.SetVariable(std::make_pair(varName, x));

		p = Point(x, tree.Eval());
		x += dx;
		});

	return true;
}
