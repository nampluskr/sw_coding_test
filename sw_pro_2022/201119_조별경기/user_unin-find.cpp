#if 0
inline void swap(int& a, int& b) { int temp = a; a = b; b = temp; }

constexpr int MAX_N = 100'001;

struct UnionFind {
    int parent[MAX_N];
    int rank[MAX_N];
    int setSize[MAX_N];
    int setCnt;
    int score[MAX_N];

    void init(int n) {
        for (int i = 1; i <= n; i++) {
            parent[i] = i;
            rank[i] = 0;
            setSize[i] = 1;
            score[i] = 0;
        }
        setCnt = n;
    }
    int find(int x) {
        // return (x == parent[x]) ? x : parent[x] = find(parent[x]);
        if (x == parent[x]) return x;
        int root = find(parent[x]);

        if (root != parent[x]) {
            score[x] += score[parent[x]];
            parent[x] = root;
	    }
	    return root;
    }
    void merge(int x, int y) {
        x = find(x), y = find(y);
        if (rank[x] < rank[y]) swap(x, y);

        parent[y] = x;
        setSize[x] += setSize[y];
        setCnt--;
        score[y] -= score[x];
        if (rank[x] == rank[y]) rank[x]++;
    }
    int size(int x) { return setSize[find(x)]; }
};
UnionFind players;

/////////////////////////////////////////////////////////////////////
void init(int N)
{
    players.init(N);
}

void updateScore(int mWinnerID, int mLoserID, int mScore) {
	players.score[players.find(mWinnerID)] += mScore;
	players.score[players.find(mLoserID)] -= mScore;
}

void unionTeam(int mPlayerA, int mPlayerB) {
    players.merge(mPlayerA, mPlayerB);
}

int getScore(int mID) {
	int root = players.find(mID);
	if (root == mID) return players.score[mID];
	return players.score[mID] + players.score[root];
}
#endif