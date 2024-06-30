#if 1
#include <vector>
#include "depq.h"
using namespace std;

#define MAX_ROBOTS  50'001
#define MAX_WORKS   50'001  // wID : 작업 ID (1 ≤ wID ≤ 50,000)

#define TRAINING    0
#define WORKING     1
#define BROKEN      2

struct Robot {
    int IQ, wID, workStartTime, totalWorkTime, state;
} robots[MAX_ROBOTS];

vector<int> robotList[MAX_WORKS];   // robotList[wID] = { rID, ... }

struct Data {
    int rID, IQ;
    // 높은 지능 우선 방식
    bool operator<(const Data& data) const {
        return (IQ < data.IQ) || (IQ == data.IQ && rID > data.rID);
    }
    // 낮은 지능 우선 방식
    bool operator>(const Data& data) const {
        return (IQ > data.IQ) || (IQ == data.IQ && rID > data.rID);
    }
};
DoubleEnededPQ<Data> pq;

////////////////////////////////////////////////////////
void init(int N)
{
    for (int i = 0; i < MAX_WORKS; i++) {
        robots[i] = {};
        robotList[i].clear();
    }
    pq.clear();
    for (int rID = 1; rID <= N; rID++) {
		robots[rID].state = TRAINING;
		pq.push({ rID, robots[rID].totalWorkTime });
	}
}

int callJob(int cTime, int wID, int mNum, int mOpt)
{
	int result = 0;

	while(!pq.empty() && mNum--) {
		auto data = (mOpt == 0) ? pq.popMax() : pq.popMin();
		int rID = data.rID;

		if (robots[rID].state == TRAINING) {
			robots[rID].state = WORKING;
			robots[rID].wID = wID;
			robots[rID].workStartTime = cTime;
			robotList[wID].push_back(rID);	// <- 초기화
			result += rID;
		}
	}
	return result;
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