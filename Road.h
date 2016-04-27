/*ROAD.h */

#include "includer.h"
#include "utility.h"

#ifndef ROAD_H
#define ROAD_H

class road
{
private:
    std::list<int> contents;
    pair srtcoord;
    pair endcoord;
    int start;
    int end;
public:
    road(int start, int finish);
    ~road();
    
};

#endif