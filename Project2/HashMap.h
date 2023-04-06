#pragma once

template<typename K, typename V>
class HashMap {
private:
    unsigned int allocated = 0;
    struct Node {
        K key;
        V value;
        Node* next = nullptr;
    };
    struct Head {
        Node* next = nullptr;
    };
    Head* data = nullptr;
public:
    ~HashMap();
    HashMap(int size = 1);
    void resize(int newSize);
    V& operator[](K &key);
};

template<typename K, typename V>
V &HashMap<K, V>::operator[](K &key) {
    Head* head = &data[key.hash() % allocated];
    Node *node = head->next;
    if(node == nullptr) {
        node = new Node();
        head->next = node;
        node->key = key;
        return node->value;
    }
    while (node->next != nullptr) {
        if (node->key == key)
            return node->value;
        node = node->next;
    }
    if (node->key == key)
        return node->value;
    node->next = new Node();
    node = node->next;
    node->key = key;
    return node->value;
}

template<typename K, typename V>
void HashMap<K, V>::resize(int newSize) {
    delete[] data;
    data = new Head[newSize];
    allocated = newSize;
}

template<typename K, typename V>
HashMap<K, V>::HashMap(int size) {
    resize(size);
}

template<typename K, typename V>
HashMap<K, V>::~HashMap() {
    for(int i = 0; i < allocated; i++){
        Node *node = data[i].next;
        while(node != nullptr) {
            Node *next = node->next;
            delete node;
            node = next;
        }
    }
    delete[] data;
}
