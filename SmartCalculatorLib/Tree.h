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
	std::shared_ptr<Node> trunk;
	TreeBuildingException tbe;

	Tree() {};
	Tree(std::string input) : originalInput{ input } {};
	Tree(Tree const& source) : originalInput{ source.originalInput }, parsedInput{ source.parsedInput }, variables{ source.variables }, trunk{ source.trunk }{};
	Tree(Tree&& source) noexcept = default; // : originalInput{ source.originalInput }, parsedInput{ source.parsedInput }, variables{ source.variables }, trunk{ std::make_unique<Node>(source.trunk.get()) }{};
	Tree& operator=(Tree source);

	~Tree() { std::cout << "Deleting tree" << std::endl; };

	void Build();
	bool BuildSuccesfully() { return !tbe.hasValue; };
	double Eval();
	int GetStartIndexException();
	int GetEndIndexException();
	bool IsFunction();
	std::string GetVaryingVariableName();
	bool GetMostLikelyVarying();
	bool GetVariables();
	bool SetVariables(std::unordered_map<std::string, double> variables);
	bool SetVariable(std::pair<std::string, double> var);

private:
	void ParseInput(std::string const & input);
	
};

