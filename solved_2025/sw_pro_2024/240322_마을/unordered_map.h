#pragma once

#include "vector.h"
#define MAX_TABLE   10'007

template<typename K, typename V>
struct UnorderedMap {
    struct Node {
        K first;
        V second;
    };
    Vector<Node> table[MAX_TABLE];

    int hashFunc(const K& key) { return key % MAX_TABLE; }
    void clear() {
        for (int i = 0; i < MAX_TABLE; i++)
            table[i].clear();
    }
    Node* find(const K& key) {
        int hash = hashFunc(key);
        for (auto& node : table[hash])      // ** 주의 **
            if (node.first == key) return &node;
        return nullptr;
    }
    void emplace(const K& key, const V& value) {
        int hash = hashFunc(key);
        table[hash].push_back({ key, value });
    }
    Node* end() { return nullptr; }
    V& operator[](const K& key) {
        auto iter = find(key);
        if (iter == end()) { emplace(key, {}); iter = find(key); }
        return iter->second;
    }
};