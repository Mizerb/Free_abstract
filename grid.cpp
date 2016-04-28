#include "includer.h"
#include "grid.h"
grid::grid()
{
    return;
}


grid::grid(int x, int y)
{
    x_size = x;
    y_size = y;

    int x_counter = 0;
    for( int i = 0 ; i < y_size ; i++)
    {
        std::vector<intersection*> local_ptrs(x_size , NULL);
        
        x_counter = 0;
        for (std::vector<intersection*>::iterator j = local_ptrs.begin(); j != local_ptrs.end(); ++j, x_counter++)
        {
            else *j = new intersection(x_counter,i);             
        }
        intersections.push_back(local_ptrs);
    }

    /*Is there anything else?*/
}

void grid::add_city(int x, int y , int pop)
{
    int temp = this->intersections[y-1][x]->get_GID();
    delete this->intersections[y-1][x];
    intersections[y-1][x] = new City(temp , pop);
    Cities.push_back((City*)intersections[y-1][x]);
}


void grid::add_road(int GID_f , int GID_t)
{
    GID_f -= rank_displace;
    GID_t -= rank_displace;

    int x = GID_f%x_size;
    int y = GID_f/y_size;

    roads.push_back(road(GID_f, GID_t));
    road.set_start( x, y );
    intersections[y][x]->add_out_road(&(roads.back()));

    x = GID_t%x_size;
    y = GID_t/y_size;
    road.set_end(x,y);
    intersections[y][x]->add_in_road(&(roads.back()));
}

void grid::add_road(bridge_intersection* from_, int GID_t)
{
    return;
}

void grid::add_road(int GID_f, bridge_intersection* to)
{
    return;
}


void grid::road_reset()
{
    for(int i =0 ; i<intersections.size(); i++)
    {
        for(int j=0; j<intersections[i].size();i++)
        {
            intersections[i][j]->clear_connections();
        }
    }
    bridges.clear();
    roads.clear();

}