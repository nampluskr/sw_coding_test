// 256 ms
#if 1
#include <vector>
using namespace std;

inline int min(int a, int b) { return (a < b)? a: b; }

struct Result {
    int mPrice;                 // 부품 가격 ( 1 ≤ mPrice ≤ 100,000 )
    int mPerformance;           // 부품 성능 ( 1 ≤ mPerformance ≤ 1,000,000 )
};

int mCharge;                    // 창고 간 운송료 ( 1 ≤ mCharge ≤ 100,000 )
vector<Result> partList[3][2];  // partList[mType][mPosition]

// decision problem: given performance = x, return price <= target
bool decision(int x, int target, Result& res) {

    // update minPrice[mType][mPosition]
    int minPrice[3][2];
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 2; j++) {
            minPrice[i][j] = 1e6;
            for (const auto& part: partList[i][j])
                if (part.mPerformance >= x)
                    minPrice[i][j] = min(minPrice[i][j], part.mPrice);
        }

    // update price[mPosition]
    int prices[3];
    prices[0] = minPrice[0][0] + minPrice[1][0] + minPrice[2][0];
    prices[1] = minPrice[0][1] + minPrice[1][1] + minPrice[2][1];
    prices[2] = mCharge + min(minPrice[0][0], minPrice[0][1])
                        + min(minPrice[1][0], minPrice[1][1])
                        + min(minPrice[2][0], minPrice[2][1]);
    int price = min(min(prices[0], prices[1]), prices[2]);

    if (price <= target) res = { price, x };
    return price <= target;
}

/////////////////////////////////////////////////////////////////////
void init(int mCharge)
{
    ::mCharge = mCharge;
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 2; j++)
            partList[i][j].clear();
}

int stock(int mType, int mPrice, int mPerformance, int mPosition)
{
    partList[mType][mPosition].push_back({ mPrice,mPerformance });
    int res = partList[mType][mPosition].size();
    return res;
}

// parametric search
Result order(int mBudget)
{
    Result res = { 0, 0 };
    int lo = 1, hi = 1'000'000;

    while (lo <= hi) {
        int mid = (lo + hi) / 2;
        if (decision(mid, mBudget, res)) lo = mid + 1;
        else hi = mid - 1;
    }
    return res;
}
#endif