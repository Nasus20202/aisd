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
    String& operator+(char array[]);
    String& operator+=(char array[]);
    String& operator+(char c);
    String& operator+=(char c);
    bool operator==(String &other);
    bool operator==(char array[]);
    bool operator!=(String &other);
    bool operator!=(char array[]);
    List<String>& split(char c);
    List<String>& split(String &other);
    stringSize_t length() const;
    stringSize_t size() const;
    stringSize_t getSize() const;
    bool contains(String &other);
    bool contains(char* array);
    void remove(char c);
    void remove(stringSize_t index);
    void remove(String &other);
    void empty();
protected:
    friend std::ostream& operator<<(std::ostream &os, String& s);
    friend std::istream& operator>>(std::istream &is, String& s);
};
