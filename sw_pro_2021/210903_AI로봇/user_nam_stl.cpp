#if 1
#include <vector>
#include <unordered_map>
#include <queue>
using namespace std;

#define MAX_ROBOTS  50'001
#define MAX_TASKS   50'001  // wID : 작업 ID (1 ≤ wID ≤ 50,000)

#define TRAINING    0
#define WORKING     1
#define BROKEN      2

int N;          // N : 로봇의 대수 (10 ≤ N ≤ 50,000)

struct Robot {
    int IQ, wID, startTime, totalTime, state;
} robots[MAX_ROBOTS];

vector<int> robotList[MAX_TASKS];   // robotList[wID] = { rID, ... }

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
void init(int N)
{
    ::N = N;
    for (int rID = 1; rID <= N; rID++) {
        robots[rID] = { 0, 0, 0, TRAINING };
    }
}

int callJob(int cTime, int wID, int mNum, int mOpt)
{
 return -1;
}

void returnJob(int cTime, int wID)
{

}

void broken(int cTime, int rID)
{

}

void repair(int cTime, int rID)
{

}

int check(int cTime, int rID)
{
 return 0;
}
#endif