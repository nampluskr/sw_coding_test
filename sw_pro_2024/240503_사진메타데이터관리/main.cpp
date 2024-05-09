#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
#include <time.h>

void init(int N, char pictureList[][200]);
void savePictures(int M, char pictureList[][200]);
int filterPictures(char mFilter[], int kTH);
int deleteOldest(void);

#define CMD_INIT 100
#define CMD_SAVE 200
#define CMD_FILTER 300
#define CMD_DELETE 400

static char initDatas[10000][200];
static char newDatas[100][200];

static bool run()
{
    bool is_ok = false;
    int query_num = 0;
    scanf("%d\n", &query_num);

    for (int q = 0; q < query_num; q++)
    {
        int query;
        scanf("%d", &query);

        if (query == CMD_INIT)
        {
            int n = 0;
            scanf("%d", &n);
            for (int i = 0; i < n; i++)
                scanf("%s", initDatas[i]);

            init(n, initDatas);
            is_ok = true;
        }
        else if (query == CMD_SAVE)
        {
            int m = 0;
            scanf("%d", &m);

            for (int i = 0; i < m; i++)
                scanf("%s", newDatas[i]);

            savePictures(m, newDatas);
        }
        else if (query == CMD_FILTER)
        {
            char filter_param[100] = { 0, };
            int k = 0;
            scanf("%s%d", filter_param, &k);

            int user_ans = filterPictures(filter_param, k);
            int correct_ans = 0;
            scanf("%d", &correct_ans);

            if (user_ans != correct_ans)
                is_ok = false;
        }
        else if (query == CMD_DELETE)
        {
            int user_ans = deleteOldest();
            int correct_ans = 0;
            scanf("%d", &correct_ans);

            if (user_ans != correct_ans)
                is_ok = false;
        }
    }
    return is_ok;
}


int main()
{
    setbuf(stdout, NULL);
    freopen("D:\problems\sw_pro_2024\240503_사진메타데이터관리\sample_input.txt", "r", stdin);

    int TC, MARK;

    scanf("%d %d", &TC, &MARK);
    for (int tc = 1; tc <= TC; ++tc)
    {
        int score = run() ? MARK : 0;
        printf("#%d %d\n", tc, score);
    }
    return 0;
}