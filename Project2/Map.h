#pragma once
#include "String.h"
#include "Vector.h"

struct City {
    struct Connection {
        City *city;
        int distance;
    };
    Vector<Connection> connections;
    String name;
};

class Map {
private:
    struct Tile {
        char type; int x, y; bool visited = false;
        Tile(int x = 0, int y = 0,char type = Map::emptyTile) : type(type), x(x), y(y) {}
    };
    int width, height;
    Vector<City> cities;
public:
    static const char emptyTile = '.', cityTile = '*', roadTile = '#';
    Map(int width, int height);
    ~Map();
    void readMap();
private: // helpers
    void loadCities(Vector<Vector<Tile>>&);
};
