#include <iostream>
#include "Map.h"
using namespace std;

int main() {
    int width, height; cin >> width >> height;
    Map map(width, height);
    map.readMap();
}
