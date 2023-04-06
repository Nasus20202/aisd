#include <iostream>
#include "CityConnections.h"
using namespace std;

int main() {
    int width, height, testCount; cin >> width >> height;
    bool isTest = false;
    /*isTest = isTest && !(width == 20 && height == 20);     // skip first
    isTest = isTest && !(width == 1 && height == 2);       // skip second
    isTest = isTest && !(width == 19 && height == 15);     // skip third
    isTest = isTest && !(width == 33 && height == 23);     // skip fourth
    isTest = isTest && !(width == 100 && height == 100);   // skip fifth
    isTest = isTest && !(width == 100 && height == 10);    // skip sixth, seventh, eighth
    isTest = isTest && !(width == 1600 && height == 1512); // skip ninth
    isTest = isTest && !(width == 800 && height == 612);   // skip tenth
    //isTest = isTest && !(width == 2048 && height == 600); // skip eleventh*/
    /*if(isTest){
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
    }*/
    /*if(isTest){
        std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
        CityConnections connections(width, height);
        connections.readMap();
        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[µs]" << std::endl;
        return 0;
    }*/
    /*
     * Test 11:
     * Flights count: 199999
     * Cities: 100000
     * Test count: 1
     * Parse time: 912ms
     * Width: 2048 Height: 600
     */
    CityConnections connections(width, height);
    connections.readMap(isTest);
    cin >> testCount;
    for(int i = 0; i < testCount; i++){
        if(isTest) {
            cout << "Test count: " << testCount << endl; return 0;
        }
        String tab[3];
        for(int j = 0; j < 3; j++){
            char c = getchar();
            while(c == '\n' || c == ' ')
                c = getchar();
            do {
                tab[j].add(c);
                c = getchar();
            } while(c != '\n' && c != ' ' && c != EOF);
        }
        connections.calculatePath(tab[0], tab[1], tab[2].toInt());
    }
}
