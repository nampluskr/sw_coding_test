#include <vector>
#include <string>
#include <unordered_map>
using namespace std;

#define MAX_NODES   12'000      // add() 함수의 호출 횟수는 12,000 이하이다.
#define MAX_DAYS    1'000'000   // 생존한 개체 수를 조사하는 날짜 ( 0 ≤ mDay ≤ 1,000,000 )

// tree node
struct Node {
    int parent, depth, mFirstday, mLastday;
} nodes[MAX_NODES];
int nodeCnt;
unordered_map<string, int> nodeMap;

int get_nodeIndex(const char mName[]) {
    int nIdx;
    auto iter = nodeMap.find(mName);
    if (iter == nodeMap.end()) {
        nIdx = nodeCnt++;
        nodeMap.emplace(mName, nIdx);
    }
    else nIdx = iter->second;
    return nIdx;
}

int get_lca(int x, int y) {
    if (nodes[x].depth < nodes[y].depth) swap(x, y);

    while (nodes[x].depth != nodes[y].depth) x = nodes[x].parent;
    while (x != y) x = nodes[x].parent, y = nodes[y].parent;
    return x;
}

struct RangeSumQuery {
    int arr[1000000];
    int groups[1000];   // base
    int sq;

    void clear() {
        memset(arr, 0, sizeof(arr));
        memset(groups, 0, sizeof(groups));
        sq = 1000;      // sqrt(MAX_DAYS)
    }
    void updateRange(int lo, int hi, int delta) {
        int s = lo / sq, e = hi / sq;
        if (s == e) {
            for (int i = lo; i <= hi; i++) arr[i] += delta;
            return;
        }
        while (lo / sq == s) arr[lo++] += delta;
        for (int i = s + 1; i <= e - 1; i++) groups[i] += delta;
        while (hi / sq == e) arr[hi--] += delta;
    }
    int queryPoint(int idx) {
        return arr[idx] + groups[idx / sq];
    }
} rsq;

/////////////////////////////////////////////////////////////////////
void init(char mAncestor[], int mLastday)
{
    for (int i = 0; i < nodeCnt; i++) nodes[i] = {};
    nodeCnt = 0;
    nodeMap.clear();
    rsq.clear();

    int nIdx = get_nodeIndex(mAncestor);
    nodes[nIdx] = { -1, 0, 0, mLastday };
    rsq.updateRange(0, mLastday, 1);
}

int add(char mName[], char mParent[], int mFirstday, int mLastday)
{
    int nIdx = get_nodeIndex(mName);
    int pIdx = get_nodeIndex(mParent);

    nodes[nIdx] = { pIdx, nodes[pIdx].depth + 1, mFirstday, mLastday };
    rsq.updateRange(mFirstday, mLastday, 1);

    int res = nodes[nIdx].depth;
    return res;
}

// LCA (Least Common Ancestor)
int distance(char mName1[], char mName2[])
{
    int x = get_nodeIndex(mName1);
    int y = get_nodeIndex(mName2);
    int lca = get_lca(x, y);
    int res = nodes[x].depth + nodes[y].depth - 2 * nodes[lca].depth;
    return res;
}

// sqrt decomposition
int count(int mDay)
{
    int res = rsq.queryPoint(mDay);
    return res;
}