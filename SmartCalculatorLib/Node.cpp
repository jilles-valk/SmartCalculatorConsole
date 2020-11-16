#define _USE_MATH_DEFINES

#include "Node.h"
#include "Exceptions.h"
#include <vector>
#include <regex>
#include <unordered_map>
#include <cmath>
#include <exception>

using namespace std;

static regex const binaryOperatorRe("[\\+\\*\\/\\^]");
static regex const unaryOperatorRe("-|sin|cos|tan|log");

static vector<char> const opOrder{ '+', '*', '/', '^' };
static vector<string> const unaryOps{ "-", "sin", "cos", "tan" };
static unordered_map<string, Oper> const mapUnaryOper = { {"-", Oper::Negative }, {"sin", Oper::Sin }, {"cos", Oper::Cos },{"tan", Oper::Tan } };
static unordered_map<string, Oper> const mapBinaryOper = { {"*", Oper::Times}, { "/", Oper::DevidedBy }, {"^", Oper::Power } , {"+", Oper::Plus } };



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
	auto tempLeft = left;

	if (*left == '(')
	{
		tempLeft = FindMatchingClosingParentheses(left, right);

		if (tempLeft != right)
		{
			left = tempLeft + 1;
		}
		else
		{
			throw ParenthesesException(left, left + 1);
		}
	}
	
	auto firstParentheses = right;
	OpIt opBeforeParentheses(right, right);

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

	OpIt opAfterParentheses(right, right);

	//operators right
	if (firstParentheses != right)
	{
		tempLeft = FindMatchingClosingParentheses(firstParentheses, right);

		if (tempLeft == right)
		{
			throw ParenthesesException(firstParentheses, firstParentheses + 1);
		}

		tempLeft++;

		if (tempLeft != right)
		{
			opAfterParentheses = FindOperator(tempLeft, right);

			if (opAfterParentheses.begOp == right)
			{
				throw ValueParseException(tempLeft, right);
			}
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
		try
		{
			return new TNode<double>(stod(string(lmr.left, lmr.right)), nullptr, nullptr);
		}
		catch (invalid_argument const& ia)
		{
			throw ValueParseException(lmr.left, lmr.right);
		}
	}

	LMR leftLMR;
	LMR rightLMR;
	string oper;

	if (lmr.left == lmr.middle.begOp)
	{
		if (lmr.middle.endOp != lmr.right)
		{
			leftLMR = FindLMR(lmr.middle.endOp, lmr.right);
		}
		else
		{
			throw ValueParseException(lmr.middle.endOp, lmr.right);
		}

		oper = string(lmr.middle.begOp, lmr.middle.endOp);

		if (mapUnaryOper.count(oper) > 0)
		{
			return new TNode<Oper>(mapUnaryOper.at(oper), BuildTreeRecursive(leftLMR));
		}
		else
		{
			throw NoValidOperatorException(lmr.middle.begOp, lmr.middle.endOp);
		}
	}
	else
	{
		leftLMR = FindLMR(lmr.left, lmr.middle.begOp);

		if (lmr.middle.endOp != lmr.right)
		{
			rightLMR = FindLMR(lmr.middle.endOp, lmr.right);
		}
		else
		{
			throw NoRightValueForOperator(lmr.middle.begOp, lmr.middle.endOp);
		}
	}

	oper = string(lmr.middle.begOp, lmr.middle.endOp);

	if (mapBinaryOper.count(oper) > 0)
	{
		return new TNode<Oper>(mapBinaryOper.at(oper), BuildTreeRecursive(leftLMR), BuildTreeRecursive(rightLMR));
	}
	else
	{
		throw NoValidOperatorException(lmr.middle.begOp, lmr.middle.endOp);
	}
}

unique_ptr<Node> BuildTree(string const & input)
{
	return unique_ptr<Node>(BuildTreeRecursive(FindLMR(cbegin(input), cend(input))));
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

	return nan("");
}
