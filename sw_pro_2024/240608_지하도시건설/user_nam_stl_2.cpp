// 599 ms (sqrt decomposion + bfs)
#if 1
#include <vector>
using namespace std;

inline int min(int a, int b) { return (a < b)? a: b; }

#define MAX_BOXES   10001   // dropBox() 함수는 최대 10,000 회 호출된다. 1 ~ 10000
#define MAX_H       1000
#define MAX_W       200000

int mH;                     // 웅덩이의 세로 방향의 길이 (8 ≤ mH ≤ 1,000)    0 ~ mH - 1
int mW;                     // 웅덩이의 가로 방향의 길이 (10 ≤ mW ≤ 200,000) 0 ~ mW - 1
struct Box { int lo, hi, A, B; } boxes[MAX_BOXES];
vector<int> boxList[MAX_H];

// range min query
struct RangeMinQuery {
    int arr[MAX_W];         // min row
    int groups[MAX_W];      // group min
    int sq;

    void clear() {
        sq = sqrt(mW);
        for (int i = 0; i < mW; i++) arr[i] = mH, groups[i] = mH;
    }
    void updateRange(int lo, int hi, int value) {
        for (int i = lo; i <= hi; i++) arr[i] = value;
        for (int i = lo / sq; i <= hi / sq; i++) groups[i] = value;
    }
    int queryRange(int lo, int hi) {
        int res = mH;
        int s = lo / sq, e = hi / sq;
        if (s == e) {
            for (int i = lo; i <= hi; i++) res = min(res, arr[i]);
            return res;
        }
        for (int i = lo; i < (s + 1) * sq; i++) res = min(res, arr[i]);
        for (int i = s + 1; i <= e - 1; i++) res = min(res, groups[i]);
        for (int i = e * sq; i <= hi; i++) res = min(res, arr[i]);
        return res;
    }
} rmq;

// bfs
struct Queue {
    int que[MAX_BOXES];
    int head, tail;

    void clear() { head = tail = 0; }
    void push(int data) { que[tail++] = data; }
    void pop() { head++; }
    int front() { return que[head]; }
    bool empty() { return head == tail; }
} q;
vector<int> adjList[MAX_BOXES];
int visited[MAX_BOXES];

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
    for (int bId: boxList[row])
        if (boxes[bId].hi + 1 == lo || boxes[bId].lo - 1 == hi) {
            adjList[mId].push_back(bId);
            adjList[bId].push_back(mId);
        }

    // add edge for bottom
    if (row < mH - 1)
        for (int bId: boxList[row + 1]) {
            if (boxes[bId].lo <= A && A <= boxes[bId].hi) adjList[mId].push_back(bId);
            if (boxes[bId].lo <= B && B <= boxes[bId].hi) adjList[mId].push_back(bId);
            if (lo <= boxes[bId].A && boxes[bId].A <= hi) adjList[bId].push_back(mId);
            if (lo <= boxes[bId].B && boxes[bId].B <= hi) adjList[bId].push_back(mId);
        }

    // drop box
    rmq.updateRange(lo, hi, row);
    boxList[row].push_back(mId);

    return row;
}

// bfs
int explore(int mIdA, int mIdB)
{
    q.clear();
    memset(visited, 0, sizeof(visited));

    visited[mIdA] = 1;
    q.push(mIdA);
    int res = -1;
    while (!q.empty()) {
        int cur = q.front(); q.pop();

        if (cur == mIdB) { res = visited[cur] - 1; break; }
        for (int next: adjList[cur]) {
            if (visited[next]) continue;
            visited[next] = visited[cur] + 1;
            q.push(next);
        }
    }
    return res;
}
#endif