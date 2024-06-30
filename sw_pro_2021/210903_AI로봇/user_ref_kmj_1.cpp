#if 0
#include <vector>
#include <queue>
using namespace std;
using ull = unsigned long long;

#define MAX_HEAP        1'000'000
#define MAX_SIZE        1'000'000
#define MAX_ROBOT       50'010
#define MAX_WORK        50'010
#define MAX_WORKTIME    1'000'000'000

struct Data {
    int rID;
    int workTime;
    ull value;

    bool operator<(const Data& data) const { return value < data.value; }
    bool operator>(const Data& data) const { return value > data.value; }
};

struct MinHeap {
    Data heap[MAX_SIZE];
    int heapSize = 0;

    bool empty() { return heapSize == 0; }
    void clear(void) { heapSize = 0; }

    void push(const Data& data)
    {
        heap[heapSize] = data;
        int current = heapSize;

        while (current > 0 && heap[current] < heap[(current - 1) / 2])
        {
            Data temp = heap[(current - 1) / 2];
            heap[(current - 1) / 2] = heap[current];
            heap[current] = temp;
            current = (current - 1) / 2;
        }
        heapSize = heapSize + 1;
    }

    Data top() { return heap[0]; }
    void pop()
    {
        heapSize = heapSize - 1;
        heap[0] = heap[heapSize];
        int current = 0;

        while (current * 2 + 1 < heapSize)
        {
            int child;
            if (current * 2 + 2 == heapSize)
            {
                child = current * 2 + 1;
            }
            else
            {
                child = heap[current * 2 + 1] < heap[current * 2 + 2] ? current * 2 + 1 : current * 2 + 2;
            }
            if (heap[current] < heap[child])
            {
                break;
            }
            Data temp = heap[current];
            heap[current] = heap[child];
            heap[child] = temp;
            current = child;
        }
    }
};

struct MaxHeap {
    Data heap[MAX_SIZE];
    int heapSize = 0;

    bool empty() { return heapSize == 0; }
    void clear(void) { heapSize = 0; }

    void push(const Data& data)
    {
        heap[heapSize] = data;
        int current = heapSize;

        while (current > 0 && heap[current] > heap[(current - 1) / 2])
        {
            Data temp = heap[(current - 1) / 2];
            heap[(current - 1) / 2] = heap[current];
            heap[current] = temp;
            current = (current - 1) / 2;
        }
        heapSize = heapSize + 1;
    }

    Data top() { return heap[0]; }
    void pop()
    {
        heapSize = heapSize - 1;
        heap[0] = heap[heapSize];
        int current = 0;

        while (current * 2 + 1 < heapSize)
        {
            int child;
            if (current * 2 + 2 == heapSize)
            {
                child = current * 2 + 1;
            }
            else
            {
                child = heap[current * 2 + 1] > heap[current * 2 + 2] ? current * 2 + 1 : current * 2 + 2;
            }

            if (heap[current] > heap[child])
            {
                break;
            }
            Data temp = heap[current];
            heap[current] = heap[child];
            heap[child] = temp;
            current = child;
        }
    }
};

// MaxHeap maxHeap;
// MinHeap minHeap;

priority_queue<Data, vector<Data>, less<Data>> maxHeap;
priority_queue<Data, vector<Data>, greater<Data>> minHeap;

struct Robot {
    int lastCallTime;      // 마지막으로 일하러 간 시간,
    int wID;               // wID가 0이면 고장, -1이면 트레이닝센터
    int workTime;          // 일하러 간 시간의 총합 //IQ = ctime - workTime
} robot[MAX_ROBOT];

vector<int> robotList[MAX_WORK];

//계속 변하는 값을 우큐에 넣으면 안된다 (이 문제의 중요한 포인트)
//IQ는 트레이닝 센터에 있는 한 계속 증가한다
//IQ(변경) = ctime(변경) - sum(workTime)(트레이닝센터에 있는 애들 한해 고정)

//우큐에서 뽑아내야 하는 대상은 트레이닝 센터에 있는 애들
//우큐에 값이 들어가야 하는 상황은 고장난 애들 또는 일하고 복귀한 애들

/////////////////////////////////////////////////////////////////////
void init(int N)
{
    minHeap = {};
    maxHeap = {};

    // maxHeap.clear();
    // minHeap.clear();
    ull value = 0;

    for (int i = 0; i < MAX_WORK; i++) {
        robotList[i].clear();
    }
    for (int rID = 1; rID <= N; rID++) {
        robot[rID].workTime = 0;
        robot[rID].wID = -1;

        // maxHeap: value가 클수록
        value = (MAX_WORKTIME - robot[rID].workTime) * (ull)MAX_ROBOT + (ull)(MAX_ROBOT - rID);
        maxHeap.push({ rID, robot[rID].workTime, value });

        // minHeap: value가 작을수록
        value = (MAX_WORKTIME - robot[rID].workTime) * (ull)MAX_ROBOT + (ull)rID;
        minHeap.push({ rID, robot[rID].workTime, value });
    }
}

int callJob(int cTime, int wID, int mNum, int mOpt)
{
    int cnt = 0;
    Data data;
    int res = 0;

    while (cnt < mNum && !maxHeap.empty() && !minHeap.empty()) {
        if (mOpt == 0) { data = maxHeap.top(); maxHeap.pop(); }
        if (mOpt == 1) { data = minHeap.top(); minHeap.pop(); }

        //팝데이터의 정합성이 일치하면 로봇을 wID에 할당
        int rID = data.rID;
        if (robot[rID].wID == -1 && robot[rID].workTime == data.workTime) {
            robot[rID].wID = wID;
            robot[rID].lastCallTime = cTime;
            robotList[wID].push_back(rID);
            res += rID;
            cnt++;
        }
    }
    return res;
}

void returnJob(int cTime, int wID)
{
    ull value = 0;

    for (int rID: robotList[wID]) {
        if (robot[rID].wID == wID) {
            robot[rID].workTime += (cTime - robot[rID].lastCallTime);
            robot[rID].wID = -1;

            value = (MAX_WORKTIME - robot[rID].workTime) * (ull)MAX_ROBOT + (ull)(MAX_ROBOT - rID);
            maxHeap.push({ rID, robot[rID].workTime, value });

            value = (MAX_WORKTIME - robot[rID].workTime) * (ull)MAX_ROBOT + (ull)rID;
            minHeap.push({ rID, robot[rID].workTime, value });
        }
    }
}

void broken(int cTime, int rID)
{
    if (robot[rID].wID > 0)
        robot[rID].wID = 0;
}

void repair(int cTime, int rID)
{
    ull value = 0;

    if (robot[rID].wID == 0)
    {
        robot[rID].workTime = cTime;
        robot[rID].wID = -1;

        value = (MAX_WORKTIME - robot[rID].workTime) * (ull)MAX_ROBOT + (ull)(MAX_ROBOT - rID);
        maxHeap.push({ rID, robot[rID].workTime, value });

        value = (MAX_WORKTIME - robot[rID].workTime) * (ull)MAX_ROBOT + (ull)rID;
        minHeap.push({ rID, robot[rID].workTime, value });
 }
}

int check(int cTime, int rID)
{
    if (robot[rID].wID == 0)        // BROKEN
        return 0;
    else if (robot[rID].wID > 0)    // WORKING
        return -robot[rID].wID;
    else
        return cTime - robot[rID].workTime;
}
#endif