## Vector

- `vector.h`

```cpp
template<typename T>
struct Vector {
    T* arr;
    int tail = -1, capacity = 1;

    Vector() { arr = new T[capacity]; }
    ~Vector() { delete[] arr; }

    void clear() {
        tail = -1;
        // delete[] arr;
        // capacity = 1;
        // arr = new T[capacity];
    }
    void push_back(const T& data) {
        if (tail == capacity - 1) resize(capacity * 2);
        arr[++tail] = data;
    }
    int size() { return tail + 1; }

    T* begin() { return arr; }
    T* end() { return arr + tail + 1; }

private:
    void resize(int newCapacity) {
        capacity = newCapacity;
        T* temp = new T[capacity];
        for (int i = 0; i <= tail; i++) temp[i] = arr[i];
        delete arr;
        arr = temp;
    }
};
```

- 참조 연산자

```cpp
    T& operator[](int idx) { return arr[idx]; }
    const T& operator[](int idx) const { return arr[idx]; }
```

- 복사 생성자

```cpp
    Vector(const Vector& other) {
        tail = other.tail;
        capacity = other.capacity;
        arr = new T[capacity];
        for (int i = 0; i <= tail; i++) arr[i] = other.arr[i];
    }
```

- 복사 연산자

```cpp
    Vector& operator=(const Vector& other) {
        if (this != &other) {
            delete[] arr;
            tail = other.tail;
            capacity = other.capacity;
            arr = new T[capacity];
            for (int i = 0; i <= tail; i++) arr[i] = other.arr[i];
        }
        return *this;
    }
```

## Queue

- `queue.h`

```cpp
template<typename T>
struct Queue {
    T* arr;
    int head = 0, tail = -1, capacity = 1;

    Queue() { arr = new T[capacity]; }
    ~Queue() { delete[] arr; }

    void clear() {
        head = 0, tail = -1;
        // delete[] arr;
        // capacity = 1;
        // arr = new T[capaicty];
    }
    void push(const T& data) {
        if (tail == capacity - 1) resize(capacity * 2);
        arr[++tail] = data;
    }
    void pop() { head++; }
    T front() { return arr[head]; }
    bool empty() { return tail + 1 - head == 0; }
    int size() { return tail + 1 - head; }

private:
    void resize(int newCapacity) {
        capacity = newCapacity;
        T* temp = new T[capacity];
        for (int i = head; i <= tail; i++>) temp[i - head] = arr[i];
        delete[] arr;
        arr = temp;
        tail = tail - head; // ** 주의 **
        head = 0;
    }
};
```

- 이터레이터

```cpp
    T* begin() { return arr + head; }
    T* end() { return arr + tail + 1; }
```

- 복사 생성자

```cpp
    Queue(const Queue& other) {
        capacity = other.capacity;
        head = other.head;
        tail = other.tail;
        arr = new T[capacity];
        for (int i = head; i <= tail; i++) arr[i] = other.arr[i];
    }
```

- 복사 연산자

```cpp
    Queue& operator=(const Queue& other) {
        if (this != &other) {
            delete[] arr;
            capacity = other.capacity;
            head = other.head;
            tail = other.tail;
            arr = new T[capacity];
            for (int i = head; i <= tail; i++) arr[i] = other.arr[i];
        }
        return *this;
    }
```

## Stack

- `stack.h`

```cpp
template<typename T>
struct Stack {
    T* arr;
    int tail = -1, capacity = 1;

    Stack() { arr = new T[capacity]; }
    ~Stack() { delete[] arr; }

    void clear() {
        tail = -1;
        // delete[] arr;
        // capacity = 1;
        // arr = new T[capacity];
    }
    void push(const T& data) {
        if (tail == capacity - 1) resize(capacity * 2);
        arr[++tail] = data;
    }
    void pop() { tail--; }
    T top() { return arr[tail]; }
    bool empty() { return tail + 1 == 0; }
    int size() { return tail + 1; }

private:
    void resize(int newCapacity) {
        capacity = newCapacity;
        T* temp = new T[capacity];
        for (int i = 0; i <= tail; i++) temp[i] = arr[i];
        delete arr;
        arr = temp;
    }
};
```

- 이터레이터

```cpp
    T* begin() { return arr; }
    T* end() { return arr + tail + 1; }
```