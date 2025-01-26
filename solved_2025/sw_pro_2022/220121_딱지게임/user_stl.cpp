<<<<<<< HEAD
﻿#if 1       // xx ms (STL)
#include <vector>

#define MAX_DDAKJIS 20'000  // 각 테스트 케이스는 최대 10,000 턴 진행
#define MAX_BUCKETS 10'001  // 파티션 최대 개수

struct Ddakji {
    int mRow, mCol, mSize, mPlayer;
} ddakjis[MAX_DDAKJIS];
int ddakjiCnt;

int bucketSize;
int bucketCnt;
std::vector<int> buckets[MAX_BUCKETS][MAX_BUCKETS];
=======
#if 1   // xx ms (STL)

#include <vector>

#define MAX_DDAKJIS 20'000

struct Ddakji {
    int mRow, mCol, mSize, mPlayer, gIdx;
} ddakjis[MAX_DDAKJIS];
int ddakjiCnt;

struct Group {
    int cnt, mPlayer;
    std::vector<int> ddakjiList;
} groups[MAX_DDAKJIS];
>>>>>>> 395401ced0861ab906b697e6a86da786d8c7265c

int N;  // 게임 판의 한 변의 길이 (10 ≤ N ≤ 100,000,000)
int M;  // 딱지의 한 변의 길이의 최대 값 (1≤ M ≤ 10,000,000)

<<<<<<< HEAD
=======
int bucketSize;
int bucketCnt;
std::vector<int> buckets[11][11];

bool isOverlapped(const Ddakji& x, const Ddakji& y) {
    if (x.mRow + x.mSize <= y.mRow || y.mRow + y.mSize <= x.mRow) return false;
    if (x.mCol + x.mSize <= y.mCol || y.mCol + y.mSize <= x.mCol) return false;
    return true;
}

void merge(int x, int y) {}

>>>>>>> 395401ced0861ab906b697e6a86da786d8c7265c
//////////////////////////////////////////////////////////////////////
void init(int N, int M)
{
    ::N = N, ::M = M;
    ddakjiCnt = 0;
<<<<<<< HEAD

    bucketSize = sqrt(N);
    bucketCnt = ceil((double)N / bucketSize);
    for (int i = 0; i <= N / bucketSize; i++)
        for (int j = 0; j <= N / bucketSize; j++)
            buckets[i][j].clear();
=======
    for (int i = 0; i < MAX_DDAKJIS; i++) {
        groups[i].ddakjiList.clear();
    }

    bucketSize = M;
    bucketCnt = N / M;
    for (int i = 0; i < bucketCnt; i++)
        for (int j = 0; j < bucketCnt; j++) {
        buckets[i][j].clear();
    }

>>>>>>> 395401ced0861ab906b697e6a86da786d8c7265c
}

int addDdakji(int mRow, int mCol, int mSize, int mPlayer)
{
<<<<<<< HEAD


 return 0;
=======
    int dIdx = ddakjiCnt++;
    ddakjis[dIdx] = { mRow, mCol, mSize, mPlayer, dIdx };
    groups[dIdx] = { 1, mPlayer, { dIdx } };

    // [부분탐색] 딱지 다른 검색하여 겹쳐 있으면 그룹으로 병합한다.
    int sR = std::max((mRow - mSize) / bucketSize, 0);
    int sC = std::max((mCol - mSize) / bucketSize, 0);
    int eR = std::min((mRow + mSize) / bucketSize, N / M);
    int eC = std::min((mCol + mSize) / bucketSize, N / M);

    for (int i = sR; i <= eR; i++)
        for (int j = sC; j <= eC; j++)
            for (int idx : buckets[i][j]) {
                if (isOverlapped(ddakjis[dIdx], ddakjis[idx]))
                    merge(ddakjis[dIdx].gIdx, ddakjis[idx].gIdx);
            }
    buckets[mRow / bucketSize][mCol / bucketSize].push_back(dIdx);

    return 0;
>>>>>>> 395401ced0861ab906b697e6a86da786d8c7265c
}

int check(int mRow, int mCol)
{
<<<<<<< HEAD
 return 0;
}
#endif
=======
    return 0;
}
#endif
>>>>>>> 395401ced0861ab906b697e6a86da786d8c7265c
