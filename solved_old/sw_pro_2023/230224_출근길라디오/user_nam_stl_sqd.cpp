#if 1
#include <vector>
using namespace std;

#define MAX_ROADS   100'000
#define MAX_TYPES   1'001

int N;          // 지점의 개수 ( 10 ≤ N ≤ 100,000 ) 도로의 개수 N - 1
int M;          // 도로의 종류 ( 1 ≤ M ≤ 1,000 )

struct Road {
    int mType;
    int mTime;
} roads[MAX_ROADS];

vector<int> roadList[MAX_TYPES];

// sqrt decomposition
struct RangeSumQuery {
    int arr[MAX_ROADS];
    int groups[MAX_ROADS];  // group sum
    int sq;                 // group size

    void clear() {
        sq = sqrt(N - 1);
        memset(arr, 0, sizeof(arr));
        memset(groups, 0, sizeof(groups));
    }
    void updatePoint(int idx, int value) {
        groups[idx / sq] -= arr[idx];
        arr[idx] = value;
        groups[idx / sq] += value;
    }
    int queryRange(int lo, int hi) {
        int res = 0;
        int s = lo / sq, e = hi / sq;

        if (s == e) {
            for (int i = lo; i <= hi; i++) res += arr[i];
            return res;
        }
        while (lo / sq == s) res += arr[lo++];
        for (int i = s + 1; i <= e - 1; i++) res += groups[i];
        while (hi / sq == e) res += arr[hi--];
        return res;
    }
} rsq;


/////////////////////////////////////////////////////////////////////
void init(int N, int M, int mType[], int mTime[])
{
    for (int i = 0; i < M; i++) roadList[i].clear();
    rsq.clear();

    ::N = N, ::M = M;
    for (int i = 0; i < N; i++) {
        roads[i] = { mType[i], mTime[i] };
        roadList[mType[i]].push_back(i);
        rsq.updatePoint(i, mTime[i]);
    }
}

void destroy() {}

void update(int mID, int mNewTime)
{
    roads[mID].mTime = mNewTime;
    rsq.updatePoint(mID, mNewTime);
}

int updateByType(int mTypeID, int mRatio256)
{
    int res = 0;
    for (int mID: roadList[mTypeID]) {
        int time = roads[mID].mTime * mRatio256 / 256;
        roads[mID].mTime = time;
        rsq.updatePoint(mID, time);
        res += time;
    }
    return res;
}

int calculate(int mA, int mB)
{
    if (mB < mA) swap(mA, mB);

    // int res = 0;
    // for (int i = mA; i <= mB - 1; i++)
    //     res += roads[i].mTime;

    int res = rsq.queryRange(mA, mB - 1);
    return res;
}
#endif