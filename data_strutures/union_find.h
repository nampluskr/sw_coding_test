#pragma once

#define MAX_SIZE    10'000

struct UninFind {
    int parent[MAX_SIZE];
    int rank[MAX_SIZE];

    void clear() {
        for (int i = 0; i < MAX_SIZE; i++) {
            parent[i] = i;
            rank[i] = 0;
        }
    }
    int find(int x) {
        if (parent[x] == x) return x;

        // Path Compression
        int root = find(parent[x]);
        if (root != parent[x]) {
            parent[x] = root;
        }
        return root;
    }
    void merge(int x, int y) {
        x = find(x);
        y = find(y);
        if (x == y) return;

        if (rank[x] < rank[y]) swap(x, y);
        parent[y] = x;
        if (rank[x] == rank[y]) rank[x]++;
    }
    void swap(int& x, int& y) { int temp = x; x = y; y = temp; }
};