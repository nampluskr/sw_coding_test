#if 1
#include <vector>
#include <queue>
using namespace std;

#define MAX_NODES	2'001	// add() 함수의 호출 횟수는 2,000 이하
#define INF			30'001	// mLimit: 도로를 이용할 수 있는 최대 중량 ( 1 ≤ mLimit ≤ 30,000 )

int N;						// N: 도시의 개수 ( 5 ≤ N ≤ 1,000 )
int K;						// K: 도로의 개수 ( 5 ≤ K ≤ 4,000 )

struct Edge { 
	int to, dist;
	bool operator<(const Edge& edge) const { return dist < edge.dist; }
};
vector<Edge> adjList[MAX_NODES];
int distDP[MAX_NODES];

/////////////////////////////////////////////////////////////////////
void add(int sCity, int eCity, int mLimit);

void init(int N, int K, int sCity[], int eCity[], int mLimit[])
{
	::N = N;
	::K = K;

	for (int i = 0; i < MAX_NODES; i++) adjList[i].clear();
	for (int i = 0; i < K; i++) add(sCity[i], eCity[i], mLimit[i]);
}

void add(int sCity, int eCity, int mLimit)
{
	adjList[sCity].push_back({ eCity, mLimit });
}

// modified dijkstra
int calculate(int sCity, int eCity)
{
	// init
	priority_queue<Edge> pq;
	for (int i = 0; i < MAX_NODES; i++) distDP[i] = 0;

	distDP[sCity] = INF;
	pq.push({ sCity, distDP[sCity]});

	int res = -1;
	while(!pq.empty()) {
		auto cur = pq.top(); pq.pop();

		// 종료조건
		if (cur.to == eCity) { res = distDP[cur.to]; break; }

		if (distDP[cur.to] > cur.dist) continue;
		for (const auto& next: adjList[cur.to]) {
			int min_dist = min(distDP[cur.to], next.dist);
			if (distDP[next.to] < min_dist) {
				distDP[next.to] = min_dist;
				pq.push({ next.to, distDP[next.to] });
			}
		}
	}
	return res;
}
#endif