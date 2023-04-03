#pragma once
#include "List.h"

template <typename T>
class Queue {
private:
    List<T, 1> list;
public:
    void enqueue(T value);
    T dequeue();
    T peek();
    int size();
    bool isEmpty();
};

template <typename T>
void Queue<T>::enqueue(T value) {
    list.pushBack(value);
}

template <typename T>
T Queue<T>::dequeue() {
    T value = list[0];
    list.remove(0);
    return value;
}

template <typename T>
T Queue<T>::peek() {
    return list[0];
}

template <typename T>
int Queue<T>::size() {
    return list.length();
}

template <typename T>
bool Queue<T>::isEmpty() {
    return list.length() == 0;
}