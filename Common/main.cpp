#include <iostream>
#include "List.h"
using namespace std;

int main() {
    List<int> l;
    //List<std::string> l2;
    //List<List<char>> l3;
    for(int i = 0; i < 2137; i++)
        l.pushBack(i);
    cout << l.getSize() << endl;
    cout << l[12] << endl;
    l[12] = 421421312;
    cout << l[12] << endl;
}
