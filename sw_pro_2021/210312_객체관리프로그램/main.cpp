#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
#include <time.h>

const int SELECT = 0;
const int ADD = 1;
const int REMOVE = 2;
const int TAG = 3;

static int n;
static char tag_str[1000][15];

extern void init();
extern int selectAll(char mTag[]);
extern int addChild(char mTag[]);
extern int removeChild(char mTag[]);
extern int addTag(char mTag[]);

/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////

static int mSeed;
static int pseudo_rand(void)
{
    mSeed = mSeed * 431345 + 2531999;
    return mSeed & 0x7FFFFFFF;
}

static void mstrcpy(char dst[], const char src[]) {
    int c = 0;
    while ((dst[c] = src[c]) != 0) ++c;
}

static void make_string(char str[])
{
    int length = 3 + pseudo_rand() % 8;
    for (int i = 0; i < length; ++i) {
    str[i] = 'a' + pseudo_rand() % 26;
    }
    str[length] = 0;
}

static int run(int Ans)
{
    int command[4] = { 0, };
    int query = 0;
    int idx;
    char args_str[15];

    scanf("%d%d", &mSeed, &n);
    for (int i = 0; i < 4; i++)
    {
        scanf("%d", command + i);
        query += command[i];
    }

    mstrcpy(tag_str[0], "root");
       for (int i = 1; i < n; i++)
       {
           make_string(tag_str[i]);
       }
       init();

    while (query)
    {
        int val = pseudo_rand() % query + 1;
        int user_ans, correct_ans;
        int i = 0;
        while (i < 4)
        {
            if (val <= command[i])
            {
                break;
            }
            val -= command[i];
            i++;
        }
        command[i]--;
        query--;

        switch (i)
        {
        case SELECT:
            idx = pseudo_rand() % n;
            mstrcpy(args_str, tag_str[idx]);
            user_ans = selectAll(args_str);
            scanf("%d", &correct_ans);
            if (correct_ans != user_ans)
            {
             Ans = 0;
            }
            break;

        case ADD:
            idx = pseudo_rand() % n;
            mstrcpy(args_str, tag_str[idx]);
            user_ans = addChild(args_str);
            scanf("%d", &correct_ans);
            if (correct_ans != user_ans)
            {
                Ans = 0;
            }
            break;

        case REMOVE:
            idx = pseudo_rand() % n;
            mstrcpy(args_str, tag_str[idx]);
            user_ans = removeChild(args_str);
            scanf("%d", &correct_ans);
            if (correct_ans != user_ans)
            {
                Ans = 0;
            }
            break;

        case TAG:
            idx = pseudo_rand() % n;
            mstrcpy(args_str, tag_str[idx]);
            user_ans = addTag(args_str);
            scanf("%d", &correct_ans);
            if (correct_ans != user_ans)
            {
                Ans = 0;
            }
            break;

        default:
            printf("ERROR\n");
            break;
        }
    }

    return Ans;
}

int main()
{
    clock_t start = clock();
    setbuf(stdout, NULL);
    freopen("sample_input.txt", "r", stdin);
   
    int T, Ans;
    scanf("%d %d", &T, &Ans);
   
    for (int tc = 1; tc <= T; tc++) {
        clock_t tc_start = clock();
        int score = run(Ans);
        int tc_result = (clock() - tc_result) / (CLOCKS_PER_SEC / 1000);
        printf("#%2d %d (%3d ms)\n", tc, score, tc_result);
    }
    int result = (clock() - start) / (CLOCKS_PER_SEC / 1000);
    printf(">> Result: %d ms\n", result);
   
    return 0;
}