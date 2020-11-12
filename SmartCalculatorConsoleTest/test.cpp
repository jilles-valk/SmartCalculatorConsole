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
	unique_ptr<Node> sinHalfPi;
	
	void SetUp() override
	{
		addOneTwo = make_unique<TNode<Oper>>(TNode<Oper>(Oper::Plus, new TNode<double>(1), new TNode<double>(2)));
		minusTwoThree = make_unique<TNode<Oper>>(TNode<Oper>(Oper::Plus, new TNode<double>(2), new TNode<double>(-3)));
		timesTwoThree = make_unique<TNode<Oper>>(TNode<Oper>(Oper::Times, new TNode<double>(2), new TNode<double>(3)));
		devideSixThree = make_unique<TNode<Oper>>(TNode<Oper>(Oper::DevidedBy, new TNode<double>(6), new TNode<double>(3)));
		powerTwoThree = make_unique<TNode<Oper>>(TNode<Oper>(Oper::Power, new TNode<double>(2), new TNode<double>(4)));
		sinHalfPi = make_unique<TNode<Oper>>(TNode<Oper>(Oper::Sin, new TNode<double>(M_PI_2)));
	}

};

TEST(BuildTreeTest, HandlesBuildingAdditionTree) {
	auto result = BuildTree("1+2");

	auto op = dynamic_cast<TNode<Oper>*>(result.get());
	ASSERT_TRUE(op != NULL);
	EXPECT_EQ(op->value, Oper::Plus);
	auto lc = dynamic_cast<TNode<double>*>(op->leftChild);
	ASSERT_TRUE(lc != NULL);
	EXPECT_DOUBLE_EQ(lc->value, 1);
	auto rc = dynamic_cast<TNode<double>*>(op->rightChild);
	ASSERT_TRUE(rc != NULL);
	EXPECT_DOUBLE_EQ(rc->value, 2);
}

TEST(BuildTreeTest, HandlesBuildingDoubleMinusTree) {
	auto result = BuildTree("1--2");

	auto op = dynamic_cast<TNode<Oper>*>(result.get());
	ASSERT_TRUE(op != NULL);
	EXPECT_EQ(op->value, Oper::Plus);
	auto lc = dynamic_cast<TNode<double>*>(op->leftChild);
	ASSERT_TRUE(lc != NULL);
	EXPECT_DOUBLE_EQ(lc->value, 1);
	auto rc = dynamic_cast<TNode<double>*>(op->rightChild);
	ASSERT_TRUE(rc != NULL);
	EXPECT_DOUBLE_EQ(rc->value, 2);
}

TEST(BuildTreeTest, HandlesBuildingSubtractionTree) {
	auto result = BuildTree("1-2");

	auto op = dynamic_cast<TNode<Oper>*>(result.get());
	ASSERT_TRUE(op != NULL);
	EXPECT_EQ(op->value, Oper::Plus);
	auto lc = dynamic_cast<TNode<double>*>(op->leftChild);
	ASSERT_TRUE(lc != NULL);
	EXPECT_DOUBLE_EQ(lc->value, 1);
	auto rc = dynamic_cast<TNode<Oper>*>(op->rightChild);
	ASSERT_TRUE(rc != NULL);
	ASSERT_TRUE(rc != NULL);
	EXPECT_EQ(rc->value, Oper::Negative);
	auto rclc = dynamic_cast<TNode<double>*>(rc->leftChild);
	ASSERT_TRUE(rc != NULL);
	EXPECT_DOUBLE_EQ(rclc->value, 2);
}

TEST(BuildTreeTest, HandlesBuildingCosTree) {
	auto result = BuildTree("cos(1.57)");

	auto op = dynamic_cast<TNode<Oper>*>(result.get());
	ASSERT_TRUE(op != NULL);
	EXPECT_EQ(op->value, Oper::Cos);
	auto lc = dynamic_cast<TNode<double>*>(op->leftChild);
	ASSERT_TRUE(lc != NULL);
	EXPECT_DOUBLE_EQ(lc->value, 1.57);
}

