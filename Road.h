/*ROAD.h */

#ifndef ROAD_H
#define ROAD_H

#include "GridObject.h"
#include "includer.h"
#include <utility>
#include "LinkedNode.h"
#include <math.h>
#include "Car.h"
#include "Intersection.h"


class Intersection;


class Road : public GridObject
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

    int weight;

    int end_GID() const { return end; }
    int get_weight() const;

    Road(int start_, int finish_): 
        weight(-1), start(start_) , 
        end(finish_), head(NULL) , 
        out(NULL), outConnection(NULL){}
    //~Road();
    
    void set_start(int x, int y){ srtcoord = std::make_pair(x,y); }
    void set_end  (int x, int y){ endcoord = std::make_pair(x,y); }
    void set_connection(Intersection* mine);
    
    int set_weight(int weight_){ weight_= weight;}

    Intersection * get_out() const { return out; }
    
    bool add_car(Car car);
    
    void process_cars();
    
    LinkedNode<Car>* getHead();
    
    ~Road();
      
};

#endif