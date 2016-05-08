/* grid.h */

#ifndef GRID_H
#define GRID_H

#include "includer.h"
#include "intersection.h"
#include "City.h"
#include "bridge_intersection.h"
#include "Road.h"
#include "Info.h"

class grid
{
private:
    int x_size;
    int y_size;

    int rank_displace;
    
    

    std::vector<std::vector<intersection*> > intersections;
    std::vector<City*> Cities;

    std::vector<bridge_intersection> bridges;

    std::vector<Road> Roads;
public:
    grid();
    ~grid();
    
    grid(int x, int y);

    void run_Tick();
    void add_city(int x , int y , int pop);
    
    
    void add_link(int x_f, int y_f, int x_t, int y_t, int weight);

    void add_Road(int GID_f , int GID_t);

    void add_Road(bridge_intersection* from_, int GID_t);
    void add_Road(int GID_f, bridge_intersection* to);

    bridge_intersection* border_Road(int GID_f, int GID_t, int other_rank);

    int get_displacement()const {return rank_displace;}
    
    void set_displacement(int dis) { rank_displace = dis;}
    
    intersection * find_GID(int GID);
    
    //The Grid is constant
    
    //The Grid does not change it's parameters
    
    //The Grid only changes to meet the needs of the Grid
    void Road_reset();
    
    //Trenton debug Fucntion
    void printCities();
    
    std::vector<City*> getCities();
    void BFS(intersection* root);
    //void BFS(bridge_intersection* root);
    void find_routing();
    
    
    void write_routes( City* start);
    void write_routes( bridge_intersection* start);
    
    void trace_back(City* path_end );
    void trace_back(bridge_intersection * path_end , City * from);
    void trace_back(bridge_intersection * path_end , bridge_intersection * from);
    void trace_back(City * path_end , bridge_intersection * from);

    
    int total_ranks;
};

#endif