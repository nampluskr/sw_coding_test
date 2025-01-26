#if 0   // 1160 ms (KMJ)
#define P_COUNT 20

const int MAX_SIZE = 20000;
int root[MAX_SIZE];
int rank[MAX_SIZE];
int totalcount[3];
int hrp;
int P_SIZE;
 
struct ListNode {
    int id;
    ListNode*next;
};
 
ListNode heap[1000000];
 
ListNode* appendListNode(int id, ListNode* oldHead) {
    ListNode* node = &heap[hrp++];
    node->id = id;
    node->next = oldHead;
    return node;
}
 
ListNode* head[P_COUNT][P_COUNT];
 
struct Group {
    int count;
    int owner;
};
 
Group group[MAX_SIZE];
 
// x번째 딱지가 속하는 그룹에서 가장 밑에 깔린 딱지를 찾아서 리턴한다.
int find(int x)
{
    if (root[x] == x)
    {
        return x;
    }
    else
    {
        return root[x] = find(root[x]);
    }
}
 
// x딱지가 속한 그룹과 y딱지가 속한 그룹을 합친다.
void Union(int x, int y)
{
    x = find(x);
    y = find(y);
 
    if (x == y)
        return;
 
    if (group[x].owner != group[y].owner) {
        totalcount[group[x].owner] -= group[x].count;
        totalcount[group[y].owner] += group[x].count;
    }
 
    group[x].owner = group[y].owner;
 
    if (rank[x] < rank[y])
    {
        group[y].count += group[x].count;
        root[x] = y;
    }
    else
    {
        root[y] = x;
        group[x].count += group[y].count;
        if (rank[x] == rank[y])
            rank[x]++;
    }
}
 
struct Ddakji {
    int r_st;
    int r_ed;
    int c_st;
    int c_ed;
};
 
int wp;
int min(int a, int b) { return a < b ? a : b; }
int max(int a, int b) { return a > b ? a : b; }
int getPartitionNo(int n) {  return n / P_SIZE;  }
Ddakji ddakji[MAX_SIZE];
 
// x,y딱지가 겹치는 부분이 있다면 true 아니면 false를 리턴한다. 
inline bool isOverlapped(int x, int y) {
    if(min(ddakji[x].r_ed, ddakji[y].r_ed) - max(ddakji[x].r_st, ddakji[y].r_st) < 0 )
        return false;
    if (min(ddakji[x].c_ed, ddakji[y].c_ed) - max(ddakji[x].c_st, ddakji[y].c_st) < 0)
        return false;
    return true;
}
 
void init(int N, int M)
{
    P_SIZE = M/2;
    if (P_SIZE == 0)
        P_SIZE = 5;
 
    wp = 0;
    hrp = 0;
    totalcount[1] = 0;
    totalcount[2] = 0;
 
    for (int i = 0; i < MAX_SIZE; i++) {
        root[i] = i;
        rank[i] = 0;
    }
 
    for (int i = 0; i < P_COUNT; i++) {
        for (int j = 0; j < P_COUNT; j++) {
            head[i][j] = 0;
        }
    }
}
 
int addDdakji(int mRow, int mCol, int mSize, int mPlayer)
{
    ddakji[wp].r_st = mRow;
    ddakji[wp].c_st = mCol;
    ddakji[wp].r_ed = mRow + mSize - 1;
    ddakji[wp].c_ed = mCol + mSize - 1;
    group[wp].count = 1;
    group[wp].owner = mPlayer;
    totalcount[mPlayer]++;
     
    int pr_st = getPartitionNo(mRow);
    int pr_ed = getPartitionNo(mRow + mSize - 1);
    int pc_st = getPartitionNo(mCol);
    int pc_ed = getPartitionNo(mCol + mSize - 1);
 
 
    for (int pr = pr_st; pr <= pr_ed; pr++) {
        for (int pc = pc_st; pc <= pc_ed; pc++) {
            ListNode* temp = head[pr][pc];
 
            while (temp != 0) {
                int i = temp->id;
                if (isOverlapped(i, wp))
                    Union(i, wp);
                temp = temp->next;
            }
        }
    }
 
    for (int pr = pr_st; pr <= pr_ed; pr++) {
        for (int pc = pc_st; pc <= pc_ed; pc++) {
            head[pr][pc] = appendListNode(wp, head[pr][pc]);
        }
    }
     
    /*
     for (int i = 0; i < wp; i++) {
         if (isOverlapped(i, wp))
             Union(i, wp);
     }
    */
    wp++;
    return totalcount[mPlayer];
}
 
int check(int mRow, int mCol)
{
    int pr = getPartitionNo(mRow);
    int pc = getPartitionNo(mCol);
    ListNode* temp = head[pr][pc];
 
    while (temp != 0) {
        int i = temp->id;
 
        if (mRow >= ddakji[i].r_st && ddakji[i].r_ed >= mRow && mCol >= ddakji[i].c_st && ddakji[i].c_ed >= mCol) {
            int gID = find(i);
            return group[gID].owner;
        }
        temp = temp->next;
    }
    /*
     for (int i = 0; i < wp; i++) {
         if (mRow < ddakji[i].r_st || ddakji[i].r_ed < mRow)
             continue;
         if (mCol < ddakji[i].c_st || ddakji[i].c_ed < mCol)
             continue;
 
         int gID = find(i);
         return group[gID].owner;
     }
    */
    return 0;
} 
#endif