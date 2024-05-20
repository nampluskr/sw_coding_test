#if 1
#include <vector>
using namespace std;

#define MAX_PLAYERS 100'001       // [4, 100,000]

struct Player {
    int score;
    int teamIdx;
} players[MAX_PLAYERS];

struct Team {
    int score;
    vector<int> playerList;
} teams[MAX_PLAYERS];


/////////////////////////////////////////////////////////////////////
void init(int N)
{
    for (int i = 1; i <= N; i++) {
        players[i].score = 0;
        players[i].teamIdx = i;
        teams[i].score = 0;
        teams[i].playerList = { i };
    }
}

void updateScore(int mWinnerID, int mLoserID, int mScore)
{
    teams[players[mWinnerID].teamIdx].score += mScore;
    teams[players[mLoserID].teamIdx].score -= mScore;
}

void unionTeam(int mPlayerA, int mPlayerB)
{
    int teamA = players[mPlayerA].teamIdx;
    int teamB = players[mPlayerB].teamIdx;

    if (teams[teamA].playerList.size() < teams[teamB].playerList.size())
        swap(teamA, teamB);

    for (int playerB: teams[teamB].playerList) {
        teams[teamA].playerList.push_back(playerB);
        players[playerB].score += teams[teamB].score - teams[teamA].score;
        players[playerB].teamIdx = teamA;
    }
    // teams[teamB].playerList.clear();
}

int getScore(int mID)
{
    return players[mID].score + teams[players[mID].teamIdx].score;
}
#endif