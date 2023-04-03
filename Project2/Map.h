#pragma once
#include "String.h"
#include "Vector.h"

struct City {
    Vector<City*> connections;
    String name;
};

class Map {
private:
    int width, height;
    Vector<City> cities;
public:
    Map(int width, int height);
    ~Map();
    void readMap();
};
