/*ROAD.h */

#include "includer.h"
#include <utility>

#ifndef ROAD_H
#define ROAD_H

class road
{
private:
    std::list<int> contents;
    std::pair<int,int> srtcoord;
    std::pair<int,int> endcoord;
    int start;
    int end;
public:
    road(int start, int finish);
    ~road();
    
    void set_start(int x, int y){ srtcoord = std::make_pair(x,y); }
    void set_end  (int x, int y){ endcoord = std::make_pair(x,y); }
      
};

#endif