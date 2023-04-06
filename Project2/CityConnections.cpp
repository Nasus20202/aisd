#include "CityConnections.h"
#include "Queue.h"
#include "PriorityQueue.h"
#include <iostream>
#include <queue>

using namespace std;
CityConnections::CityConnections(int width, int height) : width(width), height(height) {}

CityConnections::~CityConnections() {}

City *CityConnections::getCityByName(String name) {
    return cityHashMap[name];
}

void CityConnections::readMap(bool isTest) {
    Vector<Tile> tileMap(height*width); int cityCount = 0;
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            char c = getchar();
            while(c == '\n')
                c = getchar();
            if(c == (char) cityTile)
                cityCount++;
            tileMap.pushBack(Tile(j, i, c));
        }
    }
    // read cities
    cities.resize(cityCount);
    cityHashMap.resize(cityCount*3);
    Vector<Tile*> cityTiles = loadCities(tileMap);
    createCityGraph(tileMap, cityTiles);
    loadFlights(isTest);
    if(isTest)
        cout << "Cities: " << cities.size() << endl;
}

void CityConnections::calculatePath(String &from, String &to, bool showPath) {
    City *source = getCityByName(from), *destination = getCityByName(to);
    if(source == nullptr || destination == nullptr)
        throw;
    const int size = cities.size();
    Vector<int> distance(size, INT32_MAX);
    Vector<City*> previous(size, nullptr);
    Vector<bool> visited(size, false);
    priority_queue<pair<int, City*>, vector<pair<int, City*>>, greater<pair<int, City*>>> queue;
    queue.push(make_pair(0, source));
    distance[source->id] = 0;
    while(!queue.empty()){
        City* currentCity = queue.top().second;
        visited[currentCity->id] = true;
        if(currentCity->id == destination->id)
            break;
        queue.pop();
        const int neighboursCount = currentCity->connections.size();
        for(int i = 0; i < neighboursCount; i++){
            City *neighbour = currentCity->connections[i].city;
            if(visited[neighbour->id])
                continue;
            const int neighbourId = neighbour->id;
            int altDistance = distance[currentCity->id] + currentCity->connections[i].distance;
            if(altDistance < distance[neighbourId]){
                distance[neighbourId] = altDistance;
                previous[neighbourId] = currentCity;
                queue.push(make_pair(altDistance, neighbour));
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

int CityConnections::distanceBetween(City *city1, City *city2) {
    int distance = INT32_MAX;
    const int size = city1->connections.size();
    for(int i = 0; i < size; i++){
        if(city1->connections[i].city->id == city2->id && city1->connections[i].distance < distance)
            distance = city1->connections[i].distance;
    }
    return distance;
}

void CityConnections::loadFlights(bool isTest) {
    int count; cin >> count;
    if(isTest)
        cout << "Flights count: " << count << endl;
    for(int i = 0; i < count; i++){
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
        int duration = tab[2].toInt();
        City* fromCity = getCityByName(tab[0]), *toCity = getCityByName(tab[1]);
        fromCity->connections.pushBack(City::Connection(toCity, duration));
    }
}

void CityConnections::createCityGraph(Vector<Tile> &tileMap, Vector<Tile*> &cityTiles) {
    for(int i = 0; i < cityTiles.size(); i++){
        Tile* root = cityTiles[i]; bool anyConnections = false;
        for (int x = -1; x <= 1 && !anyConnections; x++) {
            for (int y = -1; y <= 1 && !anyConnections; y++) {
                if ((x == 0) == (y == 0))
                    continue;
                int newX = root->x + x, newY = root->y + y;
                if (newX < 0 || newX >= width || newY < 0 || newY >= height)
                    continue;
                Tile *nextTile = &tileMap[newY*width+newX];
                if(nextTile->type == roadTile || nextTile->type == cityTile)
                    anyConnections = true;
            }
        }
        if(!anyConnections)
            continue;
        Vector<bool> visited(height*width, false);
        Vector<int> distance(height*width, 0);
        Queue<Tile*> queue;
        queue.enqueue(root);
        visited[root->y*width+root->x] = true;
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
                    if(visited[newY*width+newX])
                        continue;
                    Tile *nextTile = &tileMap[newY*width+newX];
                    visited[newY*width+newX] = true;
                    if (nextTile->type == cityTile) {
                        root->city->connections.pushBack(City::Connection(nextTile->city, distance[tile->y*width + tile->x]+1));
                    }
                    else if(nextTile->type == roadTile) {
                        queue.enqueue(nextTile);
                        distance[newY*width+newX] = distance[tile->y*width + tile->x]+1;
                    }
                }
            }
        }
    }
}

Vector<CityConnections::Tile*> CityConnections::loadCities(Vector<Tile> &tileMap) {
    Vector<Tile*> cityTiles;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            Tile *tile = &tileMap[i*width+j];
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
                        char value = tileMap[(i + y)*width + j + x].type;
                        if (value == cityTile || value == roadTile || value == emptyTile)
                            continue;
                        startingX = j + x;
                        startingY = i + y;
                        break;
                    }
                }
                // move starting X as much to the left as possible
                while (startingX > 0) {
                    char value = tileMap[startingY*width + startingX - 1].type;
                    if (value == cityTile || value == roadTile || value == emptyTile)
                        break;
                    startingX--;
                }
                while (startingX < width) {
                    char value = tileMap[startingY*width + startingX].type;
                    if (value == cityTile || value == roadTile || value == emptyTile)
                        break;
                    currentCity->name.add(value);
                    startingX++;
                }
                currentCity->id = cities.size() - 1;
                tile->city = currentCity;
                cityHashMap[currentCity->name] = currentCity;
                cityTiles.pushBack(tile);
            }
        }
    }
    return cityTiles;
}
