#include "Node.h"
#include <vector>
#include <regex>
#include <map>
#include <sstream>
#include <cmath>

using namespace std;

regex const generalRe("\\*|\\/|\\+|\\d+|\\-\\d+");
regex const operatorRe("[\\+\\*\\/\\^]|sin|cos|tan|log");
string const operators = "*/+";
vector<char> const opOrder{ '+', '*', '/', '^' };
vector<string> const unaryOps{ "sin", "cos", "tan" };
map<string, Oper> const mapOper = { {"*", Oper::Times}, { "/", Oper::DevidedBy }, {"^", Oper::Power } , {"+", Oper::Plus }, {"sin", Oper::Sin }, {"cos", Oper::Cos },{"tan", Oper::Tan } };

struct OpIt
{
	string::const_iterator begOp;
	string::const_iterator endOp;

	OpIt(string::const_iterator begOp, string::const_iterator endOp) : begOp{ begOp }, endOp{ endOp } {};
};

struct LMR
{
	string::const_iterator left;
	OpIt middle;
	string::const_iterator right;

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

	/*regex const operMinusRegex("\\w\\-");
	regex const doubleMinusRe("--");
	regex const toBeTransformed("sin|cos|tan|sina|cosa|tana\log|sqrt");

	vector<int> insertPlusPos;

	auto tempInput =  regex_replace(input, doubleMinusRe, "+");

	auto operMinusIt = sregex_token_iterator(begin(tempInput), end(tempInput), operMinusRegex, 0);
	auto regexItEnd = sregex_token_iterator();
	auto a = end(input) - begin(input);

	for_each(operMinusIt, regexItEnd, [&tempInput](auto begEnd) {
		insertPlusPos.push_back(begEnd.first + 1 - begin(tempInput));
		});*/

	//auto toTransformIt = sregex_token_iterator(begin(input), end(input), toBeTransformed, 0);

	//for_each(toTransformIt, regexItEnd, [&input](auto begEnd) {
	//	input.insert(begEnd.first + 1, '+');
	//	});

	//map<string, string> ma = { {"q", "a"} };

	//regex_replace("rewqrw", toBeTransformed, ma["$&"]);



	return ss.str();

	/*return vector<string>(
		sregex_token_iterator(plusInput.begin(), plusInput.end(), generalRe, 0),
		sregex_token_iterator()
	);*/
}

OpIt FindOperator(string::const_iterator left, string::const_iterator right)
{
	// check if number first?

	sregex_token_iterator opPos;

	string::const_iterator parenthesesPos = find(left, right, '(');
	if (parenthesesPos < right)
	{
		right = parenthesesPos;
	}

	auto operatorReIt = sregex_token_iterator(left, right, operatorRe, 0);
	auto regexItEnd = sregex_token_iterator();

	if (operatorReIt != regexItEnd)
	{
		auto opOrderP = begin(opOrder);

		do
		{
			opPos = find(operatorReIt, sregex_token_iterator(), *opOrderP);
			opOrderP++;
		} while (opPos == regexItEnd && opOrderP != end(opOrder));

		if (opPos != regexItEnd)
		{
			return OpIt(opPos->first, opPos->second);
		}
	}
	else
	{
		return OpIt(right, right);
	}
}

LMR FindLMR(string::const_iterator left, string::const_iterator right)
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
		return new TNode<double>(stof(string(lmr.left, lmr.right)), nullptr, nullptr);
	}

	LMR leftLMR = FindLMR(lmr.left, lmr.middle.begOp);

	LMR rightLMR = FindLMR(lmr.middle.endOp, lmr.right);

	if (regex_match(lmr.middle.begOp, lmr.middle.endOp, operatorRe))
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

