// 18.734 s (/O2)
#if 0
#include <unordered_map>
#include <vector>
#include <queue>
using namespace std;

#define MAX_TRAINS      (50 + 150)
#define MAX_STATIONS    (100'000 + 1)
#define INF             1e6

int N;        // 열차역 개수 (20 ≤ N ≤ 100,000)
int K;        // 열차 개수 (3 ≤ K ≤ 50)

// trains
enum State { ADDED, REMOVED };

struct Train {
    int mId, sId, eId, mInterval;
    State state;
} trains[MAX_TRAINS];

int trainCnt;
unordered_map<int, int> trainMap;
vector<int> trainList[MAX_STATIONS];

// graph
struct Edge {
    int train, station, cost;
    bool operator<(const Edge& edge) const { return cost > edge.cost; }
};
// vector<Edge> adjList[MAX_TRAINS][MAX_STATIONS];
unordered_map<int, unordered_map<int, vector<Edge>>> adjList;

priority_queue<Edge> pq;
int costDP[MAX_TRAINS][MAX_STATIONS];

//////////////////////////////////////////////////////////////////////
int get_trainIndex(int mId) {
    int tIdx;
    auto iter = trainMap.find(mId);
    if (iter == trainMap.end()) {
        tIdx = trainCnt++;
        trainMap.emplace(mId, tIdx);
    }
    else tIdx = iter->second;
    return tIdx;
}

int dijkstra(int sTrain, int sStation, int eTrain, int eStation) {
    pq = {};
    for (int i = 0; i < K + 150; i++)
        for (int j = 0; j <= N; j++)
            costDP[i][j] = INF;

    costDP[sTrain][sStation] = 0;
    pq.push({ sTrain, sStation, costDP[sTrain][sStation] });

    int res = -1;
    while (!pq.empty()) {
        auto cur = pq.top(); pq.pop();

        if (trains[cur.train].state == REMOVED) continue;
        if (cur.train == eTrain && cur.station == eStation) { res = cur.cost; break; }
        if (costDP[cur.train][cur.station] < cur.cost) continue;

        for (const auto next : adjList[cur.train][cur.station]) {
            if (trains[next.train].state == REMOVED) continue;

            if (costDP[next.train][next.station] > next.cost + cur.cost) {
                costDP[next.train][next.station] = next.cost + cur.cost;
                pq.push({ next.train, next.station, costDP[next.train][next.station] });
            }
        }
    }
    return res;
}

//////////////////////////////////////////////////////////////////////
void add(int mId, int sId, int eId, int mInterval);

void init(int N, int K, int mId[], int sId[], int eId[], int mInterval[])
{
    ::N = N, ::K = K;
    trainCnt = 0;
    trainMap.clear();

    for (int i = 0; i <= N; i++) trainList[i].clear();

    // for (int i = 0; i < K + 150; i++)
    //     for (int j = 0; j <= N; j++)
    //         adjList[i][j].clear();
    adjList.clear();

    for (int i = 0; i < K; i++) add(mId[i], sId[i], eId[i], mInterval[i]);
}

void add(int mId, int sId, int eId, int mInterval)
{
    int tIdx = get_trainIndex(mId);
    trains[tIdx] = { mId, sId, eId, mInterval, ADDED };

    for (int i = sId; i <= eId - mInterval; i += mInterval) {
        adjList[tIdx][i].push_back({ tIdx, i + mInterval, 0 });
        adjList[tIdx][i + mInterval].push_back({ tIdx, i, 0 });
    }

    for (int i = sId; i <= eId; i += mInterval) {
        for (int train : trainList[i]) {
            adjList[tIdx][i].push_back({ train, i, 1 });
            adjList[train][i].push_back({ tIdx, i, 1 });
        }
        trainList[i].push_back(tIdx);
    }
}

void remove(int mId)
{
    int tIdx = get_trainIndex(mId);
    trains[tIdx].state = REMOVED;
}

int calculate(int sId, int eId)
{
    int res = INF;
    for (int sTrain : trainList[sId]) {
        if (trains[sTrain].state == REMOVED) continue;

        for (int eTrain : trainList[eId]) {
            if (trains[eTrain].state == REMOVED) continue;

            int minCost = dijkstra(sTrain, sId, eTrain, eId);
            if (minCost == -1) continue;
            if (minCost < res) res = minCost;
        }
    }
    if (res == INF) res = -1;
    return res;
}
#endif