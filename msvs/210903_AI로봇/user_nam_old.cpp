#if 0
#include <vector>
#include <set>
#include <queue>

using namespace std;

#define TRAINING	0
#define WORKING		1
#define BROKEN		2

struct Robot {
	//int rID;			// 로봇 ID (작은 정수)
	int IQ;				// IQ = trainTime = currentTime - totalWorkTime
	int wID;			// 작업 ID
	int workStartTime;	// 현재 작업 시작시간
	int totalWorkTime;	// 누적된 총 작업시간 totalWorkTime = currentTime - workStartTime
	int state;			// 트레이닝(default), 작업중, 고장
};

struct Work {
	//int wID;
	vector<int> robotList;
};

struct Data {
	int ID, value;	// IQ = currentTime - totalWorkTime
};

struct DoubleEndedPQ {
	struct MinData {
		int ID, value, idx;

		// 우선순위: value 작을수록, ID 작을수록
		bool operator<(const MinData& data) const {
			return (this->value > data.value) ||
				(this->value == data.value && this->ID > data.ID);
		}
	};
	struct MaxData {
		int ID, value, idx;

		// 우선순위: value1 클수록, ID 작을수록
		bool operator<(const MaxData& data) const {
			return (this->value < data.value) ||
				(this->value == data.value && this->ID > data.ID);
		}
	};

	priority_queue<MinData> minPQ;
	priority_queue<MaxData> maxPQ;

	vector<bool> isPoped;	// 원소별 pop 유/무 기록
	int cnt = 0;			// 저장된 원소 개수

	int size() {
		return cnt;
	}
	bool empty() {
		return cnt > 0;
	}
	void clear() {
		while (not minPQ.empty()) minPQ.pop();
		while (not maxPQ.empty()) maxPQ.pop();
		isPoped.clear();
		cnt = 0;
	}
	void push(const Data& data) {
		int idx = isPoped.size();
		minPQ.push({ data.ID, data.value, idx });
		maxPQ.push({ data.ID, data.value, idx });
		isPoped.push_back(false);
		idx += 1;
		cnt += 1;
	}
	Data popMin() {
		while (not minPQ.empty() && isPoped[minPQ.top().idx])
			minPQ.pop();
		auto data = minPQ.top(); minPQ.pop();
		isPoped[data.idx] = true;
		cnt -= 1;
		return Data{ data.ID, data.value };
	}
	Data popMax() {
		while (not maxPQ.empty() && isPoped[maxPQ.top().idx])
			maxPQ.pop();
		auto data = maxPQ.top(); maxPQ.pop();
		isPoped[data.idx] = true;
		cnt -= 1;
		return Data{ data.ID, data.value };
	}
};

// Main DB 작은 정수 ID (10 ≤ N ≤ 50,000)  1번부터 N번까지 서로 다른 번호
vector<Robot> robots;

// Sub DB 작은 ID (1 ≤ wID ≤ 50,000)
//vector<vector<int>> works;
vector<Work> works;
DoubleEndedPQ pq;

///////////////////////////////////////////////////////////
// 현재 시각은 0이다.
// 초기에 모든 로봇은 센터에 있고 지능지수는 0이다.
// 고장 난 로봇은 없다.
void init(int N)
{
	// Main DB 초기화
	robots.clear();
	robots.resize(N + 1);	// rID 1부터 N까지

	// Sub DB 초기화
	works.clear();
	works.resize(50001);	// wID 1부터 시작

	// 이중 우선순위 큐 초기화
	pq.clear();
	for (int rID = 1; rID <= N; rID++) {
		robots[rID].state = TRAINING;
		pq.push({ rID, robots[rID].totalWorkTime });
	}
}

int callJob(int cTime, int wID, int mNum, int mOpt)
{
	int result = 0;

	for (int i = 0; i < mNum; i++) {
		auto data = (mOpt == 0) ? pq.popMax() : pq.popMin();
		int rID = data.ID;

		if (robots[rID].state == TRAINING) {
			robots[rID].state = WORKING;
			robots[rID].wID = wID;
			robots[rID].workStartTime = cTime;
			works[wID].robotList.push_back(rID);	// <- 초기화
			result += rID;
		}
	}
	return result;
}

void returnJob(int cTime, int wID)
{
	for (auto& rID : works[wID].robotList) {
		if (robots[rID].state == WORKING && robots[rID].wID == wID) {
			robots[rID].state = TRAINING;
			robots[rID].totalWorkTime += cTime - robots[rID].workStartTime;
			pq.push({ rID, robots[rID].totalWorkTime });
		}
	}
}

void broken(int cTime, int rID)
{
	if (robots[rID].state == WORKING)
		robots[rID].state = BROKEN;
}

void repair(int cTime, int rID)
{
	if (robots[rID].state == BROKEN) {
		robots[rID].state = TRAINING;
		robots[rID].totalWorkTime = cTime;	// IQ = 0
		pq.push({ rID, robots[rID].totalWorkTime });
	}
}

int check(int cTime, int rID)
{
	if (robots[rID].state == BROKEN)
		return 0;
	else if (robots[rID].state == WORKING)
		return -robots[rID].wID;
	else
		return cTime - robots[rID].totalWorkTime;	// IQ
}
#endif