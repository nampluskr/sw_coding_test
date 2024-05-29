#if 1
#include<vector>
#include<queue>
using namespace std;

#define MAX_N			350
#define MAX_STATIONS	200
#define INF				1e6

int N;							// 맵 크기
int mRange;						// 이동 가능 거리
int (*mMap)[MAX_N];				// 맵 상태 (0 비어있음, 1 장애물)

int nodeMap[MAX_N][MAX_N];		// { mRow, mCol } -> mID
int nodeCnt;					// 전기차 대여소 개수

// bfs
int dr[] = { -1, 0, 1, 0 };
int dc[] = { 0, 1, 0, -1 };
struct Position { int r, c; };
int dist[MAX_N][MAX_N];
//struct Position { int r, c, d; };
//int visited[MAX_N][MAX_N];

// dijkstra
struct Edge {
	int mID, dist;
	bool operator<(const Edge& edge) const { return dist > edge.dist; }
};
vector<Edge> adj[MAX_STATIONS];	// 인접배열
int minDist[MAX_STATIONS];

/////////////////////////////////////////////////////////////////////
void init(int N, int mRange, int mMap[MAX_N][MAX_N])
{
	::N = N;
	::mRange = mRange;
	::mMap = mMap;
	nodeCnt = 0;

	for (int i = 0; i < MAX_STATIONS; i++)
		adj[i].clear();

	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			nodeMap[i][j] = -1;
}

// bfs
//void add(int mID, int mRow, int mCol)
//{
//	nodeMap[mRow][mCol] = mID;
//	nodeCnt++;
//
//	queue<Position> que;		// Position { r, c, d }
//	for (int i = 0; i < N; i++)
//		for (int j = 0; j < N; j++) visited[i][j] = 0;
//
//	que.push({ mRow, mCol, 0 });
//	visited[mRow][mCol] = 1;
//
//	while (!que.empty()) {
//		Position cur = que.front(); que.pop();
//
//		if (cur.d == mRange) break;		// 종료 조건
//
//		for (int k = 0; k < 4; k++) {
//			Position next = { cur.r + dr[k], cur.c + dc[k], cur.d + 1 };
//
//			if (next.r < 0 || next.r >= N || next.c < 0 || next.c >= N) continue;
//			if (mMap[next.r][next.c] == 1) continue;	// 장애물
//			if (visited[next.r][next.c]) continue;		// 방문한 적이 있음
//
//			que.push(next);
//			visited[next.r][next.c] = 1;
//
//			if (nodeMap[next.r][next.c] != -1) {
//				int next_mID = nodeMap[next.r][next.c];
//				adj[mID].push_back({ next_mID, next.d });
//				adj[next_mID].push_back({ mID, next.d });
//			}
//		}
//	}
//}

// bfs
void add(int mID, int mRow, int mCol)
{
	nodeMap[mRow][mCol] = mID;
	nodeCnt++;

	queue<Position> que;		// Position { r, c }
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++) dist[i][j] = -1;

	que.push({ mRow, mCol });
	dist[mRow][mCol] = 0;

	while (!que.empty()) {
		Position cur = que.front(); que.pop();

		if (dist[cur.r][cur.c] == mRange) break;		// 종료 조건

		for (int k = 0; k < 4; k++) {
			Position next = { cur.r + dr[k], cur.c + dc[k] };

			if (next.r < 0 || next.r >= N) continue;
			if (next.c < 0 || next.c >= N) continue;
			if (mMap[next.r][next.c] == 1) continue;	// 장애물
			if (dist[next.r][next.c] != -1) continue;	// 방문한 적이 있음

			que.push(next);
			dist[next.r][next.c] = dist[cur.r][cur.c] + 1;

			if (nodeMap[next.r][next.c] != -1) {
				int next_mID = nodeMap[next.r][next.c];
				adj[mID].push_back({ next_mID, dist[next.r][next.c] });
				adj[next_mID].push_back({ mID, dist[next.r][next.c] });
			}
		}
	}
}


// dijkstra
int distance(int mFrom, int mTo)
{
	priority_queue<Edge> pq;		// { mID, dist }
	for (int i = 0; i < nodeCnt; i++) minDist[i] = INF;

	minDist[mFrom] = 0;
	pq.push({ mFrom, minDist[mFrom] });

	while (!pq.empty()) {
		auto cur = pq.top(); pq.pop();

		if (cur.mID == mTo) return cur.dist;		// 종료 조건
		if (minDist[cur.mID] < cur.dist) continue;

		for (const auto& next : adj[cur.mID]) {
			if (minDist[next.mID] > minDist[cur.mID] + next.dist) {
				minDist[next.mID] = minDist[cur.mID] + next.dist;
				pq.push({ next.mID, minDist[next.mID] });
			}
		}
	}
	return -1;
}
#endif