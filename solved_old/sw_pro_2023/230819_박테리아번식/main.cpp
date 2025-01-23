#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
#include <time.h>

struct Result
{
    int row;
    int col;
};

struct Bacteria
{
    int id;
    int size;
    int time;
};

extern void init(int N);
extern Result putBacteria(int mTime, int mRow, int mCol, Bacteria mBac);
extern int killBacteria(int mTime, int mRow, int mCol);
extern int checkCell(int mTime, int mRow, int mCol);

/////////////////////////////////////////////////////////////////////////
#define INIT    10000
#define PUT     20000
#define KILL    30000
#define CHECK   40000

static bool run()
{

    int cmd, N, time, row, col, ans, ret;
    Result ret_bac;
    Bacteria bac;
    int Q = 0;
    bool okay = false;
    scanf("%d", &Q);

    for (int q = 0; q < Q; ++q)
    {
        scanf("%d", &cmd);
        switch (cmd)
        {
            case INIT:
                scanf("%d", &N);
                init(N);
                okay = true;
                break;

            case PUT:
                scanf("%d %d %d %d %d %d", &time, &row, &col, &bac.id, &bac.size, &bac.time);
                ret_bac = putBacteria(time, row, col, bac);
                scanf("%d %d", &row, &col);
                if (ret_bac.row != row || ret_bac.col != col) {
                    okay = false;
                }
                break;

            case KILL:
                scanf("%d %d %d %d", &time, &row, &col, &ans);
                ret = killBacteria(time, row, col);
                if (ret != ans) {
                    okay = false;
                }
                break;

            case CHECK:
                scanf("%d %d %d %d", &time, &row, &col, &ans);
                ret = checkCell(time, row, col);
                if (ret != ans) {
                    okay = false;
                }
                break;

            default:
                okay = false;
        }
    }
    return okay;
}

int main()
{
    clock_t start = clock();
    setbuf(stdout, NULL);
    freopen("sample_input.txt", "r", stdin);
    int T, MARK;
    scanf("%d %d", &T, &MARK);

    for (int tc = 1; tc <= T; tc++)
    {
        clock_t tc_start = clock();
        int score = run() ? MARK : 0;
        int tc_result = (clock() - tc_start) / (CLOCKS_PER_SEC / 1000);
        printf("#%2d %d (%3d ms)\n", tc, score, tc_result);
    }
    int result = (clock() - start) / (CLOCKS_PER_SEC / 1000);
    printf(">> Result: %d ms\n", result);
    return 0;
}