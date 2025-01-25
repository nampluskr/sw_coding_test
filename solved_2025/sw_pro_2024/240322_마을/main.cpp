#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
#include <time.h>

extern void init(int L, int R);
extern int add(int mId, int mX, int mY);
extern int remove(int mId);
extern int check(int mId);
extern int count();

//////////////////////////////////////////////////////////////
#define CMD_INIT 100
#define CMD_ADD 200
#define CMD_REMOVE 300
#define CMD_CHECK 400
#define CMD_COUNT 500

static bool run() {
	int q;
	scanf("%d", &q);

	int l, r, mId, mX, mY;
	int cmd, ans, ret = 0;
	bool okay = false;

	for (int i = 0; i < q; ++i) {
		scanf("%d", &cmd);
		switch (cmd) {
		case CMD_INIT:
			okay = true;
			scanf("%d %d", &l, &r);
			init(l, r);
			break;

		case CMD_ADD:
			scanf("%d %d %d %d", &mId, &mX, &mY, &ans);
			ret = add(mId, mX, mY);
			if (ans != ret)
				okay = false;
			break;

		case CMD_REMOVE:
			scanf("%d %d", &mId, &ans);
			ret = remove(mId);
			if (ans != ret)
				okay = false;
			break;

		case CMD_CHECK:
			scanf("%d %d", &mId, &ans);
			ret = check(mId);
			if (ans != ret)
				okay = false;
			break;

		case CMD_COUNT:
			scanf("%d", &ans);
			ret = count();
			if (ans != ret)
				okay = false;
			break;

		default:
			okay = false;
			break;
		}
	}
	return okay;
}

int main() {
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