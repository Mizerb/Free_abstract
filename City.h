#ifndef CITY_H
#define CITY_H

#include "includer.h"
#include "intersection.h"


class City : public intersection{
public:
    char that;
    int population;
    
    std::map<int,std::pair<int,int> > *ways_out; //I don't even know. 
    
public:
    
    int received;
    char type(){ return 'c';}

    void add_car(Car car)
    {
        if(car.destination == GID)
        {
            received++;
            return;
        }
        LinkedNode<Car>* new_node = new LinkedNode<Car>(car);
        if(head == NULL)
        {
            head = new_node;
            tail = new_node;
            return;
        }
        if(tail != NULL)
        {
            this->tail->next = new_node;
            new_node->prev = tail;
        }
        this->tail = new_node;
    }
    
    City();
    City(int GID_, int pop): intersection(GID_) {}
    City(int x_, int y_, int pop): intersection(x_,y_), population(pop) {received=0;}
    ~City();
    int getPop(){return population;}
    
    void Prep_way() { ways_out = new std::map<int,std::pair<int,int> >();}
};




#endif