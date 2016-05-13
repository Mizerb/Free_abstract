#ifndef CITY_H
#define CITY_H

#include <iterator>
#include "includer.h"
#include "Intersection.h"


class City : public Intersection{
public:
    char that;
    int population;
    
    std::map<int,std::pair<int,int> > *ways_out; //I don't even know. 
    
public:
    
    int received;
    char type(){ return 'c';}
    void make_Cars()
    {
        fprintf(stderr,"%d\n",(int)directions.size());
        for(int i =0; i<population/2;i++)
        {
            int rando = drand48()*directions.size();
            int j =0;
            for (std::map<int,Road*>::iterator it=directions.begin(); it!=directions.end(); ++it)
            {
                
                if(j>rando)
                {
                    add_car(Car(it->first));
                    break;
                }
            }
        }
    }
    
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
    City(int GID_, int pop): Intersection(GID_) {}
    City(int GID_ , int x_, int y_, int pop): Intersection(x_,y_, GID_), population(pop) {received=0;}
    ~City();
    int getPop(){return population;}
    
    void Prep_way() { ways_out = new std::map<int,std::pair<int,int> >();}

    int Connections(){ return directions.size();}
};




#endif