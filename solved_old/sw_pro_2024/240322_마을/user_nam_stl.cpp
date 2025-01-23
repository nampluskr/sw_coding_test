#if 1
#include <unordered_map>
#include <vector>
using namespace std;

#define MAX_HOUSES  25'000
#define MAX_GROUPS  201
#define INF         1e9
#define ADDED       0
#define REMOVED     1

// 집 정보
struct House {
    int mId, mX, mY;
    int vIdx, state;
} houses[MAX_HOUSES];
int houseCnt;
unordered_map<int, int> houseMap;

// 마을 정보
struct Village {
    int min_mId;
    int num_houses;
    vector<int> houseList;
} villages[MAX_HOUSES];
int villageCnt;

// 좌표 그룹 정보
int L;
int R = 0;
int sq = 1;     // group size = sqrt(R + 1)
vector<int> groups[MAX_GROUPS][MAX_GROUPS];

inline void swap(int& x, int& y) { int temp = x; x = y; y = temp; }

void merge(int vIdx1, int vIdx2) {
    if (vIdx1 == vIdx2) return;
    if (villages[vIdx1].num_houses < villages[vIdx2].num_houses) swap(vIdx1, vIdx2);

    // vIdx2 -> vIdx1
    villages[vIdx1].min_mId = min(villages[vIdx1].min_mId, villages[vIdx2].min_mId);
    villages[vIdx1].num_houses += villages[vIdx2].num_houses;
    villageCnt--;
    for (int hIdx2 : villages[vIdx2].houseList) {  
        if (houses[hIdx2].state == REMOVED) continue;
        villages[vIdx1].houseList.push_back(hIdx2);
        //villages[vIdx1].num_houses++;
        houses[hIdx2].vIdx = vIdx1;
    }
    //villages[vIdx2] = {};
}

/////////////////////////////////////////////////////////////////////
void init(int L, int R) {
    // clear previous values
    houseMap.clear();
    for (int i = 0; i < houseCnt; i++) {
        houses[i] = {};
        villages[i].houseList.clear();
    }
    for (int i = 0; i <= R / sq; i++)
        for (int j = 0; j <= R / sq; j++)
            groups[i][j].clear();

    // set new values
    ::L = L;
    ::R = R;
    ::sq = L;     // sqrt(R + 1)
    houseCnt = 0;
    villageCnt = 0;
}

int add(int mId, int mX, int mY) {
    int hIdx1 = houseMap[mId] = houseCnt++;
    auto& house1 = houses[hIdx1] = { mId, mX, mY, hIdx1, ADDED };
    villages[hIdx1] = { mId, 1, { hIdx1 } };
    villageCnt++;

    int sX = max((mX - L) / sq, 0);
    int sY = max((mY - L) / sq, 0);
    int eX = min((mX + L) / sq, R / sq);
    int eY = max((mY + L) / sq, R / sq);

    for (int i = sX; i <= eX; i++) 
        for (int j = sY; j <= eY; j++)
            for (int hIdx2 : groups[i][j]) {
                auto& house2 = houses[hIdx2];
                if (house2.state == REMOVED) continue;
                if (abs(house1.mX - house2.mX) + abs(house1.mY - house2.mY) > L) continue;
                merge(house1.vIdx, house2.vIdx);
            }

    groups[mX / sq][mY / sq].push_back(hIdx1);
    return villages[house1.vIdx].num_houses;
}

int remove(int mId) {
    auto iter = houseMap.find(mId);
    if (iter == houseMap.end()) return -1;
    
    int hIdx = iter->second;
    int vIdx = houses[hIdx].vIdx;
    if (houses[hIdx].state == REMOVED) return -1;

    houses[hIdx].state = REMOVED;
    villages[vIdx].num_houses--;
    if (villages[vIdx].num_houses == 0) villageCnt--;

    if (villages[vIdx].min_mId == mId) {
        villages[vIdx].min_mId = INF;
        for (int hIdx : villages[vIdx].houseList) {
            if (houses[hIdx].state == REMOVED) continue;
            villages[vIdx].min_mId = min(villages[vIdx].min_mId, houses[hIdx].mId);
        }
    }
    return villages[vIdx].num_houses;
}

int check(int mId) {
    auto iter = houseMap.find(mId);
    if (iter == houseMap.end()) return -1;

    int hIdx = iter->second;
    int vIdx = houses[hIdx].vIdx;
    if (houses[hIdx].state == REMOVED) return -1;

    return villages[vIdx].min_mId;
}

int count() {
    return villageCnt;
}
#endif