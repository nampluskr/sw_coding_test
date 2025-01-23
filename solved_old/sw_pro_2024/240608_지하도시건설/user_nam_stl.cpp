#include <vector>
#include <queue>
using namespace std;

inline int min(int a, int b) { return (a < b) ? a : b; }

#define MAX_BOXES   10001
#define MAX_H       1000
#define MAX_W       200000

int mH;
struct Box { int lo, hi, A, B; } boxes[MAX_BOXES];
vector<int> boxList[MAX_H];
int arr[MAX_W];

// bfs
vector<int> adj[MAX_BOXES];
int visited[MAX_BOXES];

///////////////////////////////////////////////////////////////////////////////
void init(int mH, int mW)
{
    ::mH = mH;
    for (int i = 0; i < mH; i++) boxList[i].clear();
    for (int i = 0; i < MAX_BOXES; i++) adj[i].clear();
    for (int i = 0; i < mW; i++) arr[i] = mH;
}

int dropBox(int mId, int mLen, int mExitA, int mExitB, int mCol)
{
    int lo = mCol, hi = mCol + mLen - 1;
    int A = mCol + mExitA, B = mCol + mExitB;
    
    // get the row number of the top box in [lo, hi]
    int row = mH;
    for (int i = lo; i <= hi; i++) row = min(row, arr[i]);
    row--;

    // add edge for side boxes
    for (int bId : boxList[row])
        if (boxes[bId].hi + 1 == lo || boxes[bId].lo - 1 == hi) {
            adj[mId].push_back(bId);
            adj[bId].push_back(mId);
        }

    // add edge for bottom boxes
    if (row < mH - 1)
        for (int bId : boxList[row + 1]) {
            if (boxes[bId].lo <= A && A <= boxes[bId].hi) adj[mId].push_back(bId);
            if (boxes[bId].lo <= B && B <= boxes[bId].hi) adj[mId].push_back(bId);
            if (lo <= boxes[bId].A && boxes[bId].A <= hi) adj[bId].push_back(mId);
            if (lo <= boxes[bId].B && boxes[bId].B <= hi) adj[bId].push_back(mId);
        }

    // drop box
    boxes[mId] = { lo, hi, A, B };
    for (int i = lo; i <= hi; i++) arr[i] = row;
    boxList[row].push_back(mId);

    return row;
}

// bfs
int explore(int mIdA, int mIdB)
{
    queue<int> que;
    memset(visited, 0, sizeof(visited));
    visited[mIdA] = 1;
    que.push(mIdA);

    while (!que.empty()) {
        int cur = que.front(); que.pop();

        if (cur == mIdB) return visited[cur] - 1;
        for (int next : adj[cur]) 
            if (!visited[next]) {
                visited[next] = visited[cur] + 1;
                que.push(next);
            }
    }
    return -1;
}