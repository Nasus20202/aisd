#pragma once
#include "String.h"
#include "Vector.h"
#include "HashMap.h"

struct City {
    struct Connection {
        City *city;
        int distance;
        Connection(City* city = nullptr, int distance = 0):city(city), distance(distance){};
    };
    Vector<Connection> connections;
    String name;
    int id = 0;
};

class CityConnections {
private:
    struct Tile {
        int x, y; char type; City* city = nullptr;
        Tile(int x = 0, int y = 0,char type = CityConnections::emptyTile) : type(type), x(x), y(y) {}
    };
    const int width, height;
    Vector<City> cities;
    HashMap<String, City*> cityHashMap;
public:
    static const char emptyTile = '.', cityTile = '*', roadTile = '#';
    CityConnections(int width, int height);
    ~CityConnections();
    void readMap();
    void calculatePath(String &from, String &to, bool showPath);
private: // helpers
    City* getCityByName(String name);
    Vector<Tile *> loadCities(Vector<Tile>&);
    void createCityGraph(Vector<Tile>&, Vector<Tile*>&);
    void loadFlights();
};
