#if 1
/*
- 문자열 파싱: 정수 계수 / 정수 지수
- 매번 각 항의 지수들까지 계산하는 것은 비효율적 => LUT (Lookup Table)
- (0 ≤ mDegreeX, mDegreeY ≤ 1,000), (-4 ≤ mX, mY ≤ 4)
- 나머지 연산: 결과 0 ~ 9999 (M = 10'000으로 나눈 나머지): (A op B + M) % M
*/
#include<unordered_map>
#include<string>
using namespace std;

#define MOD         10'000
#define BASE        1001
#define MAX_POLY    10'0000     // assign() 1000 + compute() 9000

struct Polynomial {
    unordered_map<int, int> coeffMap;
} polynomials[MAX_POLY];
unordered_map<string, int> polyMap;
int polyCnt;

unordered_map<int, int>& get_polynomial(const char mName[]) {
    int pIdx;
    auto iter = polyMap.find(mName);
    if (iter == polyMap.end()) {
        pIdx = polyCnt++;
        polyMap.emplace(mName, pIdx);
    }
    else { pIdx = iter->second; }
    return polynomials[pIdx].coeffMap;
}

int LUT[9][1001]; 
void add_value(int& res, int value) { res = (res + value + MOD) % MOD; }
static int tc;

/////////////////////////////////////////////////////////////////////
void init()
{
    polyMap.clear();
    for (int i = 0; i < MAX_POLY; i++) 
        polynomials[i].coeffMap.clear();
    polyCnt = 0;

    if (tc++ > 0) return;
    for (int i = 0; i <= 8; i++) {
        LUT[i][0] = 1;
        for (int j = 1; j <= 1000; j++)
            LUT[i][j] = (LUT[i][j - 1] * (i - 4) % MOD + MOD) % MOD;
    }
}

void assign(char mName[], char mPoly[])
{
    auto& poly = get_polynomial(mName);

    // init
    int mode = 0;                       // 계수: 0, X 지수: 1, Y 지수: 2
    int coeff = 1, degX = 0, degY = 0;  // coeff: 계수, degX: X 지수, degY: Y 지수

    for (int i = 0; mPoly[i]; i++) {
        if (mPoly[i] >= '0' && mPoly[i] <= '9') {
            int num = mPoly[i] - '0';
            if (mode == 0) coeff *= num;
            else if (mode == 1) degX *= num;
            else degY *= num;
        }
        else if (mPoly[i] == 'X') mode = 1, degX = 1;
        else if (mPoly[i] == 'Y') mode = 2, degY = 1;
        else if (mPoly[i] == '^') continue;
        else { // '+' or '-'
            add_value(poly[degX * BASE + degY], coeff);
            // reset
            mode = degX = degY = 0;
            coeff = (mPoly[i] == '+' ? 1 : -1);
        }
    }
    add_value(poly[degX * BASE + degY], coeff);
}

void compute(char mNameR[], char mNameA[], char mNameB[], int mOp)
{
    auto& polyA = get_polynomial(mNameA);
    auto& polyB = get_polynomial(mNameB);
    auto& polyR = get_polynomial(mNameR);

    if (mOp == 0) {
        for (const auto& a : polyA)
            add_value(polyR[a.first], a.second);
        for (const auto& b : polyB)
            add_value(polyR[b.first], b.second);
    } else if (mOp == 1) {
        for (const auto& a : polyA)
            add_value(polyR[a.first], a.second);
        for (const auto& b : polyB)
            add_value(polyR[b.first], -b.second);
    } else {
        for (const auto& a : polyA)
            for (const auto& b : polyB)
                add_value(polyR[a.first + b.first], a.second * b.second);
    }
}

int getCoefficient(char mName[], int mDegreeX, int mDegreeY)
{
    auto& poly = get_polynomial(mName); 
    return poly[mDegreeX * BASE + mDegreeY];
} 

int calcPolynomial(char mName[], int mX, int mY)
{
    auto& poly = get_polynomial(mName);
    int res = 0;
    for (const auto& p : poly) {
        int coeff = p.second;
        int degX = p.first / BASE;
        int degY = p.first % BASE;
        int value = (long long)coeff * LUT[mX + 4][degX] * LUT[mY + 4][degY] % MOD;
        res = (res + value + MOD) % MOD;
    }
    return res;
}
#endif
