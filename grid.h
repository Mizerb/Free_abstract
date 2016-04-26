/* grid.h */

#include <vector>

#include <intersection.h>

#ifndef GRID_H
#define GRID_H

class grid
{
public:
    grid();
    ~grid();
    
    std::vector<intersection> v;


};


#endif