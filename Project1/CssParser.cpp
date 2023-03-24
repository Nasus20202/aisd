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
        if(commands.size() < 3)
            return;
        String first = commands[0];
        char second = commands[1][0];
        String third = commands[2];
        bool isFirstInt = first.isInt(), isThirdInt = third.isInt();

        bool done = false;
        // i,S,?
        if(!done && isFirstInt && second == 'S' && third[0] == '?'){
            int id = commands[0].toInt();
            if(id-1 < blocks.size()) {
                cout << query << " == " << blocks[id - 1].selectors.size() << endl; done = true;
            }
        }
        // i,A,?
        if(!done && isFirstInt && second == 'A' && third[0] == '?') {
            int id = commands[0].toInt();
            if(id-1 < blocks.size()) {
                cout << query << " == " << blocks[id - 1].attributes.size() << endl; done = true;
            }
        }
        // i,S,j
        if(!done && isFirstInt && second == 'S' && isThirdInt){
            int blockId = first.toInt(), selectorId = third.toInt();
            if(blockId-1 < blocks.size()) {
                Block block = blocks[blockId - 1];
                if (selectorId - 1 < block.selectors.size()) {
                    cout << query << " == " << block.selectors[selectorId - 1] << endl; done = true;
                }
            }
        }
        // i,A,n
        if(!done && isFirstInt && second == 'A'){
            int blockId = first.toInt();
            if(blockId-1 < blocks.size()) {
                Block block = blocks[blockId - 1];
                Attribute *attribute = block.getAttributeByName(third);
                if (attribute != nullptr) {
                    cout << query << " == " << attribute->value << endl; done = true;
                }
            }
        }
        // n,A,?
        if(!done && second == 'A' && third[0] == '?'){
            int count = countAttribute(first);
            if(count != 0) {
                cout << query << " == " << count << endl;
                done = true;
            }
        }
        // z,S,?
        if(!done && second == 'S' && third[0] == '?'){
            int count = countSelector(first);
            if(count != 0) {
                cout << query << " == " << count << endl;
                done = true;
            }
        }
        // z,E,n
        if(!done && second == 'E'){
            Attribute* attribute = getAttributeForSelector(first, third);
            if(attribute != nullptr) {
                cout << query << " == " << attribute->value << endl; done = true;
            }
        }
        // i,D,*
        if(!done && isFirstInt && second == 'D' && third[0] == '*'){
            int blockId = first.toInt();
            if(deleteBlock(blockId-1)) {
                cout << query << " == deleted" << endl;
                done = true;
            }
        }
        // i,D,n
        if (!done && isFirstInt && second == 'D'){
            int blockId = first.toInt();
            if(deleteAttribute(blockId-1, third)) {
                cout << query << " == deleted" << endl;
                done = true;
            }
        }
    }
}

bool CssParser::deleteBlock(int blockId) {
    if(blockId >= blocks.size())
        return false;
    blocks.remove(blockId);
    return true;
}

bool CssParser::deleteAttribute(int blockId, String &attributeName) {
    if(blockId >= blocks.size())
        return false;
    Block* block = &blocks[blockId];
    Block::attributeNode *attributeNode = block->attributes.first; bool done = false;
    while(attributeNode != nullptr){
        int id = 0;
        for(auto element : attributeNode->elements) {
            if (!element.free) {
                if (element.value.name == attributeName) {
                    block->attributes.remove(id);
                    done = true;
                    break;
                }
                id++;
            }
        } if(done)
            break;
        attributeNode = attributeNode->next;
    }
    if(block->attributes.size() == 0){
        blocks.remove(blockId);
    }
    return done;
}

Attribute* CssParser::getAttributeForSelector(String &selector, String &attribute) {
    blockNode* blockNode = blocks.first;
    Attribute* result = nullptr;
    while(blockNode != nullptr){
        blockNode::Element *element;
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
        if(attribute.size() == 1)
            attribute.pushBack(String((char *) ""));
        Attribute a;
        removeUselessWhitespace(attribute[0]);
        removeUselessWhitespace(attribute[1]);
        a.name = attribute[0];
        a.value = attribute[1];
        getLastBlock()->addAttribute(a);
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

void CssParser::removeUselessWhitespace(String &line) {
    if(line.size() == 0)
        return;
    while(line.size() > 0 && (line[0] == '\t' || line[0] == ' '))
        line.remove(0);

    while(line.size() > 0 && (line[line.size()-1] == ' ' || line[line.size()-1] == '\t'))
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

