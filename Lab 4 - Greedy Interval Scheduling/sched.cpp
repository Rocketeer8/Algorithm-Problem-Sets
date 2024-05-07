/*
    Fill in this block comment before submitting.
    Name 1: Hai Ning Liu
    Name 2: None, I work alone
    If you worked alone, indicate that as well.    
*/

#include <algorithm>
#include <vector>

using namespace std;

bool sortbysec(const pair<int,int> &a, const pair<int,int> &b)
{
    return (a.second < b.second);
}

void scheduleRooms(int nRooms, vector<pair<int, int>> & roomReqs, vector<pair<int, int>> schedule[])
{
	/**************** YOUR CODE HERE! ***********************************/
	/* nRooms   = number of rooms available                             */
	/* roomReqs = vector of integer pairs. See test code in main()      */
	/* for an example of how to use vectors/pairs.                      */
	/* schedule = Array of vectors containing integer pairs.            */
	/* Your algorithm will fill this data structure.                    */
	/* Bookings placed in room #1 should be placed in schedule[1], etc. */
	/* Use schedule[0] to store requests that cannot be accommodated.   */
	/********************************************************************/

	int *satisfied_req = new int[roomReqs.size()];

	for (int i = 0; i < roomReqs.size(); i++) {
		satisfied_req[i] = -1;
	}

	sort(roomReqs.begin(), roomReqs.end(), sortbysec);
	
	/*
	cout << "Before" << endl;
	for(int i =0; i<roomReqs.size(); i++) {
		cout<<"("<<roomReqs[i].first<<","<<roomReqs[i].second<<")"<<" ";
	}
	cout << endl;
	*/

	for (int i = 1; i <= nRooms; ++i) {

		int end_time = 0;
		int ind = 0;
		for (auto pair_pt = begin(roomReqs); pair_pt != end(roomReqs); ++pair_pt) {

			// vector_pt->first is the start time of the next room
			// vector_pt->first same as (*vector_pt).first
			if (end_time <= pair_pt->first && satisfied_req[ind] != 1) {
				schedule[i].push_back({pair_pt->first, pair_pt->second});
				end_time = pair_pt->second;
				// list request at ind as satisfied
				satisfied_req[ind] = 1;
			}
			ind++;
		}
	}

	// we went through all the room, if there are remaining request, then we can't fit them
	// in the rooms and we store them in schedule[0]
	for (int i = 1; i < roomReqs.size(); ++i) {
		if (satisfied_req[i] != 1) {
			schedule[0].push_back(*(roomReqs.begin() + i));
		}
	}

	delete[] satisfied_req;

}


