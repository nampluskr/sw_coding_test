#pragma once

#include <vector>
#include <queue>
using namespace std;

template<typename T>
struct DoubleEnededPQ {
    struct Pair {
        T data;
        int idx;
        bool operator<(const Pair& p) const { return data < p.data; }
        bool operator>(const Pair& p) const { return data > p.data; }
    };
    priority_queue<Pair, vector<Pair>, less<Pair>> maxHeap;
    priority_queue<Pair, vector<Pair>, greater<Pair>> minHeap;
    vector<bool> popped;
    int idx = 0;
    int heapSize = 0;

    void clear() {
        maxHeap = {};
        minHeap = {};
        popped.clear();
        idx = 0;
        heapSize = 0;
    }
    void push(const T& data) {
        maxHeap.push({ data, idx });
        minHeap.push({ data, idx });
        popped.push_back(false);
        idx++;
        heapSize++;
    }
    void popMax() {
        while (!maxHeap.empty() && popped[maxHeap.top().idx]) maxHeap.pop();
        auto max = maxHeap.top(); maxHeap.pop();
        popped[max.idx] = true;
        heapSize--;
    }
    void popMin() {
        while (!minHeap.empty() && popped[minHeap.top().idx]) minHeap.pop();
        auto min = minHeap.top(); minHeap.pop();
        popped[min.idx] = true;
        heapSize--;
    }
    T max() {
        while (!maxHeap.empty() && popped[maxHeap.top().idx]) maxHeap.pop();
        return maxHeap.top().data;
    }
    T min() {
        while (!minHeap.empty() && popped[minHeap.top().idx]) minHeap.pop();
        return minHeap.top().data;
    }
    bool empty() { return heapSize == 0;}
};
