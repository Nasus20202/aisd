#include <iostream>
#include "CityConnections.h"
using namespace std;

int main() {
    int width, height; cin >> width >> height;
    CityConnections connections(width, height);
    connections.readMap();
}
