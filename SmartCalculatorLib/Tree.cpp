#include "Tree.h"
#include <sstream>
#include <algorithm>

Tree& Tree::operator=(Tree source) 
{
	std::swap(originalInput, source.originalInput);
	std::swap(parsedInput, source.parsedInput);
	std::swap(variables, source.variables);
	std::swap(trunk, source.trunk);
	std::swap(tbe, source.tbe);
	return *this;
}

void Tree::Build()
{
	ParseInput(originalInput);

	try
	{
		trunk = BuildTree(parsedInput);

		if (!GetVariables())
		{
			GetMostLikelyVarying();
		}
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

//compensate for difference parsedInput and originalInput
int Tree::GetStartIndexException()
{
	return tbe.left - cbegin(parsedInput);
}

int Tree::GetEndIndexException()
{
	return tbe.right - cbegin(parsedInput);
}

bool Tree::IsFunction()
{
	return trunk.get()->HasVariableNode();
}

std::string Tree::GetVaryingVariableName()
{
	std::string maxVar;
	for (auto const& var : variables)
	{
		if (maxVar.compare(var.first) < 0)
		{
			maxVar = var.first;
		}
	}
	
	return maxVar;
}

bool Tree::GetVariables()
{
	std::unordered_map<std::string, std::vector<Node**>> input;
	variables = trunk.get()->GetVariableChildNodes(input);

	return !variables.empty();
}

bool Tree::GetMostLikelyVarying()
{
	auto [name, n, depth] = trunk.get()->GetMostLikelyVarying(0);
	variables.insert(std::make_pair(name, std::vector<Node**>{ n }));

	return !variables.empty();
}

bool Tree::SetVariables(std::unordered_map<std::string, double> vars)
{
	return std::all_of(std::begin(vars), std::end(vars), [this](std::pair<std::string, double> nameVal) {
		return SetVariable(nameVal);
		});
}

bool Tree::SetVariable(std::pair<std::string, double> var)
{
	if (variables.count(var.first) > 0)
	{
		for_each(begin(variables.at(var.first)), end(variables.at(var.first)), [&var](Node** n) {
			delete *n; 
			*n = new TNode<double>(var.second);
			});

		return true;
	}
	return false;
}

void Tree::ParseInput(std::string const& input)
{
	static std::string const operators = "*/+^(";

	std::stringstream ss;

	ss << input[0];

	for (unsigned int i = 1; i < input.size(); i++)
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
