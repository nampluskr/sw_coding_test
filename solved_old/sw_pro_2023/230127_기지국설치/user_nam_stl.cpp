#if 1
#include <vector>
#include <unordered_map>
#include <algorithm>
using namespace std;

#define MAX_LOC 24'000
#define ADDED   0
#define REMOVED 1

struct Building {
    int loc;
    int state;
} buildings[MAX_LOC];
int addCnt;
int removeCnt;

unordered_map<int, int> locMap;

////////////////////////////////////////////////////////////////////
int add(int mId, int mLocation);

void init(int N, int mId[], int mLocation[])
{
    //for (int i = 0; i < MAX_LOC; i++) buildings[i] = {};
    addCnt = removeCnt = 0;
    locMap.clear();
    for (int i = 0; i < N; i++) add(mId[i], mLocation[i]);
}

// 24'000
int add(int mId, int mLocation)
{
    int bIdx;
    auto iter = locMap.find(mId);
    if (iter == locMap.end()) {
        bIdx = addCnt++;
        locMap.emplace(mId, bIdx);
    } else {
        bIdx = iter->second;
        if (buildings[bIdx].state == REMOVED) removeCnt--;
    }
    buildings[bIdx] = { mLocation, ADDED };
    return addCnt - removeCnt;
}

// 3'000 (linear search)
int remove(int mStart, int mEnd)
{
    for (int i = 0; i < addCnt; i++) {
        if (buildings[i].state == REMOVED) continue;
        if (buildings[i].loc >= mStart && buildings[i].loc <= mEnd) {
            buildings[i].state = REMOVED;
            removeCnt++;
        }
    }
    return addCnt - removeCnt;
}

// 인접한 기지국 거리를 target 이상으로 M개 선택할 수 있으면 true
int decide(const vector<int>& arr, int x, int target) {
    int cnt = 1;
    int cur = arr[0];
    for (int i = 1; i < arr.size(); i++)
        if (x <= arr[i] - cur) { cnt++; cur = arr[i]; }
    return cnt >= target;
}

int install(int M)
{
    // sort
    vector<int> arr;
    for (int i = 0; i < addCnt; i++) {
        if (buildings[i].state == REMOVED) continue;
        arr.push_back(buildings[i].loc);
    }
    sort(arr.begin(), arr.end());

    // parametric search (lo, hi, mid: 인접한 기지국 거리)
    int lo = 0, hi = arr.back() - arr.front();
    int res = lo;
    while (lo <= hi) {
        int mid = (lo + hi) / 2;
        if (decide(arr, mid, M)) res = mid, lo = mid + 1;
        else hi = mid - 1;
    }
    return res; // hi (upper bound)
}
#endif
