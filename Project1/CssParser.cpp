#include "CssParser.h"
#include <iostream>
using namespace std;

void CssParser::loadLine(String &line) {
    if(line.length() == 0)
        return;
    removeUselessWhitespace(line);
    if(line == (char*) "????"){
        parsing = false;
    }
    else if (line == (char*) "****") {
        parsing = true;
    }
    else if(!parsing){ // commands
        query(line);
    }
    else { // parse line
        parse(line);
    }
}

void CssParser::query(String &query) {
    cout << query << " == ";
    if(query == (char*) "?"){
        cout << blocks.size();
    } else {
        List<String> commands = query.split(',');
        char command = commands[1][0];
    }
    cout << endl;
}

void CssParser::parse(String &line) {
    if(blockOpen){
        if(line.contains('}')){
            blockOpen = false;
            line.remove('}');
        }
        List<String> parts = line.split('}');
        if(parts.size() > 0)
            addToBlock(parts[0]);
    } else {
        if(isGlobalAttribute(line)){
            addToBlock(line);
        } else {
            if(line.contains('{'))
                blockOpen = true;
            List<String> parts = line.split('{');
            if(parts.size() == 0)
                return;
            String selector = parts[0];
            removeUselessWhitespace(selector);
            Block* block = getBlock(selector);
            if(block != nullptr){
                currentBlock = block;
            } else {
                Block newBlock(selector);
                blocks.pushBack(newBlock);
                currentBlock = getBlock(selector);
            }
        }
    }
}

bool CssParser::isGlobalAttribute(String &line) {
    if(!line.contains(':'))
        return false;
    for(int i = 0; i < line.size(); i++){
        char c = line[i];
        if(c == ';')
            return true;
        if(c == '{' || c == '}')
            return false;
    }
    return false;
}

void CssParser::addToBlock(String &line) {
    List<String> attributes = line.split(';');
    for(int i = 0; i < attributes.size(); i++){
        List<String> attribute = attributes[i].split(':');
        Attribute a;
        removeUselessWhitespace(attribute[0]);
        removeUselessWhitespace(attribute[1]);
        a.name = attribute[0];
        a.value = attribute[1];
        if(blockOpen)
            currentBlock->addAttribute(a);
        else
            globalBlock.addAttribute(a);
    }
}

Block* CssParser::getBlock(String &selector) {
    typedef List<Block>::Node blockNode;
    blockNode* node = blocks.first;
    while(node != nullptr){
        for(auto & element : node->elements){
            if(!element.free && element.value.selector == selector)
                return &element.value;
        }
        node = node->next;
    }
    return nullptr;
}


void CssParser::removeUselessWhitespace(String &line) {
    while(line[0] == '\t' || line[0] == ' ')
        line.remove((String::stringSize_t)0);
    while(line[line.size()-1] == ' ' || line[line.size()-1] == '\t')
        line.remove(line.size()-1);
}

Block::Block(String &selector) {
    this->selector = selector;
    List<String> selectors = selector.split(',');
    for(int i = 0; i < selectors.size(); i++)
        CssParser::removeUselessWhitespace(selectors[i]);
    this->selectors = selectors;
}

Block::Block() : selector(), selectors(), attributes() {}

void Block::addAttribute(Attribute &attribute) {
    typedef List<Attribute>::Node blockNode;
    blockNode* node = attributes.first;
    while(node != nullptr){
        for(auto & element : node->elements){
            if(!element.free && element.value.name == attribute.name) {
                element.value.name = attribute.value;
                return;
            }
        }
        node = node->next;
    }
    attributes.pushBack(attribute);
}

