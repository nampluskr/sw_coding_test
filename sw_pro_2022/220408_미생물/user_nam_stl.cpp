#if 1   // 371 ms
#include <cstring>
#include <string>
#include <unordered_map>
#include <queue>
using namespace std;


#define MAX_BCNT    100
#define MAX_NAME    10
#define MAX_ADD     15000
#define MAX_SIZE    500000


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

PriorityQueue timePQ;
PriorityQueue lifePQ;

// priority_queue<int> timePQ;
// priority_queue<int> lifePQ;

struct Bacteria {
    char bName[MAX_NAME];
    int mHalfTime;
    int mCnt;
} bacteria[MAX_BCNT];
int bacteriaCnt;

struct Bottle {
    int bIdx;
    int mCnt;
    int mLife;
    int addTime;
} bottles[MAX_ADD];
int bottleNo;

int curTime;
int totalCnt;

// 완료되면 바로 다시 시작한다.
void completeAndStart() {
    while (timePQ.heapSize > 0 && timePQ.heap[0].value == curTime) {
        auto data = timePQ.heapPop();

        if (bottles[data.id].mCnt == 0)
            continue;

        bottles[data.id].mLife /= 2;
        int bIdx = bottles[data.id].bIdx;

        if (bottles[data.id].mLife < 10) {
            bacteria[bIdx].mCnt -= bottles[data.id].mCnt;
            totalCnt -= bottles[data.id].mCnt;
            bottles[data.id].mCnt = 0;
        }
        else {
            timePQ.heapPush(data.id, curTime + bacteria[bIdx].mHalfTime);
            lifePQ.heapPush(data.id, (unsigned long long)bottles[data.id].mLife * 10000000 + bottles[data.id].addTime);
        }
    }
}

void update(int tStamp) {
    while (curTime < tStamp) {
        curTime++;
        completeAndStart();
        /* tStamp 최대값이 1,000,000 이라 필수는 아니다.
         if (timePQ.heapSize > 0 && timePQ.heap[0].value <= tStamp)
             curTime = timePQ.heap[0].value - 1;
         else
             curTime = tStamp;
        */
    }
}

int get_bacteriaIndex(char bName[MAX_NAME]) {
    for (int i = 0; i < bacteriaCnt; i++) {
        if (strcmp(bacteria[i].bName, bName) == 0)
            return i;
    }
    return -1;
}

void init(int N, char bNameList[MAX_BCNT][MAX_NAME], int mHalfTime[MAX_BCNT])
{
    curTime = 0;
    totalCnt = 0;
    bottleNo = 0;
    bacteriaCnt = N;
    timePQ.heapInit();
    lifePQ.heapInit();

    for (int i = 0; i < N; i++) {
        strcpy(bacteria[i].bName, bNameList[i]);
        bacteria[i].mHalfTime = mHalfTime[i];
        bacteria[i].mCnt = 0;
    }
}

void addBacteria(int tStamp, char bName[MAX_NAME], int mLife, int mCnt)
{
    update(tStamp);
    int bIdx = get_bacteriaIndex(bName);
    bottles[bottleNo].mCnt = mCnt;
    bottles[bottleNo].mLife = mLife;
    bottles[bottleNo].bIdx = bIdx;
    bottles[bottleNo].addTime = tStamp;
    bacteria[bIdx].mCnt += mCnt;
    totalCnt += mCnt;

    timePQ.heapPush(bottleNo, curTime + bacteria[bIdx].mHalfTime);
    lifePQ.heapPush(bottleNo, (unsigned long long)mLife * 10000000 + tStamp);
    bottleNo++;
}

int takeOut(int tStamp, int mCnt)
{
    update(tStamp);

    int takeOutCnt = min(totalCnt, mCnt);
    int res = 0;

    while (takeOutCnt > 0) {
        auto data = lifePQ.heapPop();

        if (bottles[data.id].mCnt == 0) continue;
        if (bottles[data.id].mLife != data.value / 10000000) continue;

        int cnt = min(bottles[data.id].mCnt, takeOutCnt);
        takeOutCnt -= cnt;
        bottles[data.id].mCnt -= cnt;
        int bIdx = bottles[data.id].bIdx;
        bacteria[bIdx].mCnt -= cnt;

        res += cnt * bottles[data.id].mLife;

        if(bottles[data.id].mCnt > 0)
            lifePQ.heapPush(data.id, (unsigned long long)bottles[data.id].mLife * 10000000 + bottles[data.id].addTime);
    }

    return res;
}

int checkBacteria(int tStamp, char bName[MAX_NAME])
{
    update(tStamp);
    int bIdx = get_bacteriaIndex(bName);
    return bacteria[bIdx].mCnt;
}
#endif