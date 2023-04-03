#include "Map.h"
#include "Queue.h"
#include <iostream>

Map::Map(int width, int height) : width(width), height(height) {
}

Map::~Map() {

}

void Map::readMap() {
    struct Tile {
        struct Connection {
            Tile *tile;
            int distance;
        };
        int x, y; char type; bool visited = false; City *city = nullptr; Vector<Connection> connections;
        Tile(int x = 0, int y = 0, char type = emptyTile) : x(x), y(y), type(type) {}
    };
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
    cities.resize(cityCount);
    // read cities
    {
        Vector<Vector<Tile>> citiesTempMap = tileMap;
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                Tile *tile = &citiesTempMap[i][j];
                if (tile->type != cityTile)
                    continue;
                // create a new city
                if (tile->type == cityTile) {
                    cities.pushBack(City());
                    City *currentCity = &cities[cities.size()-1];
                    // read city name
                    int startingX, startingY;
                    for(int y = -1; y <= 1; y++){
                        for(int x = -1; x <= 1; x++){
                            if(i+y < 0 || i+y >= height || j+x < 0 || j+x >= width)
                                continue;
                            char value = citiesTempMap[i+y][j+x].type;
                            if(value == cityTile || value == roadTile || value == emptyTile)
                                continue;
                            startingX = j+x;
                            startingY = i+y;
                            break;
                        }
                    }
                    // move starting X as much to the left as possible
                    while (startingX > 0){
                        char value = citiesTempMap[startingY][startingX-1].type;
                        if(value == cityTile || value == roadTile || value == emptyTile)
                            break;
                        startingX--;
                    }
                    while(startingX < width){
                        char value = citiesTempMap[startingY][startingX].type;
                        if(value == cityTile || value == roadTile || value == emptyTile)
                            break;
                        currentCity->name.add(value);
                        startingX++;
                    }
                }
            }
        }
        for(int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++)
                std::cout << (char) citiesTempMap[i][j].type;
            std::cout << std::endl;
        }
    }
}
