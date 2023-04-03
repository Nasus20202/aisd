#include "CityConnections.h"
#include "Queue.h"
#include <iostream>

CityConnections::CityConnections(int width, int height) : width(width), height(height) {}

CityConnections::~CityConnections() {}

void CityConnections::readMap() {
    Vector<Vector<Tile>> tileMap(height,Vector<Tile>(width)); int cityCount = 0;
    for(int i = 0; i < height; i++){
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
    createCityGraph(tileMap);

    for(int i = 0; i < cities.length(); i++){
        std::cout << cities[i].name << " : " << std::endl;
        for(int j = 0; j < cities[i].connections.size(); j++){
            std::cout << cities[i].connections[j].city->name << " " << cities[i].connections[j].distance << std::endl;
        }
        std::cout << std::endl;
    }
}

void CityConnections::createCityGraph(Vector<Vector<Tile>> &tileMap) {
    Vector<Tile*> cityTiles = getCityTiles(tileMap);
    for(int i = 0; i < cityTiles.size(); i++){
        Tile* root = cityTiles[i];
        Vector<Vector<bool>> visited(height, Vector<bool>(width, false));
        Vector<Vector<int>> distance(height, Vector<int>(width, 0));
        Queue<Tile*> queue;
        queue.enqueue(root);
        visited[root->y][root->x] = true;
        int depth = 0;
        while(!queue.isEmpty()) {
            Tile *tile = queue.dequeue();
            if (tile->type != cityTile && tile->type != roadTile)
                continue;
            for (int x = -1; x <= 1; x++) {
                for (int y = -1; y <= 1; y++) {
                    if((x == 0) == (y == 0))
                        continue;
                    int newX = tile->x + x, newY = tile->y + y;
                    if (newX < 0 || newX >= width || newY < 0 || newY >= height)
                        continue;
                    if(visited[newY][newX])
                        continue;
                    Tile *nextTile = &tileMap[newY][newX];
                    visited[newY][newX] = true;
                    if (nextTile->type == cityTile) {
                        root->city->connections.pushBack(City::Connection(nextTile->city, distance[tile->y][tile->x]+1));
                    }
                    else if(nextTile->type == roadTile) {
                        queue.enqueue(nextTile);
                        distance[newY][newX] = distance[tile->y][tile->x]+1;
                    }
                }
            }
        }
    }
}


void CityConnections::loadCities(Vector<Vector<Tile>> &tileMap) {
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
                tile->city = currentCity;
            }
        }
    }
}

Vector<CityConnections::Tile*> CityConnections::getCityTiles(Vector<Vector<Tile>> &tileMap) {
    Vector<CityConnections::Tile*> cityTiles(cities.size());
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            Tile* tile = &tileMap[i][j];
            if(tile->type == cityTile) {
                cityTiles.pushBack(tile);
            }
        }
    }
    return cityTiles;
}
