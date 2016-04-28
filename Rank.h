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

    bool in_grid(int GID);
    int GID_to_x(int GID);
    int GID_to_y(int GID);

public:
    Rank(int x_world_size , int y_world_size);
    ~Rank();
    

    void City_Start( int * cities_, int leng);


    void Add_Roads( int *GIDs, int leng);

    void Run_Sim();

    void Send_Result();

    void Reset_Sim();

};


#endif