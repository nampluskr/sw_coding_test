#if 1
#include <vector>
#include <queue>
#include <unordered_map>
using namespace std;

#define MAX_NODES   600     // 도시의 최대 개수는 600 이하
#define INF         1e6     // ( 1 ≤ mCost[i] ≤ 100 ) * 1400

int cityCnt;
unordered_map<int, int> cityMap;

struct Edge {
    int to, cost;
    bool operator<(const Edge& edge) const { return cost > edge.cost; }
};
vector<Edge> adjList1[MAX_NODES];   // sCity -> eCity
vector<Edge> adjList2[MAX_NODES];   // eCity -> sCity
int costDP[MAX_NODES];
priority_queue<Edge> pq;

int get_cityIndex(int mCity) {
    int cIdx;
    auto iter = cityMap.find(mCity);
    if (iter == cityMap.end()) {
        cIdx = cityCnt++;
        cityMap.emplace(mCity, cIdx);
    }
    else { cIdx = iter->second; }
    return cIdx;
}

// single to single
int dijkstra1(int sIdx, int eIdx) {
    pq = {};
    for (int i = 0; i < cityCnt; i++) costDP[i] = INF;
    costDP[sIdx] = 0;
    pq.push({ sIdx, costDP[sIdx] });

    int res = 0;
    while (!pq.empty()) {
        auto cur = pq.top(); pq.pop();

        if (cur.to == eIdx) { res = cur.cost; break; }

        if (costDP[cur.to] < cur.cost) continue;
        for (const auto& next: adjList1[cur.to])
            if (costDP[next.to] > cur.cost + next.cost) {
                costDP[next.to] = cur.cost + next.cost;
                pq.push({ next.to, costDP[next.to] });
            }
    }
    return res;
}

// single to all
int dijkstra2(int sIdx) {
    int res = 0;

    // cities to hub -> use adjList1
    pq = {};
    for (int i = 0; i < cityCnt; i++) costDP[i] = INF;
    costDP[sIdx] = 0;
    pq.push({ sIdx, costDP[sIdx] });

    while (!pq.empty()) {
        auto cur = pq.top(); pq.pop();

        if (costDP[cur.to] < cur.cost) continue;
        for (const auto& next: adjList1[cur.to])
            if (costDP[next.to] > cur.cost + next.cost) {
                costDP[next.to] = cur.cost + next.cost;
                pq.push({ next.to, costDP[next.to] });
            }
    }
    for (int i = 0; i < cityCnt; i++) res += costDP[i];

    // hub to cities -> use adjList2
    pq = {};
    for (int i = 0; i < cityCnt; i++) costDP[i] = INF;
    costDP[sIdx] = 0;
    pq.push({ sIdx, costDP[sIdx] });

    while (!pq.empty()) {
        auto cur = pq.top(); pq.pop();

        if (costDP[cur.to] < cur.cost) continue;
        for (const auto& next: adjList2[cur.to])
            if (costDP[next.to] > cur.cost + next.cost) {
                costDP[next.to] = cur.cost + next.cost;
                pq.push({ next.to, costDP[next.to] });
            }
    }
    for (int i = 0; i < cityCnt; i++) res += costDP[i];

    return res;
}

/////////////////////////////////////////////////////////////////////
void add(int sCity, int eCity, int mCost);

// 도시의 총 개수를 반환한다.
int init(int N, int sCity[], int eCity[], int mCost[])
{
    cityCnt = 0;
    for (int i = 0; i < MAX_NODES; i++) adjList1[i].clear();
    for (int i = 0; i < MAX_NODES; i++) adjList2[i].clear();

    // N: 도로의 개수 ( 10 ≤ N ≤ 1400 )
    for (int i = 0; i < N; i++) add(sCity[i], eCity[i], mCost[i]);
    return cityCnt;
}

void add(int sCity, int eCity, int mCost)
{
    int sIdx = get_cityIndex(sCity);
    int eIdx = get_cityIndex(eCity);
    adjList1[sIdx].push_back({ eIdx, mCost });
    adjList2[eIdx].push_back({ sIdx, mCost });
}

// 각 도시에서 허브 도시까지 왕복에 필요한 최소 비용을 모두 합한 값을 반환한다.
int cost(int mHub)
{
    int hIdx = get_cityIndex(mHub);
    int res = 0;

    // 시간초과 (8180 ms)
    // for (int cIdx = 0; cIdx < cityCnt; cIdx++) {
    //     // if (cIdx == hIdx) continue;
    //     res += dijkstra1(cIdx, hIdx) + dijkstra1(hIdx, cIdx);
    // }

    // 시간 감소 (76 ms)
    res = dijkstra2(hIdx);
    return res;
}
#endif