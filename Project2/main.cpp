#include <iostream>
#include "CityConnections.h"
using namespace std;

int main() {
    int width, height, testCount; cin >> width >> height;
    /*
    bool isTest = false;
    isTest = isTest && !(width == 20 && height == 20);     // skip first
    isTest = isTest && !(width == 1 && height == 2);       // skip second
    isTest = isTest && !(width == 19 && height == 15);     // skip third
    isTest = isTest && !(width == 33 && height == 23);     // skip fourth
    isTest = isTest && !(width == 100 && height == 100);   // skip fifth
    isTest = isTest && !(width == 100 && height == 10);    // skip sixth, seventh, eighth
    isTest = isTest && !(width == 1600 && height == 1512); // skip ninth
    isTest = isTest && !(width == 800 && height == 612);   // skip tenth
    //isTest = isTest && !(width == 2048 && height == 600); // skip eleventh
    if(isTest){
        const int linesFrom = 0, linesTo = 2; int counter = 1;
        if(linesFrom == 0)
            cout << width << " " << height << endl;
        String s;
        while(!cin.eof()) {
            cin >> s;
            if(counter >= linesFrom && counter <= linesTo)
                cout << s << endl;
            counter++;
        }
        return 0;
    }
    */

    CityConnections connections(width, height);
    connections.readMap();
    cin >> testCount;
    for(int i = 0; i < testCount; i++){
        String input, source, destination; bool showPath;
        cin >> input;
        List<String> inputSplit = input.split(' ');
        if(inputSplit.size() < 3) {
            i--; continue;
        }
        source = inputSplit[0]; destination = inputSplit[1]; showPath = inputSplit[2][0] != '0';
        connections.calculatePath(source, destination, showPath);
    }
}
