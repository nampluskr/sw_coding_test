#if 0
#include <cstring>
#include <string>
#include <unordered_map>
#include <queue>
using namespace std;

#define MAX_BCNT    100     // N : 보관소에 보관하는 미생물의 종류 (1 ≤ N ≤ 100 )
#define MAX_NAME    10      // bNameList[] : 미생물의 이름 ( 3 ≤ bNameList[]의 길이 ≤ 9 )
#define MAX_ADD     15'000  // addBacteria() 함수의 호출 횟수는 최대 15,000

struct Bacrerica {
    char bName[MAX_NAME];
    int mHalfTime;
    int mCnt;
} bacteria[MAX_BCNT];
int bacteriaCnt;
unordered_map<string, int> bacteriaMap;

struct Bottle {
    int bIdx, mCnt, mLife, addTime;
} bottles[MAX_ADD];
int bottleNo;
int totalCnt;

struct TimeData {
    int nextHalfTime, bottleNo;
    bool operator<(const TimeData& data) const { return nextHalfTime > data.nextHalfTime; }
};
priority_queue<TimeData> timePQ;

struct LifeData { 
    int lifeLeft, addTime, bottleNo;
    bool operator<(const LifeData& data) const { 
        return (lifeLeft > data.lifeLeft) || (lifeLeft == data.lifeLeft && addTime > data.addTime);
    }
};
priority_queue<LifeData> lifePQ;

int currentTime;

int get_bacteriaIndex(const char bName[]) {
    int bIdx;
    auto iter = bacteriaMap.find(bName);
    if (iter == bacteriaMap.end()) {
        bIdx = bacteriaCnt++;
        bacteriaMap.emplace(bName, bIdx);
    }
    else bIdx = iter->second;
    return bIdx;
}

/////////////////////////////////////////////////////////////////////
void init(int N, char bNameList[MAX_BCNT][MAX_NAME], int mHalfTime[MAX_BCNT])
{
    currentTime = 0;
    totalCnt = bacteriaCnt = bottleNo = 0;

    bacteriaMap.clear();
    timePQ = {};
    lifePQ = {};

    for (int i = 0; i < N; i++) {
        int bIdx = get_bacteriaIndex(bNameList[i]);
        strcpy(bacteria[bIdx].bName, bNameList[i]);
        bacteria[bIdx].mHalfTime = mHalfTime[i];
        bacteria[bIdx].mCnt = 0;
    }
}

void addBacteria(int tStamp, char bName[MAX_NAME], int mLife, int mCnt)
{
    int bIdx = get_bacteriaIndex(bName);
    bottles[bottleNo].bIdx = bIdx;
    bottles[bottleNo].mLife = mLife;
    bottles[bottleNo].mCnt = mCnt;
    bottles[bottleNo].addTime = tStamp;

    bacteria[bIdx].mCnt += mCnt;
    totalCnt += mCnt;

    timePQ.push({ tStamp + bacteria[bIdx].mHalfTime, bottleNo });
    lifePQ.push({ mLife, tStamp, bottleNo });
    bottleNo++;
}

int takeOut(int tStamp, int mCnt)
{
    int takeOutCnt = min(totalCnt, mCnt);
    int res = 0;

    while (takeOutCnt > 0) {
        auto data = lifePQ.top(); lifePQ.pop();
        int bottleNo = data.bottleNo;

        if (bottles[bottleNo].mCnt == 0) continue;
        if (bottles[bottleNo].mLife != data.lifeLeft) continue;

        int cnt = min(bottles[bottleNo].mCnt, takeOutCnt);
        takeOutCnt -= cnt;
        bottles[bottleNo].mCnt -= cnt;
        int bIdx = bottles[bottleNo].bIdx;
        bacteria[bIdx].mCnt -= cnt;
        res += bottles[bottleNo].mLife * cnt;

        if (bottles[bottleNo].mCnt > 0)
            lifePQ.push({ bottles[bottleNo].mLife, bottles[bottleNo].addTime, bottleNo });
    }
    return res;
}

int checkBacteria(int tStamp, char bName[MAX_NAME])
{
    int bIdx = get_bacteriaIndex(bName);
    return bacteria[bIdx].mCnt;
}
#endif