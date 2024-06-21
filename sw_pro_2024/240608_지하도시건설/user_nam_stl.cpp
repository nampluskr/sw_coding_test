#include <queue>
using namespace std;

#define MAX_BOXES   10001     // dropBox() 함수는 최대 10,000 회 호출된다. 1 ~ 10000
#define MAX_H       1000
#define MAX_W       200000

int mH;                     // 웅덩이의 세로 방향의 길이 (8 ≤ mH ≤ 1,000)    0 ~ mH - 1
int mW;                     // 웅덩이의 가로 방향의 길이 (10 ≤ mW ≤ 200,000) 0 ~ mW - 1

struct Box {
    int start, end, mExitA, mExitB;
} boxes[MAX_BOXES];

struct RangeMin {
    struct Group { int base, min, idx; };
    int arr[MAX_W];             // min value
    Group groups[MAX_W / 10];   // group min, idx
    int sq;                     // group size

    void clear() {
        sq = sqrt(mW);
        for (int i = 0; i < mW; i++) arr[i] = mH;
        for (int i = 0; i < mW / sq; i++) groups[i] = { mH, -1 };
    }
    void updatePoint(int idx, int value) {
        // arr[idx] += value;

        // int gIdx = idx / sq;
        // int left = gIdx * sq, right = min((gIdx + 1) * sq - 1, mW - 1);

        
    }
    void updateRange(int left, int right, int value) {
        // int s = left / sq, e = right / sq;

        // if (s == e) {
        //     for (int i = left; i <= right; i++) updatePoint(i, value);
        //     return;
        // }
        // for (int i = left; i <= (s + 1) / sq - 1; i++) updatePoint(i, value);
        // for (int i = s + 1; i <= e - 1; i++) groups[i] = value;
        // for (int i = e * sq; i <= right; i++) updatePoint(i, value);
    }
    int queryPoint(int idx) { 
        return 0;
        // return arr[idx] + groups[idx / sq];
    }
    int queryRange(int left, int right) {
        // int res = mW;
        // int s = left / sq, e = right / sq;

        // if (s == e) {
        //     for (int i = left; i <= right; i++) res = min(res, queryPoint(i));
        //     return res;
        // }
        // for (int i = left; i <= (s + 1) / sq - 1; i++) res = min(res, queryPoint(i));
        // for (int i = s + 1; i <= e - 1; i++) res = min(res, groups[i]);
        // for (int i = e * sq; i <= right; i++) res = min(res, queryPoint(i));
        // return res;
        return 0;
    }
} rmq;

///////////////////////////////////////////////////////////////////////////////
void init(int mH, int mW)
{
    ::mH = mH, ::mW = mW;
    rmq.clear();
}

int dropBox(int mId, int mLen, int mExitA, int mExitB, int mCol)
{
    int left = mCol, right = mCol + mLen - 1;
    boxes[mId] = { left, right, mExitA, mExitB };
    rmq.updateRange(left, right, 1);

    return 0;
}

// dijkstra
int explore(int mIdA, int mIdB)
{

    return -1;
}