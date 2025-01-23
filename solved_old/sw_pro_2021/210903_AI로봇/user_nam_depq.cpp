#if 0
#include <vector>
#include <queue>
#include "depq.h"
using namespace std;

#define MAX_ROBOTS  50'001
#define MAX_WORKS   50'001

#define TRAINING    0
#define BROKEN      1
#define WORKING     2

struct Robot {
    int wID, startTime, workTime, state;
} robots[MAX_ROBOTS];

vector<int> robotList[MAX_WORKS];

// IQ => MAX_WORKTIME - workTime (IQ 는 workTime 에 반비례)
struct Data {
    int rID, workTime;

    // maxHeap: IQ 클수록 (= workTime 적을수록), rID 작을수록
    bool operator<(const Data& data) const {
        return (workTime > data.workTime) || (workTime == data.workTime && rID > data.rID);
    }
    // minHeap: IQ 작을수록 (= workTime 클수록), rID 작을수록
    bool operator>(const Data& data) const {
        return (workTime < data.workTime) || (workTime == data.workTime && rID > data.rID);
    }
};
DoubleEnededPQ<Data> depq;

/////////////////////////////////////////////////////////////////////
void init(int N)
{
    depq.clear();
    for (int i = 0; i < MAX_WORKS; i++) {
        robotList[i].clear();
    }
    for (int rID = 1; rID <= N; rID++) {
        robots[rID].state = TRAINING;
        robots[rID].workTime = 0;

        depq.push({ rID, robots[rID].workTime });
    }
}

int callJob(int cTime, int wID, int mNum, int mOpt)
{
    int res = 0;
    while (!depq.empty() && mNum--) {
        Data data;
        if (mOpt == 0) { data = depq.max(); depq.popMax(); }
        if (mOpt == 1) { data = depq.min(); depq.popMin(); }
        int rID = data.rID;

        if (robots[rID].state != TRAINING) continue;
        // if (robots[rID].workTime != data.workTime) continue;

        robots[rID].state = WORKING;
        robots[rID].wID = wID;
        robots[rID].startTime = cTime;

        robotList[wID].push_back(rID);
        res += rID;
    }
    return res;
}

void returnJob(int cTime, int wID)
{
    for (int rID: robotList[wID]) {
        if (robots[rID].wID != wID) continue;
        if (robots[rID].state == WORKING) {
            robots[rID].state = TRAINING;
            robots[rID].workTime += (cTime - robots[rID].startTime);

            depq.push({ rID, robots[rID].workTime });
        }
    }
}

void broken(int cTime, int rID)
{
    if (robots[rID].state == WORKING)
        robots[rID].state = BROKEN;
}

void repair(int cTime, int rID)
{
    if (robots[rID].state == BROKEN) {
        robots[rID].state = TRAINING;
        robots[rID].workTime = cTime;

        depq.push({ rID, robots[rID].workTime });
    }
}

int check(int cTime, int rID)
{
    if (robots[rID].state == BROKEN)
        return 0;
    else if (robots[rID].state == WORKING)
        return -robots[rID].wID;
    else
        return cTime - robots[rID].workTime;
}
#endif