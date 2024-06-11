#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
#include <time.h>

#define MAX_LEN 100

extern void init(int mLen, char mSubexp[]);
extern int append(char mLink, char mSubexp[]);
extern int erase(int mFrom);
extern int select(int mFrom, int mTo);

#define CMD_INIT 100
#define CMD_APPEND 200
#define CMD_ERASE 300
#define CMD_SELECT 400

static bool run()
{
    int query_num;
    scanf("%d", &query_num);

    int ret, ans;
    bool ok = false;
    char mSubexp[MAX_LEN+1];

    for (int q = 0; q < query_num; q++)
    {
        int query;
        scanf("%d", &query);

        if (query == CMD_INIT)
        {
            int mLen;
            scanf("%d %s", &mLen, mSubexp);
            init(mLen, mSubexp);
            ok = true;
        }
        else if (query == CMD_APPEND)
        {
            char mLink[2];
            scanf("%s %s", mLink, mSubexp);
            ret = append(mLink[0], mSubexp);
            scanf("%d", &ans);
            if (ans != ret)
            {
                ok = false;
            }
        }
        else if (query == CMD_ERASE)
        {
            int mFrom;
            scanf("%d", &mFrom);
            ret = erase(mFrom);
            scanf("%d", &ans);
            if (ans != ret)
            {
                ok = false;
            }
        }
        else if (query == CMD_SELECT)
        {
            int mFrom, mTo;
            scanf("%d %d", &mFrom, &mTo);
            ret = select(mFrom, mTo);
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