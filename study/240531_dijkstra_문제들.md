### [231124_비용과시간] 최소 time 경로 구하기 - 비용 제한 M

```cpp
#include <vector>
#include <queue>
using namespace std;

#define MAX_NODES	(100 + 1)
#define MAX_COSTS	(100 + 1)
#define MAX_TIME	(100 + 1)

int N;		// 노드 개수
int K;		// 간선 개수

struct Edge {
	int city, cost, time;
	bool operator<(const Edge& edge) const { 
		if (time != edge.time) return time > edge.time;
		return cost > edge.cost;
	}
};
vector<Edge> adjList[MAX_NODES];
int times[MAX_NODES][1005];

//////////////////////////////////////////////////////////////////////
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

// dijkstra (min time path with max cost M)
int cost(int M, int sCity, int eCity)
{
	priority_queue<Edge> pq;
	for (int i = 0; i < N; i++) 
		for (int j = 0; j <= M; j++)
			times[i][j] = 1e6;

	times[sCity][0] = 0;
	pq.push({ sCity, 0, times[sCity][0]});
	while (!pq.empty()) {
		auto cur = pq.top(); pq.pop();

		if (cur.city == eCity) return times[cur.city][cur.cost];

		if (times[cur.city][cur.cost] < cur.time) continue;
		for (const auto& next : adjList[cur.city]) {
			int next_cost = cur.cost + next.cost;
			if (next_cost > M) continue;

			if (times[next.city][next_cost] > cur.time + next.time) {
				times[next.city][next_cost] = cur.time + next.time;
				pq.push({ next.city, next_cost, times[next.city][next_cost] });
			}
		}
	}
	return -1;
}
```

### [240113_화물운송] 최대 limit 경로 구하기

```cpp
#include <vector>
#include <queue>
using namespace std;

#define MAX_NODES	(1000 + 1)	
#define MAX_EDGES	(4000 + 1)	// add() 2,000
#define INF			(30'000)

int N;	// 노드 개수	[5, 1,000]
int K;	// 간선 개수 [5, 4,000]

struct Edge { 
	int city, limit;
	bool operator<(const Edge& edge) const { return limit < edge.limit; }
};
vector<Edge> adjList[MAX_NODES];
int limits[MAX_NODES];

//////////////////////////////////////////////////////////////////////
void add(int sCity, int eCity, int mLimit);

void init(int N, int K, int sCity[], int eCity[], int mLimit[])
{
	::N = N;
	::K = K;
	for (int i = 0; i < N; i++) adjList[i].clear();
	for (int i = 0; i < K; i++) add(sCity[i], eCity[i], mLimit[i]);
}

void add(int sCity, int eCity, int mLimit)
{
	adjList[sCity].push_back({ eCity, mLimit });
}

// dijkstra (max cost)
int calculate(int sCity, int eCity)
{
	priority_queue<Edge> pq;
	for (int i = 0; i < N; i++) limits[i] = 0;

	limits[sCity] = INF;
	pq.push({ sCity, limits[sCity] });

	int res = 0;
	while (!pq.empty()) {
		auto cur = pq.top(); pq.pop();

		// 종료 조건
		if (cur.city == eCity) return limits[cur.city];

		if (limits[cur.city] > cur.limit) continue;
		for (const auto& next : adjList[cur.city]) {
			int next_limit = min(cur.limit, next.limit);
			if (limits[next.city] < next_limit) {
				limits[next.city] = next_limit;
				pq.push({ next.city, limits[next.city] });
			}
		}
	}
	return -1;
}
```

### [240223_예약버스] 출발지 - 경유지(최대 5개) - 도착지

- 조지훈

