#pragma once
#include <string>
#include <memory>
#include <utility>
#include <iostream>
#include <vector>
#include <unordered_map>

enum class Oper {None, Times, DevidedBy, Power, Plus, Negative, Sin, Cos, Tan};

class Node
{
public:
	Node* leftChild { nullptr };
	Node* rightChild{ nullptr };

	Node() : leftChild{ nullptr }, rightChild{ nullptr } {};
	Node(Node* leftChild) : leftChild{ leftChild } {};
	Node(Node* leftChild, Node* rightChild) : leftChild{ leftChild }, rightChild{ rightChild } {};
	Node(Node const & source);
	Node(Node&& source) noexcept : leftChild{ source.leftChild }, rightChild{ source.rightChild } {source.leftChild = nullptr; source.rightChild = nullptr; };
	Node& operator=(Node node);
	friend void swap(Node& b1, Node& b2);
	~Node() { delete leftChild; delete rightChild; /*cout << "Deleting node" << endl;*/ };

	bool HasVariableNode();
	std::unordered_map<std::string, std::vector<Node**>> GetVariableChildNodes(std::unordered_map<std::string, std::vector<Node**>>& vars);

	virtual void dummy() {}
};

template<typename T>
class TNode : public Node
{
public:
	T value;

	TNode(T value) : value{ value } {};
	TNode(T value, Node* leftChild) : value{ value }, Node{ leftChild }{};
	TNode(T value, Node* leftChild, Node* rightChild) : value{ value }, Node{ leftChild, rightChild }{};

	T GetValue() { return value; };
};

std::unique_ptr<Node> BuildTree(std::string const & input);
double EvalTree(Node* const & node);