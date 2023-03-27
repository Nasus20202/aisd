#include "CssParser.h"
#include <iostream>
using namespace std;

void CssParser::loadLine(String &line) {
    removeUselessWhitespace(line);
    if(line.length() == 0) // check if empty
        return;

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

void CssParser::parse(String &line) {
    String currentInput;
    for(int i = 0; i < line.size(); i++){
        char c = line[i];
        if(c<' ')
            continue;
        // open a new block if c == {
        if(c == '{'){
            blockOpen = true;
            // if { is the first character, use previous line as block selector, then empty previous
            if(i == 0){
                Block block(previous);
                blocks.pushBack(block);
                previous.empty();
            }
            // else use current input as block selector, then empty current input
            else {
                String selector = previous+currentInput;
                Block block(selector);
                blocks.pushBack(block);
                currentInput.empty();
            }
        }
        // close the block if c == }, add to current block, then empty current input
        else if(c == '}'){
            blockOpen = false;
            addToBlock(currentInput);
            currentInput.empty();
        }
        // add c to current input
        else{
            currentInput+=c;
        }
    }
    if(blockOpen) {
        addToBlock(currentInput);
    }
    if(isGlobalAttribute(currentInput))
        previous.empty();
    else
        previous += currentInput;
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

        // i,S,?
        if(isFirstInt && second == 'S' && third[0] == '?'){
            int id = commands[0].toInt();
            if(id-1 < blocks.size()) {
                cout << query << " == " << blocks[id - 1].selectors.size() << endl;
            }

        }
        // i,A,?
        else if(isFirstInt && second == 'A' && third[0] == '?') {
            int id = commands[0].toInt();
            if(id-1 < blocks.size()) {
                cout << query << " == " << blocks[id - 1].attributes.size() << endl;
            }
        }
        // i,S,j
        else if(isFirstInt && second == 'S' && isThirdInt){
            int blockId = first.toInt(), selectorId = third.toInt();
            if(blockId-1 < blocks.size()) {
                Block block = blocks[blockId - 1];
                if (selectorId - 1 < block.selectors.size()) {
                    cout << query << " == " << block.selectors[selectorId - 1] << endl;
                }
            }
        }
        // i,A,n
        else if(isFirstInt && second == 'A'){
            int blockId = first.toInt();
            if(blockId-1 < blocks.size()) {
                Block block = blocks[blockId - 1];
                Attribute *attribute = block.getAttributeByName(third);
                if (attribute != nullptr) {
                    cout << query << " == " << attribute->value << endl;
                }
            }
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
            if(attribute != nullptr) {
                cout << query << " == " << attribute->value << endl;
            }
        }
        // i,D,*
        else if(isFirstInt && second == 'D' && third[0] == '*'){
            int blockId = first.toInt();
            if(deleteBlock(blockId-1)) {
                cout << query << " == deleted" << endl;
            }
        }
        // i,D,n
        else if (isFirstInt && second == 'D'){
            int blockId = first.toInt();
            if(deleteAttribute(blockId-1, third)) {
                cout << query << " == deleted" << endl;
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

int CssParser::countAttribute(String &name) const{
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

int CssParser::countSelector(String &name) const{
    unsigned int  count = 0;
    blockNode* node = blocks.first;
    while(node != nullptr){
        for(auto & element : node->elements){
            if(!element.free) {
                List<String> *selectors = &element.value.selectors;
                bool found = false;
                for(int i = 0; i < selectors->size(); i++){
                    if(selectors->operator[](i) == name) {
                        found = true; break;
                    }
                }
                count += found;
            }
        }
        node = node->next;
    }
    return count;
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
    if(line.size() == 0)
        return;
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

Block::Block(String &selector) : selector(selector) {
    List<String> selectorsSplit = selector.split(',');
    for(int i = 0; i < selectorsSplit.size(); i++)
        CssParser::removeUselessWhitespace(selectorsSplit[i]);
    this->selectors = selectorsSplit;
}

Block::Block() : selector(), selectors(), attributes(){}

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

