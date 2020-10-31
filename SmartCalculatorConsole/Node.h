#pragma once
#include <string>
#include <memory>
using namespace std;

template<typename T>
class Node
{
private:
	T value;
	unique_ptr<Node<T>> leftChild;
	unique_ptr<Node<T>> rightChild;

public:
	Node(T value) : value{value} {};
	Node(T value, unique_ptr<Node<T>> leftChild) : value{ value }, leftChild{ move(leftChild) } {};
	Node(T value, unique_ptr<Node<T>> leftChild, unique_ptr<Node<T>> rightChild) : value{ value }, leftChild{ move(leftChild) }, rightChild{ move(rightChild) } {};
	T getValue() { return value; };
};

class OperatorNode : private Node<string>
{
//public:
//	Node getLeftChild() { return leftChild; };
//	Node getRightChild() { return rightChild; };
};

class VariableNode : Node<float>
{
public:

};