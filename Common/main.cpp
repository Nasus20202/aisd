#include <iostream>
#include "String.h"
#include "List.h"
#include "Vector.h"
#include "Queue.h"
#include "HashMap.h"
using namespace std;

int main() {
    String s1("AAQ"), s2("AB0");
    HashMap<String, int> map(100);
    map[s1] = 1;
    map[s2] = 2;
    //int a = map[s1];
    cout << map[s1] << " " << map[s2] << endl;
    //cout << map[s1] << " " << map[s2] << endl;
}
