#if 0
#include <vector>

using namespace std;

const int MAX = 1'000'000;

char formul[MAX];

// acc -> a + b + d + e => a + d + e => a + e => a
vector<int> accmulator;

int totalLen;

int appendSize;
int formulSize;

bool  isDigit[256];
/*
inline bool isDigit(char ch) {
    return '0' <= ch && ch <= '9';
}
*/

int getNumber(char exp[], int s, int e, int& len)
{
    int ret = exp[s] - '0';
    len = 1;

    // if (s + 1 <= e && isDigit(exp[s + 1]) == true) {
    if (s + 1 <= e && isDigit[exp[s + 1]] == true) {
        ret = ret * 10 + (exp[s + 1] - '0');
        len = 2;
    }

    return ret;
}


int caculate(char exp[], int s, int e, int & result)
{
    int ret = 0, len, nextNumber;

    // if (isDigit(exp[s]) == true)
    if(isDigit[exp[s]] == true)
    {
        ret = getNumber(exp, s, e, len);
        s += len;
    }
    else if(exp[s] == '+')
    {
        ret = getNumber(exp, s + 1, e, len);
        s += len + 1;
    }
    // -
    else
    {
        ret = -getNumber(exp, s + 1, e, len);
        s += len + 1;
    }

    while (s < e)
    {
        switch (exp[s])
        {
        case '*':
            nextNumber = getNumber(exp, s + 1, e, len);
            s += len + 1;

            ret *= nextNumber;
            break;
        
        case '+':
        case '-':
            s = caculate(exp, s, e, nextNumber);
            ret += nextNumber;
            result = ret;
            return s;

        default:
            break;
        }
    }

    result = ret;
    return s;
}

int append(char mLink, char mSubexp[])
{
    int start = totalLen;
    formul[totalLen++] = mLink;

    for (int i = 0; i < appendSize; i++)
        formul[totalLen++] = mSubexp[i];
    
    formul[totalLen] = 0;

    int result = 0;
    caculate(formul, start, totalLen, result);
    
    accmulator.push_back(accmulator.back() + result);
    
    return accmulator.back();
}

bool isFirst = false;

void init(int mLen, char mSubexp[])
{
    if (isFirst == false)
    {
        isFirst = true;
        isDigit['0'] = true;
        isDigit['1'] = true;
        isDigit['2'] = true;
        isDigit['3'] = true;
        isDigit['4'] = true;
        isDigit['5'] = true;
        isDigit['6'] = true;
        isDigit['7'] = true;
        isDigit['8'] = true;
        isDigit['9'] = true;
    }
    
    
    appendSize = mLen;
    formulSize = mLen + 1;

    totalLen = 0;

    accmulator.clear();
    accmulator.push_back(0);

    append('+', mSubexp);
}

int erase(int mFrom)
{
    int len = totalLen - mFrom - appendSize - 1;
    
    for (int i = 0; i < len; i++)
        formul[mFrom + i] = formul[mFrom + formulSize + i];

    int end = totalLen - appendSize - 1;

    formul[end] = 0;

    accmulator.clear();
    accmulator.push_back(0);
    totalLen = end;

    int result;
    for (int i = 0; i < end / (formulSize); i++)
    {
        int start = i * (formulSize);
        caculate(formul, start, start + appendSize, result);
        accmulator.push_back(accmulator.back() + result);
    }
    
    return accmulator.back();
}

int select(int mFrom, int mTo)
{
    int ret = 0, result;

    int start_idx = mFrom / (formulSize);
    int end_idx = mTo / (formulSize);

    // 누적합을 이용 구하는 부분
    if (start_idx + 1 <= end_idx - 1)
    {
        ret += accmulator[end_idx] - accmulator[start_idx + 1];
    }

    // 같은 구간이면,
    if (start_idx == end_idx)
    {
        caculate(formul, mFrom, mTo, result);
        ret += result;
    }
    // 짜투리
    else
    {
        caculate(formul, mFrom, start_idx * (formulSize) + appendSize, result);
        ret += result;

        caculate(formul, end_idx * (formulSize), mTo, result);
        ret += result;
    }

    return ret;
}
#endif
