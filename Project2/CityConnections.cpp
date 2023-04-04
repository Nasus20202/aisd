#include "CityConnections.h"
#include "Queue.h"
#include <iostream>
using namespace std;

CityConnections::CityConnections(int width, int height) : width(width), height(height) {}

CityConnections::~CityConnections() {}


City *CityConnections::getCityByName(String &name) {
    for(int i = 0; i < cities.size(); i++)
        if(cities[i].name == name)
            return &cities[i];
    return nullptr;
}

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
    loadFlights();

    /*for(int i = 0; i < cities.length(); i++){
        cout << cities[i].name << " : " << endl;
        for(int j = 0; j < cities[i].connections.size(); j++){
            cout << cities[i].connections[j].city->name << " " << cities[i].connections[j].distance << endl;
        }
        cout << endl;
    }*/
}

void CityConnections::calculatePath(String &from, String &to, bool showPath) {
    City *source = getCityByName(from), *destination = getCityByName(to);
    if(source == nullptr || destination == nullptr)
        throw;
    const int size = cities.size();
    Vector<bool> queue(size, true);
    Vector<int> distance(size, INT32_MAX);
    Vector<bool> visited(size, false);
    Vector<City*> previous(size, nullptr);
    distance[source->id] = 0;
    while(queue.size() > 0){
        City* currentCity = findNearestCity(queue, distance);
        if(currentCity == nullptr)
            break;
        queue[currentCity->id] = false; // remove from queue

        const int neighboursCount = currentCity->connections.size();
        for(int i = 0; i < neighboursCount; i++){
            City *neighbour = currentCity->connections[i].city;
            const int neighbourId = neighbour->id;
            if(!queue[neighbourId])
                continue;
            int altDistance = distance[currentCity->id] + distanceBetween(currentCity, neighbour);
            if(altDistance < distance[neighbourId]){
                distance[neighbourId] = altDistance;
                previous[neighbourId] = currentCity;
            }
        }
    }
    cout << distance[destination->id] << " ";
    if(showPath){
        City *path = previous[destination->id];
        Vector<City*> pathHistory;
        while(path != nullptr){
            if(path->id != source->id)
                pathHistory.pushBack(path);
            path = previous[path->id];
        }
        for(int i = pathHistory.size()-1; i >= 0; i--)
            cout << pathHistory[i]->name << " ";
    }
    cout << endl;
}

City *CityConnections::findNearestCity(Vector<bool> &citiesQueue, Vector<int> &distance) {
    int minDistance = INT32_MAX; City* city = nullptr; const int size = cities.size();
    for(int i = 0; i < size; i++){
        City *currentCity = &cities[i];
        const int id = currentCity->id;
        if(!citiesQueue[id])
            continue;
        if(distance[id] < minDistance){
            city = currentCity;
            minDistance = distance[city->id];
        }
    }
    return city;
}

int CityConnections::distanceBetween(City *city1, City *city2) {
    const int size = city1->connections.size();
    for(int i = 0; i < size; i++){
        if(city1->connections[i].city->id == city2->id)
            return city1->connections[i].distance;
    }
    return INT32_MAX;
}

void CityConnections::loadFlights() {
    int count; cin >> count;
    for(int i = 0; i < count; i++){
        String input;
        while(input.size() == 0)
            cin >> input;
        List<String> parts = input.split(' ');
        if(parts.size() < 3)
            throw;
        String from = parts[0], to = parts[1]; int duration = parts[2].toInt();
        City* fromCity = getCityByName(from), *toCity = getCityByName(to);
        if(fromCity == nullptr || toCity == nullptr)
            continue;
        const int connectionsSize = fromCity->connections.size(); bool done = false;
        for(int i = 0; i<connectionsSize; i++){
            if(fromCity->connections[i].city->id == toCity->id){
                if(fromCity->connections[i].distance > duration)
                    fromCity->connections[i].distance = duration; done = true;
                break;
            }
        }
        if(done)
            continue;
        fromCity->connections.pushBack(City::Connection(toCity, duration));
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
                currentCity->id = cities.size() - 1;
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

