/*
    *** DO NOT MODIFY THIS FILE ***
    
    Functions in knapsack.cpp must match these prototypes.
    This is how they will be called in lab5.cpp
    However - you may use as many helper functions as
    you want in knapsack.cpp.
*/

#include <vector>
using namespace std;

int knapsackBF(int weight, vector<pair<int, int>> & items, vector<pair<int, int>> & taken);
int knapsackDYN(int weight, vector<pair<int, int>> & items, vector<pair<int, int>> & taken);
