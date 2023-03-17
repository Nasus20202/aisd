#include <iostream>
#include "List.h"
using namespace std;

int main() {
    List<int> l;
    List<std::string> l2;
    List<List<char>> l3;

    l.pushBack(1);
    l.pushBack(2);
    cout << l.getSize();
}
