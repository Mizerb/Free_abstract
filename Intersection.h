/* Intersection.h */


#ifndef INTERSECTION_H
#define INTERSECTION_H

#include "LinkedNode.h"
#include "Road.h"
#include "Car.h"
#include <stdlib.h>
#include <map>
#include <vector>
#include "GridObject.h"

class Road;
template <class T>
class LinkedNode;

class Intersection : public GridObject
{
    
public:
    std::map<int,Road*> directions;
    std::map<int, std::vector<Road*> > opt_directions;
    int x_;
    int y_;
    LinkedNode<Car>* head;
    LinkedNode<Car>* tail;
    std::vector<Road*> outConnections;
    std::vector<Road*> inConnections;
    
    int dist;
    Intersection * prev;
    
    virtual char type(){return 'i';}


    int GID; //Grid ID
    Intersection(int x, int y, int GID_);
     Intersection(int x, int y);
    Intersection(int GID_) : GID(GID_), head(NULL) , x_(-1), y_(-1) {}
    ~Intersection();
    
    void add_out_Road(Road*);
    void add_in_Road(Road*);
    
    LinkedNode<Car>* getHead();
    void add_car(Car car);
    void process_cars();
    int get_GID() const{ return GID;} // should I not?
    int send_lookup (int dest) const;
    
    Road* find_road_to( int GID);	
    void del_cars();
    void clear_connections() { return; outConnections.clear(); inConnections.clear(); }
    
    void add_options();
};




#endif