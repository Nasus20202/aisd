#pragma once

template<typename T>
void swap(T& t1, T& t2){
    T temp = t1;
    t1 = t2;
    t2 = temp;
}
typedef unsigned short blockSize_t;      // integer type that is capable of storing blockSize
template<typename T, blockSize_t blockSize = 4>
class List {
public:
    typedef unsigned long listSize_t;       // max amount of elements in the list
protected:
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
    listSize_t _size = 0;

public:
    List();
    ~List();
    List(const List &other);
    void pushBack (T &element);
    void pushBack (T &&element);
    T& operator[](listSize_t index);
    List<T, blockSize>& operator=(List &other);
    List<T, blockSize> operator+(List &other);
    List<T, blockSize>& operator+=(List &other);
    bool operator==(List &other) const;
    listSize_t getSize() const;
    listSize_t length() const;
    listSize_t size() const;
    void remove(listSize_t index);
    void remove(T &search);
    void empty();
protected:
    typename Node::Element* getElement(listSize_t n);
    typename Node::Element* getElementFromNode(Node* node, listSize_t n, listSize_t &current);
    Node* getNodeOfElement(listSize_t n);
    Node* getNodeOfElement(listSize_t n, listSize_t &current);
    friend class CssParser;
    friend class Block;
};


template<typename T, blockSize_t blockSize>
void List<T, blockSize>::empty() {
    Node *node = first;
    while(node != nullptr){
        Node *copy = node;
        node = node->next;
        delete copy;
    }
    last = first = nullptr;
    _size = 0;
}

template<typename T, blockSize_t blockSize>
bool List<T, blockSize>::operator==(List &other) const{
    if(this == &other)
        return true;
    if(_size != other.getSize())
        return false;
    for(int i = 0; i < _size; i++){
        if((*this)[i] != other[i])
            return false;
    }
    return true;
}

template<typename T, blockSize_t blockSize>
List<T, blockSize> &List<T, blockSize>::operator+=(List &other) {
    if(this == &other){
        List<T, blockSize> newList = *this;
        newList += other;
        *this = newList;
    } else {
        Node *node = other.first;
        while(node != nullptr){
            for(auto element : node->elements){
                if(!element.free){
                    pushBack(element.value);
                }
            }
            node = node->next;
        }
    }
    return *this;
}

template<typename T, blockSize_t blockSize>
List<T, blockSize> List<T, blockSize>::operator+(List &other) {
    List<T, blockSize> temp(*this);
    temp += other;
    return temp;
}

template<typename T, blockSize_t blockSize>
List<T, blockSize>::List() {}

template<typename T, blockSize_t blockSize>
List<T, blockSize>::List(const List &other) : first(nullptr), last(nullptr), _size(0){
    Node *node = other.first;
    while(node != nullptr){
        for(auto element : node->elements){
            if(!element.free){
                pushBack(element.value);
            }
        }
        node = node->next;
    }
}

template<typename T, blockSize_t blockSize>
List<T, blockSize>::~List() {
    empty();
}

template<typename T, blockSize_t blockSize>
T &List<T, blockSize>::operator[](listSize_t index) {
    return getElement(index)->value;
}

template<typename T, blockSize_t blockSize>
List<T, blockSize> &List<T, blockSize>::operator=(List &other) {
    if(this == &other)
        return *this;
    List temp = other;
    swap(first, temp.first);
    swap(last, temp.last);
    _size = temp._size;
    return *this;
}

template<typename T, blockSize_t blockSize>
void List<T, blockSize>::Node::add(T &value) {
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

template<typename T, blockSize_t blockSize>
void List<T, blockSize>::Node::add(T &&value) {
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

template<typename T, blockSize_t blockSize>
void List<T, blockSize>::pushBack(T &element) {
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
    _size++;
}

template<typename T, blockSize_t blockSize>
void List<T, blockSize>::pushBack(T &&element) {
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
    _size++;
}


template<typename T, blockSize_t blockSize>
void List<T, blockSize>::remove(T &search) {
    Node *node = first;
    int id = 0;
    while(node != nullptr){
        for(auto element : node->elements){
            if(!element.free){
                if(element.value == search)
                    remove(id);
                id++;
            }
        }
        node = node->next;
    }
}

template<typename T, blockSize_t blockSize>
void List<T, blockSize>::remove(List::listSize_t index) {
    listSize_t current;
    Node* node = getNodeOfElement(index, current);
    typename Node::Element* element = getElementFromNode(node, index, current);
    element->free = true;
    node->elementCount--;
    _size--; bool empty = true;
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

template<typename T, blockSize_t blockSize>
typename List<T, blockSize>::Node *List<T, blockSize>::getNodeOfElement(List::listSize_t n, List::listSize_t &current) {
    if(n > _size)
        return nullptr;
    Node* node;
    if(n < _size / 2){ // start from the last
        node = first; current = 0;
        while(current + node->elementCount <= n){
            current += node->elementCount;
            node = node->next;
        }
        return node;

    } else {        // start from the first
        node = last; current = _size - 1;
        while(current >= node->elementCount && current - node->elementCount >= n){
            current -= node->elementCount;
            node = node->previous;
        }
        return node;
    }
}

template<typename T, blockSize_t blockSize>
typename List<T, blockSize>::Node *List<T, blockSize>::getNodeOfElement(List::listSize_t n) {
    listSize_t current;
    getNodeOfElement(n, current);
}

template<typename T, blockSize_t blockSize>
typename List<T, blockSize>::Node::Element *List<T, blockSize>::getElementFromNode(List::Node *node, List::listSize_t n, List::listSize_t &current) {
    if(n < _size / 2){ // start from the last
        typename List<T, blockSize>::Node::Element *element = node->elements;
        while(current != n || element->free){
            if(!element->free && current < n)
                current++;
            element++;
        }
        return element;

    } else {        // start from the first
        typename List<T, blockSize>::Node::Element *element = &node->elements[blockSize-1]; // start from last one in that block
        while(current != n || element->free){
            if(!element->free && current > n)
                current--;
            element--;
        }
        return element;
    }
}

template<typename T, blockSize_t blockSize>
typename List<T, blockSize>::Node::Element *List<T, blockSize>::getElement(List::listSize_t n) {
    if(n > _size)
        return nullptr;
    listSize_t current;
    Node* node = getNodeOfElement(n, current);
    return getElementFromNode(node, n, current);
}

template<typename T, blockSize_t blockSize>
typename List<T, blockSize>::listSize_t List<T, blockSize>::getSize() const {
    return _size;
}

template<typename T, blockSize_t blockSize>
typename List<T, blockSize>::listSize_t List<T, blockSize>::length() const {
    return getSize();
}

template<typename T, blockSize_t blockSize>
typename List<T, blockSize>::listSize_t List<T, blockSize>::size() const {
    return getSize();
}