#pragma once

template<typename T>
class Vector {
protected:
    T* data = nullptr;
    unsigned int elementCount = 0, allocated = 0;
    static const int resizeFactor = 2;
public:
    Vector();
    ~Vector();
    Vector(int size);
    Vector(int size, T &value);
    Vector(int size, T &&value);
    Vector(Vector &&other);
    Vector(const Vector& other);
    T& operator[](int index);
    Vector<T>& operator=(Vector &other);
    Vector<T> operator+(T &value) const;
    Vector<T>& operator+=(T &value);
    Vector<T> operator+(Vector &other) const;
    Vector<T>& operator+=(Vector &other);
    bool operator==(Vector &other) const;
    bool operator!=(Vector &other) const;
    int length() const;
    int getSize() const;
    int size() const;
    void pushBack(T &value);
    void add(T &value, int index);
    void pushBack(T &&value);
    void add(T &&value, int index);
    void remove(int index);
    void empty();
    void resize(int newSize);
};

template<typename T>
Vector<T>::Vector(int size, T &&value) {
    resize(size);
    elementCount = size;
    for(int i = 0; i < size; i++)
        data[i] = value;
}

template<typename T>
Vector<T>::Vector(int size, T &value) {
    resize(size);
    elementCount = size;
    for(int i = 0; i < size; i++)
        data[i] = value;
}

template<typename T>
Vector<T>::Vector(int size) {
    resize(size);
}

template<typename T>
void Vector<T>::add(T &&value, int index){
    add(value, index);
}

template<typename T>
void Vector<T>::pushBack(T &&value) {
    pushBack(value);
}

template<typename T>
void Vector<T>::resize(int newSize) {
    T* newData = new T[newSize];
    for(int i = 0; i < elementCount; i++){
        newData[i] = data[i];
    }
    delete[] data;
    data = newData;
    allocated = newSize;
}

template<typename T>
void Vector<T>::empty() {
    delete[] data;
    data = nullptr;
    elementCount = 0;
    allocated = 0;
}

template<typename T>
void Vector<T>::remove(int index) {
    if(index < 0 || index >= elementCount)
        return;
    for(int i = index; i < elementCount - 1; i++){
        data[i] = data[i+1];
    }
    elementCount--;
}

template<typename T>
void Vector<T>::add(T &value, int index) {
    if(index < 0)
        index = 0;
    if(index > elementCount)
        index = elementCount;
    if(elementCount == allocated){
        resize(resizeFactor*(allocated+1));
    }
    for(int i = elementCount; i > index; i--){
        data[i] = data[i-1];
    }
    data[index] = value;
    elementCount++;
}

template<typename T>
void Vector<T>::pushBack(T &value) {
    if(elementCount == allocated){
        resize(resizeFactor*(allocated+1));
    }
    data[elementCount++] = value;
}

template<typename T>
int Vector<T>::size() const {
    return getSize();
}

template<typename T>
int Vector<T>::getSize() const {
    return elementCount;
}

template<typename T>
int Vector<T>::length() const {
    return getSize();
}

template<typename T>
bool Vector<T>::operator!=(Vector<T> &other) const {
    return !(*this == other);
}

template<typename T>
bool Vector<T>::operator==(Vector<T> &other) const {
    if(this == &other)
        return true;
    if(elementCount != other.getSize())
        return false;
    for(int i = 0; i < elementCount; i++){
        if(data[i] != other[i])
            return false;
    }
    return true;
}

template<typename T>
Vector<T> &Vector<T>::operator+=(Vector<T> &other) {
    for(int i = 0; i < other.getSize(); i++){
        pushBack(other[i]);
    }
    return *this;
}

template<typename T>
Vector<T> Vector<T>::operator+(Vector<T> &other) const {
    Vector<T> result = *this;
    result += other;
    return result;
}

template<typename T>
Vector<T> &Vector<T>::operator+=(T &value) {
    pushBack(value);
    return *this;
}

template<typename T>
Vector<T> Vector<T>::operator+(T &value) const {
    Vector<T> result = *this;
    result += value;
    return result;
}

template<typename T>
Vector<T> &Vector<T>::operator=(Vector<T> &other) {
    if(this == &other)
        return *this;
    empty();
    data = new T[other.allocated];
    for(int i = 0; i < other.getSize(); i++)
        data[i] = other[i];
    elementCount = other.getSize();
    allocated = other.allocated;
    return *this;
}

template<typename T>
T &Vector<T>::operator[](int index) {
    if(index < 0 || index >= elementCount)
        throw;
    return data[index];
}

template<typename T>
Vector<T>::Vector(Vector &&other) {
    *this = other;
}
template<typename T>
Vector<T>::Vector(const Vector& other) {
    *this = other;
}

template<typename T>
Vector<T>::~Vector() {
    empty();
}

template<typename T>
Vector<T>::Vector() {

}
