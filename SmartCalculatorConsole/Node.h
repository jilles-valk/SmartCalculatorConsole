#pragma once
#include <string>
#include <memory>
using namespace std;

class BaseNode
{
private:
	unique_ptr<BaseNode> leftChild;
	unique_ptr<BaseNode> rightChild;
public:
	BaseNode() : leftChild{ nullptr }, rightChild{ nullptr } {};
	BaseNode(unique_ptr<BaseNode> leftChild) : leftChild{ move(leftChild) } {};
	BaseNode(unique_ptr<BaseNode> leftChild, unique_ptr<BaseNode> rightChild) : leftChild{ move(leftChild) }, rightChild{ move(rightChild) } {};
};

class OperatorNode : public BaseNode
{
private:
	string value;
public:
	OperatorNode(string value) : value{ value } {};
	OperatorNode(string value, unique_ptr<BaseNode> leftChild) : value{ value }, BaseNode{move(leftChild)} {};
	OperatorNode(string value, unique_ptr<BaseNode> leftChild, unique_ptr<BaseNode> rightChild) :BaseNode{ move(leftChild), move(rightChild) } {};
};

class VariableNode : public BaseNode
{
private:
	float value;
public:
	VariableNode(float value) : value{ value } {};
	VariableNode(float value, unique_ptr<BaseNode> leftChild) : value{ value }, BaseNode{ move(leftChild) } {};
	VariableNode(float value, unique_ptr<BaseNode> leftChild, unique_ptr<BaseNode> rightChild) : value{ value }, BaseNode{ move(leftChild), move(rightChild) } {};
};

BaseNode BuildTree(string& input);