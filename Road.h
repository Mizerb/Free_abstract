/*ROAD.h */

#ifndef ROAD_H
#define ROAD_H

#include "includer.h"
#include <utility>
#include "LinkedNode.h"
#include <math.h>
#include "Car.h"
#include "Intersection.h"

class Intersection;

template<class T>
class LinkedNode;

class Road
{
public:
    std::pair<int,int> srtcoord;
    std::pair<int,int> endcoord;
    LinkedNode<Car>* head;
    LinkedNode<Car>* tail;
    Intersection * outConnection;
    Intersection * out;
    int start; // the crap do these do? // EVERYTHING
    int end;
    int capacity;
    int count;

    int end_GID() const { return end; }
    int get_weight() const;

    Road(int start_, int finish_): start(start_) , end(finish_) {}
    //~Road();
    
    void set_start(int x, int y){ srtcoord = std::make_pair(x,y); }
    void set_end  (int x, int y){ endcoord = std::make_pair(x,y); }
    void set_connection(Intersection* mine);
    
    Intersection * get_out() const { return out; }
    
    bool add_car(Car car);
    
    void process_cars();
    
    LinkedNode<Car>* getHead();
    
    ~Road();
      
};

#endif