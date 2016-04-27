#ifndef CITY_H
#define CITY_H

#include "includer.h"
#include "intersection.h"


class City : public intersection{
private:
    char that;
    int population;
public:
    City();
    City(int GID_, int pop): intersection(GID_) {}
    City(int x_, int y_, int pop): intersection(x_,y_), population(pop) {}
    ~City();
};




#endif