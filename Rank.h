#ifndef RANK_H
#define RANK_H

#include "includer.h"
#include "grid.h"
#include "mpi.h"
#include <vector>

class Rank
{
    int my_rank;
    int mpi_comm_size;

    int x_world_size;
    int y_world_size;

    int y_size;


    grid * local_grid;
public:
    Rank(int x_world_size , int y_world_size);
    ~Rank();
    

    void City_Start( int * cities_, int leng);
};


#endif