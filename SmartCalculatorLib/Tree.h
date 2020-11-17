#pragma once

#include "Node.h"
#include "Exceptions.h"

class Tree
{
public:
	std::string originalInput;
	std::string parsedInput;
	std::unique_ptr<Node> trunk;
	TreeBuildingException tbe;

	Tree() {};
	Tree(std::string input) : originalInput{ input } {};

	void Build();
	bool BuildSuccesfully() { return !tbe.hasValue; };
	double Eval();
	int GetStartIndexException();
	int GetEndIndexException();
	double IsFunction();

private:
	void ParseInput(std::string const & input);
};

