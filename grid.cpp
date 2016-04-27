#include "includer.h"

grid::grid()
{
    return;
}


grid::grid(int x, int y)
{
    x_size = x;
    y_size = y;

    for( int i = 0 ; i < y_size ; i++)
    {
        std::vector<intersection*> local_ptrs(x_size , NULL);
        for (std::vector<intersection*>::iterator i = local_ptrs.begin(); i != local_ptrs.end(); ++i)
        {
            *i = new intersection();             
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
