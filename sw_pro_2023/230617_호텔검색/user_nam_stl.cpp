#if 1
#include<vector>
#include<queue>
using namespace std;

#define MAX_N   5'000
int N;

struct Hotel {
    int brand;
} hotels[MAX_N];

struct Brand {
    vector<int> hotelList;
} brands[MAX_N];

struct Edge {
    int to, dist;
    bool operator<(const Edge& edge) const { return dist > edge.dist; }
};
vector<Edge> adj[MAX_N];
int distDP[MAX_N];
priority_queue<Edge> pq;


//////////////////////////////////////////////////////////////////////
void init(int N, int mBrands[]) {
    ::N = N;
    for (int i = 0; i < MAX_N; i++)
        brands[i].hotelList.clear();

    for (int i = 0; i < N; i++) {
        adj[i].clear();
        hotels[i].brand = mBrands[i];
        brands[mBrands[i]].hotelList.push_back(i);
    }
}

void connect(int mBrandA, int mBrandB, int mDistance) {
    adj[mBrandA].push_back({ mBrandB, mDistance });
    adj[mBrandB].push_back({ mBrandA, mDistance });
}

int merge(int mHotelA, int mHotelB) {
    int brandA = hotels[mHotelA].brand;
    int brandB = hotels[mHotelB].brand;

    if (brandA != brandB)
        for (int to : brands[brandB].hotelList) {
            hotels[to].brand = brandA;
            brands[brandA].hotelList.push_back(to);
        }
    return brands[brandA].hotelList.size();
}

// dijsktra
int move(int mStart, int mBrandA, int mBrandB) {
    for (int i = 0; i < N; i++) distDP[i] = 1e6;
    pq = {};

    distDP[mStart] = 0;
    pq.push({ mStart, distDP[mStart]});

    int res = 0;
    while (!pq.empty()) {
        auto cur = pq.top(); pq.pop();

        if (cur.to != mStart && hotels[cur.to].brand == mBrandA) {
            mBrandA = -1;
            res += cur.dist;
        }
        else if (cur.to != mStart && hotels[cur.to].brand == mBrandB) {
            mBrandB = -1;
            res += cur.dist;
        }
        if (mBrandA == -1 && mBrandB == -1) break;

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