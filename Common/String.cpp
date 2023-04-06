#include "String.h"

String::String() {}

String::String(const char *array) {
    while(*array != '\0'){
        add(*array);
        array++;
    }
}

String::String(String &other) {
    if(other.allocated == 0)
        return;
    char *newChars = new char[other.allocated];
    for(int i = 0; i <= other.stringSize; i++)
        newChars[i] = other.characters[i];
    empty();
    characters = newChars;
    stringSize = other.stringSize;
    allocated = other.allocated;
}

char &String::operator[](int index) {
    return characters[index];
}


String &String::operator=(String &other) {
    String temp = other;
    swap(characters, temp.characters);
    swap(allocated, temp.allocated);
    swap(stringSize, temp.stringSize);
    return *this;
}

String &String::operator=(const char *array) {
    empty();
    while(*array != '\0'){
        *this+=*array;
        array++;
    }
    return *this;
}

String String::operator+(char c) {
    String s(*this);
    s += c;
    return s;
}

String &String::operator+=(char c) {
    add(c);
    return *this;
}

bool String::operator==(String &other) const{
    if(this->stringSize != other.stringSize)
        return false;
    for(int i = 0; i < stringSize; i++)
        if(characters[i] != other.characters[i])
            return false;
    return true;
}

bool String::operator==(const char *array) const{
    int size = 0;
    while(*array != '\0'){
        if(size >= stringSize)
            return false;
        if(characters[size] != *array)
            return false;
        array++;
        size++;
    }
    if(size != stringSize)
        return false;
    return true;
}


List<String> String::split(char c) const{
    List<String> list;
    String s;
    for(int i = 0; i < length(); i++){
        char currentChar = characters[i];
        if(currentChar == c){
            if(s.length() > 0)
                list.pushBack(s);
            s.empty();
        }
        else {
            s += currentChar;
        }
    }
    if(s.length()>0)
        list.pushBack(s);
    return list;
}

void String::remove(char c) {
    String s;
    for(int i = 0; i < stringSize; i++){
        if(characters[i] != c)
            s.add(characters[i]);
    }
    swap(characters, s.characters);
    swap(stringSize, s.stringSize);
    swap(allocated, s.allocated);
}

void String::remove(int index) {
    String s;
    for(int i = 0; i < stringSize; i++){
        if(i != index)
            s.add(characters[i]);
    }
    swap(characters, s.characters);
    swap(stringSize, s.stringSize);
    swap(allocated, s.allocated);
}


void String::empty() {
    if(characters != nullptr)
        delete[] characters;
    allocated = 0;
    stringSize = 0;
    characters = nullptr;
}

std::ostream &operator<<(std::ostream &os, String &s) {
    int size = s.length();
    for(int i = 0; i < size; i++)
        os << s[i];
    return os;
}

std::istream &operator>>(std::istream &is, String &s) {
    s.empty();
    bool done = false;
    while(!done) {
        char c = is.get();
        if(is.eof())
            break;
        if (c == '\0' || c == '\n')
            done = true;
        else
            s.add(c);
    }
    return is;
}


void String::add(char c) {
    if(stringSize+1 >= allocated){
        if(allocated == 0)
            allocated = 2;
        allocated*=2;
        char* newCharacters = new char[allocated];
        for(int i = 0 ; i < stringSize; i++)
            newCharacters[i] = characters[i];
        char* old = characters;
        characters = newCharacters;
        if(old != nullptr)
            delete[] old;
    }
    characters[stringSize] = c;
    characters[stringSize+1] = '\0';
    stringSize++;
}

int String::getSize() const {
    return stringSize;
}
int String::size() const {
    return getSize();
}
int String::length() const {
    return getSize();
}


bool String::contains(char c) const{
    for(int i = 0; i < size(); i++){
        if(characters[i] == c)
            return true;
    }
    return false;
}

int String::toInt() const{
    int result = 0;
    for(int i = 0; i < size(); i++){
        if(characters[i] == '-')
            continue;
        result *= 10;
        result += characters[i] - '0';
    }
    if(characters[0] == '-')
        result*=-1;
    return result;
}

bool String::isInt() const{
    for(int i = 0; i < size(); i++){
        char c = characters[i];
        if(i == 0 && c == '-')
            continue;
        if(c < '0' || c > '9')
            return false;
    }
    return true;
}

int String::count(char c) const{
    int count = 0;
    for(int i = 0; i < size(); i++){
        char cs = characters[i];
        if(c  == cs)
            count++;
    }
    return count;
}

String::~String() {
    empty();
}

bool String::operator!=(String &other) const{
    return !(*this == other);
}

bool String::operator!=(const char *array) const{
    return !(*this == array);
}

String String::operator+(String &s) {
    String temp(*this);
    temp += s;
    return temp;
}

String &String::operator+=(String &s) {
    for(int i = 0; i < s.stringSize; i++)
        add(s[i]);
    return *this;
}

unsigned int String::hash() const {
    unsigned int hash = 0;
    unsigned int base = 1;
    for(int i = 0; i < stringSize; i++){
        char c = characters[i] - ' ' + 1;
        hash += c*base;
        base*=97;
    }
    return hash;
}

