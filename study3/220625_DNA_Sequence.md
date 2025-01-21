### KMJ

```cpp
#include <string.h>
 #define MAX_SEQ 10000
 #define MAX_TABLE 20000

int wp;
int hrp;
int checktime[MAX_SEQ];
int currenttime;
 
unsigned long hash(const char *str){
    unsigned long hash = 5381;
    int c;
 
    while (c = *str++)
    {
        hash = (((hash << 5) + hash) + c) % MAX_TABLE;
    }
    return hash % MAX_TABLE;
}
 
unsigned long hash(const char *str, int len) {
    unsigned long hash = 5381;
    for (int c = 0; c < len; c++) {
        hash = (((hash << 5) + hash) + str[c]) % MAX_TABLE;
    }
    return hash % MAX_TABLE;
}
 
struct ListNode {
    int id;
    ListNode *next;
};
 
ListNode heap[1000000];
 
ListNode* appendListNode(int id, ListNode* oldHead) {
    ListNode* node = &heap[hrp++];
    node->id = id;
    node->next = oldHead;
    return node;
}
 
 
struct NucleicSequence {
    int mID;
    int mLen;
    char mSeq[60];
    //char compressedSeq[20];
    bool isDeleted;
};
 
NucleicSequence nucleicSequence[MAX_SEQ];
 
ListNode* headNodeByID[MAX_TABLE];
ListNode* headNodeByFullSeq[MAX_TABLE];
ListNode* headNodeBy3Char[MAX_TABLE];
 
int mstrcmp(char str1[], char str2[], int len) {
 
    int c = 0;
    while (c < len - 1 && str1[c]!= 0 && str1[c] == str2[c]) {
        c++;
    }
    return str1[c] - str2[c];
}
 
int getNucleicSequenceIdx(int mID) {
    ListNode* temp = headNodeByID[mID % MAX_TABLE];
    while (temp != 0) {
        int i = temp->id;
        if (!nucleicSequence[i].isDeleted && nucleicSequence[i].mID == mID)
            return i;
        temp = temp->next;
    }
    return -1;
}
 
int getNucleicSequenceIdx(char mSeq[]) {
    int hashkey = hash(mSeq);
    ListNode* temp = headNodeByFullSeq[hashkey];
     
    while (temp != 0) {
        int i = temp->id;
        if (!nucleicSequence[i].isDeleted && strcmp(nucleicSequence[i].mSeq, mSeq) == 0)
            return i;
        temp = temp->next;
    }
     
    return -1;
}
 
void init()
{
    hrp = 0;
    wp = 0;
    currenttime = 0;
 
    for (int i = 0; i < MAX_SEQ; i++) {
        checktime[i] = 0;
    }
 
    for (int i = 0; i < MAX_TABLE; i++) {
        headNodeByID[i] = 0;
        headNodeByFullSeq[i] = 0;
        headNodeBy3Char[i] = 0;
    }
}
 
void insertListNode(int mIdx) {
    int hashkey = hash(nucleicSequence[mIdx].mSeq);
    headNodeByFullSeq[hashkey] = appendListNode(mIdx, headNodeByFullSeq[hashkey]);
    hashkey = hash(nucleicSequence[mIdx].mSeq, 3);
    headNodeBy3Char[hashkey] = appendListNode(mIdx, headNodeBy3Char[hashkey]);
}
 
int addSeq(int mID, int mLen, char mSeq[]) {
    if (getNucleicSequenceIdx(mID) != -1) 
        return 0;
    if (getNucleicSequenceIdx(mSeq) != -1) 
        return 0;
     
    nucleicSequence[wp].isDeleted = false;
    nucleicSequence[wp].mID = mID;
    nucleicSequence[wp].mLen = mLen;
    strcpy(nucleicSequence[wp].mSeq, mSeq);
    headNodeByID[mID % MAX_TABLE] = appendListNode(wp, headNodeByID[mID % MAX_TABLE]);
    insertListNode(wp);
    wp++;
 
    return 1;
}
 
int searchSeq(int mLen, char mBegin[])
{
    currenttime++;
    int seqCount = 0;
    int mID = -1;
    ListNode*temp = headNodeBy3Char[hash(mBegin, 3)];
 
    while (temp != 0) {
        int i = temp->id;
 
        if (checktime[i] != currenttime && !nucleicSequence[i].isDeleted && nucleicSequence[i].mLen >= mLen && mstrcmp(nucleicSequence[i].mSeq, mBegin, mLen) == 0) {
            seqCount++;
            mID = nucleicSequence[i].mID;
            checktime[i] = currenttime;
        }
        temp = temp->next;
    }
    return seqCount >= 2 ? seqCount : mID;
}
 
int eraseSeq(int mID)
{
    int mIdx = getNucleicSequenceIdx(mID);
    if (mIdx == -1)
        return 0;
    nucleicSequence[mIdx].isDeleted = true;
    return 1;
}
 
int changeBase(int mID, int mPos, char mBase)
{
    int mIdx = getNucleicSequenceIdx(mID);
    if (mIdx == -1)                              // ① 식별 번호 mID인 염기 서열이 없는 경우
        return 0;
    if(mPos >= nucleicSequence[mIdx].mLen)       // ② 식별 번호 mID인 염기 서열에서 mPos번째 위치가 없는 경우
        return 0;
    if(nucleicSequence[mIdx].mSeq[mPos] == mBase)//③ 식별 번호 mID인 염기 서열의 mPos번째 문자가 mBase인 경우
        return 0;
    // ④ 식별 번호 mID인 염기 서열의 mPos번째 문자가 mBase로 변경될 경우 이미 똑같은 염기서열이 다른 식별 번호로 등록된 경우
    char mSeq[61];
    strcpy(mSeq, nucleicSequence[mIdx].mSeq);
    mSeq[mPos] = mBase;
    if (getNucleicSequenceIdx(mSeq) != -1)
        return 0;
    nucleicSequence[mIdx].mSeq[mPos] = mBase;
    insertListNode(mIdx);
    return 1;
}
```
