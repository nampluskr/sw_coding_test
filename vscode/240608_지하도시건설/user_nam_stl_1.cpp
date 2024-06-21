// 최적화 OFF 618 ms vs. 최적화 ON 350 ms
#if 0
#include <vector>
#include <queue>
// #include <set>
using namespace std;

#define MAX_BOXES   10001   // dropBox() 함수는 최대 10,000 회 호출된다. 1 ~ 10000
#define MAX_H       1000
#define MAX_W       200000

int mH;                     // 웅덩이의 세로 방향의 길이 (8 ≤ mH ≤ 1,000)    0 ~ mH - 1
int mW;                     // 웅덩이의 가로 방향의 길이 (10 ≤ mW ≤ 200,000) 0 ~ mW - 1
struct Box { int lo, hi, A, B; } boxes[MAX_BOXES];
vector<int> boxList[MAX_H];

// range min query
struct RangeMinQuery {
    int arr[MAX_W];             // min row

    void clear() { for (int i = 0; i < mW; i++) arr[i] = mH; }
    void updateRange(int lo, int hi, int value) {
        for (int i = lo; i <= hi; i++) arr[i] = value;
    }
    int queryRange(int lo, int hi) {
        int res = mH;
        for (int i = lo; i <= hi; i++) res = min(res, arr[i]);
        return res;
    }
} rmq;

// dijkstra
struct Edge {
    int to, dist;
    bool operator<(const Edge& edge) const {
        return (dist > edge.dist) || (dist == edge.dist && to > edge.to);
    }
};
// set<Edge> adjList[MAX_BOXES];   // 중복 제거 insert()
vector<Edge> adjList[MAX_BOXES];   // 중복 허용 push_back()
int distDP[MAX_BOXES*10];
priority_queue<Edge> pq;

///////////////////////////////////////////////////////////////////////////////
void init(int mH, int mW)
{
    // for (int i = 0; i < MAX_BOXES; i++) boxes[i] = {};
    for (int i = 0; i < MAX_H; i++) boxList[i].clear();
    for (int i = 0; i < MAX_BOXES; i++) adjList[i].clear();

    ::mH = mH, ::mW = mW;
    rmq.clear();
}

int dropBox(int mId, int mLen, int mExitA, int mExitB, int mCol)
{
    int lo = mCol, hi = mCol + mLen - 1;
    int A = mCol + mExitA, B = mCol + mExitB;
    boxes[mId] = { lo, hi, A, B };
    int row = rmq.queryRange(lo, hi) - 1;

    // add edge for side
    for (int bId : boxList[row])
        if (boxes[bId].hi + 1 == lo || boxes[bId].lo - 1 == hi) {
            adjList[mId].push_back({ bId, 1 });
            adjList[bId].push_back({ mId, 1 });
        }

    // add edge for bottom
    if (row < mH - 1)
        for (int bId : boxList[row + 1]) {
            if (boxes[bId].lo <= A && A <= boxes[bId].hi) adjList[mId].push_back({ bId, 1 });
            if (boxes[bId].lo <= B && B <= boxes[bId].hi) adjList[mId].push_back({ bId, 1 });
            if (lo <= boxes[bId].A && boxes[bId].A <= hi) adjList[bId].push_back({ mId, 1 });
            if (lo <= boxes[bId].B && boxes[bId].B <= hi) adjList[bId].push_back({ mId, 1 });
        }

    // drop box
    rmq.updateRange(lo, hi, row);
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

        if (cur.to == mIdB) { res = distDP[cur.to]; break; }
        if (distDP[cur.to] < cur.dist) continue;

        for (const auto& next : adjList[cur.to])
            if (distDP[next.to] > distDP[cur.to] + next.dist) {
                distDP[next.to] = distDP[cur.to] + next.dist;
                pq.push({ next.to, distDP[next.to] });
            }
    }
    return res;
}
#endif