// SmartCalculatorConsole.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include "Node.h"

using namespace std;

int main()
{
    string input = "";
    cout << "Enter input for calculation:" << endl;
    //cin >> input;

    cout << "You entered: " << input << endl; 
    auto a = make_unique<string>("hello");

    OperatorNode n3 = OperatorNode("*", make_unique<BaseNode>(OperatorNode("+")));

    OperatorNode n4 = OperatorNode("*", make_unique<BaseNode>(VariableNode(1)), make_unique<BaseNode>(VariableNode(2)));

    string s = "1*2*45*6";

    auto trunk = BuildTree(s);

}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
