#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <string>
#include <cstring>
#include <cstdlib>
#include <chrono>
//#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>

#include "mst.hpp"

using namespace std;
using namespace std::chrono; // Namespace for fancy ms timing

void printGraph(int** graph, int V);
void fillRandGraph(int **graph, int V, int E, int maxWeight = 9);
bool validateST(int** st, int** graph, int V);

int main()
{
	char name[] = "<your name here>";

	srand((int)time(NULL));
	printf("\nCCPS616 - Lab 3 - %s\n\n", name);

	/*******************************************************************/
	/* First, test algorithm correctness on simple example from classn */
	/* You should create your own additional examples here!            */
	/*******************************************************************/
	{
		const int V = 6;
		int gNums[] = { 0,1,0,0,5,0, 1,0,8,0,2,0, 0,8,0,1,0,0, 0,0,1,0,4,2, 5,2,0,4,0,0, 0,0,0,2,0,0 };
		int gMST[] = { 0,1,0,0,0,0, 1,0,0,0,2,0, 0,0,0,1,0,0, 0,0,1,0,4,2, 0,2,0,4,0,0, 0,0,0,2,0,0 };

		// 2D arrays in C/C++ are tedious. This is just creating a 2D array capable of double
		// indexing and setting up pointers to the interior of the 1D arrays above. This way
		// is more technical, but provides good cache locality because everything is contiguous.
		int **g = new int*[V];
		g[0] = gNums;
		for (int i = 1; i < V; i++)
			g[i] = g[i - 1] + V;

		int **mst = new int*[V];
		mst[0] = gMST;
		for (int i = 1; i < V; i++)
			mst[i] = mst[i - 1] + V;

		int **yourMST = new int*[V];
		yourMST[0] = new int[V*V](); // The () is a C++ trick for initializing elements to 0
		for (int i = 1; i < V; i++)
			yourMST[i] = yourMST[i - 1] + V;

		cout << "Testing simple example from the description... " << endl;
		printGraph(g, V);
		cout << endl << "MST should be:" << endl;
		printGraph(mst, V);

		minimumSpanningTree(g, yourMST, V);

		cout << endl << "You said:" << endl;
		printGraph(yourMST, V);
		cout << endl;

		//assert(validateST(yourMST, g, V));

		delete[] yourMST[0];
		delete[] yourMST;
	}

	// Number of vertexes in large random graph
	int V = 2000;

	// Create a contiguous 2D array that still allows double indexing. Good for cache locality!
	int** graph = new int*[V];
	graph[0] = new int[V*V](); // The () is a C++ trick for initializing elements to 0
	for (int i = 1; i < V; i++)
		graph[i] = graph[i - 1] + V;

	// Create similar array for storing MST
	int** graph_mst = new int*[V];
	graph_mst[0] = new int[V*V](); // The () is a C++ trick for initializing elements to 0
	for (int i = 1; i < V; i++)
		graph_mst[i] = graph_mst[i - 1] + V;

	/*****************************************************************/
	/* Time randomized test on large, SPARSE graph where |E| = 3*|V| */
	/*****************************************************************/
	{
		int E = 4 * V;
		fillRandGraph(graph, V, E);

        // reset values in graph_mst to 0
		memset(graph_mst[0], 0, V*V * sizeof(int));

		cout << "Testing on random SPARSE graph (E = " << E << ")" << endl;

		auto begin = high_resolution_clock::now();
		minimumSpanningTree(graph, graph_mst, V);
		auto dur = high_resolution_clock::now() - begin;
		int msTime = (int)duration_cast<milliseconds>(dur).count();

		// Un-comment this assertion to validate your spanning tree
		//assert(validateST(graph_mst, graph, V));
		cout << "  Done in " << msTime << "ms" << endl << endl;
	}

	/**************************************************************************/
	/* Time randomized test on large, DENSE graph where |E| = |V|*(|V|-1) / 3 */
	/**************************************************************************/
	{
		int E = V*(V - 1) / 4;
		fillRandGraph(graph, V, E);

        // reset values in graph_mst to 0
		memset(graph_mst[0], 0, V*V * sizeof(int));

		cout << "Testing on random DENSE graph (E = " << E << ")" << endl;

		auto begin = high_resolution_clock::now();
		minimumSpanningTree(graph, graph_mst, V);
		auto dur = high_resolution_clock::now() - begin;
		int msTime = (int)duration_cast<milliseconds>(dur).count();

		// Un-comment this assertion to validate your spanning tree
		//assert(validateST(graph_mst, graph, V));
		cout << "  Done in " << msTime << "ms" << endl << endl;
	}

	// Deallocate graphs
	delete[] graph[0];
	delete[] graph;

	delete[] graph_mst[0];
	delete[] graph_mst;

	printf("\n");
	//system("pause");
}



