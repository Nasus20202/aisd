#pragma once
#include "List.h"
#include "String.h"

struct Attribute {
    String name, value;
};

struct Block {
    typedef List<Attribute>::Node attributeNode;
    String selector;
    List<String> selectors;
    List<Attribute> attributes;
    bool global;
    Block();
    Block(String& selector);
    void addAttribute(Attribute &attribute);
    Attribute* getAttributeByName(String &name); // returns nullptr if not found
};

class CssParser {
protected:
    typedef List<Block>::Node blockNode;
    List<Block> blocks;
    bool blockOpen = false;
    bool parsing = true;
public:
    void loadLine(String& line);
    void query(String &query);
    void parse(String &line);
    bool isGlobalAttribute(String &line);
    static void removeUselessWhitespace(String &line);
    void addToBlock(String& line);
    Block *getBlock(String& selector); // returns nullptr if not found
    Block *getLastBlock();
    unsigned int countAttribute(String &name);
    unsigned int countSelector(String &name);
    Attribute* getAttributeForSelector(String &selector, String &attribute);
};