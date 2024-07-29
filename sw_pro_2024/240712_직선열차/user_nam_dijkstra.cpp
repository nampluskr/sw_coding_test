#if 0
#include <unordered_map>
#include <vector>
#include <queue>
using namespace std;

#define MAX_TRAINS      (50 + 150)      // 열차 개수 (3 ≤ K ≤ 50)
#define MAX_STATIONS    (100'000 + 1)   // 열차역 개수 (20 ≤ N ≤ 100,000)
#define INF             (MAX_TRAINS + 1)

int N;

// trains
enum State { ADDED, REMOVED } states[MAX_TRAINS];

int trainCnt;
unordered_map<int, int> trainMap;
vector<int> trainList[MAX_STATIONS];

// dijkstra
struct Edge {
    int train, station, cost;
    bool operator<(const Edge& edge) const { return cost > edge.cost; }
};
vector<Edge> adjList[MAX_TRAINS][MAX_STATIONS];

priority_queue<Edge> pq;
int costDP[MAX_TRAINS][MAX_STATIONS];

//////////////////////////////////////////////////////////////////////
int get_train(int mId) {
    int train;
    auto iter = trainMap.find(mId);
    if (iter == trainMap.end()) {
        train = trainCnt++;
        trainMap.emplace(mId, train);
    }
    else train = iter->second;
    return train;
}

int dijkstra(int sTrain, int sStation, int eTrain, int eStation, int curCnt) {
    pq = {};
    for (int i = 0; i < trainCnt; i++)
        for (int j = 0; j <= N; j++)
            costDP[i][j] = INF;

    costDP[sTrain][sStation] = 0;
    pq.push({ sTrain, sStation, costDP[sTrain][sStation] });

    while (!pq.empty()) {
        auto cur = pq.top(); pq.pop();

        if (states[cur.train]== REMOVED) continue;
        if (cur.cost >= curCnt) { return -1; }
        if (cur.train == eTrain && cur.station == eStation) { return cur.cost; }
        if (costDP[cur.train][cur.station] < cur.cost) continue;

        for (const auto next : adjList[cur.train][cur.station]) {
            if (states[next.train] == REMOVED) continue;

            if (costDP[next.train][next.station] > next.cost + cur.cost) {
                costDP[next.train][next.station] = next.cost + cur.cost;
                pq.push({ next.train, next.station, costDP[next.train][next.station] });
            }
        }
    }
    return -1;
}

//////////////////////////////////////////////////////////////////////
void add(int mId, int sId, int eId, int mInterval);

void init(int N, int K, int mId[], int sId[], int eId[], int mInterval[])
{
    ::N = N;
    trainCnt = 0;
    trainMap.clear();

    for (int i = 0; i <= N; i++) trainList[i].clear();

    for (int i = 0; i < K + 150; i++)
        for (int j = 0; j <= N; j++)
            adjList[i][j].clear();

    for (int i = 0; i < K; i++) add(mId[i], sId[i], eId[i], mInterval[i]);
}

void add(int mId, int sId, int eId, int mInterval)
{
    int train = get_train(mId);
    states[train] = ADDED;

    // 열차역 노드 추가 (가중치 0)
    for (int station = sId; station <= eId - mInterval; station += mInterval) {
        adjList[train][station].push_back({ train, station + mInterval, 0 });
        adjList[train][station + mInterval].push_back({ train, station, 0 });
    }

    // 열차 노드 추가 (가중치 1)
    for (int station = sId; station <= eId; station += mInterval) {
        for (int other : trainList[station]) {
            adjList[train][station].push_back({ other, station, 1 });
            adjList[other][station].push_back({ train, station, 1 });
        }
        trainList[station].push_back(train);
    }
}

void remove(int mId)
{
    int train = get_train(mId);
    states[train] = REMOVED;
}

int calculate(int sId, int eId)
{
    int res = INF;
    for (int sTrain : trainList[sId]) {
        if (states[sTrain] == REMOVED) continue;

        for (int eTrain : trainList[eId]) {
            if (states[eTrain] == REMOVED) continue;

            int minCnt = dijkstra(sTrain, sId, eTrain, eId, res);
            if (minCnt == -1) continue;
            if (minCnt < res) res = minCnt;
        }
    }
    if (res == INF) return -1;
    return res;
}
#endif