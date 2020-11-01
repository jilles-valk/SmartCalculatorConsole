#pragma once
#include <string>
#include <memory>
using namespace std;

enum class Oper {None, Times, DevidedBy, Power};

class Node
{
public:
	Oper op;
	float value;
	unique_ptr<Node> leftChild;
	unique_ptr<Node> rightChild;

	Node() : op{ Oper::None }, value{ 0 }, leftChild{ nullptr }, rightChild{ nullptr } {};
	Node(Oper op, unique_ptr<Node> leftChild, unique_ptr<Node> rightChild) : op{ op }, value{ 0 }, leftChild { move(leftChild) }, rightChild{ move(rightChild) } {};
	Node(float value, unique_ptr<Node> leftChild, unique_ptr<Node> rightChild) : op{ Oper::None }, value{ value }, leftChild{ move(leftChild) }, rightChild{ move(rightChild) } {};

	bool IsOperator() { return op != Oper::None; };
};

unique_ptr<Node> BuildTree(string& input);