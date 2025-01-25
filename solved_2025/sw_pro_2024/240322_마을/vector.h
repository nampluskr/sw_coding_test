#pragma once

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
        delete[] arr;
        arr = temp;
    }
};