#if 0
#include <vector>

using namespace std;

const int MAX = 100 + 100 * 1000 + 1;
char formula[MAX];
int length;
int wp;

vector<int> acc;
int totalResult;

inline bool isDigit(char ch)
{
	if ('0' <= ch and ch <= '9')
		return true;

	return false;
}

char* getNumber(char* s, char* e, int& ret)
{
	ret = 0;
	bool minus = false;

	if (*s == '-')
	{
		s++;
		minus = true;
	}

	ret = *s - '0';
	s++;

	if (s != e and isDigit(*s)) {
		ret = ret * 10 + *s - '0';
		s++;
	}

	if (minus)
		ret = -ret;

	return s;
}

char* caculate(char* s, char* e, int& ret)
{
	ret = 0;
	int n;

	s = getNumber(s, e, ret);

	while (s != e)
	{
		switch (*s)
		{
		case '+':
			s = caculate(s + 1, e, n);
			ret += n;
			break;

		case '-':
			s = caculate(s, e, n);
			ret += n;
			break;

		case '*':
			s = getNumber(s + 1, e, n);
			ret = ret * n;
			break;

		default:
			break;
		}

	}

	return s;
}


int append(char mLink, char mSubexp[]);

void init(int mLen, char mSubexp[])
{
	/*
		Unit Test
		int ret = 0;
		char s[] = "-1-1+1*1";
		caculate(s, s + 8, ret);
	*/
	totalResult = wp = 0;
	length = mLen;
	acc.resize(1);
	acc[0] = 0;

	append('+', mSubexp);
}

int append(char mLink, char mSubexp[])
{
	char* s = &formula[wp];

	formula[wp++] = mLink;
	for (int i = 0; i < length; i++)
		formula[wp++] = mSubexp[i];

	int ret;

	caculate(mLink == '-' ? s : s + 1, s + length + 1, ret);

	acc.push_back(acc.back() + ret);

	return acc.back();
}

int erase(int mFrom)
{
	int totalLength = (acc.size() - 1) * (length + 1);

	int left = totalLength - length - 1 - mFrom;

	for (int i = 0; i < left; i++)
		formula[mFrom + i] = formula[mFrom + i + length + 1];

	totalLength -= (length + 1);
	formula[totalLength] = 0;

	acc.resize(1);
	acc[0] = 0;

	wp = 0;

	for (int i = 0; i < totalLength / (length + 1); i++)
		append(formula[wp], &formula[wp + 1]);

	return acc.back();
}

int select(int mFrom, int mTo)
{
	int startAcc = mFrom / (length + 1), endAcc = mTo / (length + 1);
	int startPos = startAcc * (length + 1), endPos = (endAcc + 1) * (length + 1) - 1;
	int result, ret, s, e;

	if (startAcc == endAcc)
	{
		caculate(formula + mFrom + (formula[mFrom] == '+' ? 1 : 0), formula + mTo + 1, ret);
		return ret;
	}


	result = acc[endAcc + 1] - acc[startAcc];

	if (startPos != mFrom) {
		result -= acc[startAcc + 1] - acc[startAcc];

		e = (startAcc + 1) * (length + 1) - 1;
		e = min(e, mTo);

		caculate(formula + mFrom + (formula[mFrom] == '+' ? 1 : 0), formula + e + 1, ret);
		result += ret;
	}

	if (endPos != mTo) {
		result -= acc[endAcc + 1] - acc[endAcc];

		s = endAcc * (length + 1);
		s = max(s, mFrom);

		int ret;
		caculate(formula + s + (formula[s] == '+' ? 1 : 0), formula + mTo + 1, ret);
		result += ret;
	}

	return result;
}
#endif
