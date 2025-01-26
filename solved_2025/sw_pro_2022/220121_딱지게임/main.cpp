#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
#include <time.h>

extern void init(int, int);
extern int addDdakji(int, int, int, int);
extern int check(int, int);

static unsigned int seed = 5;
static int pseudo_rand(void)
{
    seed = seed * 214013 + 2531011;
    return (seed >> 16) & 0x7FFF;
}

static int board_size, ddakji_size, query_cnt;

static int run(int score)
{
    scanf("%d%d%d%d", &seed, &board_size, &ddakji_size, &query_cnt);
    init(board_size, ddakji_size);

    for (int q = 0; q < query_cnt; q++)
    {
        int r, c, size, user_ans, correct_ans;

        size = (pseudo_rand() * pseudo_rand()) % ddakji_size + 1;
        r = (pseudo_rand() * pseudo_rand()) % (board_size - size + 1);
        c = (pseudo_rand() * pseudo_rand()) % (board_size - size + 1);

        user_ans = addDdakji(r, c, size, 1);
        scanf("%d", &correct_ans);

        if (user_ans != correct_ans)
            score = 0;

        size = (pseudo_rand() * pseudo_rand()) % ddakji_size + 1;
        r = (pseudo_rand() * pseudo_rand()) % (board_size - size + 1);
        c = (pseudo_rand() * pseudo_rand()) % (board_size - size + 1);

        user_ans = addDdakji(r, c, size, 2);
        scanf("%d", &correct_ans);

        if (user_ans != correct_ans)
            score = 0;

        r = (pseudo_rand() * pseudo_rand()) % (board_size);
        c = (pseudo_rand() * pseudo_rand()) % (board_size);

        user_ans = check(r, c);
        scanf("%d", &correct_ans);

        if (user_ans != correct_ans)
            score = 0;
    }
    return score;
}

int main()
{
    clock_t start = clock();
    setbuf(stdout, NULL);
    freopen("sample_input.txt", "r", stdin);

    int tc, score;
    scanf("%d%d", &tc, &score);

    for (int t = 1; t <= tc; t++)
    {
        clock_t t_start = clock();
        int t_score = run(score);
        int t_result = (clock() - t_start) / (CLOCKS_PER_SEC / 1000);
        printf("#%2d %d (%3d ms)\n", t, t_score, t_result);
    }
    int result = (clock() - start) / (CLOCKS_PER_SEC / 1000);
    printf(">> Result: %d ms\n", result);
    return 0;
}