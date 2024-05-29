#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
#include <time.h>

#define MAX_N 350

void init(int N, int mRange, int mMap[MAX_N][MAX_N]);
void add(int mID, int mRow, int mCol);
int distance(int mFrom, int mTo);

#define INIT        0
#define ADD         1
#define DISTANCE    2

static bool run()
{
    int cmd, ans, ret;
    int N, range, id, id2, r, c;
    int Q = 0;
    bool okay = false;
    int region[MAX_N][MAX_N];
    scanf("%d", &Q);

    for (int q = 0; q < Q; ++q)
    {
        scanf("%d", &cmd);

        switch (cmd)
        {
        case INIT:
            scanf("%d %d", &N, &range);

            for (int i = 0; i < N; i++)
                for (int j = 0; j < N; j++)
                    scanf("%d", &region[i][j]);

            init(N, range, region);
            okay = true;
            break;

        case ADD:
            scanf("%d %d %d", &id, &r, &c);
            add(id, r, c);
            break;

        case DISTANCE:
            scanf("%d %d", &id, &id2);
            ret = distance(id, id2);
            scanf("%d", &ans);
            if (ret != ans)
                okay = false;
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