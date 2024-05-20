// WS 559 ms
#include <unordered_map>
#include <vector>
//#include <set>
#include <algorithm>
using namespace std;

#define MAX_SAMPLES     20'001  // addSamples()
#define MAX_CATEGORIES  11      // mC: 자료의 범주 [1, 10]
#define MAX_XY          4'001   // mX, mY: [1, 4,000]
#define MAX_BUCKETS     1'002   // ceil(4001 / 4) + 1
#define DELETED         1

int K;      // 최근접 이웃의 개수 [3, 11]
int L;      // 이상치를 판별하기 위한 길이 [4, 100]

struct Sample {
    int mX, mY, mC;
    int state;
} samples[MAX_SAMPLES];
int sampleCnt;
unordered_map<int, int> sampleMap;

vector<int> buckets[MAX_BUCKETS][MAX_BUCKETS];
int bucketCnt;      // ceil(MAX_XY / L)

struct Data {
    int dist, mX, mY, mC;

    bool operator<(const Data& data) const {
        if (dist != data.dist) return dist < data.dist;
        if (mX != data.mX) return mX < data.mX;
        return mY < data.mY;
    }
};


/////////////////////////////////////////////////////////////////////
void init(int K, int L)
{
    for (int i = 0; i < sampleCnt; i++) samples[i].state = 0;

    sampleCnt = 0;
    sampleMap.clear();
    ::K = K;
    ::L = L;
    bucketCnt = ceil((double)MAX_XY / L);

    for (int i = 0; i <= bucketCnt; i++)
        for (int j = 0; j <= bucketCnt; j++)
            buckets[i][j].clear();
}

void addSample(int mID, int mX, int mY, int mC)
{
    int sIdx = sampleMap[mID] = sampleCnt++;
    samples[sIdx].mX = mX;
    samples[sIdx].mY = mY;
    samples[sIdx].mC = mC;
    buckets[mX / L][mY / L].push_back(sIdx);
}

void deleteSample(int mID)
{
    int sIdx = sampleMap[mID];
    samples[sIdx].state = DELETED;
}

int predict(int mX, int mY)
{
    // 탐색 그룹 범위 설정
    int sX = max(0, (mX - L) / L);
    int sY = max(0, (mY - L) / L);
    int eX = min(bucketCnt - 1, (mX + L) / L);
    int eY = min(bucketCnt - 1, (mY + L) / L);

    // 거리 L 이하인 자료 후보 선정
    vector<Data> selected;
    for (int i = sX; i <= eX; i++)
        for (int j = sY; j <= eY; j++)
            for (int sIdx : buckets[i][j]) {
                auto& sample = samples[sIdx];
                int dist = abs(sample.mX - mX) + abs(sample.mY - mY);
                if (sample.state == DELETED) continue;
                if (dist > L) continue;
                selected.push_back({ dist, sample.mX, sample.mY, sample.mC });
            }

    // 후보가 K개가 안되면 이상치
    if (selected.size() < K) return -1;

    // K-최근접 이웃 선정
    sort(selected.begin(), selected.end());

    // 범주 추정
    int categories[MAX_CATEGORIES] = {};
    for (int i = 0; i < K; i++)
        categories[selected[i].mC]++;
    return max_element(categories + 1, categories + K) - categories;
}