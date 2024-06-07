#if 0
// 시간 개선: memset(), array queue
#include<vector>
#include<queue>
#include<math.h>
using namespace std;

struct Result { int row, col; };
struct Bacteria { int id, size, time; };

struct Point {
    int row, col, dist;
    bool operator<(const Point& p) const {
        if (dist != p.dist) return dist > p.dist;
        if (row != p.row) return row > p.row;
        return col > p.col;
    }
};

int N;                          // N : 배양기 한 변의 길이 ( 10 ≤ N ≤ 200 )

// mapping
int bacMap[205][205];           // { row, col } -> bac index
vector<Point> pointList[3005];  // bac.id -> { row, col } list

template<typename T>
struct Queue {
    T arr[200*200 + 5];
    int head, tail;

    void clear() { head = tail = 0; }
    void push(const T& data) { arr[tail++] = data; }
    void pop() { head++; }
    T front() { return arr[head]; }
    bool empty() { return head == tail; }
};

// check_size
Queue<Point> que;
priority_queue<Point> pq;
int visited[205][205];
int dr[] = { -1, 0, 1, 0 };
int dc[] = { 0, 1, 0, -1 };

// update
struct Time { 
    int time, id;
    bool operator<(const Time& t) const { return time > t.time; }
};
priority_queue<Time> timePQ;    // { time, id }

////////////////////////////////////////////////////////////////////

// time 까지 박테리아 소멸
void update(int mTime) {
    while (!timePQ.empty() && timePQ.top().time <= mTime) {
        int id = timePQ.top().id; timePQ.pop();
        for (const auto& p : pointList[id]) bacMap[p.row][p.col] = 0;
        pointList[id].clear();
    }
}

// bfs
bool check_size(int mRow, int mCol, int size) {
    if (bacMap[mRow][mCol]) return 0;
    if (size == 1) return 1;

    // init
    //que = {};
    que.clear();
    memset(visited, 0, sizeof(visited));
    //for (int i = 0; i <= N; i++)
    //    for (int j = 0; j <= N; j++) visited[i][j] = 0;

    int cnt = 1;
    visited[mRow][mCol] = cnt++;
    que.push({ mRow, mCol });

    while (!que.empty()) {
        auto cur = que.front(); que.pop();

        if (cnt > size) return 1;

        for (int k = 0; k < 4; k++) {
            Point next = { cur.row + dr[k], cur.col + dc[k] };

            if (next.row < 1 || next.row > N) continue;
            if (next.col < 1 || next.col > N) continue;
            if (bacMap[next.row][next.col]) continue;
            if (visited[next.row][next.col]) continue;

            que.push({ next.row, next.col });
            visited[next.row][next.col] = cnt++;
        }
    }
    return 0;
}

// bfs
Result fill_size(int mRow, int mCol, int id, int size) {
    // init
    pq = {};
    memset(visited, 0, sizeof(visited));
    //for (int i = 0; i <= N; i++)
    //    for (int j = 0; j <= N; j++) visited[i][j] = 0;

    int cnt = 0;
    visited[mRow][mCol] = 1;
    pq.push({ mRow, mCol, 0 });

    while (!pq.empty()) {
        auto cur = pq.top(); pq.pop();

        bacMap[cur.row][cur.col] = id;
        pointList[id].push_back({ cur.row, cur.col });
        cnt++;

        if (cnt == size) return { cur.row, cur.col };

        for (int i = 0; i < 4; i++) {
            Point next = { cur.row + dr[i], cur.col + dc[i] };
            next.dist = abs(next.row - mRow) + abs(next.col - mCol);
            
            if (next.row < 1 || next.row > N) continue;
            if (next.col < 1 || next.col > N) continue;
            if (bacMap[next.row][next.col]) continue;
            if (visited[next.row][next.col]) continue;

            visited[next.row][next.col] = 1;
            pq.push({ next.row, next.col, next.dist });
        }
    }
}

////////////////////////////////////////////////////////////////////
void init(int N)
{
    ::N = N;
    timePQ = {};
    memset(bacMap, 0, sizeof(bacMap));
    //for (int i = 0; i <= N; i++)
    //    for (int j = 0; j <= N; j++) bacMap[i][j] = 0;
}

Result putBacteria(int mTime, int mRow, int mCol, Bacteria mBac)
{
    pointList[mBac.id].clear();     // ???
    update(mTime);

    if (check_size(mRow, mCol, mBac.size) == 0) return { 0, 0 };

    timePQ.push({ mTime + mBac.time, mBac.id });
    auto res = fill_size(mRow, mCol, mBac.id, mBac.size);
    return res;
}

int killBacteria(int mTime, int mRow, int mCol)
{
    update(mTime);
    int id = bacMap[mRow][mCol];

    for (const auto& p : pointList[id]) bacMap[p.row][p.col] = 0;
    pointList[id].clear();

    return id;
}

int checkCell(int mTime, int mRow, int mCol)
{
    update(mTime);
    return bacMap[mRow][mCol];
}
#endif