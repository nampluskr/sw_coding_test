#if 0
#include <string.h>
#define MAX_BCNT 100
#define MAX_NAME 10
#define MAX_ADD 15000
#define MAX_SIZE 500000

int min(int a, int b) { return a < b ? a : b; }
 
struct PriorityQueue {
    struct Data {
        int id;
        unsigned long long value;
    };
 
    Data heap[MAX_SIZE];
    int heapSize = 0;
 
    void heapInit() {
        heapSize = 0;
    }
 
    void heapPush(int id, unsigned long long value) {
        heap[heapSize].id = id;
        heap[heapSize].value = value;
 
        int current = heapSize;
 
        while (current > 0 && heap[current].value < heap[(current - 1) / 2].value) {
            Data temp = heap[(current - 1) / 2];
            heap[(current - 1) / 2] = heap[current];
            heap[current] = temp;
            current = (current - 1) / 2;
        }
 
        heapSize = heapSize + 1;
    }
 
    Data heapPop() {
        Data value = heap[0];
        heapSize = heapSize - 1;
        heap[0] = heap[heapSize];
 
        int current = 0;
 
        while (current * 2 + 1 < heapSize) {
            int child;
 
            if (current * 2 + 2 == heapSize)
                child = current * 2 + 1;
            else
                child = heap[current * 2 + 1].value < heap[current * 2 + 2].value ? current * 2 + 1 : current * 2 + 2;
 
            if (heap[current].value < heap[child].value)
                break;
 
            Data temp = heap[current];
            heap[current] = heap[child];
            heap[child] = temp;
            current = child;
        }
        return value;
    }
};
 
PriorityQueue halfLife;
PriorityQueue life;
 
 
struct Bacteria {
    char bName[MAX_NAME];
    int mHalfTime;
    int mCnt;
};
 
Bacteria bacteria[MAX_BCNT];
 
struct Bottle {
    int bIdx;
    int mCnt;
    int mLife;
    int addTime;
};
 
Bottle bottle[MAX_ADD];
int bacteriaCount;
int bottleNo;
int currenttime;
int totalCnt;
 
// 완료되면 바로 다시 시작한다.
void completeAndStart() {
    while (halfLife.heapSize > 0 && halfLife.heap[0].value == currenttime) {
        PriorityQueue::Data data = halfLife.heapPop();
 
        if (bottle[data.id].mCnt == 0)
            continue;
 
        bottle[data.id].mLife /= 2;
        int bIdx = bottle[data.id].bIdx;
 
        if (bottle[data.id].mLife <= 9) {
            bacteria[bIdx].mCnt -= bottle[data.id].mCnt;
            totalCnt -= bottle[data.id].mCnt;
            bottle[data.id].mCnt = 0;
        }
        else {
            halfLife.heapPush(data.id, currenttime + bacteria[bIdx].mHalfTime);
            life.heapPush(data.id, (unsigned long long)bottle[data.id].mLife * 10000000 + bottle[data.id].addTime);
        }
    }
}
 
void update(int tStamp) {
    while (currenttime < tStamp) {
        currenttime++;
        completeAndStart();
        /* tStamp 최대값이 1,000,000 이라 필수는 아니다.
         if (halfLife.heapSize > 0 && halfLife.heap[0].value <= tStamp)
             currenttime = halfLife.heap[0].value - 1;
         else
             currenttime = tStamp;
        */
    }
}
 
int getBacteriaIdx(char bName[MAX_NAME]) {
    for (int i = 0; i < bacteriaCount; i++) {
        if (strcmp(bacteria[i].bName, bName) == 0)
            return i;
    }
    return -1;
}
 
void init(int N, char bNameList[MAX_BCNT][MAX_NAME], int mHalfTime[MAX_BCNT])
{
    currenttime = 0;
    totalCnt = 0;
    bottleNo = 0;
    bacteriaCount = N;
    halfLife.heapInit();
    life.heapInit();
 
    for (int i = 0; i < N; i++) {
        strcpy(bacteria[i].bName, bNameList[i]);
        bacteria[i].mHalfTime = mHalfTime[i];
        bacteria[i].mCnt = 0;
    }
}
 
void addBacteria(int tStamp, char bName[MAX_NAME], int mLife, int mCnt)
{
    update(tStamp);
    int bIdx = getBacteriaIdx(bName);
    bottle[bottleNo].mCnt = mCnt;
    bottle[bottleNo].mLife = mLife;
    bottle[bottleNo].bIdx = bIdx;
    bottle[bottleNo].addTime = tStamp;
    bacteria[bIdx].mCnt += mCnt;
    totalCnt += mCnt;
 
    halfLife.heapPush(bottleNo, currenttime + bacteria[bIdx].mHalfTime);
    life.heapPush(bottleNo, (unsigned long long)mLife * 10000000 + tStamp);
    bottleNo++;
}
 
int takeOut(int tStamp, int mCnt)
{
    update(tStamp);
 
    int out = min(totalCnt, mCnt);
    int lifeSum = 0;
 
    while (out > 0) {
        PriorityQueue::Data data = life.heapPop();
 
        if (bottle[data.id].mCnt == 0 || bottle[data.id].mLife != data.value / 10000000)
            continue;
        int cnt = min(bottle[data.id].mCnt, out);
        out -= cnt;
        bottle[data.id].mCnt -= cnt;
        lifeSum += cnt * bottle[data.id].mLife;
        int bIdx = bottle[data.id].bIdx;
        bacteria[bIdx].mCnt -= cnt;
 
        if(bottle[data.id].mCnt > 0)
            life.heapPush(data.id, (unsigned long long)bottle[data.id].mLife * 10000000 + bottle[data.id].addTime);
    }
     
    return lifeSum;
}
 
int checkBacteria(int tStamp, char bName[MAX_NAME])
{
    update(tStamp);
    int bIdx = getBacteriaIdx(bName);
    return bacteria[bIdx].mCnt;
}
#endif