/* grid.h */

#ifndef GRID_H
#define GRID_H

#include "includer.h"
#include "intersection.h"
#include "City.h"
#include "bridge_intersection.h"

class grid
{
private:
    int x_size;
    int y_size;

    std::vector<std::vector<intersection*> > intersections;
    std::vector<City*> Cities;

    std::vector<bridge_intersection*> top_border;
    std::vector<bridge_intersection*> bot_border;
public:
    grid();
    ~grid();
    
    grid(int x, int y);


    void add_city(int x , int y , int pop);
    

    void add_link(int x_1, int y_1, int x_2, int y_2, int weight);

    void add_border_link(int x);
    //The Grid is constant
    
    //The Grid does not change it's parameters
    
    //The Grid only changes to meet the needs of the Grid
    
    

};

#endif