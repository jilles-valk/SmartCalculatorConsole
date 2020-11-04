// SmartCalculatorConsole.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include "Node.h"

using namespace std;

int main()
{
    while (true)
    {
        string input = "";

        std::cout << "Enter input for calculation:" << endl;
        cin >> input;

        if (input == "q") break;

        auto tree = BuildTree(input);

        auto res = EvalTree(tree);

        cout << input << " = " << res << endl;
    }


    Node* bp = new Node();
    Node* op = new TNode<string>("*");

    Node* op1 = new TNode<string>("*");

    //auto s1 = (*op1).GetValue();

    TNode<string>* opb = dynamic_cast<TNode<string>*>(op);

    auto s1 = (*opb).GetValue();

    Node* tb = new TNode<string>("/", new TNode<int>(1), new TNode<int>(2));

    bp = tb;

    Node* bp1 = tb;

    auto a = make_unique<string>("hello");

    string s = "3*3*10";

    auto trunk = BuildTree(s);

    auto d1 = EvalTree(new TNode<double>(1.5));

    auto d2 = EvalTree(trunk);

    s = "5+13";

    trunk = BuildTree(s);

    auto d3 = EvalTree(trunk);

    s = "5-3-3+5-5";

    trunk = BuildTree(s);

    auto d4 = EvalTree(trunk);

}