```cpp
#if 0
#include <vector>	// 인접 리스트
#include <queue>	// 우선순위큐 -> 다익스트라
using namespace std;

#define MAX_NODES	501	// 노드의 수 500, 1~500까지 접근
#define INF			1e6	// 1 -> 2 -> 3 -> 4-> 5-> .. -> N : (N - 1 ) *30 = 499*30

struct Edge { 
	int to, cost;
	bool operator<(const Edge& edge) const { return cost > edge.cost; }
};
vector<Edge> adjList[MAX_NODES];	// 인접 리스트 선언
int dist[MAX_NODES];

int N;	// 노드 개수
int K;	// 간선 개수

////////////////////////////////////////////////////////////////////
// Helper functions

void dijkstra(int start, int pass1, int pass2)
{
	priority_queue<Edge> pq;
	for (int i = 1; i <= N; i++) dist[i] = INF;

	dist[start] = 0;
	pq.push({ start, dist[start] });

	while (!pq.empty()) {
		auto cur = pq.top(); pq.pop();

		if (dist[cur.to] < cur.cost) continue;
		for (const auto& next : adjList[cur.to]) {
			if (next.to == pass1 || next.to == pass2) continue;

			if (dist[next.to] > cur.cost + next.cost) {
				dist[next.to] = cur.cost + next.cost;
				pq.push({ next.to, dist[next.to] });
			}
		}
	}
}

int path[7];		// 경유지가 5개 + 시작 + 종료
int cost[7][5];		// 최단 경로 저장
int pass[7][2];
int M;
int visit[5];

int search(int n, int cur, int sum)
{
	// 마지막 경유지까지 탐색했으면
	// 최종 비용 = 현재까지의 비용 + 마지막 경유지로 이동하는 비용
	if (n == M)
		return sum + cost[M + 1][cur];

	int ret = INF;
	for (int i = 0; i < M; i++)
	{
		// 이미 서치한 경우 탐색을 하지 않는다.
		if (visit[i] == 1)
			continue;

		// 이번에 i번째 탐색할거라고 표시
		visit[i] = 1;

		// n + 1 번째를 탐색
		// 최소값을 갱신해줍니다.
		ret = min(ret, search(n + 1, i, sum + cost[cur][i]));

		// 탐색 했으면 탐색 끝났다고 표시
		visit[i] = 0;
	}

	return ret;
}

////////////////////////////////////////////////////////////////////
void addRoad(int mRoadA, int mRoadB, int mLen);

void init(int N, int K, int mRoadAs[], int mRoadBs[], int mLens[])
{
	::N = N;
	::K = K;
	for (int i = 1; i <= N; i++) adjList[i].clear();
	for (int i = 0; i < K; i++) addRoad(mRoadAs[i], mRoadBs[i],mLens[i]);
}

void addRoad(int mRoadA, int mRoadB, int mLen)
{
	adjList[mRoadA].push_back({ mRoadB, mLen });
	adjList[mRoadB].push_back({ mRoadA, mLen });
}

int findPath(int mStart, int mEnd, int M, int mStops[])
{
	::M = M;
	for (int i = 0; i < M; i++) {
		path[i] = mStops[i];
		pass[i][0] = mStart;
		pass[i][1] = mEnd;
	}

	path[M] = mStart;
	pass[M][0] = mEnd, pass[M][1] = -1;
	path[M + 1] = mEnd;
	pass[M + 1][0] = mStart;
	pass[M + 1][1] = -1;

	for (int i = 0; i < M + 2; i++) {
		dijkstra(path[i], pass[i][0], pass[i][1]);

		// 결과값을 cost 배열에 갱신
		for (int j = 0; j < M; j++)
			cost[i][j] = dist[path[j]];
	}
	// 전체 탐색
	int ret = search(0, M, 0);
	if (ret >= INF) ret = -1;
	return ret;
}
#endif
```

- 김태현

