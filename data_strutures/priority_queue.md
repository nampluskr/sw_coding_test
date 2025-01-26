## Priority Queue

- Max. Heap

```cpp
#pragma once

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

/* TOTO
- 복사 생성자 / 복사 연산자 정의
- 이터레이터 정의
- heapify()
- topk()
*/
