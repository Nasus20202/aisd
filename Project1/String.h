#pragma once
#include "List.h"
#include <istream>

class String {
protected:
    char* characters = nullptr;
    unsigned int stringSize = 0;
    unsigned int allocated = 0;
public:
    String();
    ~String();
    String(char array[]);
    String(String &other);
    char& operator[](int index);
    String& operator=(String &other);
    String& operator=(char array[]);
    String& operator+(char c);
    String& operator+=(char c);
    bool operator==(String &other);
    bool operator==(char array[]);
    bool operator!=(String &other);
    bool operator!=(char array[]);
    List<String>& split(char c);
    int length() const;
    int size() const;
    int getSize() const;
    bool contains(char c);
    void remove(char c);
    void remove(int index);
    void empty();
    int toInt();
    bool isInt();
    int count(char c);
    void add(char c);
protected:
    friend std::ostream& operator<<(std::ostream &os, String& s);
    friend std::istream& operator>>(std::istream &is, String& s);
};
