// 1068 ms
#if 0
#define MAX_N    200'001	// N: 스트리머 수 ( 10 ≤ N ≤ 200,000 ) mId = 1부터 N까지

const int INF = 1e6;
inline int min(int a, int b) { return (a > b) ? b : a; }
inline int max(int a, int b) { return (a > b) ? a : b; }

int sqrt(int n) {
    double x1 = n, x2 = (x1 + n / x1) / 2.0;
    while (x1 - x2 > 0.01) { x1 = x2, x2 = (x1 + n / x1) / 2.0; }
    return (int)x2;
}

struct RangeQuery {
    struct Group {
        int sum, min, max;
    } groups[MAX_N];

    int arr[MAX_N];		// mSubsriber[mId]
    int sq;             // group size
    int N;              // max index

    void build(int n, int mSubscriber[]) {
        N = n;
        sq = sqrt(N + 1);
        for (int i = 0; i <= N / sq; i++) groups[i] = { 0, INF, -INF };

        for (int i = 1; i <= N; i++) {
            int value = mSubscriber[i - 1];
            arr[i] = value;
            groups[i / sq].sum += value;
            groups[i / sq].min = min(groups[i / sq].min, value);
            groups[i / sq].max = max(groups[i / sq].max, value);
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
            for (int i = lo; i <= hi; i++)
                res = { min(res.min, arr[i]), max(res.max, arr[i]) };
            return res.max - res.min;
        }
        for (int i = lo; i <= (s + 1) * sq - 1; i++)
            res = { min(res.min, arr[i]), max(res.max, arr[i]) };
        for (int i = s + 1; i <= e - 1; i++)
            res = { min(res.min, groups[i].min), max(res.max, groups[i].max) };
        for (int i = e * sq; i <= hi; i++)
            res = { min(res.min, arr[i]), max(res.max, arr[i]) };
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
    return rng.arr[mId];
}

int unsubscribe(int mId, int mNum)
{
    rng.update(mId, -mNum);
    return rng.arr[mId];
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