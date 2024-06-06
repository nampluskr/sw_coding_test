#if 1
#include <vector>
#include <queue>
using namespace std;

#define MAX_N       350
#define MAX_NODES   200         // addGate() 및 removeGate() 함수의 호출 횟수는 각각 200 이하

int N;                          // 던전 지도의 크기 ( 12 ≤ N ≤ 350 )
int mMaxStamina;                // 기사의 최대 체력(이동 가능 거리) ( 5 ≤ mMaxStamina ≤ 100 )
int (*mMap)[MAX_N];             // 던전의 지형 정보( N x N ) ( 0 ≤ mMap[][] ≤ 1 )

// mapping
struct Point { int row, col; }; // 던전 지도의 좌표 ( 0 ≤ mRow, mCol ≤ N - 1 )
int nodeMap[MAX_N][MAX_N];      // { mRow, mCol } -> mGateID
Point pointMap[MAX_NODES];      // mGateID : 게이트 ID ( 1 ≤ mGateID ≤ 200 ) -> { mRow, mCol }

// bfs
queue<Point> que;
int visited[MAX_N][MAX_N];      // default = 0
int dr[] = { -1, 0, 1, 0};
int dc[] = { 0, 1, 0, -1};

// dijkstra
struct Edge {
    int to, dist;
    bool operator<(const Edge& edge) const { return dist > edge.dist; }
};
vector<Edge> adjList[MAX_NODES];
priority_queue<Edge> pq;
int distDP[MAX_NODES];          // default = INF (MAX_N * MAX_N)


///////////////////////////////////////////////////////////////////
void init(int N, int mMaxStamina, int mMap[MAX_N][MAX_N])
{
    ::N = N;
    ::mMaxStamina = mMaxStamina;
    ::mMap = mMap;

    for (int i = 0; i < MAX_N; i++)
        for (int j = 0; j < MAX_N; j++)
            nodeMap[i][j] = 0;

    for (int i = 0; i < MAX_NODES; i++) {
        adjList[i].clear();
        // pointMap[i] = { -1, -1 };
    }
}

// bfs
void addGate(int mGateID, int mRow, int mCol)
{
    // mapping
    pointMap[mGateID] = { mRow, mCol };
    nodeMap[mRow][mCol] = mGateID;

    // init
    que = {};
	for (int i = 0; i < MAX_N; i++)
		for (int j = 0; j < MAX_N; j++) visited[i][j] = 0;

	visited[mRow][mCol] = 1;
	que.push({ mRow, mCol });

	while (!que.empty()) {
		auto cur = que.front(); que.pop();

        // 종료 조건
		if (visited[cur.row][cur.col] > mMaxStamina) break;

		for (int k = 0; k < 4; k++) {
            Point next = { cur.row + dr[k], cur.col + dc[k] };

            if (next.row < 0 || next.row >= N) continue;
            if (next.col < 0 || next.col >= N) continue;
            if (visited[next.row][next.col] != 0) continue;
            if (mMap[next.row][next.col] == 1) continue;

            visited[next.row][next.col] = visited[cur.row][cur.col] + 1;
            que.push({ next.row, next.col });

            if (nodeMap[next.row][next.col] > 0) {
                int node = nodeMap[next.row][next.col];
                adjList[node].push_back({ mGateID, visited[next.row][next.col] - 1 });
		        adjList[mGateID].push_back({ node, visited[next.row][next.col] - 1 });
            }
		}
	}
}

void removeGate(int mGateID)
{
    auto point = pointMap[mGateID];
    nodeMap[point.row][point.col] = 0;  // ( 1 ≤ mGateID ≤ 200 )
    // pointMap[mGateID] = { -1, -1 };
    // adjList[mGateID].clear();        // 인접 리스트 삭제시, dijkstra 노드 삭제 여부 미확인
}

// dijstra
int getMinTime(int mStartGateID, int mEndGateID)
{
    // init
    pq = {};
    for (int i = 0; i < MAX_NODES; i++) distDP[i] = 1e6;

    distDP[mStartGateID] = 0;
    pq.push({ mStartGateID, distDP[mStartGateID] });

    int res = -1;
    while (!pq.empty()) {
        auto cur = pq.top(); pq.pop();

        // 종료조건
        if (cur.to == mEndGateID) { res = distDP[cur.to]; break; };

        if (distDP[cur.to] < cur.dist) continue;
        for (const auto& next: adjList[cur.to]) {
            // 삭제 여부 확인
            auto point = pointMap[next.to];
            if (nodeMap[point.row][point.col] == 0) continue;

            if (distDP[next.to] > distDP[cur.to] + next.dist) {
                distDP[next.to] = distDP[cur.to] + next.dist;
                pq.push({ next.to, distDP[next.to] });
            }
        }
    }
	return res;
}
#endif