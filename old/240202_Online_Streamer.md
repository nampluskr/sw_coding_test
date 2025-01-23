### [풀이중]

```cpp
#if 1
#include <cmath>
// N명의 스트리머가 주어진다. 각 스트리머는 1부터 N까지 ID값을 가진다.
#define MAX_N	200'001		// N: 스트리머 수 ( 10 ≤ N ≤ 200,000 )

inline int min(int a, int b) { return (a > b) ? b : a; }
inline int max(int a, int b) { return (a > b) ? a : b; }

struct RangeQuery {
	struct Pair { int value, idx; };
	struct Group {
		int sum;
		Pair min, max;
	} groups[MAX_N];

	int arr[MAX_N];			// mSubsriber[mId]
	int sq;					// group size
	const int INF = 1e6;
	int N;					// max index

	void build(int n, int mSubscriber[]) {
		N = n;
		sq = sqrt(N + 1);
		for (int i = 0; i <= N / sq; i++) {
			groups[i].sum = 0;
			groups[i].min.value = INF;
			groups[i].max.value = -INF;
		}

		for (int i = 0; i < N; i++) {
			arr[i + 1] = mSubscriber[i];
			groups[(i + 1) / sq].sum += mSubscriber[i];
			if (groups[(i + 1) / sq].min.value > mSubscriber[i]) groups[(i + 1) / sq].min = { mSubscriber[i], i };
			if (groups[(i + 1) / sq].max.value < mSubscriber[i]) groups[(i + 1) / sq].max = { mSubscriber[i], i };
		}
	}
	void update(int idx, int delta) {
		arr[idx] += delta;
		groups[idx / sq].sum += delta;

		// update min, max
		int gIdx = idx / sq;
		int lo = gIdx * sq, hi = min((gIdx + 1) * sq - 1, N);

		if (groups[gIdx].min.idx == idx) {
			groups[gIdx].min.value = INF;
			for (int i = lo; i <= hi; i++)
				if (groups[gIdx].min.value > arr[i]) groups[gIdx].min = { arr[i], i };
		} else if (groups[gIdx].min.value > arr[idx]) groups[gIdx].min = { arr[idx], idx };

		if (groups[gIdx].max.idx == idx) {
			groups[gIdx].max.value = -INF;
			for (int i = lo; i <= hi; i++)
				if (groups[gIdx].max.value < arr[i]) groups[gIdx].max = { arr[i], i };
		}
		else if (groups[gIdx].max.value < arr[idx]) groups[gIdx].max = { arr[idx], idx };
	}
	int queryPoint(int idx) {
		return arr[idx];
	}
	int querySum(int lo, int hi) {
		int res = 0;
		int s = lo / sq, e = hi / sq;

		if (s == e) {
			for (int i = lo; i <= hi; i++) res += arr[i];
			return res;
		}
		while (lo / sq == s) res += arr[lo++];
		for (int i = s + 1; i <= e - 1; i++) res += groups[i].sum;
		while (hi / sq == e) res += arr[hi--];
		return res;
	}
	int queryMinMAX(int lo, int hi) {
		struct Res { int min, max; } res = { INF, -INF };
		int s = lo / sq, e = hi / sq;

		if (s == e) {
			for (int i = lo; i <= hi; i++) {
				res.min = min(res.min, arr[i]);
				res.max = max(res.max, arr[i]);
			}
			return res.max - res.min;
		}
		for (int i = lo; i <= (s + 1) * sq - 1; i++) {
			res.min = min(res.min, arr[i]);
			res.max = max(res.max, arr[i]);
		}
		for (int i = s + 1; i <= e - 1; i++) {
			res.min = min(res.min, groups[i].min.value);
			res.max = max(res.max, groups[i].max.value);
		}
		for (int i = e * sq; i <= hi; i++) {
			res.min = min(res.min, arr[i]);
			res.max = max(res.max, arr[i]);
		}
		return res.max - res.min;
	}
} rng;


////////////////////////////////////////////////////////////////////
void init(int N, int mSubscriber[])
{
	rng.build(N, mSubscriber);
}

int subscribe(int mId, int mNum)
{
	rng.update(mId, +mNum);
	return rng.queryPoint(mId);
}

int unsubscribe(int mId, int mNum)
{
	rng.update(mId, -mNum);
	return rng.queryPoint(mId);
}

int count(int sId, int eId)
{
	return rng.querySum(sId, eId);
}

int calculate(int sId, int eId)
{
	return rng.queryMinMAX(sId, eId);
}
#endif
```

