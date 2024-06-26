// 709 ms
#if 1
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
    struct Pair { int value, idx; };
    struct Group {
        int sum;
        Pair min, max;
    } groups[MAX_N];

    int arr[MAX_N];		// mSubsriber[mId]
    int sq;             // group size
    int N;              // max arr index

    void build(int n, int mSubscriber[]) {
        N = n;
        sq = sqrt(N + 1);
        for (int i = 0; i <= N / sq; i++) {
            groups[i].sum = 0;
            groups[i].min.value = INF;
            groups[i].max.value = -INF;
        }
        for (int i = 1; i <= N; i++) {
            int value = mSubscriber[i - 1];
            arr[i] = value;
            groups[i / sq].sum += value;
            if (groups[i / sq].min.value > value) groups[i / sq].min = { value, i };
            if (groups[i / sq].max.value < value) groups[i / sq].max = { value, i };
        }
    }
    void update(int idx, int delta) {
        arr[idx] += delta;
        groups[idx / sq].sum += delta;

        int gIdx = idx / sq;
        int lo = gIdx * sq, hi = min((gIdx + 1) * sq - 1, N);

        // update min { value, idx }
        if (groups[gIdx].min.idx == idx) {
            groups[gIdx].min.value = INF;
            for (int i = lo; i <= hi; i++)
                if (groups[gIdx].min.value > arr[i]) groups[gIdx].min = { arr[i], i };
        } else if (groups[gIdx].min.value > arr[idx]) groups[gIdx].min = { arr[idx], idx };

        // update max { value, idx }
        if (groups[gIdx].max.idx == idx) {
            groups[gIdx].max.value = -INF;
            for (int i = lo; i <= hi; i++)
                if (groups[gIdx].max.value < arr[i]) groups[gIdx].max = { arr[i], i };
        } else if (groups[gIdx].max.value < arr[idx]) groups[gIdx].max = { arr[idx], idx };
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