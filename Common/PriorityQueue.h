#pragma once
#include "Vector.h"
// Reference: https://www.geeksforgeeks.org/priority-queue-using-binary-heap/

template <typename T>
class PriorityQueue {
private:
    bool isMaxHeap = true;
    Vector<T> data;
    int elementCount = 0;
    bool compare(T &a, T &b);
    static int getLeftChildIndex(int index);
    static int getRightChildIndex(int index);
    static int getParentIndex(int index);

};