### [느린 버젼]

```cpp
#if 0
#include <cmath>

// N명의 스트리머가 주어진다. 각 스트리머는 1부터 N까지 ID값을 가진다.
#define MAX_N	200'001		// N: 스트리머 수 ( 10 ≤ N ≤ 200,000 )

inline int min(int a, int b) { return (a > b) ? b : a; }
inline int max(int a, int b) { return (a > b) ? a : b; }

struct RangeQuery {
	struct Group {
		int sum, min, max;
	} groups[MAX_N];

	int arr[MAX_N];			// mSubsriber[mId]
	int sq;					// group size
	const int INF = 1e6;
	int N;					// max index

	void build(int n, int mSubscriber[]) {
		N = n;
		sq = sqrt(N + 1);
		for (int i = 0; i <= N / sq; i++) groups[i] = { 0, INF, -INF };

		for (int i = 0; i < N; i++) {
			arr[i + 1] = mSubscriber[i];
			groups[(i + 1) / sq].sum += mSubscriber[i];
			groups[(i + 1) / sq].min = min(groups[(i + 1) / sq].min, mSubscriber[i]);
			groups[(i + 1) / sq].max = max(groups[(i + 1) / sq].max, mSubscriber[i]);
		}
	}
	void update(int idx, int delta) {
		arr[idx] += delta;
		groups[idx / sq].sum += delta;

		// update min, max
		int gIdx = idx / sq;
		int lo = gIdx * sq, hi = min((gIdx + 1) * sq - 1, N);
		groups[gIdx].min = INF;
		groups[gIdx].max = -INF;
		for (int i = lo; i <= hi; i++) {
			groups[gIdx].min = min(groups[gIdx].min, arr[i]);
			groups[gIdx].max = max(groups[gIdx].max, arr[i]);
		}
	}
	int queryPoint(int idx) {
		return arr[idx];
	}
	int querySum(int lo, int hi) {
		int res = 0;
		int s = lo / sq, e = hi / sq;

		if (s == e) {
			for (int i = lo; i <= hi; i++) res += arr[i];
			return res;
		}
		while (lo / sq == s) res += arr[lo++];
		for (int i = s + 1; i <= e - 1; i++) res += groups[i].sum;
		while (hi / sq == e) res += arr[hi--];
		return res;
	}
	int queryMinMAX(int lo, int hi) {
		struct Res { int min, max; } res = { INF, -INF };
		int s = lo / sq, e = hi / sq;

		if (s == e) {
			for (int i = lo; i <= hi; i++) {
				res.min = min(res.min, arr[i]);
				res.max = max(res.max, arr[i]);
			}
			return res.max - res.min;
		}
		for (int i = lo; i <= (s + 1) * sq - 1; i++) {
			res.min = min(res.min, arr[i]);
			res.max = max(res.max, arr[i]);
		}
		for (int i = s + 1; i <= e - 1; i++) {
			res.min = min(res.min, groups[i].min);
			res.max = max(res.max, groups[i].max);
		}
		for (int i = e * sq; i <= hi; i++) {
			res.min = min(res.min, arr[i]);
			res.max = max(res.max, arr[i]);
		}
		return res.max - res.min;
	}
} rng;


////////////////////////////////////////////////////////////////////
void init(int N, int mSubscriber[])
{
	rng.build(N, mSubscriber);
}

int subscribe(int mId, int mNum)
{
	rng.update(mId, +mNum);
	return rng.queryPoint(mId);
}

int unsubscribe(int mId, int mNum)
{
	rng.update(mId, -mNum);
	return rng.queryPoint(mId);
}

int count(int sId, int eId)
{
	return rng.querySum(sId, eId);
}

int calculate(int sId, int eId)
{
	return rng.queryMinMAX(sId, eId);
}
#endif
```

### [공유코드]

