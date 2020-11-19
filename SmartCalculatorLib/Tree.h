#pragma once

#include "Node.h"
#include "Exceptions.h"
#include <vector>

class Tree
{
public:
	std::string originalInput;
	std::string parsedInput;
	std::unordered_map<std::string, std::vector<Node**>> variables;
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
	bool GetVariables();
	bool SetVariables(std::unordered_map<std::string, double> variables);
	bool SetVariable(std::pair<std::string, double> var);

private:
	void ParseInput(std::string const & input);
};

