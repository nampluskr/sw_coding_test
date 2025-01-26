#if 1   // 480 ms (STL)
#include "unordered_map.h"
#include "vector.h"

inline void swap(int& a, int& b) { int temp = a; a = b; b = temp; }
inline int min(int a, int b) { return (a < b) ? a : b; }
inline int max(int a, int b) { return (a < b) ? b : a; }
inline int abs(int x) { return (x > 0) ? x : -x; }

#define MAX_HOUSES  25'000      // add() 함수의 호출 횟수는 25,000 이하
#define INF         1e9 + 1     // mId: 집 ID ( 1 ≤ mId ≤ 1,000,000,000 )
#define MAX_BUCKETS 141         // 파티션 최대 크기

enum State { REMOVED, ADDED };

struct House {
    int mId, mX, mY, vIdx;
    State state;
} houses[MAX_HOUSES];
int houseCnt;
UnorderedMap<int, int> houseMap;

struct Village {
    int minId, size;
    Vector<int> houseList;
} villages[MAX_HOUSES];
int villageCnt;

int L;              // L: 마을을 이루는 기준 거리 ( 10 ≤ L ≤ 200 )
int R;              // R: X좌표와 Y좌표의 최댓값 ( 25 ≤ R ≤ L x 100 )
int bucketSize;     // 2D 파티션 한변 크기 sqrt(R + 1)
Vector<int> buckets[MAX_BUCKETS][MAX_BUCKETS];

/////////////////////////////////////////////////////////////////////
// Helper Functions

void merge(int x, int y) {
    if (x == y) return;
    if (villages[x].size < villages[y].size)
        swap(x, y);

    // y (small vilage) -> x (large village)
    villages[x].minId = min(villages[x].minId, villages[y].minId);
    villages[x].size += villages[y].size;
    villageCnt--;

    for (int hIdx : villages[y].houseList) {
        if (houses[hIdx].state == REMOVED) continue;
        villages[x].houseList.push_back(hIdx);
        houses[hIdx].vIdx = x;
    }
    // villages[y].houseList.clear();
}

int getNewIndex(int mId) {
    int idx;
    auto iter = houseMap.find(mId);
    if (iter == houseMap.end()) {
        idx = houseCnt++;
        houseMap.emplace(mId, idx);
    }
    else { idx = iter->second; }    // houses[idx].state == REMOVED
    if (houses[idx].state == ADDED) return -1;  // 에러 체크
    return idx;
}

int findHouseIndex(int mId) {
    auto iter = houseMap.find(mId);
    if (iter == houseMap.end()) return -1;

    int idx = iter->second;
    if (houses[idx].state == REMOVED) return -1;
    return idx;
}

/////////////////////////////////////////////////////////////////////
void init(int L, int R) {
    ::L = L;
    ::R = R;
    ::bucketSize = L;

    houseCnt = 0;
    villageCnt = 0;
    houseMap.clear();

    for (int i = 0; i < MAX_HOUSES; i++) {
        houses[i] = {};
        villages[i].houseList.clear();
    }
    for (int i = 0; i < R / bucketSize; i++)
        for (int j = 0; j < R / bucketSize; j++)
            buckets[i][j].clear();
}

int add(int mId, int mX, int mY) {
    int hIdx = getNewIndex(mId);
    auto& house = houses[hIdx];
    house = { mId, mX, mY, hIdx, ADDED };
    villages[hIdx].minId = mId;
    villages[hIdx].size = 1;
    villages[hIdx].houseList.push_back(hIdx);
    villageCnt++;

    // L 거리내에 있는 집들과 merge
    int sX = max((mX - L) / bucketSize, 0);
    int sY = max((mY - L) / bucketSize, 0);
    int eX = min((mX + L) / bucketSize, R / bucketSize);
    int eY = max((mY + L) / bucketSize, R / bucketSize);

    for (int i = sX; i <= eX; i++)
        for (int j = sY; j <= eY; j++)
            for (int idx : buckets[i][j]) {
                auto& other = houses[idx];
                if (other.state == REMOVED) continue;
                int dist = abs(house.mX - other.mX) + abs(house.mY - other.mY);
                if (dist > L) continue;
                merge(house.vIdx, other.vIdx);
            }

    buckets[mX / bucketSize][mY / bucketSize].push_back(hIdx);
    return villages[house.vIdx].size;
}

int remove(int mId) {
    int hIdx = findHouseIndex(mId);
    if (hIdx == -1) return -1;

    houses[hIdx].state = REMOVED;
    int vIdx = houses[hIdx].vIdx;
    villages[vIdx].size--;
    if (villages[vIdx].size == 0) villageCnt--;

    // 최소 집 ID 업데이트
    if (villages[vIdx].minId == mId) {
        villages[vIdx].minId = INF;
        for (int idx : villages[vIdx].houseList) {
            if (houses[idx].state == REMOVED) continue;
            villages[vIdx].minId = min(villages[vIdx].minId, houses[idx].mId);
        }
    }
    return villages[vIdx].size;
}

int check(int mId) {
    int hIdx = findHouseIndex(mId);
    if (hIdx == -1) return -1;

    int vIdx = houses[hIdx].vIdx;
    return villages[vIdx].minId;
}

int count() {
    return villageCnt;
}
#endif
#endif