```cpp
#define MAX_N 200000
#define INF 0x7fffffff

inline int min(int a, int b) { return (a < b) ? a : b; }
inline int max(int a, int b) { return (a < b) ? b : a; }
inline int ceil(int a, int b) { return (a + b - 1) / b; }

int sqrt(int n) {
    double x1 = n, x2 = (x1 + n / x1) / 2.0;
    while (x1 - x2 > 0.01) { x1 = x2, x2 = (x1 + n / x1) / 2.0; }
    return (int)x2;
}

struct Pair {
    int idx, value;
};

struct Block {
    int sum;
    Pair max;
    Pair min;
};

struct Partition {
    int arr[MAX_N];
    int N;
    Block blocks[MAX_N];
    int bSize;
    int bCnt;

    void init(int N) {
        this->N = N;
        bSize = sqrt(N);
        bCnt = ceil(N, bSize);
        for (int i = 0; i < N; i++) { arr[i] = 0; }
        for (int i = 0; i < bCnt; i++) { blocks[i] = { 0, {-1, -INF}, {-1, INF} }; }
    }
    void update(int idx, int diff) {
        arr[idx] += diff;
        blocks[idx / bSize].sum += diff;

        int bIdx = idx / bSize;
        int left = bIdx * bSize;
        int right = min((bIdx + 1) * bSize - 1, N - 1);

        if (idx == blocks[bIdx].max.idx) {
            blocks[bIdx].max.value = -INF;
            for (int i = left; i <= right; i++)
                if (blocks[bIdx].max.value < arr[i])
                    blocks[bIdx].max = { i, arr[i] };
        }
        else if (blocks[bIdx].max.value < arr[idx])
            blocks[bIdx].max = { idx, arr[idx] };

        if (idx == blocks[bIdx].min.idx) {
            blocks[bIdx].min.value = INF;
            for (int i = left; i <= right; i++)
                if (blocks[bIdx].min.value > arr[i])
                    blocks[bIdx].min= { i, arr[i] };
        }
        else if (blocks[bIdx].min.value > arr[idx])
            blocks[bIdx].min = { idx, arr[idx] };
    }
    int querySum(int left, int right) {
        int res = 0;
        int s = left / bSize;
        int e = right / bSize;

        if (s == e) {
            for (int i = left; i <= right; i++)
                res += arr[i];
            return res;
        }
        for (int i = left; i <= (s + 1) * bSize - 1; i++) { res += arr[i]; }
        for (int i = s + 1; i <= e - 1; i++) { res += blocks[i].sum; }
        for (int i = e * bSize; i <= right; i++) { res += arr[i]; }
        return res;
    }
    int queryMaxMin(int left, int right) {
        int resMax = -INF;
        int resMin = INF;
        int s = left / bSize;
        int e = right / bSize;

        if (s == e) {
            for (int i = left; i <= right; i++) {
                resMax = max(resMax, arr[i]);
                resMin = min(resMin, arr[i]);
            }
            return resMax - resMin;
        }
        for (int i = left; i <= (s + 1) * bSize - 1; i++) {
            resMax = max(resMax, arr[i]);
            resMin = min(resMin, arr[i]);
        }
        for (int i = s + 1; i <= e - 1; i++) {
            resMax = max(resMax, blocks[i].max.value);
            resMin = min(resMin, blocks[i].min.value);
        }
        for (int i = e * bSize; i <= right; i++) {
            resMax = max(resMax, arr[i]);
            resMin = min(resMin, arr[i]);
        }
        return resMax - resMin;
    }
};
Partition P;

/////////////////////////////////////////////////////////////////////
void init(int N, int mSubscriber[])
{
    P.init(N);
    for (int i = 0; i < N; i++)
        P.update(i, mSubscriber[i]);
}

int subscribe(int mId, int mNum)
{
    P.update(mId - 1, mNum);
    return P.arr[mId - 1];
}

int unsubscribe(int mId, int mNum)
{
    P.update(mId - 1, -mNum);
    return P.arr[mId - 1];
}

int count(int sId, int eId)
{
    return P.querySum(sId - 1, eId - 1);
}

int calculate(int sId, int eId)
{
    return P.queryMaxMin(sId - 1, eId - 1);
}
```
