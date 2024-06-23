#if 1
#include <queue>
#include <vector>
using namespace std;

#define MAX_BAC     50000
#define MAX_SIZE    201     // 1부터 시작
#define KILLED      1

int dr[] = { -1, 0, 1, 0 };
int dc[] = { 0, 1, 0, -1 };

/////////////////////////////////////////////////////////////////////
struct Result {
    int row;
    int col;
};

struct Bacteria {
    int id;
    int size;
    int time;
};

/////////////////////////////////////////////////////////////////////
struct BacInfo {
    int time;                       //소멸 예정 시간. 생성시간  + bacteria time
    vector<Result> posList;         //박테리아가 번식한 위치 정보를 갖는다.
    int state;                      //소멸 여부

};

BacInfo bac[MAX_BAC];               // bacDB[id]
int bacMap[MAX_SIZE][MAX_SIZE];     // 투입된 박테리아의 종류 ( 1 ≤ mBac.id ≤ 3,000 )
int N;                              // N : 배양기 한 변의 길이 ( 10 ≤ N ≤ 200 )

struct PosData {
    int row;
    int col;
    int dist;

    bool operator<(const PosData& pos) const {
        if (dist > pos.dist) return true;
        if (dist == pos.dist && row > pos.row) return true;
        if (dist == pos.dist && row == pos.row && col > pos.col) return true;
        return false;
    }
};

struct BacData {
    int time;
    int id;
    bool operator<(const BacData& bac) const { return time > bac.time; }
};

int get_dist(int row1, int col1, int row2, int col2) {
    return abs(row1 - row2) + abs(col1 - col2);
}

priority_queue<BacData> bacQ;
priority_queue<PosData> posQ;

/////////////////////////////////////////////////////////////////////
void init(int _N)
{
    N = _N;

    for (int i = 0; i < MAX_BAC; i++)
        bac[i] = {};

    for (int i = 1; i <= N; i++)
        for (int j = 1; j <= N; j++)
            bacMap[i][j] = -1;

    while (!bacQ.empty()) bacQ.pop();
}


//시간보다 작거나 같은 것이 있다면 소멸 시킴
void update(int mTime) {
    while (!bacQ.empty() && bacQ.top().time <= mTime) {
        auto cur = bacQ.top(); bacQ.pop();

        if (bac[cur.id].state == KILLED)
            continue;

        for (const auto& pos : bac[cur.id].posList)
            bacMap[pos.row][pos.col] = -1;
    }
}

int bfs(int mRow, int mCol, Bacteria mBac)
{
    while (!posQ.empty()) posQ.pop();
    int copyMap[MAX_SIZE][MAX_SIZE];        //임시맵정보
    for (int i = 1; i <= N; i++)
        for (int j = 1; j <= N; j++)
            copyMap[i][j] = bacMap[i][j];
    //memcpy(copyMap, bacMap, MAX_SIZE * MAX_SIZE * 4);

    posQ.push({ mRow, mCol, 0 });
    copyMap[mRow][mCol] = mBac.id;

    vector<Result> popped;
    int cnt = 0;
    while (!posQ.empty() && cnt < mBac.size) {
        auto cur = posQ.top(); posQ.pop();

        cnt += 1;
        popped.push_back({ cur.row, cur.col });  //임시 벡터에  값 저장

        for (int i = 0; i < 4; i++) {
            Result next = { cur.row + dr[i], cur.col + dc[i] };

            if (next.row < 1 || next.row > N) continue;
            if (next.col < 1 || next.col > N) continue;

            //이미 다름 박테리아가 있을경우
            if (copyMap[next.row][next.col] != -1) continue;

            int dist = get_dist(mRow, mCol, next.row, next.col);
            posQ.push({ next.row, next.col, dist });
            copyMap[next.row][next.col] = mBac.id; //방문
        }
    }

    //전부 번식하지 못했다.
    if (cnt != mBac.size)
        return 0;

    //번식했음으로 번식 좌표를 저장하고 map을 채운다.
    //좌표의 map정보를 채운다.

    for (const auto& pos : popped) {
        bacMap[pos.row][pos.col] = mBac.id;
        bac[mBac.id].posList.push_back(pos);
    }
    return mBac.id;
}

Result putBacteria(int mTime, int mRow, int mCol, Bacteria mBac)
{
    update(mTime);

    Result res = { 0, 0 };
    // 배양기의(mRow, mCol) 셀에 이미 다른 종류의 박테리아가 살아 있어 빈 셀이 아닐 경우,
    // 아무 동작도 하지 않고[Result.row = 0, Result.col = 0]을 반환한다.
    if (bacMap[mRow][mCol] != -1)
    {
        return res;
    }

    //박테리아를 번식시켜본다.
    int id = bfs(mRow, mCol, mBac);
    if (id == 0) return res;

    bac[mBac.id].time = mTime + mBac.time;  //소멸예정시간
    bacQ.push({ mTime + mBac.time, mBac.id });

    //마지막 좌표정보
    res = bac[id].posList.back();
    return res;
}

int killBacteria(int mTime, int mRow, int mCol)
{
    update(mTime);

    //해당셀에 박테리가 없는경우
    if (bacMap[mRow][mCol] == -1) return 0;

    int id = bacMap[mRow][mCol];
    for (const auto& pos : bac[id].posList)
        bacMap[pos.row][pos.col] = -1;

    //소멸플래스 지정
    bac[id].state = KILLED;
    bacMap[mRow][mCol] = -1;
    return id;
}

int checkCell(int mTime, int mRow, int mCol)
{
    update(mTime);

    //해당셀에 박테리아가 없는경우
    if (bacMap[mRow][mCol] == -1) return 0;

    int res = bacMap[mRow][mCol];
    return res;
}
#endif