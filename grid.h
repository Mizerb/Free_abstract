/* grid.h */

#ifndef GRID_H
#define GRID_H

#include "includer.h"
#include "intersection.h"
#include "City.h"
#include "bridge_intersection.h"
#include "Road.h"

class grid
{
private:
    int x_size;
    int y_size;

    int rank_displace;

    std::vector<std::vector<intersection*> > intersections;
    std::vector<City*> Cities;

    std::vector<bridge_intersection> bridges;

    std::vector<road> roads;
public:
    grid();
    ~grid();
    
    grid(int x, int y);


    void add_city(int x , int y , int pop);
    

    void add_link(int x_f, int y_f, int x_t, int y_t, int weight);

    void add_road(int GID_f , int GID_t);
    void add_road(bridge_intersection* from_, int GID_t);
    void add_road(int GID_f, bridge_intersection* to);

    void add_border_link(int x);

    int get_displacement()const {return rank_displace;}
    //The Grid is constant
    
    //The Grid does not change it's parameters
    
    //The Grid only changes to meet the needs of the Grid
    void road_reset();
    

};

#endif