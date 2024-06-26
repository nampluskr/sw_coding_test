#if 1
#define MAX_N   200'000
#define INF 1'000'000

inline int min(int a, int b) { return (a < b) ? a : b; }
inline int max(int a, int b) { return (a < b) ? b : a; }
inline int ceil(int a, int b) { return (a + b - 1) / b; }

int sqrt(int n) {
    double x1 = n, x2 = (x1 + n / x1) / 2.0;
    while (x1 - x2 > 0.01) { x1 = x2, x2 = (x1 + n / x1) / 2.0; }
    return (int)x2;
}

// sqrt decomposition
struct RangeQuery {
    struct Pair { int value, idx; };
    struct Group { 
        int sum;
        Pair min, max;
    } groups[MAX_N];

    int arr[MAX_N];
    int N;
    int sq;             // group size

    void init(int n) {
        N = n;
        sq = sqrt(N + 1);
        for (int i = 1; i <= N; i++) arr[i] = 0;
        for (int i = 0; i <= N / sq; i++) groups[i] = { 0, { INF, -1 }, { -INF, -1 } };
    }
    void update(int idx, int delta) {
        arr[idx] += delta;
        groups[idx / sq].sum += delta;

        int gIdx = idx / sq;
        int lo = gIdx * sq, hi = min((gIdx + 1) * sq - 1, N);

        if (idx == groups[gIdx].min.idx) {
            groups[gIdx].min.value = INF;
            for (int i = lo; i <= hi; i++)
                if (groups[gIdx].min.value > arr[i]) groups[gIdx].min = { arr[i], i };
        } else if (groups[gIdx].min.value > arr[idx]) groups[gIdx].min = { arr[idx], idx };

        if (idx == groups[gIdx].max.idx) {
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
    int queryMinMax(int lo, int hi) {
        int resMin = INF, resMax = -INF;
        int s = lo / sq, e = hi / sq;

        if (s == e) {
            for (int i = lo; i <= hi; i++) {
                resMin = min(resMin, arr[i]);
                resMax = max(resMax, arr[i]);
            }
            return resMax - resMin;
        }
        for (int i = lo; i <= (s + 1) * sq - 1; i++) {
            resMin = min(resMin, arr[i]);
            resMax = max(resMax, arr[i]);
        }
        for (int i = s + 1; i <= e - 1; i++) {
            resMin = min(resMin, groups[i].min.value);
            resMax = max(resMax, groups[i].max.value);
        }
        for (int i = e * sq; i <= hi; i++) {
            resMin = min(resMin, arr[i]);
            resMax = max(resMax, arr[i]);
        }
        return resMax - resMin;
    }
} rng;

/////////////////////////////////////////////////////////////////////
void init(int N, int mSubscriber[])
{
    rng.init(N);
    for (int i = 1; i <= N; i++)
        rng.update(i, mSubscriber[i - 1]);
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
    return rng.queryMinMax(sId, eId);
}
#endif