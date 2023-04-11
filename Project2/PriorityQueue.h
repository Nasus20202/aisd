#pragma once
#include "Vector.h"
// Reference: https://www.geeksforgeeks.org/priority-queue-using-binary-heap/ (no code was copied from this site)
// and https://gist.github.com/jizhilong/6774742 (no code was copied from this site)

template <typename T>
class PriorityQueue {
private:
    void swap(int index1, int index2);
    bool isMaxHeap = true;
    Vector<T> data;
    int elementCount = 0;
    bool compare(T &a, T &b);
    bool compareIndex(int index1, int index2);
    static int getLeftChildIndex(int index);
    static int getRightChildIndex(int index);
    static int getParentIndex(int index);
    void shiftUp(int index);
    void shiftDown(int index);
public:
    int size();
    void push(T &value);
    void push(T &&value);
    T pop();
    T& peek();
    T& operator[](int index);
    explicit PriorityQueue(bool isMaxHeap = true) : isMaxHeap(isMaxHeap) {}
};

template<typename T>
void PriorityQueue<T>::swap(int index1, int index2) {
    T temp = data[index1];
    data[index1] = data[index2];
    data[index2] = temp;
}

template<typename T>
T &PriorityQueue<T>::operator[](int index) {
    return data[index];
}

template<typename T>
void PriorityQueue<T>::push(T &&value) {
    elementCount++;
    if(elementCount <= data.size())
        data[elementCount-1] = value;
    else
        data.pushBack(value);
    shiftUp(elementCount-1);
}


template<typename T>
void PriorityQueue<T>::push(T &value) {
    elementCount++;
    if(elementCount <= data.size())
        data[elementCount-1] = value;
    else
        data.pushBack(value);
    shiftUp(elementCount-1);
}

template<typename T>
int PriorityQueue<T>::size() {
    return elementCount;
}

template<typename T>
T &PriorityQueue<T>::peek() {
    return data[0];
}

template<typename T>
T PriorityQueue<T>::pop() {
    T value = data[0];
    swap(0, elementCount-1);
    elementCount--;
    shiftDown(0);
    return value;
}

template<typename T>
void PriorityQueue<T>::shiftDown(int index) {
    T value = data[index];
    int leftChildIndex = getLeftChildIndex(index);
    while (leftChildIndex < elementCount) {
        if(leftChildIndex+1 < elementCount && compareIndex(leftChildIndex, leftChildIndex+1))
            leftChildIndex++;
        if(compareIndex(index, leftChildIndex)){
            data[index] = data[leftChildIndex];
            index = leftChildIndex;
            leftChildIndex = getLeftChildIndex(index);
        } else
            break;
    }
    data[index] = value;
}

template<typename T>
void PriorityQueue<T>::shiftUp(int index) {
    for(int i = getParentIndex(index); i>=0 && compareIndex(i, index) && i != index; index = i, i = getParentIndex(index))
        swap(i, index);
}


template<typename T>
bool PriorityQueue<T>::compareIndex(int index1, int index2) {
    return compare(data[index1], data[index2]);
}

template<typename T>
bool PriorityQueue<T>::compare(T &a, T &b) {
    if(isMaxHeap)
        return !(a > b);
    return a > b;
}

template<typename T>
int PriorityQueue<T>::getLeftChildIndex(int index) { return (2*index + 1); }

template<typename T>
int PriorityQueue<T>::getRightChildIndex(int index) { return (2*index + 2); }

template<typename T>
int PriorityQueue<T>::getParentIndex(int index) { return (index-1)/2; }
