#pragma once
#include "String.h"
#include "Vector.h"

struct City {
    struct Connection {
        City *city;
        int distance;
        Connection(City* city = nullptr, int distance = 0):city(city), distance(distance){};
    };
    Vector<Connection> connections;
    String name;
};

class Map {
private:
    struct Tile {
        int x, y; char type; City* city = nullptr;
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
    void createCityGraph(Vector<Vector<Tile>>&);
    Vector<Tile *> getCityTiles(Vector<Vector<Tile>>&);
};
