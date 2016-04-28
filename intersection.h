/* Intersection.h */

#include "LinkedNode.h"
#include "Road.h"

#ifndef INTERSECTION_H
#define INTERSECTION_H

class intersection
{
    int * look_up;
    int x_;
    int y_;
    LinkedNode<Car>* head;
    LinkedNode<Car>* tail;
    std::vector<road*> outConnections;
    std::vector</*int*/road*> inConnections;
public:
    virtual char type(){return 'i';}


    int GID; //Grid ID
    intersection(int x, int y);
    intersection(int GID_) : GID(GID_){}
    ~intersection(){delete look_up;}
    
    void add_out_road(road*);
    void add_in_road(road*);

    LinkedNode<Car>* getHead();
    void add_car(Car car);
    void process_cars();
    int get_GID() const{ return GID;} // should I not?
    int send_lookup (int dest) const;
    	

    void clear_connections(){ outConnections.clear(); inConnections.clear();}
};




#endif