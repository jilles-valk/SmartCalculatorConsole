#include "pch.h"
#include "Tree.h"
#include "Function.h"

class BasicOperatorEvalTest : public :: testing::Test
{
protected:
	std::unique_ptr<Node> addOneTwo;
	std::unique_ptr<Node> minusTwoThree;
	std::unique_ptr<Node> timesTwoThree;
	std::unique_ptr<Node> devideSixThree;
	std::unique_ptr<Node> powerTwoThree;
	std::unique_ptr<Node> sinHalfPi;
	
	void SetUp() override
	{
		addOneTwo = std::make_unique<TNode<Oper>>(TNode<Oper>(Oper::Plus, new TNode<double>(1), new TNode<double>(2)));
		minusTwoThree = std::make_unique<TNode<Oper>>(TNode<Oper>(Oper::Plus, new TNode<double>(2), new TNode<double>(-3)));
		timesTwoThree = std::make_unique<TNode<Oper>>(TNode<Oper>(Oper::Times, new TNode<double>(2), new TNode<double>(3)));
		devideSixThree = std::make_unique<TNode<Oper>>(TNode<Oper>(Oper::DevidedBy, new TNode<double>(6), new TNode<double>(3)));
		powerTwoThree = std::make_unique<TNode<Oper>>(TNode<Oper>(Oper::Power, new TNode<double>(2), new TNode<double>(4)));
		sinHalfPi = std::make_unique<TNode<Oper>>(TNode<Oper>(Oper::Sin, new TNode<double>(M_PI_2)));
	}

};

TEST(BuildTreeTest, HandlesBuildingAdditionTree) {
	Tree tree = Tree("1+2");
	tree.Build();

	auto op = dynamic_cast<TNode<Oper>*>(tree.trunk.get());
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
	Tree tree = Tree("1--2");
	tree.Build();

	auto op = dynamic_cast<TNode<Oper>*>(tree.trunk.get());
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
	Tree tree = Tree("1-2");
	tree.Build();

	auto op = dynamic_cast<TNode<Oper>*>(tree.trunk.get());
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
	Tree tree = Tree("cos(1.57)");
	tree.Build();

	auto op = dynamic_cast<TNode<Oper>*>(tree.trunk.get());
	ASSERT_TRUE(op != NULL);
	EXPECT_EQ(op->value, Oper::Cos);
	auto lc = dynamic_cast<TNode<double>*>(op->leftChild);
	ASSERT_TRUE(lc != NULL);
	EXPECT_DOUBLE_EQ(lc->value, 1.57);
}

TEST(BuildTreeTest, HandlesBuildingCosTreeBinaryBehind) {
	Tree tree = Tree("cos(1.57)+3");
	tree.Build();

	auto op = dynamic_cast<TNode<Oper>*>(tree.trunk.get());
	ASSERT_TRUE(op != NULL);
	EXPECT_EQ(op->value, Oper::Plus);
	auto lc = dynamic_cast<TNode<Oper>*>(op->leftChild);
	ASSERT_TRUE(lc != NULL);
	EXPECT_EQ(lc->value, Oper::Cos);
}

TEST(ParenthesesTreeBuildingTest, HandlesSinglePointlessParentheses)
{
	Tree tree = Tree("(1-2)");
	tree.Build();

	auto op = dynamic_cast<TNode<Oper>*>(tree.trunk.get());
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
	Tree tree = Tree("(((((1-2)))))");
	tree.Build();

	auto op = dynamic_cast<TNode<Oper>*>(tree.trunk.get());
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
	Tree tree = Tree("(1-2)*(5+5)");
	tree.Build();

	auto op = dynamic_cast<TNode<Oper>*>(tree.trunk.get());
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
	Tree tree = Tree("-(5+3)");
	tree.Build();

	auto op = dynamic_cast<TNode<Oper>*>(tree.trunk.get());
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
	Tree tree = Tree("((1-2)*(5+5))/5");
	tree.Build();

	auto op = dynamic_cast<TNode<Oper>*>(tree.trunk.get());
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
	Tree tree = Tree("(1-2)*cos(2*3.14)");
	tree.Build();

	auto op = dynamic_cast<TNode<Oper>*>(tree.trunk.get());
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
	Tree tree = Tree("5+3-5-5+9+9-4-2-5+8+2354-54+8+2+54-5--54");
	tree.Build();
	auto result = tree.Eval();

	EXPECT_DOUBLE_EQ(result, 2426);
}

TEST(NondebugableComplexMathBuildEvalTest, HandlesManyBinaryOperators)
{
	Tree tree = Tree("(5*(35-(3*874)+99*89))/(5+700-3*9)");
	tree.Build();
	auto result = tree.Eval();

	EXPECT_DOUBLE_EQ(result, 45.89970501474926);
}

TEST(BuildFunctionTreeTest, HandlesOneVariable)
{
	Tree tree = Tree("2*x^2+3*x-5");
	tree.Build();
	ASSERT_TRUE(tree.IsFunction());
}

