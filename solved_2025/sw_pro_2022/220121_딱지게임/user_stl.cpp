#if 1       // xx ms (STL)
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

int N;  // 게임 판의 한 변의 길이 (10 ≤ N ≤ 100,000,000)
int M;  // 딱지의 한 변의 길이의 최대 값 (1≤ M ≤ 10,000,000)

//////////////////////////////////////////////////////////////////////
void init(int N, int M)
{
    ::N = N, ::M = M;
    ddakjiCnt = 0;

    bucketSize = sqrt(N);
    bucketCnt = ceil((double)N / bucketSize);
    for (int i = 0; i <= N / bucketSize; i++)
        for (int j = 0; j <= N / bucketSize; j++)
            buckets[i][j].clear();
}

int addDdakji(int mRow, int mCol, int mSize, int mPlayer)
{


 return 0;
}

int check(int mRow, int mCol)
{
 return 0;
}
#endif