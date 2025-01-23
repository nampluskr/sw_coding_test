#if 0   // 466 ms
#include "array.h"
inline void swap(int& a, int& b) { int temp = a; a = b; b = temp; }

#define MAX_PLAYERS 100'001 // N: 조별 경기에 참가한 선수들의 수

// Main DB
struct Player {
    int mScore;
    int teamID;
} players[MAX_PLAYERS];     // 선수 ID 1부터 시작

struct Team {
    int mScore;
    Array<int> playerList;
} teams[MAX_PLAYERS];

//////////////////////////////////////////////////////////////////////
void init(int N)
{
    for (int i = 1; i <= N; i++) {
        players[i] = { 0, i };
        teams[i].playerList.clear();
        teams[i].mScore = 0;
        teams[i].playerList.push_back(i);
    }
}

void updateScore(int mWinnerID, int mLoserID, int mScore)
{
    int winnerTeam = players[mWinnerID].teamID;
    int loserTeam = players[mLoserID].teamID;
    teams[winnerTeam].mScore += mScore;
    teams[loserTeam].mScore -= mScore;
}

void unionTeam(int mPlayerA, int mPlayerB)
{
    int teamA = players[mPlayerA].teamID;
    int teamB = players[mPlayerB].teamID;
    if (teams[teamA].playerList.size() < teams[teamB].playerList.size())
        swap(teamA, teamB);

    for (int playerID: teams[teamB].playerList) {
        players[playerID].mScore += teams[teamB].mScore - teams[teamA].mScore;
        players[playerID].teamID = teamA;
        teams[teamA].playerList.push_back(playerID);
    }
    // teams[teamB].playerList.clear();
}

int getScore(int mID)
{
    return players[mID].mScore + teams[players[mID].teamID].mScore;
}
#endif