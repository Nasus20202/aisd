#include <iostream>
#include "List.h"
using namespace std;

int main() {
    List<int> l;
    for(int i = 0; i < 10; i++){
        l.pushBack(i);
    }
    for(int i = 0; i < l.getSize(); i++){
        cout << l[i] << " ";
    }
    List<int> l2;
    l2.pushBack(12);
    l2 = l;
    cout << endl;
    for(int i = 0; i < l2.getSize(); i++){
        cout << l2[i] << " ";
    }
    List<List<int>> a;
    for(int i = 0; i <= 11; i++) {
        List<int> aa;
        for (int j = 0; j < i; j++)
            aa.pushBack(j);
        a.pushBack(aa);
    }
    for(int i = 0; i < a.getSize(); i++){
        for(int j = 0; j < a[i].getSize(); j++){
            cout << a[i][j] << " ";
        }
        cout << endl;
    }
}
