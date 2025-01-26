#if 0   // 875 ms (wsl)
#include <vector>
using namespace std;

#define MAXCARD 20000

int max(int a, int b) {
    if (a > b) { return a; }
    return b;
}

int min(int a, int b) {
    if (a < b) { return a; }
    return b;
}

class Ddakji {
public:
    int row;
    int col;
    int size;
    int groupIdx;

    Ddakji() {
        row = -1;
        col = -1;
        size = 0;
        groupIdx = -1;
    }
    void init(int mRow, int mCol, int mSize) {
        row = mRow;
        col = mCol;
        size = mSize;
    }
};

class Node {
public:
    int idx;
    Node* next;

    Node() {
        idx = -1;
        next = nullptr;
    }
};

class LinkedList {
public:
    Node* head;
    Node* tail;
    int length;
    int player;

    LinkedList() {
        head = nullptr;
        tail = nullptr;
        length = 0;
        player = 0;
    }
    void push(int a) {
        Node* node = new Node();
        node->idx = a;
        node->next = head;

        if (!tail) { tail = node; }
        head = node;
        length++;
    }
};

int dIdx, gIdx;
Ddakji ddakjis[MAXCARD];
LinkedList group[MAXCARD];

void assembleLink(int idx1, int idx2) {
    Node* node = group[idx2].head;
    while (node) {
        ddakjis[node->idx].groupIdx = idx1;
        node = node->next;
    }
    group[idx1].tail->next = group[idx2].head;
    group[idx1].tail = group[idx2].tail;
    group[idx1].length += group[idx2].length;
    group[idx2] = LinkedList();
}

bool checkOverlap(int idx1, int idx2) {
    int maxRow = max(ddakjis[idx1].row, ddakjis[idx2].row);
    int minRow = min(ddakjis[idx1].row + ddakjis[idx1].size, ddakjis[idx2].row + ddakjis[idx2].size);
    int maxCol = max(ddakjis[idx1].col, ddakjis[idx2].col);
    int minCol = min(ddakjis[idx1].col + ddakjis[idx1].size, ddakjis[idx2].col + ddakjis[idx2].size);

    if (maxRow < minRow && maxCol < minCol) { return true; }
    return false;
}

bool checkGroup(int iGroup, int idx) {
    Node* node = group[iGroup].head;
    while (node) {
        if (checkOverlap(node->idx, idx)) {
            return true;
        }
        node = node->next;
    }
    return false;
}

int getPlayerCards(int mPlayer) {
    int ans = 0;
    for (int i = 0; i < gIdx; i++) {
        if (group[i].player == mPlayer) {
            ans += group[i].length;
        }
    }
    return ans;
}

void init(int N, int M)
{
    dIdx = 0;
    gIdx = 0;
    for (int i = 0; i < MAXCARD; i++) {
        group[i] = LinkedList();
    }
}

int addDdakji(int mRow, int mCol, int mSize, int mPlayer)
{
    ddakjis[dIdx].init(mRow, mCol, mSize);
    vector<int> groupIndices;

    for (int i = 0; i < gIdx; i++) {
        if (checkGroup(i, dIdx)) {
            groupIndices.push_back(i);
        }
    }

    if (groupIndices.empty()) {
        ddakjis[dIdx].groupIdx = gIdx;
        group[gIdx].player = mPlayer;
        group[gIdx++].push(dIdx);
    }
    else {
        for (int i = 1; i < groupIndices.size(); i++) {
            assembleLink(groupIndices[0], groupIndices[i]);
            group[groupIndices[i]].player = mPlayer;
        }
        ddakjis[dIdx].groupIdx = groupIndices[0];
        group[groupIndices[0]].push(dIdx);
        group[groupIndices[0]].player = mPlayer;
    }
    dIdx++;
    return getPlayerCards(mPlayer);
}

int check(int mRow, int mCol)
{
    for (int i = 0; i < dIdx; i++) {
        if (ddakjis[i].row <= mRow && mRow < ddakjis[i].row + ddakjis[i].size &&
            ddakjis[i].col <= mCol && mCol < ddakjis[i].col + ddakjis[i].size) {
            return group[ddakjis[i].groupIdx].player;
        }
    }
    return 0;
}
#endif