#pragma once
#include <initializer_list>

template<typename T>
void swap(T& t1, T& t2){
    T temp = t1;
    t1 = t2;
    t2 = temp;
}

template<typename T>
class List {
public:
    typedef unsigned long listSize_t;       // max amount of elements in the list
protected:
    typedef unsigned char blockSize_t;      // integer type that is capable of storing blockSize
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
    ~List();
    List(List &other);
    List(std::initializer_list<T> list);
    void pushBack (T &element);
    void pushBack (T &&element);
    T& operator[](listSize_t index);
    List<T>& operator=(List &other);
    List<T>& operator+(List &other);
    List<T>& operator+=(List &other);
    bool operator==(List &other);
    listSize_t getSize() const;
    void remove(listSize_t index);
    void remove(T &element);
    void empty();
protected:
    typename Node::Element* getElement(listSize_t n);
    typename Node::Element* getElementFromNode(Node* node, listSize_t n, listSize_t &current);
    Node* getNodeOfElement(listSize_t n);
    Node* getNodeOfElement(listSize_t n, listSize_t &current);
};

template<typename T>
void List<T>::empty() {
    Node *node = first, *copy;
    while(node != nullptr){
        copy = node;
        node = node->next;
        delete copy;
    }
    last = first = nullptr;
    size = 0;
}

template<typename T>
bool List<T>::operator==(List &other) {
    if(this == &other)
        return true;
    if(size != other.getSize())
        return false;
    for(int i = 0; i < size; i++){
        if((*this)[i] != other[i])
            return false;
    }
    return true;
}

template<typename T>
List<T> &List<T>::operator+=(List &other) {
    if(this == &other){
        List<T> newList = *this;
        for(int i = 0; i < other.getSize(); i++){
            newList.pushBack(other[i]);
        }
        *this = newList;
    } else {
        for (int i = 0; i < other.getSize(); i++) {
            pushBack(other[i]);
        }
    }
    return *this;
}

template<typename T>
List<T> &List<T>::operator+(List &other) {
    List<T> *temp = new List<T>();
    *temp += *this;
    *temp += other;
    return *temp;
}

template<typename T>
List<T>::List() {}

template<typename T>
List<T>::List(List &other) : first(nullptr), last(nullptr), size(0){
    for(int i = 0; i < other.getSize(); i++){
        pushBack(other[i]);
    }
}


template<typename T>
List<T>::List(std::initializer_list<T> list) {
    for(T elem : list)
        pushBack(elem);
}

template<typename T>
List<T>::~List() {
    empty();
}

template<typename T>
T &List<T>::operator[](listSize_t index) {
    return getElement(index)->value;
}

template<typename T>
List<T> &List<T>::operator=(List &other) {
    if(this == &other)
        return *this;
    List temp = other;
    swap(first, temp.first);
    swap(last, temp.last);
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
void List<T>::remove(T &element) {
    for(int i = 0; i < getSize(); i++){
        if((*this)[i] == element){
            remove(i);
            i--;
        }
    }
}

template<typename T>
void List<T>::remove(List::listSize_t index) {
    listSize_t current;
    Node* node = getNodeOfElement(index, current);
    typename Node::Element* element = getElementFromNode(node, index, current);
    element->free = true;
    node->elementCount--;
    size--; bool empty = true;
    for(int i = 0; i < blockSize; i++){
        if(!node->elements[i].free){
            empty = false;
            break;
        }
    }
    if(empty){
        if(node->previous != nullptr)
            node->previous->next = node->next;
        if(node->next != nullptr)
            node->next->previous = node->previous;
        if(node == first)
            first = node->next;
        if(node == last)
            last = node->previous;
        delete node;
    }
}

template<typename T>
typename List<T>::Node *List<T>::getNodeOfElement(List::listSize_t n, List::listSize_t &current) {
    if(n > size)
        return nullptr;
    Node* node;
    if(n < size/2){ // start from the last
        node = first; current = 0;
        while(current + node->elementCount <= n){
            current += node->elementCount;
            node = node->next;
        }
        return node;

    } else {        // start from the first
        node = last; current = size-1;
        while(current >= node->elementCount && current - node->elementCount >= n){
            current -= node->elementCount;
            node = node->previous;
        }
        return node;
    }
}

template<typename T>
typename List<T>::Node *List<T>::getNodeOfElement(List::listSize_t n) {
    listSize_t current;
    getNodeOfElement(n, current);
}

template<typename T>
typename List<T>::Node::Element *List<T>::getElementFromNode(List::Node *node, List::listSize_t n, List::listSize_t &current) {
    if(n < size/2){ // start from the last
        typename List<T>::Node::Element *element = node->elements;
        while(current != n || element->free){
            if(!element->free && current < n)
                current++;
            element++;
        }
        return element;

    } else {        // start from the first
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
typename List<T>::Node::Element *List<T>::getElement(List::listSize_t n) {
    if(n > size)
        return nullptr;
    listSize_t current;
    Node* node = getNodeOfElement(n, current);
    return getElementFromNode(node, n, current);
}

template<typename T>
typename List<T>::listSize_t List<T>::getSize() const {
    return size;
}