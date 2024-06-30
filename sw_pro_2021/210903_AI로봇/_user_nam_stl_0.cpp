#if 0
#include <vector>
#include <unordered_map>
#include <queue>
using namespace std;

#define MAX_ROBOTS  50'001
#define MAX_TASKS   50'001  // wID : 작업 ID (1 ≤ wID ≤ 50,000)

#define TRAINING    0
#define WORKING     1
#define BROKEN      2

int N;          // N : 로봇의 대수 (10 ≤ N ≤ 50,000)

struct Robot {
    int IQ, wID, startTime, totalTime, state;
} robots[MAX_ROBOTS];

vector<int> robotList[MAX_TASKS];   // robotList[wID] = { rID, ... }

struct Data {
    int IQ, rID;
    // 높은 지능 우선 방식
    bool operator<(const Data& data) const {
        return (IQ < data.IQ) || (IQ == data.IQ && rID > data.rID);
    }
    // 낮은 지능 우선 방식
    bool operator>(const Data& data) const {
        return (IQ > data.IQ) || (IQ == data.IQ && rID > data.rID);
    }
};

template<typename T>
struct DoubleEnededPQ {
    struct Pair {
        T data; int idx;
        bool operator<(const Pair& p) const { return data < p.data; }
        bool operator>(const Pair& p) const { return data > p.data; }
    };
    priority_queue<Pair, vector<Pair>, less<Pair>> maxHeap;
    priority_queue<Pair, vector<Pair>, greater<Pair>> minHeap;
    vector<bool> popped;
    int idx = 0;

    void clear() {
        maxHeap = {};
        minHeap = {};
        popped.clear();
        idx = 0;
    }
    void push(const Data& data) {
        maxHeap.push({ data, idx });
        minHeap.push({ data, idx });
        popped.push_back(false);
        idx++;
    }
    void popTop() {
        while (!maxHeap.empty() && popped[maxHeap.top().idx]) maxHeap.pop();
        auto top = maxHeap.top(); maxHeap.pop();
        popped[top.idx] = true;
    }
    void popBottom() {
        while (!minHeap.empty() && popped[minHeap.top().idx]) minHeap.pop();
        auto bot = minHeap.top(); minHeap.pop();
        popped[bot.idx] = true;
    }
    T top() {
        while (!maxHeap.empty() && popped[maxHeap.top().idx]) maxHeap.pop();
        return maxHeap.top().data;
    }
    T bottom() {
        while (!minHeap.empty() && popped[minHeap.top().idx]) minHeap.pop();
        return minHeap.top().data;
    }
    bool empty() { return idx == 0;}
};
DoubleEnededPQ<Data> depq;


/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
void init(int N)
{
    depq.clear();

    ::N = N;
    for (int rID = 1; rID <= N; rID++) {
        robots[rID] = { 0, 0, 0, TRAINING };    // { IQ, wID, startTime, totalTime, state }
        depq.push({ robots[rID].totalTime, rID });                  // { IQ, rID }
    }
}

// 현재 시각이 cTime이다. ID가 wID인 작업에 mNum대의 로봇을 투입한다.
// 해당 작업에 투입된 로봇들의 고유 번호를 모두 합한 값을 반환한다.
// 고장 나거나 작업 중인 로봇은 해당 작업에 투입되지 않는다.
// 각 테스트 케이스에서 wID는 해당 함수가 처음 호출할 때 1이고 호출될 때마다 1씩 증가한다.
int callJob(int cTime, int wID, int mNum, int mOpt)
{
    int res = 0;
    while (!depq.empty() && mNum--) {
        Data data;
        if (mOpt == 0) { data = depq.top(); depq.popTop(); }
        else { data = depq.bottom(); depq.popBottom(); }
        int rID = data.rID;

        if (robots[rID].state == TRAINING) {
            robots[rID].state = WORKING;
            robots[rID].wID = wID;
            robots[rID].startTime = cTime;

            robotList[wID].push_back(rID);
            res += rID;
        }
    }
    return res;
}

// ID가 wID인 작업에 투입된 로봇들이 센터로 복귀하고 트레이닝을 바로 시작한다.
void returnJob(int cTime, int wID)
{
	for (int rID : robotList[wID]) {
		if (robots[rID].state == WORKING && robots[rID].wID == wID) {
			robots[rID].state = TRAINING;
			robots[rID].totalTime += cTime - robots[rID].startTime;
			depq.push({ robots[rID].totalTime, rID });
		}
	}
}

// 작업 중이고 고유 번호가 rID인 로봇이 고장 난다.
void broken(int cTime, int rID)
{
    if (robots[rID].state == WORKING)
        robots[rID].state = BROKEN;
}

// 번호가 rID인 로봇의 수리가 완료된다.
// 수리가 완료된 로봇은 지능지수가 0이 되고 트레이닝을 시작한다.
void repair(int cTime, int rID)
{
    if (robots[rID].state == BROKEN) {
		robots[rID].state = TRAINING;
		robots[rID].totalTime = cTime;	// IQ = 0
		depq.push({ robots[rID].totalTime, rID });
	}
}

int check(int cTime, int rID)
{
    int res = cTime - robots[rID].totalTime;	// IQ
	if (robots[rID].state == BROKEN)
		res = 0;
	else if (robots[rID].state == WORKING)
		res = -robots[rID].wID;
    return res;
}
#endif