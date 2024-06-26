#if 1
#include <vector>
using namespace std;

struct Result {
	int mCost;
	int mContent;
};

#define REMOVED		1

int mShipFee;
struct Data { int mCost, mContent, state; };
vector<Data> mineralList[2][3];		// mineralList[mMineId][mType]

// decision problem
bool decision(int content, int mBudget, Result& res, int resMineId[], int resIdx[]) {

	// 최소 비용 갱신
	struct Cost {
		int value, idx;
	} minCost[2][3];

	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 3; j++) {
			minCost[i][j].value = 1e6;
			int idx = 0;
			for (const auto& mineral : mineralList[i][j]) {
				if (mineral.state != REMOVED && content <= mineral.mContent)
					if (mineral.mCost < minCost[i][j].value)
						minCost[i][j] = { mineral.mCost, idx };
				idx++;
			}
		}
	}
	int cost0 = minCost[0][0].value + minCost[0][1].value + minCost[0][2].value + mShipFee;
	int cost1 = minCost[1][0].value + minCost[1][1].value + minCost[1][2].value + mShipFee;
	int cost2 = min(minCost[0][0].value, minCost[1][0].value) +
				min(minCost[0][1].value, minCost[1][1].value) +
				min(minCost[0][2].value, minCost[1][2].value) + mShipFee * 2;
	int resCost = min(min(cost0, cost1), cost2);

	// 선택된 광물 정보 기록
	if (resCost <= mBudget) {
		res = { resCost, content };

		if (cost0 == resCost)
			for (int mType = 0; mType < 3; mType++) {
				resMineId[mType] = 0; resIdx[mType] = minCost[0][mType].idx;
			}
		else if (cost1 == resCost)
			for (int mType = 0; mType < 3; mType++) {
				resMineId[mType] = 1; resIdx[mType] = minCost[1][mType].idx;
			}
		else
			for (int mType = 0; mType < 3; mType++) {
				if (minCost[0][mType].value < minCost[1][mType].value) {
					resMineId[mType] = 0; resIdx[mType] = minCost[0][mType].idx;
				}
				else {
					resMineId[mType] = 1; resIdx[mType] = minCost[1][mType].idx;
				}
			}
	}
	return resCost <= mBudget;
}

///////////////////////////////////////////////////////////////////
void init(int mShipFee)
{
	::mShipFee = mShipFee;

	for (int i = 0; i < 2; i++)
		for (int j = 0; j < 3; j++)
			mineralList[i][j].clear();
}

int gather(int mMineId, int mType, int mCost, int mContent)
{
	mineralList[mMineId][mType].push_back({ mCost, mContent });

	int res = 0;
	for (const auto& mineral : mineralList[mMineId][mType])
		if (mineral.state != REMOVED) res++;
	return res;
}

Result mix(int mBudget)
{
	Result res = { 0, 0 };
	int resMineId[3], resIdx[3];	// resMineId[mType], resIdx[mType]
	int lo = 1, hi = 1000000;		// 1 ≤ mContent ≤ 1,000,000

	while (lo <= hi) {
		int mid = (lo + hi) / 2;

		if (decision(mid, mBudget, res, resMineId, resIdx)) lo = mid + 1;
		else hi = mid - 1;
	}

	// 최종 선택된 광물 삭제 처리
	if (res.mCost && res.mContent)	// res != { 0, 0 }
		for (int mType = 0; mType < 3; mType++) {
			int mMineId = resMineId[mType];
			int idx = resIdx[mType];
			mineralList[mMineId][mType][idx].state = REMOVED;
		}
	return res;
}
#endif
