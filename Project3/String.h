#pragma once
#include "Vector.h"
#include <istream>

class String {
protected:
    char* characters = nullptr;
    unsigned int stringSize = 0;
    unsigned int allocated = 0;
public:
    String();
    ~String();
    String(const char array[]);
    String(String &other);
    char& operator[](int index);
    String& operator=(String &other);
    String& operator=(const char array[]);
    String operator+(char c);
    String& operator+=(char c);
    String operator+(String& s);
    String& operator+=(String& s);
    bool operator==(String &other) const;
    bool operator==(const char array[]) const;
    bool operator!=(String &other) const;
    bool operator!=(const char array[]) const;
    Vector<String> split(char c) const;
    int length() const;
    int size() const;
    int getSize() const;
    bool contains(char c) const;
    bool startsWith(const char array[]) const;
    void remove(char c);
    void remove(int index);
    void empty();
    int toInt() const;
    bool isInt() const;
    int count(char c) const;
    void add(char c);
    unsigned int hash() const;
protected:
    friend std::ostream& operator<<(std::ostream &os, String& s);
    friend std::istream& operator>>(std::istream &is, String& s);
};