```cpp
#if 1
/*
dijkstra + dfs
*/
#include <cstring>
#include <queue>
#include <vector>
#include <algorithm>
using namespace std;

using pii = pair<int, int>;
constexpr int LM = 501;
constexpr int INF = 0x0f0f0f0f;

int N, tab[LM][LM];  // tab[i][j] : i정류장과 j정류장 사이 최단거리, tab[i][j]=0이라면 경로가 없는 경우
vector<pii> adj[LM]; // 인접 배열
vector<pii> prr;     // tab[i][j]값을 채운 경우 0으로 초기화 할 목록들, 최대 40개이다.

/////////////////////////////////////////////////////////////////////
int dist[LM];
int arr[10], M;
int used[LM], ucnt;

int dijkstra(int src) { // 큐를 이용하여 src로부터 최단거리 구하기
	priority_queue<pii> pq;   // <first:-dist, second:node>
	memset(dist + 1, 15, sizeof(int) * N);

	dist[src] = 0;
	pq.push({ 0, src });
	++ucnt;

	while (!pq.empty()) {
		int u = pq.top().second;
		int d = dist[u];            // src로부터 u까지 거리 d가
		pq.pop();
		if (used[u] == ucnt) continue;   // 현재까지 최단거리 dist[u]보다 큰 경우
		used[u] = ucnt;
		int i, len = (int)adj[u].size();
		for (i = 0; i < len; ++i) { // u의 인접노드 v에 대하여
			int v = adj[u][i].first;
			int nd = d + adj[u][i].second; // src에서 u를 거처 v로 가는 거리 nd가
			if (dist[v] > nd) {            // 더 짧은 경우
				dist[v] = nd;              // 최단거리 갱신하기
				if (v != arr[0] && v != arr[M]) // 출발지와 도착지가 아니라면 추가
					pq.push({ -nd, v });
			}
		}
	}

	for (int i = 0; i <= M; ++i) { // 경로가 존재하지 않는 경우를 확인하기
		if (dist[arr[i]] == INF) return 0;
	}

	for (int i = 0; i <= M; ++i) { // 최단거리 테이블에 저장하기
		tab[src][arr[i]] = tab[arr[i]][src] = dist[arr[i]];
		prr.push_back({ src, arr[i] }); // 정류장 정보 백업
	}
	return 1;
}

int ret, visited[10];
void dfs(int lev, int u, int dist) {
	if (lev == M) {        // u가 도착지 직전 예약 정류장인 경우
		if (tab[u][arr[M]]) // u로부터 도착지까지 경로가 존재하는 경우
			ret = min(ret, dist + tab[u][arr[M]]); // 답안 업데이트
		return;
	}
	for (int i = 1; i < M; ++i) {
		int v = arr[i];
		if (visited[i] == 0 && tab[u][v]) { // 사용된적이 없고 경로가 있다면
			visited[i] = 1;
			dfs(lev + 1, v, dist + tab[u][v]);
			visited[i] = 0;
		}
	}
}

/////////////////////////////////////////////////////////////////////
void addRoad(int s, int e, int d)  // 인접 배열 구성
{
	adj[s].push_back({ e, d });
	adj[e].push_back({ s, d });
}

void init(int mN, int mK, int a[], int b[], int d[])
{
	for (int i = 1; i <= N; ++i)   // 인접배열 초기화
		adj[i].clear();
	N = mN;
	for (int i = 0; i < mK; ++i)   // 인접배열 구성
		addRoad(a[i], b[i], d[i]);
}

int findPath(int mStart, int mEnd, int mM, int stops[])
{
	M = mM, ret = INF;
	int i;
	for (i = 0; i < M; ++i) arr[i + 1] = stops[i];
	arr[0] = mStart, arr[++M] = mEnd;

	// 각 예약 정류장으로 부터 나머지 정류장까지 최단거리 구하기
	for (i = 1; i < M && dijkstra(arr[i]); ++i);

	if (i == M) {  // 경로가 존재하는 경우
		dfs(1, arr[0], 0); // dfs를 이용하여 예약 정류장에 대한 순열을 생성해보기
	}

	int len = (int)prr.size();
	for (i = 0; i < len; ++i) {  // 최단거리 테이블 초기화
		tab[prr[i].first][prr[i].second] = 0;
		tab[prr[i].second][prr[i].first] = 0;
	}
	prr.clear();                 // 백업 테이블 초기화

	return ret < INF ? ret : -1;
}
#endif
```

