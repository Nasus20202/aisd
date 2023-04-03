#include "Map.h"
#include "Queue.h"
#include <iostream>

Map::Map(int width, int height) : width(width), height(height) {
}

Map::~Map() {

}

void Map::readMap() {
    Vector<Vector<char>> charMap(height);
    for(int i = 0; i < height; i++){
        charMap.pushBack(Vector<char>(width));
        for(int j = 0; j < width; j++){
            char c = getchar();
            while(c == '\n')
                c = getchar();
            charMap[i].pushBack(c);
        }
    }
    for(int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++)
            std::cout << charMap[i][j];
        std::cout << std::endl;
    }
}
