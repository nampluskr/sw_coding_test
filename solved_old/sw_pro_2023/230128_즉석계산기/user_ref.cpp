#include <string>
using namespace std;

string S;
int M;
int ret[1003], total;

int val[5], op[5], vn, on;
void calc() {
    while (on) {
        if (op[on] == '*') val[vn - 1] *= val[vn];
        if (op[on] == '+') val[vn - 1] += val[vn];
        if (op[on] == '-') val[vn - 1] -= val[vn];
        vn--, on--;
    }
}

/*
* [s:e-1) 계산결과 반환
* 숫자  : value stack 추가
* +,-  : 이전 연산 수행 후 op stack 추가
* *    : op stack 추가
*/
int calc(int s, int e) {
    vn = on = 0;
    if (!isdigit(S[s])) val[++vn] = 0;  // 맨 앞이 숫자가 아닌 경우

    for (int i = s; i < e; i++) {
        if (isdigit(S[i])) {
            if (i > s && isdigit(S[i - 1])) val[vn] = val[vn] * 10 + S[i] - '0';
            else val[++vn] = S[i] - '0';
            continue;
        }
        else if (S[i] != '*') calc();
        op[++on] = S[i];
    }
    calc();
    return val[1];
}

int append(char mLink, char mSubexp[]) {
    S += mLink;
    S += mSubexp;

    int L = S.size();
    total += ret[L / M - 1] = calc(L - M, L);
    return total;
}

/////////////////////////////////////////////////////////////////////
void init(int mLen, char mSubexp[]) {
    S.clear();
    M = mLen + 1;
    total = 0;

    append('+', mSubexp);
}

int erase(int mFrom) {
    S.erase(mFrom, M);

    int gid = mFrom / M;            // mFrom이 속한 그룹
    int gcnt = S.size() / M;        // 지워진 이후의 그룹 개수

    if (gid * M == mFrom) {     // 그룹이 통째로 지워지는 경우
        total -= ret[gid];
        for (int i = gid; i < gcnt; i++) ret[i] = ret[i + 1];
    }

    else {                      // 두 그룹이 걸쳐 있는 경우
        total -= ret[gid] + ret[gid + 1];
        for (int i = gid + 1; i < gcnt; i++) ret[i] = ret[i + 1];
        total += ret[gid] = calc(gid * M, gid * M + M);
    }

    return total;
}

int select(int mFrom, int mTo) {
    mTo++;

    int s = (mFrom + M) / M * M;
    int e = mTo / M * M;

    /* 범위가 한 개의 그룹 내에 있는 경우 */
    if (e < s) return calc(mFrom, mTo);

    /* 범위가 여러 그룹에 걸쳐 있는 경우 */
    int result = calc(mFrom, s) + calc(e, mTo);             // 왼쪽, 오른쪽 자투리 계산
    for (int i = s; i < e; i += M) result += ret[i / M];    // 전부 포함되는 그룹 계산
    return result;
}
