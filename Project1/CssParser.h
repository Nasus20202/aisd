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
    Block();
    Block(String& selector);
    void addAttribute(Attribute &attribute);
    Attribute* getAttributeByName(String &name); // returns nullptr if not found
};

class CssParser {
protected:
    typedef List<Block>::Node blockNode;
    String previous;
    bool blockOpen = false;
    bool parsing = true;
    List<Block> blocks;
    void addToBlock(String& line);
    Block *getLastBlock();
    unsigned int countAttribute(String &name);
    unsigned int countSelector(String &name);
    Attribute* getAttributeForSelector(String &selector, String &attribute);
    bool deleteBlock(int blockId);
    bool deleteAttribute(int blockId, String& attributeName);
    bool isGlobalAttribute(String &line);
public:
    void loadLine(String& line);
    void query(String &query);
    void parse(String &line);
    static void removeUselessWhitespace(String &line);
};