TEST(BuildTreeTest, HandlesBuildingCosTreeBinaryBehind) {
	auto result = BuildTree("cos(1.57)+3");

	auto op = dynamic_cast<TNode<Oper>*>(result.get());
	ASSERT_TRUE(op != NULL);
	EXPECT_EQ(op->value, Oper::Plus);
	auto lc = dynamic_cast<TNode<Oper>*>(op->leftChild);
	ASSERT_TRUE(lc != NULL);
	EXPECT_EQ(lc->value, Oper::Cos);
}

TEST_F(BasicOperatorEvalTest, HandlesAddition)
{
	auto result = EvalTree(addOneTwo.get());
	EXPECT_DOUBLE_EQ(result, 3);
}

TEST_F(BasicOperatorEvalTest, HandlesSubtraction)
{
	auto result = EvalTree(minusTwoThree.get());
	EXPECT_DOUBLE_EQ(result, -1);
}

TEST_F(BasicOperatorEvalTest, HandlesMultiply)
{
	auto result = EvalTree(timesTwoThree.get());
	EXPECT_DOUBLE_EQ(result, 6);
}

TEST_F(BasicOperatorEvalTest, HandlesDevide)
{
	auto result = EvalTree(devideSixThree.get());
	EXPECT_DOUBLE_EQ(result, 2);
}

TEST_F(BasicOperatorEvalTest, HandlesPower)
{
	auto result = EvalTree(powerTwoThree.get());
	EXPECT_DOUBLE_EQ(result, 16);
}

TEST_F(BasicOperatorEvalTest, HandlesSin)
{
	auto result = EvalTree(sinHalfPi.get());
	EXPECT_DOUBLE_EQ(result, 1);
}

TEST(ParenthesesTreeBuildingTest, HandlesSinglePointlessParentheses)
{
	auto result = BuildTree("(1-2)");

	auto op = dynamic_cast<TNode<Oper>*>(result.get());
	ASSERT_TRUE(op != NULL);
	EXPECT_EQ(op->value, Oper::Plus);
	auto lc = dynamic_cast<TNode<double>*>(op->leftChild);
	ASSERT_TRUE(lc != NULL);
	EXPECT_EQ(lc->value, 1);
	auto rc = dynamic_cast<TNode<Oper>*>(op->rightChild);
	ASSERT_TRUE(rc != NULL);
	EXPECT_EQ(rc->value, Oper::Negative);
	auto rclc = dynamic_cast<TNode<double>*>(rc->leftChild);
	ASSERT_TRUE(rclc != NULL);
	EXPECT_EQ(rclc->value, 2);
}

TEST(ParenthesesTreeBuildingTest, HandlesMultipleNestedPointlessParentheses)
{
	auto result = BuildTree("(((((1-2)))))");

	auto op = dynamic_cast<TNode<Oper>*>(result.get());
	ASSERT_TRUE(op != NULL);
	EXPECT_EQ(op->value, Oper::Plus);
	auto lc = dynamic_cast<TNode<double>*>(op->leftChild);
	ASSERT_TRUE(lc != NULL);
	EXPECT_EQ(lc->value, 1);
	auto rc = dynamic_cast<TNode<Oper>*>(op->rightChild);
	ASSERT_TRUE(rc != NULL);
	EXPECT_EQ(rc->value, Oper::Negative);
	auto rclc = dynamic_cast<TNode<double>*>(rc->leftChild);
	ASSERT_TRUE(rclc != NULL);
	EXPECT_EQ(rclc->value, 2);
}

TEST(ParenthesesTreeBuildingTest, HandlesMultipleParentheses)
{
	auto result = BuildTree("(1-2)*(5+5)");

	auto op = dynamic_cast<TNode<Oper>*>(result.get());
	ASSERT_TRUE(op != NULL);
	EXPECT_EQ(op->value, Oper::Times);
	auto lc = dynamic_cast<TNode<Oper>*>(op->leftChild);
	ASSERT_TRUE(lc != NULL);
	EXPECT_EQ(lc->value, Oper::Plus);
	auto lcrc = dynamic_cast<TNode<Oper>*>(lc->rightChild);
	ASSERT_TRUE(lcrc != NULL);
	EXPECT_EQ(lcrc->value, Oper::Negative);
	auto rc = dynamic_cast<TNode<Oper>*>(op->rightChild);
	ASSERT_TRUE(rc != NULL);
	EXPECT_EQ(rc->value, Oper::Plus);
}

