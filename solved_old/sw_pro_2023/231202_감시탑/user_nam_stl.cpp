#if 1
#include <vector>
// #include <queue>
using namespace std;

#define INF         25'000'001  // 5000 * 5000
#define MAX_N       5001
#define MAX_TOWERS  50'000      // buildTower() 함수의 호출 횟수는 최대 50,000
#define MAX_COLORS  6           // mColor : 설치하는 감시탑의 고유 색깔 ( 1 ≤ mColor ≤ 5 )
#define MAX_GROUPS  501

#define BUILT       0
#define REMOVED     1

inline int min(int a, int b) { return (a < b)? a: b; }
inline int max(int a, int b) { return (a < b)? b: a; }
inline int abs(int x) { return (x < 0)? -x: x; }


struct Tower {
    int mRow, mCol, mColor;
    int state;
} towers[MAX_TOWERS];
int towerCnt;
int towerMap[MAX_N][MAX_N];

// sqrt decomposition
int N;      // N : 평원 한 변의 길이 ( 10 ≤ N ≤ 5,000 )
int sq;     // group size
vector<int> groups[MAX_COLORS][MAX_GROUPS][MAX_GROUPS];

// queue
template<typename T>
struct Queue {
    T arr[MAX_N];
    int head = 0, tail = 0;

    void clear() { head = tail = 0; }
    void push(const T& data) { arr[tail++] = data; }
    void pop() { head++; }
    bool empty() { return head == tail; }
    T front() { return arr[head]; }
};

// bfs
struct Point { int row, col; };
int visited[MAX_GROUPS][MAX_GROUPS];
int dr[] = { -1, 0, 1, 0 };
int dc[] = { 0, 1, 0, -1 };
Queue<Point> que;

/////////////////////////////////////////////////////////////////////
void init(int N)
{
    ::N = N;                // max row index, col index
    ::sq = sqrt(N + 1);     // group size (max group index = N / sq)
    towerCnt = 0;

    for (int i = 0; i <= N; i++)
        for (int j = 0; j <= N; j++) towerMap[i][j] = -1;

    // for (int i = 0; i < MAX_TOWERS; i++) towers[i] = {};

    for (int i = 0; i <= N / sq; i++)
        for (int j = 0; j <= N / sq; j++)
            for (int k = 0; k < MAX_COLORS; k++)
                groups[k][i][j].clear();
}

void buildTower(int mRow, int mCol, int mColor)
{
    int tIdx = towerCnt++;
    towers[tIdx] = { mRow, mCol, mColor, BUILT };
    towerMap[mRow][mCol] = tIdx;

    groups[0][mRow / sq][mCol / sq].push_back(tIdx);
    groups[mColor][mRow / sq][mCol / sq].push_back(tIdx);
}

void removeTower(int mRow, int mCol)
{
    int tIdx = towerMap[mRow][mCol];
    towers[tIdx].state = REMOVED;
}

int countTower(int mRow, int mCol, int mColor, int mDis)
{
    int sR = max((mRow - mDis) / sq, 0), eR = min((mRow + mDis) / sq, N / sq);
    int sC = max((mCol - mDis) / sq, 0), eC = min((mCol + mDis) / sq, N / sq);

    int cnt = 0;
    for (int i = sR; i <= eR; i++)
        for (int j = sC; j <= eC; j++)
            for (int tIdx: groups[mColor][i][j]) {
                if (towers[tIdx].state == REMOVED) continue;
                if (abs(towers[tIdx].mRow - mRow) > mDis) continue;
                if (abs(towers[tIdx].mCol - mCol) > mDis) continue;
                cnt += 1;
            }
    return cnt;
}

// bfs
int getClosest(int mRow, int mCol, int mColor)
{
    // init
    que.clear();
    for (int i = 0; i <= N / sq; i++)
        for (int j = 0; j <= N / sq; j++) visited[i][j] = 0;

    visited[mRow / sq][mCol / sq] = 1;
    que.push({ mRow / sq, mCol / sq });

    int minDepth = 0;
    int minDist = INF;

    while (!que.empty()) {
        auto cur = que.front(); que.pop();

        // 고유 색깔이 mColor 인 가장 가까운 감시탑과의 거리 계산
        for (int tIdx: groups[mColor][cur.row][cur.col]) {
            if (towers[tIdx].state == REMOVED) continue;
            int dist = abs(towers[tIdx].mRow - mRow) + abs(towers[tIdx].mCol - mCol);
            if (dist < minDist) {
                minDist = dist;
                minDepth = visited[cur.row][cur.col];
            }
        }
        // 가지치기
        if (minDist != INF && visited[cur.row][cur.col] > minDepth + 1) continue;

        for (int k = 0; k < 4; k++) {
            Point next = { cur.row + dr[k], cur.col + dc[k] };

            if (next.row < 0 || next.row > N / sq) continue;
            if (next.col < 0 || next.col > N / sq) continue;
            if (visited[next.row][next.col]) continue;

            visited[next.row][next.col] = visited[cur.row][cur.col] + 1;
            que.push({ next.row, next.col });
        }
    }
    if (minDist == INF) return -1;
    return minDist;
}
#endif