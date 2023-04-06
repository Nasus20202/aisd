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
    Vector<Node> data;
    void heapify(int index);
public:
    void push(T &&value, int priority = 1);
    void push(T &value, int priority = 1);
    int size();
    T& peek();
    T pop();
};

template<typename T>
int PriorityQueue<T>::size() {
    return data.size();
}

template<typename T>
T PriorityQueue<T>::pop() {
    T value = data[0].value;
    data[0] = data[data.size()-1];
    data.remove(data.size()-1);
    for(int i = data.size()/2-1; i >= 0; i--)
        heapify(i);
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
        for(int i = data.size()/2-1; i >= 0; i--)
            heapify(i);
    }
}

template<typename T>
void PriorityQueue<T>::heapify(int index) {
    int left = 2 * index + 1;
    int right = 2 * index + 2;
    int largest = index;
    if (left < data.size() && data[left].priority > data[largest].priority)
        largest = left;
    if (right < data.size() && data[right].priority > data[largest].priority)
        largest = right;

    if (largest != index) {
        Node temp = data[index];
        data[index] = data[largest];
        data[largest] = temp;
        heapify(largest);
    }
}
