#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
#include <time.h>

struct Result {
    int mCost;
    int mContent;
};

extern void init(int mShipFee);
extern int gather(int mMineId, int mType, int mCost, int mContent);
extern Result mix(int mBudget);

/////////////////////////////////////////////////////////////////////////

#define INIT 0
#define GATHER 1
#define MIX    2

static bool run() {
    int cmd, ans, ans2, ret;
    Result sRet;
    int in, in2, in3, in4;
    int Q = 0;
    bool okay = false;

    scanf("%d", &Q);
    for (int q = 0; q < Q; ++q) {
        scanf("%d", &cmd);
        switch (cmd) {
        case INIT:
            scanf("%d", &in);
            init(in);
            okay = true;
            break;
        case GATHER:
            scanf("%d %d %d %d", &in, &in2, &in3, &in4);
            ret = gather(in, in2, in3, in4);
            scanf("%d", &ans);
            if (ret != ans)
                okay = false;
            break;
        case MIX:
            scanf("%d", &in);
            sRet = mix(in);
            scanf("%d %d", &ans, &ans2);
            if (sRet.mCost != ans || sRet.mContent != ans2) {
                // okay = false;
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
    for (int tc = 1; tc <= T; tc++) {
        clock_t tc_start = clock();
        int score = run() ? MARK : 0;
        int tc_result = (clock() - tc_start) / (CLOCKS_PER_SEC / 1000);
		printf("#%2d %d (%3d ms)\n", tc, score, tc_result);
    }
    int result = (clock() - start) / (CLOCKS_PER_SEC / 1000);
	printf(">> Result: %d ms\n", result);

    return 0;
}