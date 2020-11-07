#include "pch.h"
#include "../SmartCalculatorConsole/Node.h"

TEST(BuildTreeTest, AddTwoNumbers) {
	string input = "1+2";
	auto result = BuildTree(input);
	unique_ptr<Node> expected = make_unique<TNode<Oper>>(TNode<Oper>(Oper::Plus, new TNode<double>(1), new TNode<double>(2)));

	auto op = dynamic_cast<TNode<Oper>*>(result.get());
	EXPECT_EQ(op->value, Oper::Plus);
	auto lc = dynamic_cast<TNode<double>*>(op->leftChild);
	EXPECT_EQ(lc->value, 1);
	auto rc = dynamic_cast<TNode<double>*>(op->rightChild);
	EXPECT_EQ(rc->value, 2);
}

TEST(EvalTreeTest, AddTwoNumbers)
{
	unique_ptr<Node> expected = make_unique<TNode<Oper>>(TNode<Oper>(Oper::Plus, new TNode<double>(1), new TNode<double>(2)));
	auto result = EvalTree(expected.get());
	EXPECT_EQ(result, 3);
}
