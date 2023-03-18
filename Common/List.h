#pragma once
#include <initializer_list>

template<typename T>
class List {
protected:
    typedef unsigned char blockSize_t;      // integer type that is capable of storing blockSize
    typedef unsigned long listSize_t;       // max amount of elements in the list
    static const blockSize_t blockSize = 8; // max number of elements allocated in one node

    struct Node {
        struct Element{
            T value;
            bool free = true;
        };
        Element elements[blockSize];
        blockSize_t elementCount = 0, freeSpace = blockSize;
        Node *previous = nullptr, *next = nullptr;
        void add(T &element);
        void add(T &&element);
    };

    Node *first = nullptr, *last = nullptr;
    listSize_t size = 0;

public:
    List();
    List(std::initializer_list<T> list);
    ~List();
    List(List &other);
    void pushBack (T &element);
    void pushBack (T &&element);
    T& operator[](listSize_t index);
    List<T>& operator=(List &other);
    long getSize() const;
    void remove(listSize_t index);
protected:
    typename Node::Element* getElement(listSize_t n);
    Node* getNodeOfElement(listSize_t n);
};

template<typename T>
List<T>::List() {}

template<typename T>
List<T>::List(std::initializer_list<T> list) {
    for(T elem : list)
        pushBack(elem);
}

template<typename T>
List<T>::List(List &other) : first(nullptr), last(nullptr), size(0){
    for(int i = 0; i < other.getSize(); i++){
        pushBack(other[i]);
    }
}

template<typename T>
List<T>::~List() {
    Node *node = first, *copy;
    while(node != nullptr){
        copy = node;
        node = node->next;
        delete copy;
    }
}

template<typename T>
T &List<T>::operator[](listSize_t index) {
    return getElement(index)->value;
}

template<typename T>
List<T> &List<T>::operator=(List &other) {
    List temp = other;
    std::swap(first, temp.first);
    std::swap(last, temp.last);
    size = temp.size;
    return *this;
}

template<typename T>
void List<T>::Node::add(T &value) {
    if(freeSpace > 0){
        Node::Element element;
        element.value = value;
        element.free = false;
        elements[blockSize-freeSpace] = element;
        elementCount++;
        freeSpace--;
    } else{
        throw; // no space in block
    }
}

template<typename T>
void List<T>::Node::add(T &&value) {
    if(freeSpace > 0){
        Node::Element element;
        element.value = std::move(value);
        element.free = false;
        elements[blockSize-freeSpace] = element;
        elementCount++;
        freeSpace--;
    } else{
        throw; // no space in block
    }
}

template<typename T>
void List<T>::pushBack(T &element) {
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
void List<T>::pushBack(T &&element) {
    if(first == nullptr){
        first = new Node();
        last = first;
    }
    if(last->freeSpace == 0){
        Node* newNode = new Node();
        newNode->add(std::move(element));
        last->next = newNode;
        newNode->previous = last;
        last = newNode;
    } else {
        last->add(std::move(element));
    }
    size++;
}


template<typename T>
void List<T>::remove(List::listSize_t index) {

}


template<typename T>
typename List<T>::Node::Element *List<T>::getElement(List::listSize_t n) {
    if(n > size)
        return nullptr;
    Node* node; listSize_t current;
    if(n < size/2){ // start from the last
        node = first; current = 0;
        while(current + node->elementCount <= n){
            current += node->elementCount;
            node = node->next;
        }
        typename List<T>::Node::Element *element = node->elements;
        while(current != n || element->free){
            if(!element->free && current < n)
                current++;
            element++;
        }
        return element;

    } else {        // start from the first
        node = last; current = size-1;
        while(current >= node->elementCount && current - node->elementCount >= n){
            current -= node->elementCount;
            node = node->previous;
        }
        typename List<T>::Node::Element *element = &node->elements[List<T>::blockSize-1]; // start from last one in that block
        while(current != n || element->free){
            if(!element->free && current > n)
                current--;
            element--;
        }
        return element;
    }
}

template<typename T>
long List<T>::getSize() const {
    return size;
}