#if 1
#include <vector>
#include <unordered_map>
#include <queue>
using namespace std;

#define MAX_SAMPLES 20000	// addSample() 함수의 호출 횟수 20,000 이하
#define MAX_N		4000	// 자료의 x축, y축 위치 (1 ≤ mX, mY ≤ 4,000)
#define MAX_TYPE	10		// 자료의 범주 (1 ≤ mC ≤ 10)
#define MAX_BUCKETS	200		// 파티션 전체 개수

#define ADDED		0
#define REMOVED		1

int N = MAX_N;				// max x축, y축
int S = sqrt(MAX_N + 1);	// bucket size => max bucket index = N / S

struct Sample {
	int mX, mY, mC, state;
} samples[MAX_SAMPLES];
int sampleCnt;
unordered_map<int, int> sampleMap;
vector<int> sampleList[MAX_BUCKETS][MAX_BUCKETS];

struct Data {
	int dist, mX, mY, sIdx;
	bool operator<(const Data & data) const {
		if (dist != data.dist) return dist > data.dist;
		if (mX != data.mX) return mX > data.mX;
		return mY > data.mY;
	}
};

int K;		// 근접 이웃 개수
int L;		// 아웃라이어 범위

////////////////////////////////////////////////////////////////////
void init(int K, int L)
{
	::K = K;
	::L = L;
	//for (int i = 0; i < sampleCnt; i++) { samples[i] = {}; }
	sampleCnt = 0;
	sampleMap.clear();

	for (int i = 0; i <= N / S; i++)
		for (int j = 0; j <= N / S; j++)
			sampleList[i][j] = {};
}

void addSample(int mID, int mX, int mY, int mC)
{
	int sIdx = sampleCnt++;
	sampleMap.emplace(mID, sIdx);
	samples[sIdx] = { mX, mY, mC, ADDED };
	sampleList[mX / S][mY / S].push_back(sIdx);
}

void deleteSample(int mID)
{
	int sIdx = sampleMap[mID];
	samples[sIdx].state = REMOVED;
}

int predict(int mX, int mY)
{
	int sX = max((mX - L) / S, 0);
	int sY = max((mY - L) / S, 0);
	int eX = min((mX + L) / S, N / S);
	int eY = min((mY + L) / S, N / S);

	priority_queue<Data> Q;
	for (int i = sX; i <= eX; i++)
		for (int j = sY; j <= eY; j++)
			for (int sIdx : sampleList[i][j]) {
				auto& sample = samples[sIdx];
				int dist = abs(sample.mX - mX) + abs(sample.mY - mY);
				if (sample.state == REMOVED) continue;
				if (dist > L) continue;
				Q.push({ dist, sample.mX, sample.mY, sIdx });
			}
	if (Q.size() < K) return -1;

	// KNN 선택
	int cnt = K;
	int selected[MAX_TYPE + 1] = {};
	while (!Q.empty() && cnt--) {
		auto data = Q.top(); Q.pop();
		selected[samples[data.sIdx].mC]++;
	}

	// 범주 판정
	int res = 1;
	for (int mC = 2; mC <= MAX_TYPE; mC++)
		if (selected[mC] > selected[res]) res = mC;
		else if (selected[mC] == selected[res] && mC < res) res = mC;
	return res;
}
#endif