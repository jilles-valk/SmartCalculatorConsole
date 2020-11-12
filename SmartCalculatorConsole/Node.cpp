#define _USE_MATH_DEFINES

#include "Node.h"
#include <vector>
#include <regex>
#include <map>
#include <sstream>
#include <cmath>

using namespace std;

static regex const binaryOperatorRe("[\\+\\*\\/\\^]");
static regex const unaryOperatorRe("-|sin|cos|tan|log");
static string const operators = "*/+";
static vector<char> const opOrder{ '+', '*', '/', '^' };
static vector<string> const unaryOps{ "-", "sin", "cos", "tan" };
static map<string, Oper> const mapOper = { {"*", Oper::Times}, { "/", Oper::DevidedBy }, {"^", Oper::Power } , {"+", Oper::Plus }, {"-", Oper::Negative }, {"sin", Oper::Sin }, {"cos", Oper::Cos },{"tan", Oper::Tan } };

struct OpIt
{
	string::const_iterator begOp;
	string::const_iterator endOp;

	OpIt() {};
	OpIt(string::const_iterator begOp, string::const_iterator endOp) : begOp{ begOp }, endOp{ endOp } {};
};

struct LMR
{
	string::const_iterator left;
	OpIt middle;
	string::const_iterator right;

	LMR() {};
	LMR(string::const_iterator left, string::const_iterator begOp, string::const_iterator endOp, string::const_iterator right) : left{ left }, middle{begOp, endOp }, right{ right } {};
	LMR(string::const_iterator left, OpIt middle, string::const_iterator right) : left{ left }, middle{ middle }, right{ right } {};
};


//copy constructor
Node::Node(Node & const source)
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

Node & Node::operator=(Node & const node)
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

void swap(Node & b1, Node & b2)
{
	swap(b1.leftChild, b2.leftChild);
	swap(b1.rightChild, b2.rightChild);
};

string ParseInput(string const & input)
{
	stringstream ss;

	ss << input[0];

	for (int i = 1; i < input.size(); i++) 
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

	return ss.str();
}

string::const_iterator FindMatchingClosingParentheses(string::const_iterator left, string::const_iterator& right)
{
	int parenthesisLevel = 0;

	while (left < right)
	{
		if (*left == '(')
		{
			parenthesisLevel++;
		}
		else if (*left == ')' && --parenthesisLevel == 0)
		{

			return left;
		}
		left++;
	}
	return right;
}

OpIt FindOperator(string::const_iterator & left, string::const_iterator & right)
{
	// check if number first?

	if (*left == '(')
	{
		left = FindMatchingClosingParentheses(left, right) + 1;
		
		return FindOperator(left, right);
	}
	
	auto tempLeft = left;
	auto firstParentheses = right;
	OpIt opBeforeParentheses(right, right);
	OpIt opAfterParentheses(right, right);


	firstParentheses = find(left, right, '(');

	auto opPos = firstParentheses;

	auto opOrderP = begin(opOrder);

	// binary operators left
	do
	{
		opPos = find(left, firstParentheses, *opOrderP);
		opOrderP++;
	} while (opPos == firstParentheses && opOrderP != end(opOrder));

	if (opPos != firstParentheses)
	{
		opBeforeParentheses = OpIt(opPos, opPos + 1);
		// + is first, so no need to look for other operators
		if (*opBeforeParentheses.begOp == '+') return opBeforeParentheses;
	}

	//operators right
	if (firstParentheses != right)
	{
		tempLeft = FindMatchingClosingParentheses(firstParentheses, right) + 1;

		if (tempLeft != right)
		{
			opAfterParentheses = FindOperator(tempLeft, right);

			// + is first, so no need to look for other operators
			if (*opAfterParentheses.begOp == '+') return opAfterParentheses;
		}
	}

	if (opBeforeParentheses.begOp != right && opAfterParentheses.begOp != right)
	{
		if (find(begin(opOrder), end(opOrder), *opBeforeParentheses.begOp) >= find(begin(opOrder), end(opOrder), *opAfterParentheses.begOp))
		{
			return opAfterParentheses;
		}
		else
		{
			return opBeforeParentheses;
		}
	}

	if (opAfterParentheses.begOp != right) return opAfterParentheses;

	if (opBeforeParentheses.begOp != right) return opBeforeParentheses;

	auto unaryOPP = begin(unaryOps);
	auto s = string(left, firstParentheses);
	size_t pos;

	// unary operators
	do
	{
		pos = s.find(*unaryOPP);
	} while (pos == string::npos && ++unaryOPP != end(unaryOps));

	if (pos != string::npos) return OpIt(left + pos, left + pos + size(*unaryOPP));

	return OpIt(right, right);
}

LMR FindLMR(string::const_iterator left, string::const_iterator right)
{
	bool hasOuterParentheses = true;
	string::iterator opPos;
	auto tempLeft = left;

	while (hasOuterParentheses && *left == '(' && *(right - 1) == ')')
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
				tempLeft++;

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
		return LMR(left, FindOperator(tempLeft, right), right);
	}
	else
	{
		return LMR(left, FindOperator(left, right), right);
	}
}

Node* BuildTreeRecursive(LMR const & lmr)
{
	if (lmr.middle.begOp == lmr.right)
	{
		return new TNode<double>(stod(string(lmr.left, lmr.right)), nullptr, nullptr);
	}

	LMR leftLMR;
	LMR rightLMR;

	if (lmr.left == lmr.middle.begOp)
	{
		leftLMR = FindLMR(lmr.middle.endOp, lmr.right);

		if (regex_match(lmr.middle.begOp, lmr.middle.endOp, unaryOperatorRe))
		{
			return new TNode<Oper>(mapOper.at(string(lmr.middle.begOp, lmr.middle.endOp)), BuildTreeRecursive(leftLMR));
		}
	}
	else
	{
		leftLMR = FindLMR(lmr.left, lmr.middle.begOp);

		rightLMR = FindLMR(lmr.middle.endOp, lmr.right);
	}

	if (regex_match(lmr.middle.begOp, lmr.middle.endOp, binaryOperatorRe))
	{
		return new TNode<Oper>(mapOper.at(string(lmr.middle.begOp, lmr.middle.endOp)), BuildTreeRecursive(leftLMR), BuildTreeRecursive(rightLMR));
	}
}

unique_ptr<Node> BuildTree(string const & input)
{
	auto parsedInput = ParseInput(input);
	auto beginInput = begin(parsedInput);
	auto endInput = end(parsedInput);

	return unique_ptr<Node>(BuildTreeRecursive(FindLMR(beginInput, endInput)));
}

double EvalTree(Node* const &node)
{
	auto op = dynamic_cast<TNode<Oper>*>(node);

	if (op != NULL)
	{
		switch (op->GetValue())
		{
		case Oper::Power:
			return pow(EvalTree(node->leftChild), EvalTree(node->rightChild));
		case Oper::Times:
			return EvalTree(node->leftChild) * EvalTree(node->rightChild);
		case Oper::DevidedBy :
			return EvalTree(node->leftChild) / EvalTree(node->rightChild);
		case Oper::Plus:
			return EvalTree(node->leftChild) + EvalTree(node->rightChild);
		case Oper::Negative:
			return -EvalTree(node->leftChild);
		case Oper::Sin:
			return sin(EvalTree(node->leftChild));
		case Oper::Cos:
			return cos(EvalTree(node->leftChild));
		case Oper::Tan:
			return tan(EvalTree(node->leftChild));
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

