// SmartCalculatorConsole.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <chrono>
#include "Node.h"

using namespace std;

template<typename F, typename... Args>
double TimeFunction(int numRuns, F func, Args&&... args)
{
    auto start = chrono::high_resolution_clock::now();

    int i = 0;
    while (i < numRuns)
    {
        func(std::forward<Args>(args)...);
        i++;
    }

    auto duration = chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - start).count();
    cout << "Function executed " << numRuns << " times in " << duration << " microseconds" << endl;
    
    return duration;
}

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

    string s = "(5*(35-(3*874)+99*89))/(5+700-3*9)";
    //string s = "((5+3)/(5-3))*500";

    auto dur = TimeFunction(1000, BuildTree, s);

    auto trunk = BuildTree(s);
    auto dur2 = TimeFunction(1000, EvalTree, trunk);

    auto start = chrono::high_resolution_clock::now();
    int i = 0;
    while (i < 1000)
    {
        find(begin(s), end(s), ')');
        i++;
    }
    find(begin(s), end(s), ')');

    auto duration = chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - start).count();
    cout << "Find executed in " << duration << " microseconds" << endl;

    auto d4 = EvalTree(trunk);


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

    s = "3*3*10";

    trunk = BuildTree(s);

    auto d1 = EvalTree(new TNode<double>(1.5));

    auto d2 = EvalTree(trunk);

    s = "5+13";

    trunk = BuildTree(s);

    auto d3 = EvalTree(trunk);



}
