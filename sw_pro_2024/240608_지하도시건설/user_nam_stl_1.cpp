#if 1
#include <vector>
#include <queue>
#include <set>
using namespace std;

#define MAX_BOXES   10001   // dropBox() 함수는 최대 10,000 회 호출된다. 1 ~ 10000
#define MAX_H       1000
#define MAX_W       200000

int mH;                     // 웅덩이의 세로 방향의 길이 (8 ≤ mH ≤ 1,000)    0 ~ mH - 1
int mW;                     // 웅덩이의 가로 방향의 길이 (10 ≤ mW ≤ 200,000) 0 ~ mW - 1

struct Box {
    int left, right, mExitA, mExitB;
} boxes[MAX_BOXES];

vector<int> boxList[MAX_H];

// range min query
struct RangeMinQuery {
    int arr[MAX_W];             // min row

    void clear() {
        for (int i = 0; i < mW; i++) arr[i] = mH;
    }
    void updateRange(int left, int right, int value) {
        for (int i = left; i <= right; i++) arr[i] = value;
    }
    int queryRange(int left, int right) {
        int res = mH;
        for (int i = left; i <= right; i++) res = min(res, arr[i]);
        return res;
    }
} rmq;

// dijkstra
struct Edge { 
    int to, dist;
    bool operator<(const Edge& edge) const { return dist > edge.dist; }
};
vector<Edge> adjList[MAX_BOXES];
int distDP[MAX_BOXES];
priority_queue<Edge> pq;

///////////////////////////////////////////////////////////////////////////////
void init(int mH, int mW)
{
    // for (int i = 0; i < MAX_BOXES; i++) boxes[i] = {};
    for (int i = 0; i < MAX_H; i++) boxList[i].clear();
    ::mH = mH, ::mW = mW;
    rmq.clear();
}

int dropBox(int mId, int mLen, int mExitA, int mExitB, int mCol)
{
    int left = mCol, right = mCol + mLen - 1;
    boxes[mId] = { left, right, mExitA, mExitB };
    int row = rmq.queryRange(left, right) - 1;

    // add edge for side
    for (int bId: boxList[row])
        if (boxes[bId].right + 1 == left || boxes[bId].left - 1 == right) {
            adjList[mId].push_back({ bId, 2 });
            adjList[bId].push_back({ mId, 2 });
        }

    // add edge for bottom
    if (row < mH - 1)
        for (int bId: boxList[row + 1]) {
            if (boxes[bId].left <= mExitA && mExitA <= boxes[bId].right) adjList[mId].push_back({ bId, 1 });
            if (boxes[bId].left <= mExitB && mExitB <= boxes[bId].right) adjList[mId].push_back({ bId, 1 });
            if (left <= boxes[bId].mExitA && boxes[bId].mExitA <= right) adjList[bId].push_back({ mId, 1 });
            if (left <= boxes[bId].mExitB && boxes[bId].mExitB <= right) adjList[bId].push_back({ mId, 1 });
        }

    // drop box
    rmq.updateRange(left, right, row);
    boxList[row].push_back(mId);

    return row;
}

// dijkstra
int explore(int mIdA, int mIdB)
{
    pq = {};
    for (int i = 0; i <= MAX_BOXES; i++) distDP[i] = 1e6;

    distDP[mIdA] = 0;
    pq.push({ mIdA, distDP[mIdA] });

    int res = -1;
    while (!pq.empty()) {
        auto cur = pq.top(); pq.pop();

        if (cur.to == mIdB) { res = cur.dist; break; }
        if (distDP[cur.to] < cur.dist) continue;

        for (const auto& next: adjList[cur.to]) 
            if (distDP[next.to] > distDP[cur.to] + next.dist) {
                distDP[next.to] = distDP[cur.to] + next.dist;
                pq.push({ next.to, distDP[next.to] });
            }
    }
    return res;
}
#endif