#pragma once
#include "Vector.h"
// Reference: https://www.programiz.com/dsa/priority-queue

template<typename T>
class PriorityQueue {
private:
    struct Node {
        T value ;
        int priority;
        Node() : value(T()), priority(1) {}
        Node(T value, int priority) : value(value), priority(priority) {}
    };
    int offset = 0;
    Vector<Node> data;
    void heapify(int index);
public:
    void setDataSize(int size);
    void pushWithoutHeapify(T &value, int priority = 1);
    void pushWithoutHeapify(T &&value, int priority = 1);
    void heapifyAll();
    void push(T &&value, int priority = 1);
    void push(T &value, int priority = 1);
    void changePriority(T& value, int priority = 1);
    int size();
    T& peek();
    T pop();
};

template<typename T>
void PriorityQueue<T>::setDataSize(int size) {
    data.resize(size);
}

template<typename T>
void PriorityQueue<T>::pushWithoutHeapify(T &value, int priority) {
    data.pushBack(Node(value, priority));
}

template<typename T>
void PriorityQueue<T>::pushWithoutHeapify(T &&value, int priority) {
    data.pushBack(Node(value, priority));
}

template<typename T>
void PriorityQueue<T>::heapifyAll() {
    for(int i = data.size() / 2 - 1; i >= 0; i--)
        heapify(i);
}

template<typename T>
void PriorityQueue<T>::changePriority(T &value, int priority) {
    for(int i = offset; i < data.size(); i++) {
        if(data[i].value == value) {
            data[i].priority = priority;
            heapifyAll();
            return;
        }
    }
}

template<typename T>
int PriorityQueue<T>::size() {
    return data.size();
}

template<typename T>
T PriorityQueue<T>::pop() {
    T value = data[0].value;
    data[0] = data[data.size()-1];
    data.remove(data.size()-1);
    heapifyAll();
    return value;
}

template<typename T>
T &PriorityQueue<T>::peek() {
    return data[0].value;
}


template<typename T>
void PriorityQueue<T>::push(T &&value, int priority) {
    push(value, priority);
}

template<typename T>
void PriorityQueue<T>::push(T &value, int priority) {
    if(data.size() == 0)
        data.pushBack(Node(value, priority));
    else {
        data.pushBack(Node(value, priority));
        heapifyAll();
    }
}

template<typename T>
void PriorityQueue<T>::heapify(int index) {       // min heap
    int left = 2 * index + 1;
    int right = 2 * index + 2;
    int smallest = index;
    if (left < data.size() && data[left].priority < data[smallest].priority)
        smallest = left;
    if (right < data.size() && data[right].priority < data[smallest].priority)
        smallest = right;

    if (smallest != index) {
        Node temp = data[index];
        data[index] = data[smallest];
        data[smallest] = temp;
        heapify(smallest);
    }
}
