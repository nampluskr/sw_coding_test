#include <vector>
#include <queue>
using namespace std;

#define MAX_NODES   101
#define MAX_COST    1001                // M: 사용 가능한 총 비용 ( 1 ≤ M ≤ 1,000 )
#define MAX_TIME    100 * MAX_NODES     // mTime: 도로의 소요 시간 ( 1 ≤ mTime ≤ 100 )

int N;      // N: 도시의 개수 ( 5 ≤ N ≤ 100 )
int K;      // K: 도로의 개수 ( 3 ≤ K ≤ 500 )

struct Edge {
    int to, cost, time;
    bool operator<(const Edge& edge) const { return time > edge.time; }
};
vector<Edge> adjList[MAX_COST];
int timeDP[MAX_NODES][MAX_COST];

/////////////////////////////////////////////////////////////////////
void add(int sCity, int eCity, int mCost, int mTime);

void init(int N, int K, int sCity[], int eCity[], int mCost[], int mTime[])
{
    ::N = N;
    ::K = K;
    for (int i = 0; i < N; i++) adjList[i].clear();
    for (int i = 0; i < K; i++) add(sCity[i], eCity[i], mCost[i], mTime[i]);
}

void add(int sCity, int eCity, int mCost, int mTime)
{
    adjList[sCity].push_back({ eCity, mCost, mTime });
}

// dijkstra
int cost(int M, int sCity, int eCity)
{
    priority_queue<Edge> pq;
    for (int i = 0; i < MAX_NODES; i++)
        for (int j = 0; j <= M; j++) timeDP[i][j] = MAX_TIME;

    timeDP[sCity][0] = 0;
    pq.push({ sCity, 0, timeDP[sCity][0] });

    int res = -1;
    while (!pq.empty()) {
        auto cur = pq.top(); pq.pop();

        // 종료 조건
        if (cur.to == eCity) { res = cur.time; break; }

        if (timeDP[cur.to][cur.cost] < cur.time) continue;
        for (const auto& next: adjList[cur.to]) {
            int next_cost = cur.cost + next.cost;
            if (next_cost > M) continue;

            if (timeDP[next.to][next_cost] > cur.time + next.time) {
                timeDP[next.to][next_cost] = cur.time + next.time;
                pq.push({ next.to, next_cost, timeDP[next.to][next_cost] });
            }
        }
    }
    return res;
}