TEST(ParenthesesTreeBuildingTest, HandlesNegativeParentheses)
{
	auto result = BuildTree("-(5+3)");

	auto op = dynamic_cast<TNode<Oper>*>(result.get());
	ASSERT_TRUE(op != NULL);
	EXPECT_EQ(op->value, Oper::Negative);
	auto lc = dynamic_cast<TNode<Oper>*>(op->leftChild);
	ASSERT_TRUE(lc != NULL);
	EXPECT_EQ(lc->value, Oper::Plus);
	auto lclc = dynamic_cast<TNode<double>*>(lc->rightChild);
	ASSERT_TRUE(lclc != NULL);
	EXPECT_EQ(lclc->value, 3);
}

TEST(ParenthesesTreeBuildingTest, HandlesMultipleNestedParentheses)
{
	auto result = BuildTree("((1-2)*(5+5))/5");

	auto op = dynamic_cast<TNode<Oper>*>(result.get());
	ASSERT_TRUE(op != NULL);
	EXPECT_EQ(op->value, Oper::DevidedBy);
	auto lc = dynamic_cast<TNode<Oper>*>(op->leftChild);
	ASSERT_TRUE(lc != NULL);
	EXPECT_EQ(lc->value, Oper::Times);
	auto lclc = dynamic_cast<TNode<Oper>*>(lc->leftChild);
	ASSERT_TRUE(lclc != NULL);
	EXPECT_EQ(lclc->value, Oper::Plus);
	auto lclcrc = dynamic_cast<TNode<Oper>*>(lclc->rightChild);
	ASSERT_TRUE(lclcrc != NULL);
	EXPECT_EQ(lclcrc->value, Oper::Negative);
	auto lcrc = dynamic_cast<TNode<Oper>*>(lc->rightChild);
	ASSERT_TRUE(lcrc != NULL);
	EXPECT_EQ(lcrc->value, Oper::Plus);
	auto rc = dynamic_cast<TNode<double>*>(op->rightChild);
	ASSERT_TRUE(rc != NULL);
	EXPECT_EQ(rc->value, 5);
}

TEST(ParenthesesTreeBuildingTest, HandlesMultipleParenthesesWithUnary)
{
	auto result = BuildTree("(1-2)*cos(2*3.14)");

	auto op = dynamic_cast<TNode<Oper>*>(result.get());
	ASSERT_TRUE(op != NULL);
	EXPECT_EQ(op->value, Oper::Times);
	auto lc = dynamic_cast<TNode<Oper>*>(op->leftChild);
	ASSERT_TRUE(lc != NULL);
	EXPECT_EQ(lc->value, Oper::Plus);
	auto lcrc = dynamic_cast<TNode<Oper>*>(lc->rightChild);
	ASSERT_TRUE(lcrc != NULL);
	EXPECT_EQ(lcrc->value, Oper::Negative);
	auto rc = dynamic_cast<TNode<Oper>*>(op->rightChild);
	ASSERT_TRUE(rc != NULL);
	EXPECT_EQ(rc->value, Oper::Cos);
	auto rcrc = dynamic_cast<TNode<Oper>*>(rc->leftChild);
	ASSERT_TRUE(rcrc != NULL);
	EXPECT_EQ(rcrc->value, Oper::Times);
	auto rcrcrc = dynamic_cast<TNode<double>*>(rcrc->rightChild);
	ASSERT_TRUE(rcrcrc != NULL);
	EXPECT_EQ(rcrcrc->value, 3.14);
}

TEST(NondebugableComplexMathBuildEvalTest, HandlesManyPlusMinus)
{
	auto tree = BuildTree("5+3-5-5+9+9-4-2-5+8+2354-54+8+2+54-5--54");
	auto result = EvalTree(tree.get());

	EXPECT_DOUBLE_EQ(result, 2426);
}

TEST(NondebugableComplexMathBuildEvalTest, HandlesManyBinaryOperators)
{
	auto tree = BuildTree("(5*(35-(3*874)+99*89))/(5+700-3*9)");
	auto result = EvalTree(tree.get());

	EXPECT_DOUBLE_EQ(result, 45.89970501474926);
}
