#include <string>
#include <unordered_map>
#include <queue>
using namespace std;

#define MAX_BCNT    100
#define MAX_NAME    10
#define MAX_ADD     15000


// bacteria
struct Bacteria { int mHalfTime, mCnt; } bacteria[MAX_BCNT];
int bacCnt;
unordered_map<string, int> bacMap;

// samples
struct Sample { int bIdx, mCnt, mLife, addTime; } samples[MAX_ADD];
int sIdx;       // sample index

// global
int curTime;
int totalCnt;

// pq
struct TimeData {
    int mHalfTime, sIdx;
    bool operator<(const TimeData& data) const { return mHalfTime > data.mHalfTime; }
};
priority_queue<TimeData> timePQ;

struct LifeData {
    int mLife, addTime, sIdx;
    bool operator<(const LifeData& data) const {
        return (mLife > data.mLife) || (mLife == data.mLife && addTime > data.addTime);
    }
};
priority_queue<LifeData> lifePQ;


void update(int curTime) {
    while (!timePQ.empty() && timePQ.top().mHalfTime == curTime) {
        int sIdx = timePQ.top().sIdx; timePQ.pop();

        samples[sIdx].mLife /= 2;

        int bIdx = samples[sIdx].bIdx;
        if (samples[sIdx].mLife < 10) {
            bacteria[bIdx].mCnt -= samples[sIdx].mCnt;
            samples[sIdx].mCnt = 0;
            totalCnt -= samples[sIdx].mCnt;
        } 
        else {
            timePQ.push({ curTime + bacteria[bIdx].mHalfTime, sIdx });
            lifePQ.push({ samples[sIdx].mLife, samples[sIdx].addTime, sIdx });
        }
    }
}

int get_bacIndex(const char bName[]) {
    int bIdx;
    auto iter = bacMap.find(bName);
    if (iter == bacMap.end()) {
        bIdx = bacCnt++;
        bacMap.emplace(bName, bIdx);
    }
    else bIdx = iter->second;
    return bIdx;
}


////////////////////////////////////////////////////////////////////
void init(int N, char bNameList[MAX_BCNT][MAX_NAME], int mHalfTime[MAX_BCNT])
{
    curTime = 0;
    totalCnt = 0;
    sIdx = 0;
    bacCnt = 0;
    
    bacMap.clear();
    timePQ = {};
    lifePQ = {};

    for (int i = 0; i < N; i++) {
        int bIdx = get_bacIndex(bNameList[i]);
        bacteria[bIdx] = { mHalfTime[i], 0 };
    }
}

void addBacteria(int tStamp, char bName[MAX_NAME], int mLife, int mCnt)
{
    while (curTime < tStamp) update(++curTime);

    int bIdx = get_bacIndex(bName);
    samples[sIdx] = { bIdx, mCnt, mLife, tStamp };
    bacteria[bIdx].mCnt += mCnt;
    totalCnt += mCnt;

    timePQ.push({ curTime + bacteria[bIdx].mHalfTime, sIdx });
    lifePQ.push({ mLife, tStamp, sIdx });
    sIdx++;
}

int takeOut(int tStamp, int mCnt)
{
    while (curTime < tStamp) update(++curTime);

    int takeOutCnt = min(totalCnt, mCnt);
    int res = 0;

    while (!lifePQ.empty() && takeOutCnt > 0) {
        auto data = lifePQ.top(); lifePQ.pop();
        int sIdx = data.sIdx;

        if (samples[sIdx].mLife != data.mLife) continue;

        int cnt = min(samples[sIdx].mCnt, takeOutCnt);
        takeOutCnt -= cnt;
        samples[sIdx].mCnt -= cnt;
        int bIdx = samples[sIdx].bIdx;
        bacteria[bIdx].mCnt -= cnt;

        res += samples[sIdx].mLife * cnt;

        if (samples[sIdx].mCnt > 0)
            lifePQ.push({ samples[sIdx].mLife, samples[sIdx].addTime, sIdx });
    }
    return res;
}

int checkBacteria(int tStamp, char bName[MAX_NAME])
{
    while (curTime < tStamp) update(++curTime);

    int bIdx = get_bacIndex(bName);
    return bacteria[bIdx].mCnt;
}
