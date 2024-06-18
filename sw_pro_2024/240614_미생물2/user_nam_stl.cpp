#include <queue>
using namespace std;

struct Bacterica {
    int mLifeSpan;
    int mHalfTime;
} bacteria[30'000 + 1];
//int bacteriaCnt;

struct TimeData {
    int mHalfTime, mID;
    bool operator<(const TimeData& data) const { return mHalfTime > data.mHalfTime; }
};
priority_queue<TimeData> timePQ;

struct LifeData {
    int mLifeSpan, mID;
    bool operator<(const LifeData& data) const {
        return (mLifeSpan > data.mLifeSpan) ||
            (mLifeSpan == data.mLifeSpan && mID > data.mID);
    }
};
priority_queue<LifeData> lifePQ;

int currentTime;

struct SqrtDecomp {
    int arr[1'000'001];     // bacteria cnt at lifeSpan
    int groups[1'001];      // group sum
    int sq = 1'001;         // group size = sqrt(1'000'001)

    void clear() { 
        memset(arr, 0, sizeof(arr)); 
        memset(groups, 0, sizeof(groups));
    }
    void update(int idx, int value) {
        arr[idx] += value;
        groups[idx / sq] += value;
    }
    int query(int left, int right) {
        int res = 0;
        int s = left / sq, e = right / sq;

        if (s == e) {
            for (int i = left; i <= right; i++) res += arr[i];
            return res;
        }
        for (int i = left; i < (s + 1) * sq; i++) res += arr[i];
        for (int i = s + 1; i <= e - 1; i++) res += groups[i];
        for (int i = e * sq; i <= right; i++) res += arr[i];
        return res;
    }
} SQ;


void update(int currentTime) {
    while (!timePQ.empty() && timePQ.top().mHalfTime == currentTime) {
        int mID = timePQ.top().mID; timePQ.pop();

        SQ.update(bacteria[mID].mLifeSpan, -1);
        bacteria[mID].mLifeSpan /= 2;
        SQ.update(bacteria[mID].mLifeSpan, +1);

        if (bacteria[mID].mLifeSpan < 100) {
            //SQ.update(bacteria[mID].mLifeSpan, -1);   // 100 <= mMinSpan
            continue;
        }
        timePQ.push({ currentTime + bacteria[mID].mHalfTime, mID });
        lifePQ.push({ bacteria[mID].mLifeSpan, mID });
    }
}

//////////////////////////////////////////////////////////////////////
void init()
{
    //for (int i = 1; i <= bacteriaCnt; i++) bacteria[i] = {};
    currentTime = 0;
    //bacteriaCnt = 0;
    timePQ = {};
    lifePQ = {};
    SQ.clear();
}

// 30'000
void addBacteria(int tStamp, int mID, int mLifeSpan, int mHalfTime)
{
    while (currentTime < tStamp) update(++currentTime);

    //bacteriaCnt = mID;
    bacteria[mID].mLifeSpan = mLifeSpan;
    bacteria[mID].mHalfTime = mHalfTime;

    timePQ.push({ currentTime + mHalfTime, mID });
    lifePQ.push({ mLifeSpan, mID });
    SQ.update(mLifeSpan, +1);
}

// 1'000
int getMinLifeSpan(int tStamp)
{
    while (currentTime < tStamp) update(++currentTime);

    int res = -1;
    while (!lifePQ.empty() && lifePQ.top().mLifeSpan > 99) {
        auto data = lifePQ.top(); lifePQ.pop();
        int mID = data.mID;

        //if (data.mLifeSpan < 100) continue;
        if (data.mLifeSpan != bacteria[mID].mLifeSpan) continue;

        res = mID;
        lifePQ.push(data);
        break;
    }
    return res;
}

// 15'000
int getCount(int tStamp, int mMinSpan, int mMaxSpan)
{
    while (currentTime < tStamp) update(++currentTime);

    // [방법-1] for-loop: 30'000 -> 시간 초과
    //int res = 0;
    //for (int i = 1; i <= bacteriaCnt; i++) {
    //    if (bacteria[i].mLifeSpan < mMinSpan) continue;
    //    if (bacteria[i].mLifeSpan > mMaxSpan) continue;
    //    res++;
    //}

    // [방법-2] Sqrt Decomposition: sqrt(1'000'001) = 1'001
    int res = SQ.query(mMinSpan, mMaxSpan);

    return res;
}