### [240309_던전탈출] 230901_전기차대여소 (최소 거리 경로 구하기) + 노드 삭제 추가

```cpp
#if 1
#include<vector>
#include<queue>
#include<algorithm>
using namespace std;

#define MAX_N       350
#define MAX_NODES   205

// graph
int N;                      // 맵 크기
int nodeCnt;                // 게이트 최대 번호
int maxStamina;             // 최대 체력
int(*map)[MAX_N];           // 맵(0:길, 1:기둥, 2~:gateID+1)

struct Edge { 
    int ID, dist;  // { dist, ID }
    bool operator<(const Edge& edge) const { return dist > edge.dist; }
    bool operator==(const Edge& edge) const { return dist == edge.dist && ID == edge.ID; }
};
vector<Edge> adjList[MAX_NODES];   // 인접배열 

// bfs
struct Point { int row, col, dist; };
bool visited[MAX_N][MAX_N];
int dr[] = { -1, 0, 1, 0 };
int dc[] = { 0, 1, 0, -1 };

// dijkstra
int dist[MAX_NODES];

Point pointMap[MAX_NODES];          // { row, col }
int get_gateID[MAX_N][MAX_N];       // { row, col } -> gateID

//////////////////////////////////////////////////////////////////////
void init(int N, int mMaxStamina, int mMap[MAX_N][MAX_N])
{
    ::N = N;
    nodeCnt = 0;
    map = mMap;
    maxStamina = mMaxStamina;
    for (int i = 0; i < MAX_NODES; i++) adjList[i].clear();
}

void addGate(int mGateID, int mRow, int mCol)
{
    nodeCnt++;
    map[mRow][mCol] = mGateID + 1;
    pointMap[mGateID] = { mRow, mCol };

    queue<Point> que;
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            visited[i][j] = 0;

    que.push({ mRow, mCol, 0 });
    visited[mRow][mCol] = 1;

    while (!que.empty()) {
        auto cur = que.front(); que.pop();

        if (cur.dist == maxStamina) break;

        for (int i = 0; i < 4; i++) {
            Point next = { cur.row + dr[i], cur.col + dc[i], cur.dist + 1 };

            if (next.row < 0 || next.row >= N) continue;
            if (next.col < 0 || next.col >= N) continue;
            if (visited[next.row][next.col] != 0) continue;
            if (map[next.row][next.col] == 1) continue;

            que.push({ next.row, next.col, next.dist });
            visited[next.row][next.col] = visited[cur.row][cur.col] + 1;

            if (map[next.row][next.col] > 1) {
                int next_ID = map[next.row][next.col] - 1;
                adjList[mGateID].push_back({ next_ID, next.dist });
                adjList[next_ID].push_back({ mGateID, next.dist });
            }
        }
    }
}

void removeGate(int mGateID)
{
    for (const auto& p : adjList[mGateID]) {
        auto& adj = adjList[p.ID];
        adj.erase(find(adj.begin(), adj.end(), Edge{ mGateID, p.dist }));
    }
    adjList[mGateID].clear();
    map[pointMap[mGateID].row][pointMap[mGateID].col] = 0;
}

// dijkstra
int getMinTime(int mStartGateID, int mEndGateID)
{
    priority_queue<Edge> pq;
    for (int i = 0; i <= nodeCnt; i++) dist[i] = 1e9;

    dist[mStartGateID] = 0;
    pq.push({ mStartGateID, 0 });

    while (!pq.empty()) {
        auto cur = pq.top(); pq.pop();

        if (cur.ID == mEndGateID) return cur.dist;

        if (dist[cur.ID] < cur.dist) continue;
        for (const auto& next : adjList[cur.ID]) {
            if (dist[next.ID] > cur.dist + next.dist) {
                dist[next.ID] = cur.dist + next.dist;
                pq.push({ next.ID, dist[next.ID] });
            }
        }
    }
    return -1;
}
#endif
```
