#if 1   // 530 ms (STL)
#include <vector>
#include <queue>
#include <unordered_map>
#include <cmath>

#define MAX_SAMPLES 200'000     // addSample() 함수의 호출 횟수는 20,000 이하
#define MAX_X       4'000       // mX : x축 위치 (1 ≤ mX ≤ 4,000)
#define MAX_Y       4'000       // mY : y축 위치 (1 ≤ mY ≤ 4,000)
#define MAX_COLORS  10		    // 자료의 범주 (1 ≤ mC ≤ 10)

#define BUCKET_SIZE 63          // 파티션 최대 크기 (int)sqrt(MAX_X + 1)
#define MAX_BUCKETS	64		    // 파티션 최대 개수 ceil(MAX_X / BUCKET_SIZE)

enum State { DELETED, ADDED };

struct Sample {
    int mX, mY, mC;
    State state;
} samples[MAX_SAMPLES];

int sampleCnt; 
std::unordered_map<int, int> sampleMap;
std::vector<int> sampleList[MAX_BUCKETS][MAX_BUCKETS];

int K;                      // K : 최근접 이웃의 개수 (3 ≤ K ≤ 11)
int L;                      // L : 이상치를 판별하기 위한 길이 (4 ≤ L ≤ 100)
int bucketSize = BUCKET_SIZE;

struct Data {
	int dist, mX, mY, sIdx;

	bool operator<(const Data & other) const {
        return (dist > other.dist) || 
               (dist == other.dist && mX > other.mX) ||
               (dist == other.dist && mX == other.mX && mY > other.mY);
	}
};

//////////////////////////////////////////////////////////////////////
void init(int K, int L)
{
    ::K = K, ::L = L;
    sampleCnt = 0;
    sampleMap.clear();

    for (int i = 0; i < MAX_X / bucketSize; i++)
        for (int j = 0; j < MAX_Y / bucketSize; j++)
            sampleList[i][j].clear();
}

void addSample(int mID, int mX, int mY, int mC)
{
    int sIdx = sampleCnt++;
    sampleMap.emplace(mID, sIdx);
    samples[sIdx] = { mX, mY, mC, ADDED };
    sampleList[mX / bucketSize][mY / bucketSize].push_back(sIdx);
}

void deleteSample(int mID)
{
    int sIdx = sampleMap[mID];
    samples[sIdx].state = DELETED;
}

int predict(int mX, int mY)
{
    int sX = std::max((mX - L) / bucketSize, 0);
    int sY = std::max((mY - L) / bucketSize, 0);
    int eX = std::min((mX + L) / bucketSize, MAX_X / bucketSize);
    int eY = std::min((mY + L) / bucketSize, MAX_Y / bucketSize);

    std::priority_queue<Data> pq;
    for (int i = sX; i <= eX; i++)
        for (int j = sY; j <= eY; j++)
            for (int sIdx : sampleList[i][j]) {
                auto& sample = samples[sIdx];
                int dist = abs(sample.mX - mX) + abs(sample.mY - mY);
                if (sample.state == DELETED) continue;
                if (dist > L) continue;
                pq.push({ dist, sample.mX, sample.mY, sIdx });
            }
    if (pq.size() < K) return -1;

    int cnt = K;
    int colors[MAX_COLORS + 1] = {};
    while (!pq.empty() && cnt--) {
        auto data = pq.top(); pq.pop();
        colors[samples[data.sIdx].mC]++;
    }

    // 범주 판정
    int res = 1;
    for (int mC = 2; mC <= MAX_COLORS; mC++) {
        if (colors[mC] > colors[res]) res = mC;
        else if (colors[mC] == colors[res] && mC < res) res = mC;
    }
    return res;
}
#endif