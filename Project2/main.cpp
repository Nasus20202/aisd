#include <iostream>
#include "CityConnections.h"
using namespace std;

int main() {
    int width, height, testCount; cin >> width >> height;
    CityConnections connections(width, height);
    connections.readMap();
    cin >> testCount;
    for(int i = 0; i < testCount; i++){
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
