#if 1
#include<vector>
#include<queue>
using namespace std;

#define MAX_N			350
#define MAX_NODES		200
#define INF				1e6

int N;							// 맵 크기
int mRange;						// 이동 가능 거리
int (*mMap)[MAX_N];				// 맵 상태 (0 비어있음, 1 장애물)

int nodeMap[MAX_N][MAX_N];		// { mRow, mCol } -> mID
int nodeCnt;					// 전기차 대여소 개수

// bfs
int dr[] = { -1, 0, 1, 0 };
int dc[] = { 0, 1, 0, -1 };
struct Point { int row, col; };
int visited[MAX_N][MAX_N];

// dijkstra
struct Edge {
	int mID, cost;
	bool operator<(const Edge& edge) const { return cost > edge.cost; }
};
vector<Edge> adj[MAX_NODES];	// 인접배열
int dist[MAX_NODES];

/////////////////////////////////////////////////////////////////////
void init(int N, int mRange, int mMap[MAX_N][MAX_N])
{
	::N = N;
	::mRange = mRange;
	::mMap = mMap;
	nodeCnt = 0;

	for (int i = 0; i < MAX_NODES; i++)
		adj[i].clear();

	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			nodeMap[i][j] = -1;
}

// bfs
void add(int mID, int mRow, int mCol)
{
	nodeMap[mRow][mCol] = mID;
	nodeCnt++;

	queue<Point> que;		// Point { row, col }
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++) visited[i][j] = -1;

	que.push({ mRow, mCol });
	visited[mRow][mCol] = 0;

	while (!que.empty()) {
		Point cur = que.front(); que.pop();

		if (visited[cur.row][cur.col] == mRange) break;		// 종료 조건

		for (int k = 0; k < 4; k++) {
			Point next = { cur.row + dr[k], cur.col + dc[k] };

			if (next.row < 0 || next.row >= N) continue;
			if (next.col < 0 || next.col >= N) continue;
			if (mMap[next.row][next.col] == 1) continue;		// 장애물
			if (visited[next.row][next.col] != -1) continue;	// 방문한 적이 있음

			que.push(next);
			visited[next.row][next.col] = visited[cur.row][cur.col] + 1;

			if (nodeMap[next.row][next.col] != -1) {
				int next_mID = nodeMap[next.row][next.col];
				adj[mID].push_back({ next_mID, visited[next.row][next.col] });
				adj[next_mID].push_back({ mID, visited[next.row][next.col] });
			}
		}
	}
}


// dijkstra
int distance(int mFrom, int mTo)
{
	priority_queue<Edge> pq;		// { mID, cost }
	for (int i = 0; i < nodeCnt; i++) dist[i] = INF;

	dist[mFrom] = 0;
	pq.push({ mFrom, dist[mFrom] });

	while (!pq.empty()) {
		auto cur = pq.top(); pq.pop();

		// 종료 조건
		if (cur.mID == mTo) return cur.cost;

		if (dist[cur.mID] < cur.cost) continue;
		for (const auto& next : adj[cur.mID]) {
			if (dist[next.mID] > dist[cur.mID] + next.cost) {
				dist[next.mID] = dist[cur.mID] + next.cost;
				pq.push({ next.mID, dist[next.mID] });
			}
		}
	}
	return -1;
}
#endif