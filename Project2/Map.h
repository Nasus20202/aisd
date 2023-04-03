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
    static const char emptyTile = '.', cityTile = '*', roadTile = '#';
    int width, height;
    Vector<City> cities;
public:
    Map(int width, int height);
    ~Map();
    void readMap();
};
