#if 1
#include <unordered_map>
#include <vector>
using namespace std;

#define MAX_TRAINS      (50 + 150)      // 열차 개수 (3 ≤ K ≤ 50), add() 50번
#define MAX_STATIONS    (100'000 + 1)   // 열차역 개수 (20 ≤ N ≤ 100,000), 1부터 시작
#define INF             (MAX_TRAINS + 1)

// trains
enum State { ADDED, REMOVED } states[MAX_TRAINS];

struct Train { 
    int sId, eId, mInterval;
} trains[MAX_TRAINS];

int trainCnt;
unordered_map<int, int> trainMap;
vector<int> trainList[MAX_STATIONS];

// bfs
vector<int> adjList[MAX_TRAINS];
int visited[MAX_TRAINS];

struct Queue {
    int arr[MAX_TRAINS];
    int head, tail;

    void clear() { head = tail = 0; }
    void push(int data) { arr[tail++] = data; }
    void pop() { head++; }
    int front() { return arr[head]; }
    bool empty() { return head == tail; }
} que;

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

// 열차 train 의 열차역 중에 station 이 포함되어 있는지 검사
bool isStation(int train, int station) {
    if (states[train] == REMOVED) return false;
    if (station < trains[train].sId) return false;
    if (trains[train].eId < station) return false;
    return (station - trains[train].sId) % trains[train].mInterval == 0;
}

int bfs(int sTrain, int eStation, int curCnt) {
    que.clear();
    for (int i = 0; i < trainCnt; i++) visited[i] = 0;

    visited[sTrain] = 1;
    que.push(sTrain);

    while (!que.empty()) {
        int cur = que.front(); que.pop();

        if (states[cur] == REMOVED) continue;
        if (visited[cur] > curCnt) { return -1; }
        if (isStation(cur, eStation)) { return visited[cur] - 1; }
        
        for (int next : adjList[cur]) {
            if (states[next] == REMOVED) continue;
            if (visited[next] > 0) continue;
            visited[next] = visited[cur] + 1;
            que.push(next);
        }
    }
    return -1;
}

//////////////////////////////////////////////////////////////////////
void add(int mId, int sId, int eId, int mInterval);

void init(int N, int K, int mId[], int sId[], int eId[], int mInterval[])
{
    trainCnt = 0;
    trainMap.clear();
    for (int i = 0; i <= N; i++) trainList[i].clear();
    for (int i = 0; i < MAX_TRAINS; i++) adjList[i].clear();

    for (int i = 0; i < K; i++)
        add(mId[i], sId[i], eId[i], mInterval[i]);
}

void add(int mId, int sId, int eId, int mInterval)
{
    int train = get_train(mId);
    trains[train] = { sId, eId, mInterval };
    states[train] = ADDED;

    for (int station = sId; station <= eId; station += mInterval) {
        for (int other : trainList[station]) {
            if (states[train] == REMOVED) continue;
            if (isStation(other, station)) {
                adjList[train].push_back(other);
                adjList[other].push_back(train);
            }
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

        int minCnt = bfs(sTrain, eId, res);
        if (minCnt == -1) continue;
        if (minCnt < res) res = minCnt;
    }
    if (res == INF) return -1;
    return res;
}
#endif