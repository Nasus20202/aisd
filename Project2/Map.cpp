#include "Map.h"
#include "Queue.h"
#include <iostream>

Map::Map(int width, int height) : width(width), height(height) {

}

Map::~Map() {

}

void Map::readMap() {
    Vector<Vector<Tile>> tileMap(height); int cityCount = 0;
    for(int i = 0; i < height; i++){
        tileMap.pushBack(Vector<Tile>(width));
        for(int j = 0; j < width; j++){
            char c = getchar();
            while(c == '\n')
                c = getchar();
            if(c == (char) cityTile)
                cityCount++;
            tileMap[i].pushBack(Tile(j, i, c));
        }
    }
    // read cities
    cities.resize(cityCount);
    loadCities(tileMap);
    Queue<Tile*> queue;
    //queue.enqueue(&tileMap[rootY][rootY]);
    //std::cout << queue.peek()->x << queue.peek()->y << std::endl;
    for(int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++)
            std::cout << tileMap[i][j].type;
        std::cout << std::endl;
    }
    for(int i = 0; i < cities.length(); i++){
        std::cout << cities[i].name << std::endl;
    }
}

void Map::loadCities(Vector<Vector<Tile>> &tileMap) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            Tile *tile = &tileMap[i][j];
            if (tile->type != cityTile)
                continue;
            // create a new city
            if (tile->type == cityTile) {
                cities.pushBack(City());
                City *currentCity = &cities[cities.size() - 1];
                // read city name
                int startingX, startingY;
                for (int y = -1; y <= 1; y++) {
                    for (int x = -1; x <= 1; x++) {
                        if (i + y < 0 || i + y >= height || j + x < 0 || j + x >= width)
                            continue;
                        char value = tileMap[i + y][j + x].type;
                        if (value == cityTile || value == roadTile || value == emptyTile)
                            continue;
                        startingX = j + x;
                        startingY = i + y;
                        break;
                    }
                }
                // move starting X as much to the left as possible
                while (startingX > 0) {
                    char value = tileMap[startingY][startingX - 1].type;
                    if (value == cityTile || value == roadTile || value == emptyTile)
                        break;
                    startingX--;
                }
                while (startingX < width) {
                    char value = tileMap[startingY][startingX].type;
                    if (value == cityTile || value == roadTile || value == emptyTile)
                        break;
                    currentCity->name.add(value);
                    startingX++;
                }
            }
        }
    }
}
