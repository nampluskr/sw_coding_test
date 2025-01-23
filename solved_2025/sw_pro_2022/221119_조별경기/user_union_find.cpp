#if 1   // 434 ms (Union Find)
#define MAX_SIZE    100'001

void swap(int& x, int& y) { int temp = x; x = y; y = temp; }

int teams[MAX_SIZE];    // player 가 속한 team ID
int ranks[MAX_SIZE];    // 트리의 높이
int scores[MAX_SIZE];   // player 의 score

// find
int findTeam(int x) {
    if (x == teams[x]) return x;
    int teamID = findTeam(teams[x]);

    // Path Compression
    if (teamID != teams[x]) {
        scores[x] += scores[teams[x]];
        teams[x] = teamID;
    }
    return teamID;
}

//////////////////////////////////////////////////////////////////////
void init(int N)
{
    for (int i = 1; i <= N; i++) {
        teams[i] = i;
        ranks[i] = 0;
        scores[i] = 0;
    }
}

void updateScore(int mWinnerID, int mLoserID, int mScore)
{
    scores[findTeam(mWinnerID)] += mScore;
    scores[findTeam(mLoserID)] -= mScore;
}

// merge or union
void unionTeam(int mPlayerA, int mPlayerB)
{
    int x = findTeam(mPlayerA);
    int y = findTeam(mPlayerB);
    // if (x == y) return;  // mPlyerA != mPlayerB (문제에서 명시)
    if (ranks[x] < ranks[y]) swap(x, y);

    teams[y] = x;
    scores[y] -= scores[x];
    if (ranks[x] == ranks[y]) ranks[x]++;
}

int getScore(int mID)
{
    int teamID = findTeam(mID);
    if (teamID == mID) return scores[mID];
    return scores[mID] + scores[teamID];
}
#endif