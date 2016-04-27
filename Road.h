/*ROAD.h */

#include "includer.h"

#ifndef ROAD_H
#define ROAD_H

class road
{
private:
    std::list<int> contents;
    int start;
    int end;
public:
    road(int start, int finish);
    ~road();
    
};

#endif