TEST(BuildFunctionTreeTest, GetOneVariable)
{
	Tree tree = Tree("2*x^2+3*x-5");
	tree.Build();
	ASSERT_TRUE(tree.GetVariables());

	auto var = dynamic_cast<TNode<std::string>*>(*tree.variables["x"][0]);
	ASSERT_TRUE(var != NULL);
	EXPECT_EQ(var->value, "x");

	var = dynamic_cast<TNode<std::string>*>(*tree.variables["x"][1]);
	ASSERT_TRUE(var != NULL);
	EXPECT_EQ(var->value, "x");
}

TEST(BuildFunctionTreeTest, GetMultipleVariables)
{
	Tree tree = Tree("2*x^2+3*y-z");
	tree.Build();
	ASSERT_TRUE(tree.GetVariables());

	auto var = dynamic_cast<TNode<std::string>*>(*tree.variables["x"][0]);
	ASSERT_TRUE(var != NULL);
	EXPECT_EQ(var->value, "x");

	var = dynamic_cast<TNode<std::string>*>(*tree.variables["y"][0]);
	ASSERT_TRUE(var != NULL);
	EXPECT_EQ(var->value, "y");

	var = dynamic_cast<TNode<std::string>*>(*tree.variables["z"][0]);
	ASSERT_TRUE(var != NULL);
	EXPECT_EQ(var->value, "z");
}

TEST(BuildFunctionTreeTest, GetVaryingVariable)
{
	Tree tree = Tree("aa+ab+ac+aab");
	tree.Build();
	ASSERT_TRUE(tree.GetVariables());

	EXPECT_EQ(tree.GetVaryingVariableName(), "ac");
}

TEST(BuildFunctionTreeTest, GetMostLikelyVarying)
{
	Tree tree = Tree("sqrt(1+2*3)");
	tree.Build();

	auto var = dynamic_cast<TNode<double>*>(*tree.variables["default"][0]);

	ASSERT_TRUE(var != NULL);
	EXPECT_DOUBLE_EQ(var->value, 3);
}

TEST(SetFunctionTreeTest, SetOneVariable)
{
	Tree tree = Tree("2*x^2+3*x-5");
	tree.Build();
	ASSERT_TRUE(tree.SetVariable(std::make_pair<std::string, double>("x", 0)));

	auto var = dynamic_cast<TNode<double>*>(*tree.variables["x"][0]);
	ASSERT_TRUE(var != NULL);
	EXPECT_EQ(var->value, 0);

	var = dynamic_cast<TNode<double>*>(*tree.variables["x"][1]);
	ASSERT_TRUE(var != NULL);
	EXPECT_EQ(var->value, 0);

	double result = tree.Eval();
	ASSERT_DOUBLE_EQ(result, -5);
}

TEST(SetFunctionTreeTest, SetOneVariableTwice)
{
	Tree tree = Tree("2*x^2+3*x-5");
	tree.Build();
	ASSERT_TRUE(tree.SetVariable(std::make_pair<std::string, double>("x", 0)));

	auto var = dynamic_cast<TNode<double>*>(*tree.variables["x"][0]);
	ASSERT_TRUE(var != NULL);
	EXPECT_EQ(var->value, 0);

	double result = tree.Eval();
	ASSERT_DOUBLE_EQ(result, -5);

	ASSERT_TRUE(tree.SetVariable(std::make_pair<std::string, double>("x", 1)));

    result = tree.Eval();
	ASSERT_DOUBLE_EQ(result, 0);
}

TEST(SetFunctionTreeTest, SetMultipleVariables)
{
	Tree tree = Tree("2*x^2+3*y-z");
	tree.Build();

	auto map = std::unordered_map<std::string, double>{ {"x", 4}, { "y", 5 }, { "z", 6 }};
	ASSERT_TRUE(tree.SetVariables(map));

	auto var = dynamic_cast<TNode<double>*>(*tree.variables["x"][0]);
	ASSERT_TRUE(var != NULL);
	EXPECT_DOUBLE_EQ(var->value, 4);

	var = dynamic_cast<TNode<double>*>(*tree.variables["y"][0]);
	ASSERT_TRUE(var != NULL);
	EXPECT_DOUBLE_EQ(var->value, 5);

	var = dynamic_cast<TNode<double>*>(*tree.variables["z"][0]);
	ASSERT_TRUE(var != NULL);
	EXPECT_DOUBLE_EQ(var->value, 6);
}

TEST(GraphingTest, PlotDefaultView)
{
	Tree tree = Tree("2*x");
	tree.Build();

	Function f = Function(tree);
	ASSERT_TRUE(f.MakeGraph());

	EXPECT_DOUBLE_EQ(f.graph[0].x, -10);
	EXPECT_DOUBLE_EQ(f.graph[0].y, -20);
	EXPECT_DOUBLE_EQ(f.graph[f.graph.size() - 1].x, 10);
	EXPECT_DOUBLE_EQ(f.graph[f.graph.size() - 1].y, 20);
}


