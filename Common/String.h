#pragma once
#include "List.h"
#include <istream>

class String {
public:
    typedef List<char>::listSize_t stringSize_t;
protected:
    typedef List<char> charList;
    charList characters;
public:
    String();
    String(char array[]);
    String(String &other);
    char& operator[](charList::listSize_t index);
    String& operator=(String &other);
    String& operator=(char array[]);
    String& operator+(String &other);
    String& operator+=(String &other);
    String& operator+(char c);
    String& operator+=(char c);
    bool operator==(String &other);
    bool operator==(char array[]);
    List<String>& split(char c);
    stringSize_t length() const;
    void remove(char c);
    void remove(stringSize_t index);
    void empty();
protected:
    friend std::ostream& operator<<(std::ostream &os, String& s);
    friend std::istream& operator>>(std::istream &is, String& s);
};
