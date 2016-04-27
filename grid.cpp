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
            if(i == 0 || i== y_size-1) *j = new bridge_intersection(x_counter, i);
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
