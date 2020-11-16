#include <iostream>
#include <string>
#include <chrono>
#include <sstream>
#include <iomanip>
#include "Tree.h"

using namespace std;

static string const operators = "*/+";

template<typename F, typename... Args>
double TimeFunction(int numRuns, F& const func, Args&&... args)
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
    }

    string s = "(5*(35-(3*874)+99*89))/(5+700-3*9)/sin(2)";

    tree = Tree(s);

    tree.Build();
    
    auto dur = TimeFunction(1000, BuildTree, tree.parsedInput);

    auto dur2 = TimeFunction(1000, EvalTree, tree.trunk.get());

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
}
