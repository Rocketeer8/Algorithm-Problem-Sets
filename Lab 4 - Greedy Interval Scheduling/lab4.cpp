#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <assert.h>

#include "sched.hpp"

using namespace std;

void printRequests(vector<pair<int, int>> & roomReqs);
void printBookings(int nRooms, vector<pair<int, int>> bookings[]);

void generateIntervals(int n, vector<pair<int, int>> & intervals);
bool validateSchedule(int nRooms, vector<pair<int, int>> & intervals, vector<pair<int, int>> bookings[]);

int main()
{
	char name[] = "Alex Ufkes";
	srand((int)time(NULL));
	printf("\nCCPS616 - Lab 4 - %s\n\n", name);


	/*****************************************************************/
	/* First, test algorithm on simple example from lab description  */
	/* You should create your own additional examples here!          */
	/*****************************************************************/
	{
		// 5 requests: [(1, 2), (3, 6), (5, 8), (6, 7), (6, 8)]
		vector<pair<int, int>> requests;
		requests.push_back(pair<int, int>(1, 2));
		requests.push_back(pair<int, int>(3, 6));
		requests.push_back(pair<int, int>(5, 8));
		requests.push_back(pair<int, int>(6, 7));
		requests.push_back(pair<int, int>(6, 8));

		// Allocate array of vectors for three rooms.
		// nRooms+1 to account for bookings that can't be accommodated
		vector<pair<int, int>> three_rooms[4];
		vector<pair<int, int>> two_rooms[3];

		/** Call your scheduleRooms() function here **/
		scheduleRooms(3, requests, three_rooms);
		scheduleRooms(2, requests, two_rooms);
		/*********************************************/

		// Compare your results (printed below) with the optimal results
		// given in the lab description. No asserts or anything this time,
		// just verify your small-scale tests manually.
		cout << "Room request intervals:" << endl;
		printRequests(requests);
		cout << endl << "Scheduling with three rooms:" << endl;
		printBookings(3, three_rooms);
		cout << endl << "Scheduling with two rooms:" << endl;
		printBookings(2, two_rooms);
		cout << endl;
	}

	/*****************************************************************/
	/* Next, test algorithm on increasingly large random samples     */
	/*****************************************************************/
	{
		int numIntervals[] = { 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 65536 };
		for (int n : numIntervals)
		{
			vector<pair<int, int>> reqs;
			generateIntervals(n, reqs);

			// sqrt(n) seems like a reasonable number of rooms for n total bookings...
			int nRooms = (int) sqrt(n);
			vector<pair<int, int>> *n_rooms = new vector<pair<int, int>>[nRooms + 1];

			cout << "Testing " << n << " intervals... ";

			/** Call your scheduleRooms() function here **/
			scheduleRooms(nRooms, reqs, n_rooms);
			/*********************************************/

			// I will uncomment this assert when I test your code. You should too!
			assert(validateSchedule(nRooms, reqs, n_rooms));

			cout << "Passed!" << endl;

			delete[] n_rooms;
		}
	}

	printf("\n");
}

void printRequests(vector<pair<int, int>> & roomReqs)
{
	cout << "[ ";
	for (pair<int, int> p : roomReqs)
		cout << "(" << p.first << ", " << p.second << ") ";
	cout << "]" << endl;
}

void printBookings(int nRooms, vector<pair<int, int>> bookings[])
{
	for (int i = 1; i < nRooms + 1; i++) {
		cout << "Room #" << i << ": [ ";
		for (pair<int, int> p : bookings[i])
			cout << "(" << p.first << ", " << p.second << ") ";
		cout << "]" << endl;
	}
	cout << "No room: [ ";
	for (pair<int, int> p : bookings[0])
		cout << "(" << p.first << ", " << p.second << ") ";
	cout << "]" << endl;
}

// Function for generating random vectors of intervals. This function
// is very primitive, feel free to tweak it if you want to add more
// sophistication to the distribution of the intervals.
void generateIntervals(int n, vector<pair<int, int>> & intervals)
{
	for (int i = 0; i < n; i++)
	{
		int start = rand() % (i + 1) * 2 + 1;
		int dif = rand() % (i + 1) + 1;
		intervals.push_back(pair<int, int>(start, start + dif));
	}
}

// Function for validating scheduling. Does not check for optimal solution,
// Just verifies that no bookings overlap. Checks for feasible solution!
bool validateSchedule(int nRooms, vector<pair<int, int>> & intervals, vector<pair<int, int>> bookings[])
{
	for (int i = 1; i < nRooms + 1; i++)
	{
		// Check that intervals in each room don't overlap
		for (int j = 1; j < (int)bookings[i].size(); j++)
			if (bookings[i][j - 1].second > bookings[i][j].first) {
				cout << "Bookings overlap!" << endl;
				return false;
			}
	}

	int count = 0;
	for (int i = 0; i < nRooms + 1; i++)
	{
		// Check that interval in schedule exists in vector of intervals
		for (pair<int, int> p : bookings[i])
			if (std::find(intervals.begin(), intervals.end(), p) == intervals.end()) {
				cout << "Non-existent booking!" << endl;
				return false;
			}

		// Ensure all intervals are present in schedule (includes no room)
		count += bookings[i].size();
	}
    
	if (count != intervals.size()) {
		cout << "Wrong number of bookings!" << endl;
		return false;
	}

	return true;
}