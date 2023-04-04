#include <iostream>
#include "CityConnections.h"
using namespace std;

int main() {
    int width, height, testCount; cin >> width >> height;
    CityConnections connections(width, height);
    connections.readMap();
    cin >> testCount;
    for(int i = 0; i < testCount; i++){
        String input, source, destination; bool showPath;
        cin >> input;
        List<String> inputSplit = input.split(' ');
        if(inputSplit.size() < 3) {
            i--;continue;
        }
        source = inputSplit[0]; destination = inputSplit[1]; showPath = inputSplit[2][0] == '0' ? false : true;
        connections.calculatePath(source, destination, showPath);
    }
}
