#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
#include <time.h>

extern void init(int mH, int mW);
extern int dropBox(int mId, int mLen, int mExitA, int mExitB, int mCol);
extern int explore(int mIdA, int mIdB);

#define CMD_INIT 1
#define CMD_DROP 2
#define CMD_EXPLORE 3

static bool run()
{
    int query_num;
    scanf("%d", &query_num);
    int ans;
    bool ok = false;

    for (int q = 0; q < query_num; q++)
    {
        int query;
        scanf("%d", &query);
        if (query == CMD_INIT)
        {
            int mH, mW;
            scanf("%d %d", &mH, &mW);
            init(mH, mW);
            ok = true;
        }
        else if (query == CMD_DROP)
        {
            int mId, mLen, mExitA, mExitB, mCol;
            scanf("%d %d %d %d %d", &mId, &mLen, &mExitA, &mExitB, &mCol);

            if (mId == 1) {
                int k = 1;
            }

            int ret = dropBox(mId, mLen, mExitA, mExitB, mCol);
            scanf("%d", &ans);
            if (ans != ret)
            {
                ok = false;
            }
        }
        else if (query == CMD_EXPLORE)
        {
            int mIdA, mIdB;
            scanf("%d %d", &mIdA, &mIdB);
            int ret = explore(mIdA, mIdB);
            scanf("%d", &ans);
            if (ans != ret)
            {
                ok = false;
            }
        }
    }
    return ok;
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