#if 0   // 593 ms (가독성 개선)
#include <vector>

#define MAX_PLAYERS 100'001 // N: 조별 경기에 참가한 선수들의 수

// Main DB
struct Player {
    int mScore;
    int teamID;
} players[MAX_PLAYERS];

struct Team {
    int mScore;
    std::vector<int> playerList;
} teams[MAX_PLAYERS];

//////////////////////////////////////////////////////////////////////
void init(int N)
{
    // 선수 ID 1부터 시작
    for (int i = 1; i <= N; i++) {
        players[i] = { 0, i };
        teams[i].playerList.clear();
        teams[i] = { 0, {i} };
    }
}

void updateScore(int mWinnerID, int mLoserID, int mScore)
{
    auto& winnerTeam = teams[players[mWinnerID].teamID];
    auto& loserTeam = teams[players[mLoserID].teamID];

    winnerTeam.mScore += mScore;
    loserTeam.mScore -= mScore;
}

void unionTeam(int mPlayerA, int mPlayerB)
{
    int teamA_size = teams[players[mPlayerA].teamID].playerList.size();
    int teamB_size = teams[players[mPlayerB].teamID].playerList.size();
    if (teamA_size < teamB_size) 
        std::swap(mPlayerA, mPlayerB);

    auto& teamA = teams[players[mPlayerA].teamID];
    auto& teamB = teams[players[mPlayerB].teamID];

    for (int mID: teamB.playerList) {
        auto& playerB = players[mID];
        playerB.mScore += teamB.mScore - teamA.mScore;
        playerB.teamID = players[mPlayerA].teamID;
        teamA.playerList.push_back(mID);
    }
    // teamB.playerList.clear();
}

int getScore(int mID)
{
    auto& player = players[mID];
    return player.mScore + teams[player.teamID].mScore;
}
#endif