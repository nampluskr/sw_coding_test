#if 1
#include <vector>
#include <queue>
using namespace std;
using ull = unsigned long long;

#define MAX_HEAP        1'000'000
#define MAX_SIZE        1'000'000
#define MAX_ROBOT       50'010
#define MAX_WORK        50'010
#define MAX_WORKTIME    1'000'000'000

struct Robot {
    int lastCallTime;      // 마지막으로 일하러 간 시간,
    int wID;               // wID가 0이면 고장, -1이면 트레이닝센터
    int workTime;          // 일하러 간 시간의 총합 //IQ = ctime - workTime
} robot[MAX_ROBOT];

vector<int> robotList[MAX_WORK];

struct Data {
    int rID, workTime;      // IQ => MAX_WORKTIME - workTime (IQ 는 workTime 에 반비례)
    
    // maxHeap: IQ 클수록, rID 작을수록
    bool operator<(const Data& data) const { 
        return (workTime > data.workTime) || (workTime == data.workTime && rID > data.rID);
    }
    // minHeap: IQ 작을수록, rID 작을수록
    bool operator>(const Data& data) const { 
        return (workTime < data.workTime) || (workTime == data.workTime && rID > data.rID);
    }
};
priority_queue<Data, vector<Data>, less<Data>> maxHeap;
priority_queue<Data, vector<Data>, greater<Data>> minHeap;

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

    for (int i = 0; i < MAX_WORK; i++) {
        robotList[i].clear();
    }
    for (int rID = 1; rID <= N; rID++) {
        robot[rID].workTime = 0;
        robot[rID].wID = -1;

        maxHeap.push({ rID, robot[rID].workTime });
        minHeap.push({ rID, robot[rID].workTime });
    }
}

int callJob(int cTime, int wID, int mNum, int mOpt)
{
    Data data;
    int cnt = 0;
    int res = 0;

    while (cnt < mNum && !maxHeap.empty() && !minHeap.empty()) {
        if (mOpt == 0) { data = maxHeap.top(); maxHeap.pop(); }
        if (mOpt == 1) { data = minHeap.top(); minHeap.pop(); }
        int rID = data.rID;

        //팝데이터의 정합성이 일치하면 로봇을 wID에 할당
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

            maxHeap.push({ rID, robot[rID].workTime });
            minHeap.push({ rID, robot[rID].workTime });
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

        maxHeap.push({ rID, robot[rID].workTime });
        minHeap.push({ rID, robot[rID].workTime });
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