#if 0
/*
- 문자열 파싱: 정수 계수 / 정수 지수
- 매번 각 항의 지수들까지 계산하는 것은 비효율적 => LUT (Lookup Table)
- (0 ≤ mDegreeX, mDegreeY ≤ 1,000), (-4 ≤ mX, mY ≤ 4)
- 나머지 연산: 결과 0 ~ 9999 (M = 10'000으로 나눈 나머지): (A op B + M) % M
*/

#include <vector>
#include <unordered_map>
#include <string>
#include <cstring>
using namespace std;

#define MAX_POLY    200'000     // 모든 다항식의 항의 개수의 총합은 200,000 이하
#define MAX_LEN     101         // 다항식 (1 ≤ |mPolynomial| ≤ 100) + 1 (문자열 끝에는 ‘\0’)
#define M           10'000

struct Polynomials {
    char mName[11];
    char mPolynomial[101];
} poly[MAX_POLY];
int polyCnt;
unordered_map<string, int> polyMap;

/////////////////////////////////////////////////////////////////////
void init()
{
    polyCnt = 0;
}

void assign(char mName[], char mPolynomial[])
{
    int pIdx = polyMap[mName] = polyCnt++;
    strcpy(poly[pIdx].mName, mName);
    strcpy(poly[pIdx].mPolynomial, mPolynomial);
}

void compute(char mNameR[], char mNameA[], char mNameB[], int mOp)
{
    int rIdx = polyMap[mNameR] = polyCnt++;
    int aIdx = polyMap[mNameA], bIdx = polyMap[mNameB];
}

int getCoefficient(char mName[], int mDegreeX, int mDegreeY)
{
    return -1;
}

int calcPolynomial(char mName[], int mX, int mY)
{
    return -1;
}
#endif