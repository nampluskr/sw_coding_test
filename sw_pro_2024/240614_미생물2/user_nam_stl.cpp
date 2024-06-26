#include <queue>
using namespace std;

struct Bacterica { int mLifeSpan, mHalfTime; } bacteria[30001];
int curTime;

// priority queue
struct TimeData {
    int mHalfTime, mID;
    bool operator<(const TimeData& data) const {
        return mHalfTime > data.mHalfTime;
    }
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

// sqrt decomposition
struct RangeSumQuery {
    int cnt[1000001];   // cnt at lifeSpan
    int sum[1001];      // group sum
    int sq = 1001;      // group size = sqrt(1000001)

    void clear() {
        memset(cnt, 0, sizeof(cnt));
        memset(sum, 0, sizeof(sum));
    }
    void update(int idx, int value) {
        cnt[idx] += value;
        sum[idx / sq] += value;
    }
    int query(int lo, int hi) {
        int res = 0;
        int s = lo / sq, e = hi / sq;

        if (s == e) {
            for (int i = lo; i <= hi; i++) res += cnt[i];
            return res;
        }
        while (lo / sq == s) res += cnt[lo++];
        for (int i = s + 1; i <= e - 1; i++) res += sum[i];
        while (hi / sq == e) res += cnt[hi--];
        return res;
    }
} rsq;

void update(int curTime) {
    while (!timePQ.empty() && timePQ.top().mHalfTime == curTime) {
        int mID = timePQ.top().mID; timePQ.pop();

        rsq.update(bacteria[mID].mLifeSpan, -1);
        bacteria[mID].mLifeSpan /= 2;
        rsq.update(bacteria[mID].mLifeSpan, +1);

        if (bacteria[mID].mLifeSpan < 100) continue;
        timePQ.push({ curTime + bacteria[mID].mHalfTime, mID });
        lifePQ.push({ bacteria[mID].mLifeSpan, mID });
    }
}

//////////////////////////////////////////////////////////////////////
void init()
{
    curTime = 0;
    timePQ = {};
    lifePQ = {};
    rsq.clear();
}

void addBacteria(int tStamp, int mID, int mLifeSpan, int mHalfTime)
{
    while (curTime < tStamp) update(++curTime);

    bacteria[mID] = { mLifeSpan, mHalfTime };
    timePQ.push({ curTime + mHalfTime, mID });
    lifePQ.push({ mLifeSpan, mID });
    rsq.update(mLifeSpan, +1);
}

int getMinLifeSpan(int tStamp)
{
    while (curTime < tStamp) update(++curTime);

    int res = -1;
    while (!lifePQ.empty() && lifePQ.top().mLifeSpan > 99) {
        auto data = lifePQ.top(); lifePQ.pop();

        if (data.mLifeSpan != bacteria[data.mID].mLifeSpan) continue;
        res = data.mID;
        lifePQ.push(data);
        break;
    }
    return res;
}

int getCount(int tStamp, int mMinSpan, int mMaxSpan)
{
    while (curTime < tStamp) update(++curTime);

    return rsq.query(mMinSpan, mMaxSpan);
}