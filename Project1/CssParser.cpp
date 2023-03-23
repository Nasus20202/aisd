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
    if(query == (char*) "?"){
        cout << query << " == " << blocks.size() << endl;
    } else {
        List<String> commands = query.split(',');
        String first = commands[0];
        char second = commands[1][0];
        String third = commands[2];
        bool isFirstInt = first.isInt(), isThirdInt = third.isInt();


        // i,S,?
        if(isFirstInt && second == 'S' && third[0] == '?'){
            int id = commands[0].toInt();
            if(id-1 >= blocks.size())
                return;
            cout << query << " == " << blocks[id-1].selectors.size() << endl;
        }
        // i,A,?
        else if(isFirstInt && second == 'A' && third[0] == '?') {
            int id = commands[0].toInt();
            if(id-1 >= blocks.size())
                return;
            cout << query << " == " << blocks[id-1].attributes.size() << endl;
        }
        // i,S,j
        else if(isFirstInt && second == 'S' && isThirdInt){
            int blockId = first.toInt(), selectorId = third.toInt();
            if(blockId-1 >= blocks.size())
                return;
            Block block = blocks[blockId-1];
            if(selectorId-1 >= block.selectors.size())
                return;
            cout << query << " == " << block.selectors[selectorId-1] << endl;
        }
        // i,A,n
        else if(isFirstInt && second == 'A'){
            int blockId = first.toInt();
            if(blockId-1 >= blocks.size())
                return;
            Block block = blocks[blockId-1];
            Attribute *attribute = block.getAttributeByName(third);
            if(attribute == nullptr)
                return;
            cout << query << " == " << attribute->value << endl;
        }
        // n,A,?
        else if(second == 'A' && third[0] == '?'){
            cout << query << " == " << countAttribute(first) << endl;
        }
        // z,S,?
        else if(second == 'S' && third[0] == '?'){
            cout << query << " == " << countSelector(first) << endl;
        }
        // z,E,n
        else if(second == 'E'){
            Attribute* attribute = getAttributeForSelector(first, third);
            if(attribute == nullptr)
                return;
            cout << query << " == " << attribute->value << endl;
        }
        // i,D,*
        else if(isFirstInt && second == 'D' && third[0] == '*'){

        }
        // i,D,n
        else if (isFirstInt && second == 'D'){

        }
        else {
            cout << "Not found: " << query;
            throw;
        }
    }
}

Attribute* CssParser::getAttributeForSelector(String &selector, String &attribute) {
    blockNode* blockNode = blocks.first;
    Attribute* result = nullptr;
    while(blockNode != nullptr){
        for(auto & blockElement : blockNode->elements){
            if(!blockElement.free){
                bool containsSelector = false;
                List<String>::Node *selectorNode = blockElement.value.selectors.first;
                while(selectorNode != nullptr){
                    for(auto & selectorElement : selectorNode->elements){
                        if(!selectorElement.free && selectorElement.value == selector) {
                            containsSelector = true; break;
                        }
                    }if(containsSelector)
                        break;
                    selectorNode=selectorNode->next;
                }
                if(!containsSelector)
                    continue;
                Block::attributeNode* attributeNode = blockElement.value.attributes.first;
                while(attributeNode != nullptr){
                    for(auto & attributeElement : attributeNode->elements){
                        if(!attributeElement.free && attributeElement.value.name == attribute) {
                            result = &attributeElement.value;
                        }
                    }
                    attributeNode = attributeNode->next;
                }
            }
        }
        blockNode = blockNode->next;
    }
    return result;
}

unsigned int CssParser::countAttribute(String &name) {
    unsigned int count = 0;
    blockNode* node = blocks.first;
    while(node != nullptr){
        for(auto & element : node->elements){
            if(!element.free) {
                Attribute *attributeNode = element.value.getAttributeByName(name);
                if(attributeNode != nullptr)
                    count++;
            }
        }
        node = node->next;
    }
    return count;
}

unsigned int CssParser::countSelector(String &name) {
    unsigned int  count = 0;
    blockNode* node = blocks.first;
    while(node != nullptr){
        for(auto & element : node->elements){
            if(!element.free) {
                List<String> *selectors = &element.value.selectors;
                for(int i = 0; i < selectors->size(); i++){
                    if(selectors->operator[](i) == name)
                        count++;
                }
            }
        }
        node = node->next;
    }
    return count;
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
            if(getLastBlock() == nullptr || !getLastBlock()->global){
                Block block;
                blocks.pushBack(block);
            }
            addToBlock(line);
        } else {
            if(line.contains('{'))
                blockOpen = true;
            List<String> parts = line.split('{');
            if(parts.size() == 0)
                return;
            String selector = parts[0];
            removeUselessWhitespace(selector);
            if(getLastBlock() == nullptr || getLastBlock()->selector != selector){
                Block block(selector);
                blocks.pushBack(block);
            }
            if(parts.size() > 1){
                String attributes = parts[1];
                if(attributes.contains('}')) {
                    blockOpen = false;
                    attributes.remove('}');
                }
                addToBlock(attributes);
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
        getLastBlock()->addAttribute(a);
        int k = 2;
    }
}

Block *CssParser::getLastBlock() {
    blockNode *node = blocks.last;
    Block *block = nullptr;
    if(node == nullptr)
        return block;
    for(auto & element : node->elements){
        if(!element.free)
            block = &element.value;
    }
    return block;
}

Block* CssParser::getBlock(String &selector) {
    blockNode* node = blocks.first;
    Block* result = nullptr;
    while(node != nullptr){
        for(auto & element : node->elements){
            if(!element.free && element.value.selector == selector)
                result = &element.value;
        }
        node = node->next;
    }
    return result;
}


void CssParser::removeUselessWhitespace(String &line) {
    while(line[0] == '\t' || line[0] == ' ')
        line.remove((String::stringSize_t)0);
    while(line[line.size()-1] == ' ' || line[line.size()-1] == '\t')
        line.remove(line.size()-1);
}

Block::Block(String &selector) : global(false) {
    this->selector = selector;
    List<String> selectors = selector.split(',');
    for(int i = 0; i < selectors.size(); i++)
        CssParser::removeUselessWhitespace(selectors[i]);
    this->selectors = selectors;
}

Block::Block() : selector(), selectors(), attributes(), global(true) {}

void Block::addAttribute(Attribute &attribute) {
    attributeNode* node = attributes.first;
    while(node != nullptr){
        for(auto & element : node->elements){
            if(!element.free && element.value.name == attribute.name) {
                element.value.value = attribute.value;
                return;
            }
        }
        node = node->next;
    }
    attributes.pushBack(attribute);
}

Attribute* Block::getAttributeByName(String &name) {
    attributeNode* node = attributes.first;
    while(node != nullptr){
        for(auto & element : node->elements){
            if(!element.free && element.value.name == name) {
                return &element.value;
            }
        }
        node = node->next;
    }
    return nullptr;
}

