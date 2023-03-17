#pragma once

template<typename T>
class List {
    typedef unsigned char blockSize_t;      // integer type that is capable of storing blockSize
    typedef unsigned long listSize_t;       // max amount of elements in the list
    static const blockSize_t blockSize = 8; // max number of elements allocated in one node

    struct Node {
        T elements[blockSize];
        blockSize_t elementCount = 0, freeSpace = blockSize;
        Node *previous = nullptr, *next = nullptr;
        void add(const T &element);
    };

    Node *first = nullptr, *last = nullptr;
    listSize_t size = 0;

public:
    List();
    List(T array[], listSize_t size);
    ~List();
    void pushBack(const T &element);
    T popBack();
    T& operator[](listSize_t index);
    long getSize();
private:
    Node* getN(listSize_t n);
};

template<typename T>
typename List<T>::Node *List<T>::getN(List::listSize_t n) {
    Node* node;
    if(n > size/2){ // start from the last
        node = last;
    } else {        // start from the first
        node = first;
    }
}

template<typename T>
long List<T>::getSize() {
    return size;
}


template<typename T>
List<T>::List() {}


template<typename T>
List<T>::List(T *array, listSize_t size) {
    for(listSize_t i = 0; i < size; i++)
        pushBack(array[i]);
}

template<typename T>
List<T>::~List() {

}

template<typename T>
T &List<T>::operator[](listSize_t index) {

}

template<typename T>
void List<T>::Node::add(const T &element) {
    if(freeSpace > 0){
        elements[blockSize-freeSpace] = element;
        freeSpace--;
    } else{
        throw; // no space in block
    }
}

template<typename T>
void List<T>::pushBack(const T &element) {
    if(first == nullptr){
        first = new Node();
        last = first;
    }
    if(last->freeSpace == 0){
        Node* newNode = new Node();
        newNode->add(element);
        last->next = newNode;
        newNode->previous = last;
        last = newNode;
    } else {
        last->add(element);
    }
    size++;
}

template<typename T>
T List<T>::popBack() {

}