#if 1
#include<vector>
#include<queue>
using namespace std;

#define MAX_N			350
#define MAX_NODES		200

int N;							// 맵 크기
int mRange;						// 이동 가능 거리
int (*mMap)[MAX_N];				// 맵 상태 (0 비어있음, 1 장애물)

// mapping
struct Point { int row, col; };
int nodeMap[MAX_N][MAX_N];		// mID : 대여소의 고유번호 ( 0 ≤ mID < 200 )

// bfs
queue<Point> que;
int visited[MAX_N][MAX_N];
int dr[] = { -1, 0, 1, 0 };
int dc[] = { 0, 1, 0, -1 };

// dijkstra
struct Edge {
	int to, dist;
	bool operator<(const Edge& edge) const { return dist > edge.dist; }
};
vector<Edge> adj[MAX_NODES];	// 인접배열
priority_queue<Edge> pq;
int distDP[MAX_NODES];

/////////////////////////////////////////////////////////////////////
void init(int N, int mRange, int mMap[MAX_N][MAX_N])
{
	::N = N;
	::mRange = mRange;
	::mMap = mMap;

	for (int i = 0; i < MAX_NODES; i++)
		adj[i].clear();

	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			nodeMap[i][j] = -1;
}

// bfs
void add(int to, int mRow, int mCol)
{
	nodeMap[mRow][mCol] = to;

	// init
	que = {};
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++) visited[i][j] = 0;

	que.push({ mRow, mCol });
	visited[mRow][mCol] = 1;

	while (!que.empty()) {
		auto cur = que.front(); que.pop();

		if (visited[cur.row][cur.col] > mRange) break;		// 종료 조건

		for (int k = 0; k < 4; k++) {
			Point next = { cur.row + dr[k], cur.col + dc[k] };

			if (next.row < 0 || next.row >= N) continue;
			if (next.col < 0 || next.col >= N) continue;
			if (mMap[next.row][next.col] == 1) continue;	// 장애물
			if (visited[next.row][next.col]) continue;		// 방문한 적이 있음

			que.push(next);
			visited[next.row][next.col] = visited[cur.row][cur.col] + 1;

			if (nodeMap[next.row][next.col] > -1) {
				int node = nodeMap[next.row][next.col];
				adj[to].push_back({ node, visited[next.row][next.col] - 1 });
				adj[node].push_back({ to, visited[next.row][next.col] - 1 });
			}
		}
	}
}

// dijkstra
int distance(int mFrom, int mTo)
{
	// init
	pq = {};
	for (int i = 0; i < MAX_NODES; i++) distDP[i] = MAX_N * MAX_N;

	distDP[mFrom] = 0;
	pq.push({ mFrom, distDP[mFrom] });

	int res = -1;
	while (!pq.empty()) {
		auto cur = pq.top(); pq.pop();

		// 종료 조건
		if (cur.to == mTo) { res = distDP[cur.to]; break; }

		if (distDP[cur.to] < cur.dist) continue;
		for (const auto& next : adj[cur.to]) {
			if (distDP[next.to] > distDP[cur.to] + next.dist) {
				distDP[next.to] = distDP[cur.to] + next.dist;
				pq.push({ next.to, distDP[next.to] });
			}
		}
	}
	return res;
}
#endif