// Generate a random connected graph in the form of an adjacency matrix
void fillRandGraph(int **graph, int V, int E, int maxWeight)
{
	int max_edges = V*(V - 1) / 2;
	std::random_device rd;
    std::mt19937 g(rd());

	assert(E >= V - 1);	    // Minimum number of edges while still being connected
	assert(E <= max_edges); // Maximum number of edges without duplicate edges

							// Set everything to zero, just in case it isn't already. This is better done 
							// with memset(), but doing that correctly depends on how graph is allocated.
	for (int i = 0; i < V; i++)
		for (int j = 0; j < V; j++)
			graph[i][j] = 0;

	// initialize vectors for visited and unvisited vertexes
	vector<int> visited, unvisited;
	// To start, everything is unvisited (vertex ID 0 to V-1)
	for (int i = 0; i < V; i++)
		unvisited.push_back(i);
	// Shuffle vector of unvisited vertexes
	std::shuffle(unvisited.begin(), unvisited.end(), g);
	// Add last element to visited, remove from unvisited
	visited.push_back(unvisited.back());
	unvisited.pop_back();

	// Randomly connect unvisited vertexes until none remain
	// This results in a spanning tree (V-1 edges)
	for (int uv : unvisited) {
		int vv = visited[rand() % visited.size()];
		graph[vv][uv] = graph[uv][vv] = rand() % maxWeight + 1;
		visited.push_back(uv);
	}
	assert(visited.size() == V);

	// Create vector of vertex pairs without edges. This seems like a 
	// hacky and costly way of doing this, but meh...
	vector<pair<int, int>> non_edges;
	for (int i = 0; i < V; i++)
		for (int j = i + 1; j < V; j++)
			if (graph[i][j] == 0)
				non_edges.push_back(pair<int, int>(i, j));
	assert(non_edges.size() + (V - 1) == max_edges);

	// Insert remaining E-(V-1) edges, shuffling non_edges ensures randomness
	int edgesToAdd = E - (V - 1);
	if (edgesToAdd > 0) {
		std::shuffle(non_edges.begin(), non_edges.end(), g);
		for (int i = 0; i < edgesToAdd; i++) {
			int v1 = non_edges[i].first;
			int v2 = non_edges[i].second;
			int wgt = rand() % maxWeight + 1;
			graph[v1][v2] = graph[v2][v1] = wgt;
		}
	}

	// Sanity check #1. Ensure adjacency matrix is symmetrical
	for (int i = 0; i < V; i++)
		for (int j = i + 1; j < V; j++)
			assert(graph[i][j] == graph[j][i]);

	// Sanity check #2. Count non-zero elements in graph, should == E 
	int nEdge = 0;
	for (int i = 0; i < V; i++)
		for (int j = i + 1; j < V; j++)
			nEdge = graph[i][j] != 0 ? nEdge + 1 : nEdge;
	assert(nEdge == E);
}

// Simple function for printing an adjacency matrix
void printGraph(int** graph, int V)
{
	printf("V = %d\n", V);
	for (int i = 0; i < V; i++) {
		for (int j = 0; j < V; j++)
			printf("%-3d", graph[i][j]);
		printf("\n");
	}
}

// Function for verifying that st is a spanning tree for graph
// Does NOT determine if the spanning tree is minimum
// Just checks for the existence and right number of edges
bool containsCycle(int** graph, int V);
bool cycleUtil(int** graph, int V, int vert, int *visited, int parent);
bool validateST(int** st, int** graph, int V)
{
	// Count edges, should = V-1. If not, st cannot be a spanning tree
	int e = 0;
	for (int i = 0; i < V; i++)
		for (int j = i + 1; j < V; j++)
			e = st[i][j] != 0 ? e + 1 : e;
	if (e != V - 1) {
		cout << "Wrong number of edges! (e = " << e << ")" << endl;
		return false;
	}

	// Ensure edges present in st are actually in the original graph
	for (int i = 0; i < V; i++)
		for (int j = i + 1; j < V; j++)
			if (st[i][j] != 0 && graph[i][j] != st[i][j]) {
				cout << "MST contains edges not in original graph!" << endl;
				return false;
			}

	// Check for cycle in spanning tree
	if (containsCycle(st, V)) {
		cout << "MST contains cycle(s)!" << endl;
		return false;
	}

	// If no cycle, we're all set
	return true;
}

bool containsCycle(int** graph, int V)
{
	int *visited = new int[V]();
	return cycleUtil(graph, V, 0, visited, -1);
}

bool cycleUtil(int** graph, int V, int vert, int *visited, int parent)
{
	visited[vert] = 1;

	for (int j = 0; j < V; j++) {
		if (graph[vert][j] != 0)
		{
			if (!visited[j]) {
				if (cycleUtil(graph, V, j, visited, vert))
					return true;
			}
			else if (j != parent)
				return true;
		}
	}
	return false;
}