/* Intersection.h */

#include "LinkedNode.h"
#include "Road.h"
#include <stdlib.h>
#include <map>

#ifndef INTERSECTION_H
#define INTERSECTION_H

class intersection
{
    
public:
    std::map<int,Road*> directions;
    int x_;
    int y_;
    LinkedNode<Car>* head;
    LinkedNode<Car>* tail;
    std::vector<Road*> outConnections;
    std::vector<Road*> inConnections;
    
    int dist;
    intersection * prev;
    
    virtual char type(){return 'i';}


    int GID; //Grid ID
    intersection(int x, int y);
    intersection(int GID_) : GID(GID_){}
    ~intersection();
    
    void add_out_Road(Road*);
    void add_in_Road(Road*);
    
    LinkedNode<Car>* getHead();
    void add_car(Car car);
    void process_cars();
    int get_GID() const{ return GID;} // should I not?
    int send_lookup (int dest) const;
    
    Road* find_road_to( int GID);	
    void del_cars();
    void clear_connections(){ outConnections.clear(); inConnections.clear(); }
};




#endif