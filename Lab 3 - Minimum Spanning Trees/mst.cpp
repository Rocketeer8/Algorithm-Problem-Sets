/*
    Fill in this block comment before submitting.
    Name 1: Hai Ning Liu
    Name 2: None, I work alone
    If you worked alone, indicate that as well.
*/


#include <iostream>
#include <string>
#include <algorithm>
#include <climits>

using namespace std;

void minimumSpanningTree(int **graph, int **mst, int V)
{
	/**************** YOUR CODE HERE! ****************/
	// Parameter V = number of vertices = size of adjacency matrix
	// You may assume parameters graph and mst are properly
	// allocated 2D arrays of size [V][V]
	// You need not allocate or deallocate them in this function

	// 0 is not visited, 1 is visited
	int *visited = new int[V];
	// shortest distance (from a previous vertex)
	int *shortest_dist = new int[V];
	// previous vertext num for shortest distance
	int *shortest_pt = new int[V];


	// initialize tracking lists
	for(int i = 0; i < V; i++) {
		visited[i] = 0;
		(i == 0) ? shortest_dist[i] = -1 : shortest_dist[i] = INT_MAX;
		// -1 means shorest previous pointer hasn't been found
		shortest_pt[i] = -1;
	}


	int cur_v = 0;
	// i is the vertext number
	for(int i = 0; i < V; i++) {
		// j is all current i's neighbors, if they exist, and update shortest dist when neccessary
		// only need to traverse through top right half of matrix, since it's mirrored. 
		// that's why j starts at i + 1, n^2/2 is still O(n^2) tho, but improvement nonetheless
		visited[cur_v] = 1;
		for (int j = 0; j < V; j++) {
			// graph[cur_v][j] != 0 means weight exist between cur_v and j that means j is neighbor of cur_v
			// since we are storing parent pointer we don't want to visit vertex that's been visited already  
			if (graph[cur_v][j] != 0 && visited[j] == 0 && shortest_dist[j] > graph[cur_v][j]) {
				shortest_dist[j] = graph[cur_v][j];
				shortest_pt[j] = cur_v;
			}
		}
		// find the next vertex, with the smallest distance that's not visited
		// we know the fist vertex is visited for sure, so we start from v = 1 not 0
		int shortest = INT_MAX;
		for (int k = 1; k < V; k++) {
			if (visited[k] == 0 && shortest > shortest_dist[k]) {
				shortest = shortest_dist[k];
				cur_v = k;
			}
		}
	}

	// constrcut the mst, backtrack from the last v (with its shoest neighbor) to the first v 
	for(int i = 1; i < V; i++) {
		mst[i][shortest_pt[i]] = mst[shortest_pt[i]][i] = shortest_dist[i];
	}

	delete[] visited;
	delete[] shortest_dist;
	delete[] shortest_pt;
}
