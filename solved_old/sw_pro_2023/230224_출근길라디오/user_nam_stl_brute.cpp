#if 0
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


/////////////////////////////////////////////////////////////////////
void init(int N, int M, int mType[], int mTime[])
{
    for (int i = 0; i < M; i++) roadList[i].clear();

    ::N = N, ::M = M;
    for (int i = 0; i < N; i++) {
        roads[i] = { mType[i], mTime[i] };
        roadList[mType[i]].push_back(i);
    }
}

void destroy() {}

void update(int mID, int mNewTime)
{
    roads[mID].mTime = mNewTime;
}

int updateByType(int mTypeID, int mRatio256)
{
    int res = 0;
    for (int mID: roadList[mTypeID]) {
        int time = roads[mID].mTime * mRatio256 / 256;
        roads[mID].mTime = time;
        res += time;
    }
    return res;
}

int calculate(int mA, int mB)
{
    if (mB < mA) swap(mA, mB);

    int res = 0;
    for (int i = mA; i <= mB - 1; i++)
        res += roads[i].mTime;

    return res;
}
#endif