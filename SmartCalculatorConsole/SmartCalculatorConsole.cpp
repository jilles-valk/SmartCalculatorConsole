#include <iostream>
#include <string>
#include <chrono>
#include <sstream>
#include <iomanip>
#include "Tree.h"

using namespace std;

static string const operators = "*/+";

template<typename F, typename... Args>
double TimeFunction(int numRuns, string name, F const & func, Args&&... args)
{
    auto start = chrono::high_resolution_clock::now();

    int i = 0;
    while (i < numRuns)
    {
        func(std::forward<Args>(args)...);
        i++;
    }

    double duration = chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - start).count();
    cout << "Function " << name << " executed " << numRuns << " times in " << duration << " microseconds" << endl;
    
    return duration;
}

int main()
{
    Tree tree;

    while (true)
    {
        string input = "";

        std::cout << "Enter input for calculation:" << endl;
        cin >> input;

        if (input == "q") break;

        tree = Tree(input);
        tree.Build();

        auto res = tree.Eval();

        cout << input << " = " << std::setprecision(15) << res << endl;

        auto dur = TimeFunction(1000, "BuildTree", BuildTree, tree.parsedInput);

        auto dur2 = TimeFunction(1000, "EvalTree", EvalTree, tree.trunk.get());
    }

    string s = "(5*(35-(3*874)+99*89))/(5+700-3*9)/sin(2)";

    tree = Tree(s);

    tree.Build();
    
    auto dur = TimeFunction(1000, "BuildTree", BuildTree, tree.parsedInput);

    auto dur2 = TimeFunction(1000, "EvalTree", EvalTree, tree.trunk.get());
}
