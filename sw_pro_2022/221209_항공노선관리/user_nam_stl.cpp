#include <vector>
#include <queue>
using namespace std;

#define MAX_NODES   60
#define INF         1e9     // (mPrice ≤ 5,000), (mTravelTime ≤ 23) add() 함수의 호출 횟수는 30,000 이하

int N;                      // N : 공항의 수 (3 ≤ N ≤ 60)

// time info
struct Edge { int to, start_time, travel_time; };
vector<Edge> timeList[MAX_NODES];

// dijkstra for time
struct Time { 
    int to, time;
    bool operator<(const Time& edge) const { return time > edge.time; }
};
int timeDP[MAX_NODES];

// dijkstra for price
struct Price {
    int to, price;
    bool operator<(const Price& edge) const { return price > edge.price; }
};
vector<Price> priceList[MAX_NODES];
int priceDP[MAX_NODES];

/////////////////////////////////////////////////////////////////////
void init(int N)
{
    ::N = N;
    for (int i = 0; i < N; i++) { 
        timeList[i].clear();
        priceList[i].clear();
    }
}

void add(int mStartAirport, int mEndAirport, int mStartTime, int mTravelTime, int mPrice)
{
    timeList[mStartAirport].push_back({ mEndAirport, mStartTime, mTravelTime });
    priceList[mStartAirport].push_back({ mEndAirport, mPrice });
}

// dijkstra
// 소요 시간을 최대한 적게 하면서 이동할 때의 소요 시간을 반환한다.
// 갈 수 없으면, -1을 반환한다.
int minTravelTime(int mStartAirport, int mEndAirport, int mStartTime)
{
    priority_queue<Time> pq;
    for (int i = 0; i < N; i++) timeDP[i] = INF;    // 최소 소요 시간 저장
    timeDP[mStartAirport] = mStartTime;
    pq.push({ mStartAirport, timeDP[mStartAirport] });

    int res = -1;
    while (!pq.empty()) {
        auto cur = pq.top(); pq.pop();

        if (cur.to == mEndAirport) { res = cur.time - mStartTime; break; }

        if (timeDP[cur.to] < cur.time) continue;
        for (const auto& next: timeList[cur.to]) {
            int cur_start_time = cur.time % 24;
            int wait_time;
            if (cur_start_time > next.start_time)
                wait_time = 24 - (cur_start_time - next.start_time);
            else 
                wait_time = next.start_time - cur_start_time;

            if (timeDP[next.to] > cur.time + next.travel_time + wait_time) {
                timeDP[next.to] = cur.time + next.travel_time + wait_time;
                pq.push({ next.to, timeDP[next.to] });
            }
        }
    }
    return res;
}

// dijkstra
// 비용을 최대한 적게 하면서 이동할 때의 비용을 달러 단위로 반환한다.
// 갈 수 없으면, -1을 반환한다.
int minPrice(int mStartAirport, int mEndAirport)
{
    priority_queue<Price> pq;
    for (int i = 0; i < N; i++) priceDP[i] = INF;   // 최소 비용 저장
    priceDP[mStartAirport] = 0;
    pq.push({ mStartAirport, priceDP[mStartAirport] });

    int res = -1;
    while (!pq.empty()) {
        auto cur = pq.top(); pq.pop();

        if (cur.to == mEndAirport) { res = cur.price; break; }

        if (priceDP[cur.to] < cur.price) continue;
        for (const auto& next: priceList[cur.to])
            if (priceDP[next.to] > cur.price + next.price) {
                priceDP[next.to] = cur.price + next.price;
                pq.push({ next.to, priceDP[next.to] });
            }
    }
    return res;
}