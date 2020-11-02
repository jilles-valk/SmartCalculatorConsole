#include "Node.h"
#include <vector>
#include <regex>
#include <map>
#include <sstream>

using namespace std;

regex const operatorRe("\\*|\\/|\\+");
regex const generalRe("\\*|\\/|\\+|\\d+|\\-\\d+");
map<string, Oper> mapOper = { {"*", Oper::Times}, { "/", Oper::DevidedBy }, {"^", Oper::Power } , {"+", Oper::Plus }, {"-", Oper::Minus } };

//copy constructor
Node::Node(Node& const source)
{
	if (source.leftChild != nullptr && source.rightChild != nullptr)
	{
		leftChild = new Node(source.leftChild, source.rightChild);
	}
	else if (source.leftChild != nullptr)
	{
		leftChild = new Node(source.leftChild);
	}
}

Node& Node::operator=(Node& const node)
{
	if (node.leftChild != nullptr)
	{
		swap(leftChild, node.leftChild);
	}
	else
	{
		leftChild = nullptr;
	}

	if (node.rightChild != nullptr)
	{
		swap(rightChild, node.rightChild);
	}
	else
	{
		rightChild = nullptr;
	}

	return *this;
}

void swap(Node& b1, Node& b2)
{
	swap(b1.leftChild, b2.leftChild);
	swap(b1.rightChild, b2.rightChild);
};

vector<string> ParseInput(string& const input)
{
	stringstream ss;

	for (int i = 0; i < input.size(); i++) 
	{
		if (input[i] == '-')
		{
			ss << '+';
		}
		ss << input[i];
	}

	string plusInput = ss.str();

	return vector<string>(
		sregex_token_iterator(plusInput.begin(), plusInput.end(), generalRe, 0),
		sregex_token_iterator()
	);
}

vector<string>::iterator FindMiddle(vector<string>::iterator left, vector<string>::iterator right)
{
	return find_if(left, right, [](string elem) {return regex_match(elem, operatorRe); });
}

Node* BuildTreeRecursive(vector<string>::iterator left, vector<string>::iterator middle, vector<string>::iterator right)
{
	if (middle == right)
	{
		return new TNode<double>(stof(*left), nullptr, nullptr);
	}

	if (regex_match(*middle, operatorRe))
	{
		return new TNode<Oper>(mapOper[*middle], BuildTreeRecursive(left, FindMiddle(left, middle - 1), middle - 1), BuildTreeRecursive(middle + 1, FindMiddle(middle + 1, right), right));
	}

	if (middle == left && middle == right)
	{
		return new TNode<double>(stof(*middle), nullptr, nullptr);
	}
	
	if (middle == left)
	{
		return new TNode<double>(stof(*middle), nullptr, BuildTreeRecursive(middle++, middle += 2, left));
	}

	if (middle == right)
	{
		return new TNode<double>(stof(*(--middle)), BuildTreeRecursive(left, middle -= 2, middle), nullptr);
	}

}

Node* BuildTree(string& input)
{
	auto seperatedInput = ParseInput(input);

	return BuildTreeRecursive(begin(seperatedInput), FindMiddle(begin(seperatedInput), end(seperatedInput)), end(seperatedInput));
}

double EvalTree(Node* const node)
{
	auto op = dynamic_cast<TNode<Oper>*>(node);

	if (op != NULL)
	{
		switch (op->GetValue())
		{
		case Oper::Times:
			return EvalTree(node->leftChild) * EvalTree(node->rightChild);
		case Oper::DevidedBy :
			return EvalTree(node->leftChild) / EvalTree(node->rightChild);
		case Oper::Plus:
			return EvalTree(node->leftChild) + EvalTree(node->rightChild);
		case Oper::Minus:
			return EvalTree(node->leftChild) - EvalTree(node->rightChild);
		default:
			break;
		}
	}

	auto val = dynamic_cast<TNode<double>*>(node);

	if (val != NULL)
	{
		return val->value;
	}
}

