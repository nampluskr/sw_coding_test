#include<unordered_map>
#include<string>
using namespace std;

#define MOD     10'000      // 나머지 연산: 결과 0 ~ 9999 (M = 10'000으로 나눈 나머지)
#define BASE    1001        // (0 ≤ mDegreeX, mDegreeY ≤ 1,000) key = mDegreeX * 1001 + mDegreeY

// { name, { X지수*1001 + Y지수, 계수 } }
unordered_map<string, unordered_map<int, int>> poly;

// (0 ≤ mDegreeX, mDegreeY ≤ 1,000), (-4 ≤ mX, mY ≤ 4)
// (mX)^mDegreeX = LUT[mX + 4][mDegreeX] 
// (mY)^mDegreeY = LUT[mY + 4][mDegreeY]
int LUT[10][1003];

// 계수 계산 (0 ~ 9999)
void cal_coeff(int& coeff, int x) { coeff = (coeff + x + MOD) % MOD; }

static int tc;

/////////////////////////////////////////////////////////////////////
void init()
{
    poly.clear();
    if (tc++ == 0) {
        for (int i = 0; i <= 8; i++) {
            LUT[i][0] = 1;
            for (int j = 1; j <= 1000; j++)
                LUT[i][j] = (LUT[i][j - 1] * (i - 4) % MOD + MOD) % MOD;
        }
    }
}

void assign(char mName[], char mPoly[])
{
    int coeff = 1, degX = 0, degY = 0;  // coeff: 계수, degX: X 지수, degY: Y 지수
    int type = 0;                        // 계수: 0, X 지수: 1, Y 지수: 2
    auto& P = poly[mName];

    for (int i = 0; mPoly[i]; i++) {
        if (mPoly[i] == '+' || mPoly[i] == '-') {
            cal_coeff(P[degX * BASE + degY], coeff);
            type = degX = degY = 0;
            coeff = (mPoly[i] == '+' ? 1 : -1);
        }
        else if (mPoly[i] == 'X') type = 1, degX = 1;
        else if (mPoly[i] == 'Y') type = 2, degY = 1;
        else if (mPoly[i] == '^') continue;
        else {      // 한자리 수
            int num = mPoly[i] - '0';
            if (type == 0) coeff *= num;
            else if (type == 1) degX *= num;
            else degY *= num;
        }
    }
    cal_coeff(P[degX * BASE + degY], coeff);
}

void compute(char mNameR[], char mNameA[], char mNameB[], int mOp)
{
    auto& R = poly[mNameR];

    if (mOp < 2) {
        for (auto p : poly[mNameA]) cal_coeff(R[p.first], p.second);
        for (auto p : poly[mNameB]) cal_coeff(R[p.first], mOp ? -p.second : p.second);
    }
    else {
        for (auto p : poly[mNameA])
            for (auto q : poly[mNameB])
                cal_coeff(R[p.first + q.first], p.second * q.second);
    }
}

int getCoefficient(char mName[], int mDegreeX, int mDegreeY)
{
    return poly[mName][mDegreeX * BASE + mDegreeY];
}

int calcPolynomial(char mName[], int mX, int mY)
{
    int res = 0;
    for (const auto& p : poly[mName]) {
        int degX = p.first / BASE;
        int degY = p.first % BASE;
        int coeff = p.second;
        res = (res + (long long)coeff * LUT[mX + 4][degX] * LUT[mY + 4][degY]) % MOD;
    }
    return res;
}