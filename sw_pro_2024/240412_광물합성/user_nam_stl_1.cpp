#include <vector>
using namespace std;

struct Result {
    int mCost;
    int mContent;
};

int mShipFee;
vector<Result> mineralList[2][3];   // mineralList[mMindId][mType]

bool decision(int x, int target) {
    return false;
}

/////////////////////////////////////////////////////////////
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
    return mineralList[mMineId][mType].size();
}

Result mix(int mBudget)
{
    Result res = { 0, 0 };
    int lo = 1, hi = 1'000'000;
    while (lo <= hi) {
        int mid = (lo + hi) / 2;
        if (decision(mid, mBudget)) lo = mid + 1;
        else hi = mid - 1;
    }

    return res;
}