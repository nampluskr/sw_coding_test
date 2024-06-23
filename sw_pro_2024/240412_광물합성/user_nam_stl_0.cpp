#if 0
#include <vector>
using namespace std;

struct Result {
    int mCost;          // 광물 비용 ( 1 ≤ mCost ≤ 100,000 )
    int mContent;       // 광물내 성분 함유량 ( 1 ≤ mContent ≤ 1,000,000 )
};

int mShipFee;           // 운송 비용 ( 1 ≤ mShipFee ≤ 40,000 )
vector<Result> mineralList[2][3];  // mineralList[mMineId][mType]

bool decision(int content, int target, Result& res) {
    // update minCost[mMineId][mType]
    Result minCost[2][3];
    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 3; j++) {
            minCost[i][j] = { 1000000, 0 };
            for (const auto& mineral: mineralList[i][j])
                if (mineral.mContent <= content)
                    if (mineral.mCost < minCost[i][j].mCost)
                        minCost[i][j] = { mineral.mCost, mineral.mContent };
        }

    // update costs[mMineId]
    int costs[3];
    costs[0] = mShipFee + minCost[0][0] + minCost[0][1] + minCost[0][2];
    costs[1] = mShipFee + minCost[1][1] + minCost[1][1] + minCost[2][2];
    costs[2] = mShipFee * 2 + min(minCost[0][0], minCost[1][0])
                            + min(minCost[0][1], minCost[1][1])
                            + min(minCost[0][2], minCost[1][2]);
    int cost = min(min(costs[0], costs[1]), costs[2]);

    if (cost <= target) res = { cost, x };
    return cost <= target;
}

/////////////////////////////////////////////////////////////////////
void init(int mShipFee)
{
    ::mShipFee = mShipFee;
    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 3; j++)
            mineralList[i][j].clear();
}

// mMineId와 mType이 같은 광물의 남은 개수를 반환한다.
int gather(int mMineId, int mType, int mCost, int mContent)
{
    mineralList[mMineId][mType].push_back({ mCost, mContent });
    int res = mineralList[mMineId][mType].size();
    return res;
}

// 합성 물질을 만든 후 사용된 모든 광물은 제거된다.
Result mix(int mBudget)
{
    Result res = { 0, 0 };
    int lo = 1, hi = 1'000'000;
    while (lo <= hi) {
        int mid = (lo + hi) / 2;
        if (decision(mid, mBudget, res)) lo = mid + 1;
        else hi = mid - 1;
    }

    // 선택된 3개 광물 제거 처리
    // mineralList[0, 1][0] = { cost0, content0 }
    // mineralList[0, 1][1] = { cost1, content1 }
    // mineralList[0, 1][2] = { cost2, content2 }


    return res;
}
#endif