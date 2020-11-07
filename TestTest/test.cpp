#include "pch.h"
#include "../SmartCalculatorConsole/Node.h"

TEST(BuildTreeTest, AddTwoNumbers) {
	string input = "1+2";
	//auto result = BuildTree(input);
	//auto au = make_unique<Node>(TNode<Oper>(Oper::Plus, new TNode<double>(1), new TNode<double>(2)));
	unique_ptr<Node> expected = make_unique<Node>(TNode<Oper>(Oper::Plus, new TNode<double>(1), new TNode<double>(2)));
	//EXPECT_EQ(au, result);
}

TEST(EvalTreeTest, AddTwoNumbers)
{
	unique_ptr<Node> expected = make_unique<Node>(TNode<Oper>(Oper::Plus, new TNode<double>(1), new TNode<double>(2)));
	auto result = EvalTree(expected.get());
	EXPECT_EQ(result, 3);
}
