## Max. Heap: 

- `priority_queue.h`

```cpp
template<typename T>
struct PriorityQueue {
    T* arr;
    int arrSize = 0, capacity = 1;

    PriorityQueue() { arr = new T[capacity]; }
    ~PriorityQueue() { delete[] arr; }

    // void clear() { arrSize = 0; }
    void push(const T& data) {
        if (arrSize == capacity) resize(capacity * 2);
        arr[arrSize] = data;
        siftUp(arrSize);
        arrSize++;
    }
    void pop() {
        arrSize--;
        swap(arr[0], arr[arrSize]);
        siftDown(0);
    }
    T top() { return arr[0]; }
    bool empty() { return arrSize == 0; }
    int size() { return arrSize; }

private:
    void resize(int newCapacity) {
        capacity = newCapacity;
        T* temp = new T[capacity];
        for (int i = 0; i < arrSize; i++) temp[i] = arr[i];
        delete[] arr;
        arr = temp;
    }
    void siftUp(int curr) {
        int parent = (curr - 1) / 2;
        while (curr > 0) {
            if (arr[curr] < arr[parent]) break;
            swap(arr[curr], arr[parent]);
            curr = parent;
            parent = (curr - 1) / 2;
        }
    }
    void siftDown(int curr) {
        int child = 2 * curr + 1;
        while (child < arrSize) {   // *** 주의 ***
            if (child + 1 < arrSize && arr[child] < arr[child + 1]) child++;
            if (arr[child] < arr[curr]) break;
            swap(arr[child], arr[curr]);
            curr = child;
            child = 2 * child + 1;
        }
    }
    void swap(T& a, T& b) { T temp = a; a = b; b = temp; }
};
```

- 복사 생성자

```cpp
    PriorityQueue(const PriorityQueue& other) {
        arrSize = other.arrSize;
        capacity = other.capacity;
        arr = new T[capacity];
        for (int i = 0; i < arrSize; i++) {
            arr[i] = other.arr[i];
        }
    }
```

- 복사 대입 연산자

```cpp
    PriorityQueue& operator=(const PriorityQueue& other) {
        if (this != &other) {   // 자기 자신을 대입하는 경우 체크
            delete[] arr;       // 기존 메모리 해제
            
            arrSize = other.arrSize;
            capacity = other.capacity;
            arr = new T[capacity];
            for (int i = 0; i < arrSize; i++) {
                arr[i] = other.arr[i];
            }
        }
        return *this;
    }
```

### heapify

```cpp
    void heapify(T newArr[], int n) {
        delete[] arr;             // 기존 데이터 초기화
        arrSize = capacity = n;
        arr = new T[capacity];    // 새로운 메모리 할당

        // 배열 복사     
        for (int i = 0; i < n; i++)
            arr[i] = newArr[i];
        
        // 마지막 비단말 노드부터 루트까지 siftDown 수행
        for (int i = (n / 2) - 1; i >= 0; i--)
            siftDown(i);
    }
```

## Min. Heap

```cpp

```

## Top-K Selection Using Binary Heap

### Offline k-th largest element selection

- 전체 배열을 한 번에 Max Heap으로 구성합니다.
- k-1번 pop 연산을 수행하여 k번째 큰 원소를 찾습니다.
- 시간 복잡도: O(n + klogn)

```cpp
template<typename T>
T findKthLargest(T arr[], int n, int k) {
    PriorityQueue<T> pq;
    
    // 전체 배열을 힙으로 변환
    pq.heapify(arr, n);
    
    // k-1번 pop하여 k번째 큰 원소를 찾음
    for (int i = 0; i < k-1; i++) {
        pq.pop();
    }
    return pq.top();
}
```

### 1. Online k-th largest element selection

- Min Heap을 사용하여 k개의 가장 큰 원소들을 유지합니다.
- 힙의 크기를 k로 제한하고, 새로운 원소가 들어올 때마다 갱신합니다.
- 힙의 root가 k번째 큰 원소가 됩니다.
- 시간 복잡도: 각 원소 추가당 O(logk)

```cpp
template<typename T>
class KthLargest {
private:
    PriorityQueue<T> pq;
    int k;
    
public:
    KthLargest(int k) : k(k) {}
    
    void add(T val) {
        pq.push(val);
        if (pq.size() > k) {
            pq.pop();
        }
    }
    
    T getKthLargest() {
        return pq.top();
    }
};
```

### 2. Min Heap 최적화

- 시간 복잡도: O(logk) per operation
- 메모리: O(k)
- 장점: 안정적인 성능, 구현이 간단
- 단점: k가 클 경우 메모리 사용량 증가

```cpp
template<typename T>
class KthLargest {
private:
    int k;
    PriorityQueue<T> pq;  // Min Heap으로 동작하도록 비교 연산자 변경
    
    void siftUp(int curr) {
        int parent = (curr - 1) / 2;
        while (curr > 0) {
            if (arr[curr] > arr[parent]) break;  // 비교 연산자 반대로
            swap(arr[curr], arr[parent]);
            curr = parent;
            parent = (curr - 1) / 2;
        }
    }
    
    void siftDown(int curr) {
        int child = 2 * curr + 1;
        while (child < arrSize) {
            if (child + 1 < arrSize && arr[child] > arr[child + 1]) child++;
            if (arr[child] > arr[curr]) break;
            swap(arr[child], arr[curr]);
            curr = child;
            child = 2 * child + 1;
        }
    }
    
public:
    KthLargest(int k) : k(k) {}
    
    void add(T val) {
        if (pq.size() < k) {
            pq.push(val);
        } else if (val > pq.top()) {
            pq.pop();
            pq.push(val);
        }
    }
    
    T getKthLargest() {
        return pq.top();
    }
};
```

### 3. 부분 정렬 유지

- 시간 복잡도: O(k) per operation
- 메모리: O(k)
- 장점: 작은 k에 대해 캐시 효율적, 메모리 접근이 연속적
- 단점: k가 클 경우 성능 저하 (k가 작은 경우(k < 10) 효율적)

```cpp
template<typename T>
class KthLargest {
private:
    vector<T> data;
    int k;
    
    void maintainPartialSort(T val) {
        int pos = data.size() - 1;
        while (pos > 0 && data[pos-1] < val) {
            data[pos] = data[pos-1];
            pos--;
        }
        data[pos] = val;
    }
    
public:
    KthLargest(int k) : k(k) {}
    
    void add(T val) {
        if (data.size() < k) {
            data.push_back(val);
            maintainPartialSort(val);
        } else if (val > data[k-1]) {
            data[k-1] = val;
            maintainPartialSort(val);
        }
    }
    
    T getKthLargest() {
        return data[k-1];
    }
};
```

