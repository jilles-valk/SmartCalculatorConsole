#include "pch.h"
#include "../SmartCalculatorConsole/Node.h"

class BasicOperatorEvalTest : public :: testing::Test
{
protected:
	unique_ptr<Node> addOneTwo;
	unique_ptr<Node> minusTwoThree;
	unique_ptr<Node> timesTwoThree;
	unique_ptr<Node> devideSixThree;
	unique_ptr<Node> powerTwoThree;
	
	void SetUp() override
	{
		addOneTwo = make_unique<TNode<Oper>>(TNode<Oper>(Oper::Plus, new TNode<double>(1), new TNode<double>(2)));
		minusTwoThree = make_unique<TNode<Oper>>(TNode<Oper>(Oper::Plus, new TNode<double>(2), new TNode<double>(-3)));
		timesTwoThree = make_unique<TNode<Oper>>(TNode<Oper>(Oper::Times, new TNode<double>(2), new TNode<double>(3)));
		devideSixThree = make_unique<TNode<Oper>>(TNode<Oper>(Oper::DevidedBy, new TNode<double>(6), new TNode<double>(3)));
		powerTwoThree = make_unique<TNode<Oper>>(TNode<Oper>(Oper::Power, new TNode<double>(2), new TNode<double>(4)));
	}

};

TEST(BuildTreeTest, HandlesBuildingAdditionTree) {
	auto result = BuildTree("1+2");

	auto op = dynamic_cast<TNode<Oper>*>(result.get());
	EXPECT_EQ(op->value, Oper::Plus);
	auto lc = dynamic_cast<TNode<double>*>(op->leftChild);
	EXPECT_EQ(lc->value, 1);
	auto rc = dynamic_cast<TNode<double>*>(op->rightChild);
	EXPECT_EQ(rc->value, 2);
}

TEST(BuildTreeTest, HandlesBuildingDoubleMinusTree) {
	auto result = BuildTree("1--2");

	auto op = dynamic_cast<TNode<Oper>*>(result.get());
	EXPECT_EQ(op->value, Oper::Plus);
	auto lc = dynamic_cast<TNode<double>*>(op->leftChild);
	EXPECT_EQ(lc->value, 1);
	auto rc = dynamic_cast<TNode<double>*>(op->rightChild);
	EXPECT_EQ(rc->value, 2);
}

TEST(BuildTreeTest, HandlesBuildingSubtractionTree) {
	auto result = BuildTree("1-2");

	auto op = dynamic_cast<TNode<Oper>*>(result.get());
	EXPECT_EQ(op->value, Oper::Plus);
	auto lc = dynamic_cast<TNode<double>*>(op->leftChild);
	EXPECT_EQ(lc->value, 1);
	auto rc = dynamic_cast<TNode<double>*>(op->rightChild);
	EXPECT_EQ(rc->value, -2);
}

TEST_F(BasicOperatorEvalTest, HandlesAddition)
{
	auto result = EvalTree(addOneTwo.get());
	EXPECT_EQ(result, 3);
}

TEST_F(BasicOperatorEvalTest, HandlesSubtraction)
{
	auto result = EvalTree(minusTwoThree.get());
	EXPECT_EQ(result, -1);
}

TEST_F(BasicOperatorEvalTest, HandlesMultiply)
{
	auto result = EvalTree(timesTwoThree.get());
	EXPECT_EQ(result, 6);
}

TEST_F(BasicOperatorEvalTest, HandlesDevide)
{
	auto result = EvalTree(devideSixThree.get());
	EXPECT_EQ(result, 2);
}

TEST_F(BasicOperatorEvalTest, HandlesPower)
{
	auto result = EvalTree(powerTwoThree.get());
	EXPECT_EQ(result, 16);
}

TEST(BracketTreeBuildingTest, HandlesSinglePointlessBrackets)
{
	auto result = BuildTree("(1-2)");

	auto op = dynamic_cast<TNode<Oper>*>(result.get());
	EXPECT_EQ(op->value, Oper::Plus);
	auto lc = dynamic_cast<TNode<double>*>(op->leftChild);
	EXPECT_EQ(lc->value, 1);
	auto rc = dynamic_cast<TNode<double>*>(op->rightChild);
	EXPECT_EQ(rc->value, -2);
}

TEST(BracketTreeBuildingTest, HandlesMultipleNestedPointlessBrackets)
{
	auto result = BuildTree("(((((1-2)))))");

	auto op = dynamic_cast<TNode<Oper>*>(result.get());
	EXPECT_EQ(op->value, Oper::Plus);
	auto lc = dynamic_cast<TNode<double>*>(op->leftChild);
	EXPECT_EQ(lc->value, 1);
	auto rc = dynamic_cast<TNode<double>*>(op->rightChild);
	EXPECT_EQ(rc->value, -2);
}

TEST(BracketTreeBuildingTest, HandlesMultipleBrackets)
{
	auto result = BuildTree("(1-2)*(5+5)");

	auto op = dynamic_cast<TNode<Oper>*>(result.get());
	EXPECT_EQ(op->value, Oper::Times);
	auto lc = dynamic_cast<TNode<Oper>*>(op->leftChild);
	EXPECT_EQ(lc->value, Oper::Plus);
	auto lcrc = dynamic_cast<TNode<double>*>(lc->rightChild);
	EXPECT_EQ(lcrc->value, -2);
	auto rc = dynamic_cast<TNode<Oper>*>(op->rightChild);
	EXPECT_EQ(rc->value, Oper::Plus);
}

TEST(BracketTreeBuildingTest, HandlesMultipleNestedBrackets)
{
	auto result = BuildTree("((1-2)*(5+5))/5");

	auto op = dynamic_cast<TNode<Oper>*>(result.get());
	EXPECT_EQ(op->value, Oper::DevidedBy);
	auto lc = dynamic_cast<TNode<Oper>*>(op->leftChild);
	EXPECT_EQ(lc->value, Oper::Times);
	auto lclc = dynamic_cast<TNode<Oper>*>(lc->leftChild);
	EXPECT_EQ(lclc->value, Oper::Plus);
	auto lclcrc = dynamic_cast<TNode<double>*>(lclc->rightChild);
	EXPECT_EQ(lclcrc->value, -2);
	auto lcrc = dynamic_cast<TNode<Oper>*>(lc->rightChild);
	EXPECT_EQ(lcrc->value, Oper::Plus);
	auto rc = dynamic_cast<TNode<double>*>(op->rightChild);
	EXPECT_EQ(rc->value, 5);
}