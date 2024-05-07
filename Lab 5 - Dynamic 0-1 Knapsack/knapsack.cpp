/*
    Fill in this block comment before submitting.
    Name 1: Hai Ning Liu
    Name 2: None, I work alone
    If you worked alone, indicate that as well.    
*/

#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;

vector<pair<int, int>> knapsackBFHelper(int weight, int i, vector<pair<int, int>> items, vector<pair<int, int>> taken) {

	//base case, return taken
	if (i == items.size()) {
		return taken;
	}

	vector<pair<int, int>> v1 = {};
	vector<pair<int, int>> v2 = {};
	// don't take the item
	v1 = knapsackBFHelper(weight, i + 1, items, taken);

	// only insert if adding the current element into taken will not exceed weight
	// taken[0].first is the current weight that's taken, items[i].first is weight of the new item
	if (taken[0].first + items[i].first <= weight) {
		taken.push_back({items[i].first, items[i].second});
		taken[0].first = taken[0].first + items[i].first;
		taken[0].second = taken[0].second + items[i].second;
		v2 = knapsackBFHelper(weight, i + 1, items, taken);
	}

	if (v2.size() == 0 || v1[0].second > v2[0].second) {
		return v1;
	} else {
		return v2;
	}
}

int knapsackBF(int weight, vector<pair<int, int>> & items, vector<pair<int, int>> & taken)
{
	/************ BRUTE FORCE CODE HERE! ***************/
	// weight = maximum weight knapsack can hold
	// items  = vector of items to choose from
	// taken  = vector to hold chosen items
	/***************************************************/
	int bestValue = 0;

	// first pair in taken will include current weight and current value of the taken vector
	taken.push_back({0, 0});

	taken = knapsackBFHelper(weight, 0, items, taken);
	
	bestValue = taken[0].second;
	// remove the first element which is the total weight and value
	taken.erase(taken.begin());

	// best value should be the sum of all values in 'taken'
	return bestValue;
}

int knapsackDYN(int weight, vector<pair<int, int>> & items, vector<pair<int, int>> & taken)
{
	/******** DYNAMIC PROGRAMMING CODE HERE! ***********/
	// weight = maximum weight knapsack can hold
	// items  = vector of items to choose from
	// taken  = vector to hold chosen items
	/***************************************************/
	int bestValue = 0;
	const int itemsSize = items.size();

	// itemsize is the row, weight is the column
	int** memTable = new int* [itemsSize];
	// tracktable tracks if the current cell took the item, if so, set it to 1, else 0
	int** trackTable = new int* [itemsSize];

	// initialize 2d arrays of dynamic sizes
	for(int i = 0; i < itemsSize; ++i) {
		memTable[i] = new int[weight + 1];
		trackTable[i] = new int[weight + 1];
		// base case, fill in the first column to be 0 
		memTable[i][0] = 0;
		trackTable[i][0] = 0;
	}

	// base case, fill in the first row (either take the item if wk <= current weight or don't take the item)
	for(int curWeight = weight; curWeight >= 0; --curWeight) {
		if (items[0].first <= curWeight) {
			// fill from right to left bc I feel like it
			memTable[0][curWeight] = items[0].second;
			trackTable[0][curWeight] = 1;
		} else {
			memTable[0][curWeight] = 0;
			trackTable[0][curWeight] = 0;
		}
	}

	// populate memtable, for each row you can take at most from 0 to the kth item
	for(int k = 1; k < itemsSize; ++k) {
		for(int curWeight = 0; curWeight < weight + 1; ++curWeight) {
			if(items[k].first > curWeight) {
				// if current item weight is bigger than current weight, it's not possible to take it
				memTable[k][curWeight] = memTable[k-1][curWeight];
				trackTable[k][curWeight] = 0;
			} else {
				// take the max of not taking the current item or taking the current item
				int notTakeValue = memTable[k-1][curWeight];
				int takeValue = memTable[k-1][curWeight - items[k].first] + items[k].second;

				if (takeValue > notTakeValue) {
					memTable[k][curWeight] = takeValue;
					trackTable[k][curWeight] = 1;
				} else {
					memTable[k][curWeight] = notTakeValue;
					trackTable[k][curWeight] = 0;
				}
			}
		}
	}

	// the last cell is the bestvalue
	bestValue = memTable[itemsSize-1][weight];

	// populate taken items. we start from the last cell and trace backwards
	int w = weight;
	for (int k = itemsSize - 1; k >= 0; --k) {
		// if current cell is 1 that means we took the kth item
		if(trackTable[k][w] == 1) {
			taken.push_back({items[k].first, items[k].second});
			// if we took the item, the next cell(going up) will not have that items's 
			// weight anymore, so we subtract that items weight(items[k].first) from current weight
			w = w - items[k].first;
		}
		// else we didn't take the item, move directly up one spot from the table
	}

	for(int i = 0; i < itemsSize; i++){
		delete[] memTable[i];
		delete[] trackTable[i];
	}
	delete[] memTable;
	delete[] trackTable;
	
	// best value should be the sum of all values in 'taken'
	return bestValue;
}