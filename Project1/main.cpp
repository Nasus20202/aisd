#include "CssParser.h"
#include <iostream>
using namespace std;

int main() {
    CssParser parser;
    String s;
    while(!cin.eof()){
        cin >> s;
        parser.loadLine(s);
    }
}
