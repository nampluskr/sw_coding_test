#if 0
// 문자열 파싱 -> 숫자, 연산자 (+, -, *)
// 문자열 수식 계산: 스택 (연산자 우선 순위 * > +, -)
// 1D 배열 Decomposition: 일정 길이 (mLen) 만큼의 결과 미리 계산
#include <vector>
#include <cstring>
#include <string>
using namespace std;

#define MAX_LEN     101     // mLen: 주어지는 수식의 길이 (5 ≤ mLen ≤ 100)

char subexp[MAX_LEN];
int mLen;
int totalLen;

/////////////////////////////////////////////////////////////////////
void init(int mLen, char mSubexp[])
{
    ::mLen = mLen;
    totalLen = 0;
    strcpy(subexp, mSubexp);
    totalLen += mLen;
}

// 수식에 추가할 연결 연산자 mLink와 추가할 문자열 mSubexp를 제공된다.
// 수식 추가 후 계산 결과 (1,000)
int append(char mLink, char mSubexp[])
{
    subexp[totalLen++] = mLink;
    strcat(subexp, mSubexp);
    totalLen += mLen;

    return 0;
}

// 기존 수식 문자열에서 위치 mFrom 번째 문자부터 (mLen + 1)개의 문자를 삭제한다.
// 문자열 삭제 후 계산 결과 (300)
int erase(int mFrom)
{
    subexp[mFrom] = '\0';
    return 0;
}

// 선택된 문자열의 계산 결과 (20,000)
// 수식에서 mFrom번째 문자와 mTo 번째 문자는 숫자임이 보장
int select(int mFrom, int mTo)
{
    return 0;
}
#endif