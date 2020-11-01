#include "Node.h"
#include <vector>
#include <regex>
#include <map>

using namespace std;

regex const operatorRe("\\*|\\/");
regex const generalRe("\\*|\\/|\\d+");
map<string, Oper> mapOper = { {"*", Oper::Times}, { "/", Oper::DevidedBy }, {"^", Oper::Power } };

vector<string> ParseInput(string input)
{
	return vector<string>(
		sregex_token_iterator(input.begin(), input.end(), generalRe, 0),
		sregex_token_iterator()
	);
}

vector<string>::iterator FindMiddle(vector<string>::iterator left, vector<string>::iterator right)
{
	return find_if(left, right, [](string elem) {return regex_match(elem, operatorRe); });
}

unique_ptr<Node> BuildTreeRecursive(vector<string>::iterator left, vector<string>::iterator middle, vector<string>::iterator right)
{
	if (middle == right)
	{
		return make_unique<Node>(Node(stof(*left), nullptr, nullptr));
	}

	if (regex_match(*middle, operatorRe))
	{
		return make_unique<Node>(Node(mapOper[*middle], BuildTreeRecursive(left, FindMiddle(left, middle - 1), middle - 1), BuildTreeRecursive(middle + 1, FindMiddle(middle + 1, right), right)));
	}

	if (middle == left && middle == right)
	{
		return make_unique<Node>(Node(stof(*middle), nullptr, nullptr));
	}
	
	if (middle == left)
	{
		return make_unique<Node>(Node(stof(*middle), nullptr, BuildTreeRecursive(middle++, middle += 2, left)));
	}

	if (middle == right)
	{
		return make_unique<Node>(Node(stof(*(--middle)), BuildTreeRecursive(left, middle -= 2, middle), nullptr));
	}

}

unique_ptr<Node> BuildTree(string& input)
{
	auto seperatedInput = ParseInput(input);

	return BuildTreeRecursive(begin(seperatedInput), FindMiddle(begin(seperatedInput), end(seperatedInput)), end(seperatedInput));
}

