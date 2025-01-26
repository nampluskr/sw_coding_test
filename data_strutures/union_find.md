## Union-find

- 공통

```cpp
#define MAX_SIZE    10000

void swap(int& x, int& y) { int temp = x; x = y; y = temp; }
```

### 레퍼런스 코드

```cpp
int parent[MAX_SIZE];       // 각 원소의 부모 노드를 저장
int rank[MAX_SIZE];         // 각 집합의 트리 높이(랭크)

init(int n) {
    for (int i = 0; i < n; i++) {
        parent[i] = i;
        rank[i] = 0;
    }
}

int find(int x) {
    if (parent[x] == x) return x;
    return parent[x] = find(parent[x]);
}

void merge(int x, int y) {
    x = find(x);
    y = find(y);
    if (x == y) return;

    if (rank[x] < rank[y]) swap(x, y);
    parent[y] = x;          // y 집합을 x 집합에 병합
    if (rank[x] == rank[y]) rank[x]++;
}
```

### 각 집합의 크기를 관리

```cpp
int parent[MAX_SIZE];       // 각 원소의 부모 노드를 저장
int rank[MAX_SIZE];         // 각 집합의 트리 높이(랭크)
int size[MAX_SIZE];         // 각 집합의 크기

init(int n) {
    for (int i = 0; i < n; i++) {
        parent[i] = i;
        rank[i] = 0;
        size[i] = 1;        // 초기에는 모든 원소가 크기 1인 집합
    }
}

int find(int x) {
    if (parent[x] == x) return x;
    return parent[x] = find(parent[x]);
}

void merge(int x, int y) {
    x = find(x);
    y = find(y);
    if (x == y) return;

    if (rank[x] < rank[y]) swap(x, y);
    parent[y] = x;          // y 집합을 x 집합에 병합
    size[x] += size[y];     // x의 집합에 y의 집합을 합치므로 크기를 더함
    if (rank[x] == rank[y]) rank[x]++;
}

int getSize(int x) { 
    return size[find(x)];
}
```

### 각 집합에 공통 값을 더하는 기능 구현 (Lazy Propagation)

```cpp
int parent[MAX_SIZE];       // 각 원소의 부모 노드를 저장
int rank[MAX_SIZE];         // 각 집합의 트리 높이(랭크)
int size[MAX_SIZE];         // 각 집합의 크기
int groupScore[MAX_SIZE];   // 각 집합의 공통 점수
int offset[MAX_SIZE];       // 각 원소의 groupScore와의 offset 값

init(int n) {
    for (int i = 0; i < n; i++) {
        parent[i] = i;
        rank[i] = 0;
        size[i] = 1;
        groupScore[i] = 0;  // 초기 공통 점수는 0
        offset[i] = 0;      // 초기 offset은 0
    }
}

// find 함수 수정: offset 값 업데이트
int find(int x) {
    if (parent[x] == x) return x;

    int root = find(parent[x]);
    offset[x] += offset[parent[x]];     // 경로 압축 시 offset 값 갱신 (순서 주의)
    parent[x] = root;
    return root;
}

// merge 함수 수정: offset 값 조정
void merge(int x, int y) {
    x = find(x);
    y = find(y);
    if (x == y) return;

    if (rank[x] < rank[y]) swap(x, y);
    parent[y] = x;                              // y 집합을 x 집합에 병합
    offset[y] = groupScore[x] - groupScore[y];  // offset 조정
    size[x] += size[y];                         // x의 집합에 y의 집합을 합치므로 크기를 더함
    if (rank[x] == rank[y]) rank[x]++;
}

int getSize(int x) { 
    return size[find(x)];
}

// 특정 집합의 모든 원소에 값을 더하는 함수
void addScore(int x, int value) {
    groupScore[find(x)] += value;
}

// 특정 원소의 현재 점수를 반환하는 함수
int getScore(int x) {
    return groupScore[find(x)] + offset[x];
}
```

### 각 원소의 삭제 여부 관리

```cpp
int parent[MAX_SIZE];       // 각 원소의 부모 노드를 저장
int rank[MAX_SIZE];         // 각 집합의 트리 높이(랭크)
int size[MAX_SIZE];         // 각 집합의 크기
int groupScore[MAX_SIZE];   // 각 집합의 공통 점수
int offset[MAX_SIZE];       // 각 원소의 groupScore와의 offset 값
bool deleted[MAX_SIZE];     // 각 원소의 삭제 여부

void init(int n) {
    for (int i = 0; i < n; i++) {
        parent[i] = i;
        rank[i] = 0;
        size[i] = 1;
        groupScore[i] = 0;
        offset[i] = 0;
        deleted[i] = false;  // 초기에는 모든 원소가 삭제되지 않은 상태
    }
}

int find(int x) {
    // 삭제된 노드를 만나면 그 위의 부모를 찾아감
    while (deleted[x] && x != parent[x]) x = parent[x];
    if (x == parent[x]) return x;

    int root = find(parent[x]);
    offset[x] += offset[parent[x]];
    parent[x] = root;
    return root;
}

void merge(int x, int y) {
    if (deleted[x] || deleted[y]) return;

    x = find(x);
    y = find(y);
    if (x == y) return;

    if (rank[x] < rank[y]) swap(x, y);

    parent[y] = x;
    offset[y] = groupScore[y] - groupScore[x];
    size[x] += size[y];

    if (rank[x] == rank[y]) rank[x]++;
}

int getSize(int x) { 
    return size[find(x)];
}

void deleteNode(int x) {
    if (deleted[x]) return;
    deleted[x] = true;
    size[find(x)]--;
}

void addScore(int x, int value) {
    if (deleted[x]) return;
    groupScore[find(x)] += value;
}

int getScore(int x) {
    if (deleted[x]) return -1;
    return groupScore[find(x)] + offset[x];
}

int getSize(int x) {
    if (deleted[x]) return 0;
    return size[find(x)];
}
```