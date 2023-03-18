#include "String.h"

String::String() {}

String::String(char *array) {
    while(*array != '\0'){
        *this+=*array;
        array++;
    }
}

String::String(String &other) {
    characters = other.characters;
}

char &String::operator[](String::stringSize_t index) {
    return characters[index];
}


String &String::operator=(String &other) {
    String temp = other;
    swap(characters, temp.characters);
    return *this;
}

String &String::operator=(char *array) {
    characters.empty();
    while(*array != '\0'){
        *this += *array;
        array++;
    }
    return *this;
}

String &String::operator+(String &other) {
    String *temp = new String(*this);
    *temp += other;
    return *temp;
}

String &String::operator+=(String &other) {
    for(stringSize_t i = 0; i < other.length(); i++){
        *this += other[i];
    }
    return *this;
}

String &String::operator+(char c) {
    String *temp = new String(*this);
    *temp += c;
    return *temp;
}

String &String::operator+=(char c) {
    characters.pushBack(c);
    return *this;
}

bool String::operator==(String &other) {
    if(this == &other)
        return true;
    if(length() != other.length())
        return false;
    for(int i = 0; i < length(); i++)
        if((*this)[i] != other[i])
            return false;
    return true;
}

bool String::operator==(char *array) {
    stringSize_t index = 0;
    while(*array != '\0'){
        if(*array != (*this)[index])
            return false;
        array++; index++;
    }
    if(index != length())
        return false;
    return true;
}

List<String>& String::split(char c) {
    List<String> *list = new List<String>();
    String s; char currentChar;
    for(stringSize_t i = 0; i < length(); i++){
        currentChar = (*this)[i];
        if(currentChar == c){
            if(s.length() > 0)
                list->pushBack(s);
            s.empty();
        }
        else {
            s += currentChar;
        }
    }
    if(s.length()>0)
        list->pushBack(s);
    return *list;
}

void String::remove(char c) {
    characters.remove(c);
}

void String::remove(String::stringSize_t index) {
    characters.remove(index);
}

void String::empty() {
    characters.empty();
}

std::ostream &operator<<(std::ostream &os, String &s) {
    String::stringSize_t size = s.length();
    for(String::stringSize_t i = 0; i < size; i++)
        os << s[i];
    return os;
}

std::istream &operator>>(std::istream &is, String &s) {
    //s.empty();
    char c; bool done = false;
    while(!done) {
        c = is.get();
        if (c == '\0' || c == '\n')
            done = true;
        else
            s.characters.pushBack(c);
    }
    return is;
}
String::stringSize_t String::getSize() const {
    return characters.getSize();
}
String::stringSize_t String::size() const {
    return getSize();
}
String::stringSize_t String::length() const {
    return getSize();
}


