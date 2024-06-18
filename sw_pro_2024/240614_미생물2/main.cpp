#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
#include <time.h>

extern void init();
extern void addBacteria(int tStamp, int mID, int mLifeSpan, int mHalfTime);
extern int getMinLifeSpan(int tStamp);
extern int getCount(int tStamp, int mMinSpan, int mMaxSpan);

#define CMD_INIT   0
#define CMD_ADD    1
#define CMD_MINSPAN	2
#define CMD_GET    3

static bool run()
{
	int Q, time, id, minSpan, maxSpan, halftime;
	int ret, ans;
	bool ok = false;
	scanf("%d", &Q);

	for (int q = 0; q < Q; q++)
	{
		int cmd;
		scanf("%d", &cmd);

		if (cmd == CMD_INIT)
		{
			init();
			ok = true;
		}
		else if (cmd == CMD_ADD)
		{
			scanf("%d %d %d %d", &time, &id, &maxSpan, &halftime);
			addBacteria(time, id, maxSpan, halftime);
		}
		else if (cmd == CMD_MINSPAN)
		{
			scanf("%d", &time);
			ret = getMinLifeSpan(time);
			scanf("%d", &ans);
			if (ans != ret) {
				ok = false;
			}
		}
		else if (cmd == CMD_GET)
		{
			scanf("%d %d %d", &time, &minSpan, &maxSpan);
			int ret = getCount(time, minSpan, maxSpan);
			scanf("%d", &ans);
			if (ans != ret) {
				ok = false;
			}
		}
		else ok = false;
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