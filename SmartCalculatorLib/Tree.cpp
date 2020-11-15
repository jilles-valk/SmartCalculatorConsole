#include "Tree.h"
#include <sstream>

void Tree::Build()
{
	ParseInput(originalInput);

	try
	{
		trunk = BuildTree(parsedInput);
	}
	catch(TreeBuildingException e)
	{
		tbe = e;
	}
}

double Tree::Eval()
{
	return EvalTree(trunk.get());
}

int Tree::GetStartIndexException()
{
	return tbe.left - cbegin(parsedInput);
}

int Tree::GetEndIndexException()
{
	return tbe.right - cbegin(parsedInput);
}


void Tree::ParseInput(std::string const& input)
{
	static std::string const operators = "*/+";

	std::stringstream ss;

	ss << input[0];

	for (int i = 1; i < input.size(); i++)
	{
		// turn -x into + -x so that the order of adding and subtracting does not matter
		if (input[i] == '-')
		{
			if (i + 1 < input.size() && input[i + 1] == '-')
			{
				ss << '+';
				i += 2;
			}
			else if (find(begin(operators), end(operators), input[i - 1]) == end(operators))
			{
				ss << '+';
			}
		}
		ss << input[i];
	}

	parsedInput = ss.str();
}
