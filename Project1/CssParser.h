#pragma once
#include "List.h"
#include "String.h"

struct Attribute {
    String name, value;
};

struct Block {
    String selector;
    List<String> selectors;
    List<Attribute> attributes;
    Block();
    Block(String& selector);
    void addAttribute(Attribute &attribute);
};

class CssParser {
protected:
    List<Block> blocks;
    Block globalBlock;
    bool blockOpen = false;
    bool parsing = true;
    Block* currentBlock = nullptr;
public:
    void loadLine(String& line);
    void query(String &query);
    void parse(String &line);
    bool isGlobalAttribute(String &line);
    static void removeUselessWhitespace(String &line);
    void addToBlock(String& line);
    Block *getBlock(String& selector); // returns nullptr if not found
};