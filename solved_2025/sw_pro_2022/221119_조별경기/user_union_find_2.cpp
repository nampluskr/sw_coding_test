#if 1   // xx ms (Union Find)
#define MAX_SIZE    100'001

void swap(int& x, int& y) { int temp = x; x = y; y = temp; }

int teams[MAX_SIZE];        // 각 플레이어가 속한 팀 ID
int ranks[MAX_SIZE];        // 트리의 높이
int groupScore[MAX_SIZE];   // 각 그룹의 총 점수
int offset[MAX_SIZE];       // 각 플레이어가 그룹 점수에 대해 가지는 상대적 offset

// find: 특정 플레이어가 속한 팀의 루트를 찾음 (경로 압축 포함)
int findTeam(int x) {
    if (x != teams[x]) {
        int root = findTeam(teams[x]);          // 상위 노드의 루트를 찾음
        offset[x] += offset[teams[x]];          // 상위 노드의 offset을 누적
        teams[x] = root;                        // 경로 압축
    }
    return teams[x];
}

// 초기화: 모든 플레이어를 독립적인 팀으로 초기화
void init(int N) {
    for (int i = 1; i <= N; i++) {
        teams[i] = i;         // 초기에는 자기 자신이 팀의 루트
        ranks[i] = 0;         // 초기 랭크는 0
        groupScore[i] = 0;    // 초기 그룹 점수는 0
        offset[i] = 0;        // 초기 offset은 0
    }
}

// 점수 업데이트: 승자와 패자의 그룹 점수를 조정
void updateScore(int mWinnerID, int mLoserID, int mScore) {
    int winnerRoot = findTeam(mWinnerID);
    int loserRoot = findTeam(mLoserID);

    groupScore[winnerRoot] += mScore;   // 승자 그룹에 점수 추가
    groupScore[loserRoot] -= mScore;   // 패자 그룹에서 점수 차감
}

// 팀 병합: 두 플레이어가 속한 팀을 병합
void unionTeam(int mPlayerA, int mPlayerB) {
    int rootA = findTeam(mPlayerA);
    int rootB = findTeam(mPlayerB);

    if (rootA == rootB) return; // 이미 같은 팀이면 병합하지 않음

    // 랭크(rank)에 따라 병합 순서를 결정
    if (ranks[rootA] < ranks[rootB]) swap(rootA, rootB);

    teams[rootB] = rootA;                       // B팀을 A팀에 병합
    offset[rootB] += groupScore[rootA] - groupScore[rootB]; // 상대적 offset 조정

    // groupScore[rootA] += groupScore[rootB];     // A팀에 B팀 점수를 합산

    if (ranks[rootA] == ranks[rootB]) ranks[rootA]++;
}

// 특정 플레이어의 실제 점수를 반환
int getScore(int mID) {
    // findTeam(mID);                              // 경로 압축 및 offset 계산
    return groupScore[findTeam(mID)] + offset[mID];
}

#endif