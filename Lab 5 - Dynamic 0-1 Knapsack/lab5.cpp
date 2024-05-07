#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <stdlib.h>
#include <time.h>
#include <assert.h>

#include "knapsack.hpp"

using namespace std;

int getWeight(vector<pair<int, int>> & knapsack);
int getValue(vector<pair<int, int>> & knapsack);
void generateItems(int n, vector<pair<int, int>> & items);
void printItems(vector<pair<int, int>> & items);


int main()
{
	char name[] = "<Your name(s) here>";
	srand((int)time(NULL));
	printf("\nCCPS616 - Lab 5 - %s\n\n", name);


	/*********************************************************/
	/* First, test algorithm on example from lecture slides  */
	/* You should create your own additional examples here!  */
	/*********************************************************/
	{
		// Example from lecture slides:
		// 5 items: [(1, 1), (2, 6), (5, 18), (6, 22), (7, 28)]
		int weight = 11, bestValue = 40;
		vector<pair<int, int>> items;
		items.push_back(pair<int, int>(1, 1));
		items.push_back(pair<int, int>(2, 6));
		items.push_back(pair<int, int>(5, 18));
		items.push_back(pair<int, int>(6, 22));
		items.push_back(pair<int, int>(7, 28));

		int bfValue, dynValue;
		vector<pair<int, int>> bfTaken, dynTaken;

		printf("Testing simple class example... \n");
		printf("Items: ");
		printItems(items);

		bfValue = knapsackBF(11, items, bfTaken);
		dynValue = knapsackDYN(11, items, dynTaken);

		printf("  Brute force picked: ");
		printItems(bfTaken);
		printf("  Dynamic picked:     ");
		printItems(dynTaken);
		printf("  Should be:          [ (5, 18) (6, 22) ]\n");

		int bfWeight = getWeight(bfTaken);
		int dynWeight = getWeight(dynTaken);

		if (bfWeight > weight || dynWeight > weight) {
			printf("  Weight limit exceeded!\n");
			printf("  Brute force weight: %d\n", bfWeight);
			printf("  Dynamic weight:     %d\n", dynWeight);
			printf("  Target weight:      %d\n\n", weight);
		}
		if (bfValue != bestValue || dynValue != bestValue) {
			printf("  Value discrepancy!\n");
			printf("  Brute force value:  %d\n", bfValue);
			printf("  Dynamic value:      %d\n", dynValue);
			printf("  Both should be:     %d\n\n", bestValue);
		}
		else
			printf("  Passed!\n\n");
	}

	/******************************************************************/
	/* Next, test algorithm on several larger, random samples         */
	/* Veryify correctness by comparing brute force to dynamic result */
	/******************************************************************/
	{
		int numItems[] = { 5, 10, 15, 20 };

		for (int n : numItems)
		{
			vector<pair<int, int>> items;
			generateItems(n, items);
			int weight = n * 2;
			vector<pair<int, int>> bfTaken, dynTaken;

			printf("%d items, weight = %d... \n", n, weight);

			//printf("Items: ");
			//printItems(items);

			int bfValue = knapsackBF(weight, items, bfTaken);
			int dynValue = knapsackDYN(weight, items, dynTaken);

			// Make sure value returned matches value of items taken
			assert(bfValue == getValue(bfTaken));
			assert(dynValue == getValue(dynTaken));

			printf("  Brute force picked: ");
			printItems(bfTaken);
			printf("  Dynamic picked:     ");
			printItems(dynTaken);

			int bfWeight = getWeight(bfTaken);
			int dynWeight = getWeight(dynTaken);

			if (bfWeight > weight || dynWeight > weight) {
				printf("  Weight limit exceeded!\n");
				printf("  Brute force weight: %d\n", bfWeight);
				printf("  Dynamic weight:     %d\n", dynWeight);
				printf("  Target weight:      %d\n", weight);
				printf("  Breaking...\n\n");
				break;
			}
			else if (bfValue != dynValue) {
				printf("  Value discrepancy!\n");
				printf("  Brute force value: %d\n", bfValue);
				printf("  Dynamic value:     %d\n", dynValue);
				printf("  Breaking...\n\n");
				break;
			}
			else
				printf("  Total value: %d\n\n", bfValue);


		}
	}


	printf("\n");
}

// Function for getting weight of items in knapsack. 
int getWeight(vector<pair<int, int>> & knapsack)
{
	int total = 0;
	for (pair<int, int> item : knapsack)
		total += item.first;
	return total;
}

// Function for getting value of items in knapsack. 
int getValue(vector<pair<int, int>> & knapsack)
{
	int total = 0;
	for (pair<int, int> item : knapsack)
		total += item.second;
	return total;
}

// Function for generating a random vector of items. 
void generateItems(int n, vector<pair<int, int>> & items)
{
    int i = 0;
	while (i < n)
	{
		int weight = rand() % n + 1;
		int value = (rand() % n + 1) * 3;
        pair<int, int> p = pair<int, int>(weight, value);

        // Disallow duplicate items
        if (std::find(items.begin(), items.end(), p) == items.end()) {
        	items.push_back(p);
            i++;
        }
	}
}

void printItems(vector<pair<int, int>> & items)
{
	cout << "[ ";
	for (pair<int, int> p : items)
		cout << "(" << p.first << ", " << p.second << ") ";
	cout << "]" << endl;
}