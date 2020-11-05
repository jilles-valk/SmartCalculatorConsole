#include "Node.h"
#include <vector>
#include <regex>
#include <map>
#include <sstream>

using namespace std;

regex const operatorRe("\\*|\\/|\\+");
regex const generalRe("\\*|\\/|\\+|\\d+|\\-\\d+");
string const operators = "*/+-";
map<char, Oper> mapOper = { {'*', Oper::Times}, { '/', Oper::DevidedBy }, {'^', Oper::Power } , {'+', Oper::Plus } };

struct LMR
{
	string::iterator left;
	string::iterator middle; 
	string::iterator right;

	LMR(string::iterator left, string::iterator middle, string::iterator right) : left{ left }, middle{ middle }, right{ right } {};
};

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

string ParseInput(string& const input)
{
	stringstream ss;

	ss << input[0];

	for (int i = 1; i < input.size(); i++) 
	{
		// turn -x into + -x so that the order of adding and subtracting does not matter
		if (input[i] == '-' && input[i - 1] != '*' && input[i - 1] != '/' && input[i - 1] != '+')
		{
			ss << '+';
		}
		//multiple -
		ss << input[i];
	}

	return ss.str();

	/*return vector<string>(
		sregex_token_iterator(plusInput.begin(), plusInput.end(), generalRe, 0),
		sregex_token_iterator()
	);*/
}

string::iterator FindOperator(string::iterator& left, string::iterator& right)
{
	string::iterator opPos;
	auto tempRight = right;;

	string::iterator parenthesesPos = find(left, right, '(');
	if (parenthesesPos < right)
	{
		tempRight = parenthesesPos;
	}

	opPos = find(left, tempRight, '+');
	if (opPos == tempRight)
		opPos = find(left, tempRight, '*');
	if (opPos == tempRight)
		opPos = find(left, tempRight, '/');

	return opPos;
}

LMR FindLMR(string::iterator& left, string::iterator& right)
{
	bool hasOuterParentheses = true;
	string::iterator opPos;
	auto tempLeft = left;

	while (hasOuterParentheses && *left == '(')
	{
		tempLeft = left;
		int parenthesesDepth = 0;

		while (tempLeft < right - 1)
		{
			if ( *tempLeft == '(')
			{
				parenthesesDepth++;
			}
			else if (*tempLeft == ')' && --parenthesesDepth == 0)
			{
				hasOuterParentheses = false;
				
				break;
			}
			tempLeft++;
		}

		if (hasOuterParentheses)
		{
			left++;
			right--;
		}
	}

	if (*left == '(')
	{
		opPos = FindOperator(tempLeft, right);
	}
	else
	{
		opPos = FindOperator(left, right);
	}

	return  LMR(left, opPos, right);
}

Node* BuildTreeRecursive(LMR lmr)
{
	if (lmr.middle == lmr.right)
	{
		return new TNode<double>(stof(string(lmr.left, lmr.right)), nullptr, nullptr);
	}

	auto leftRight = lmr.middle;
	LMR leftLMR = FindLMR(lmr.left, leftRight);

	auto rightLeft = lmr.middle + 1;
	LMR rightLMR = FindLMR(rightLeft, lmr.right);

	if (find(begin(operators), end(operators), *lmr.middle) != end(operators))
	{
		return new TNode<Oper>(mapOper[*lmr.middle], BuildTreeRecursive(leftLMR), BuildTreeRecursive(rightLMR));
	}
}

Node* BuildTree(string& input)
{
	auto parsedInput = ParseInput(input);
	auto beginInput = begin(parsedInput);
	auto endInput = end(parsedInput);

	return BuildTreeRecursive(FindLMR(beginInput, endInput));
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

