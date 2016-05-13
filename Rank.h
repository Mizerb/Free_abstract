#ifndef RANK_H
#define RANK_H

#include "Info.h"
#include "includer.h"
#include "grid.h"
#include "mpi.h"
#include <vector>

class Rank
{
    int x_world_size;
    int y_world_size;


    grid * local_grid;

    bool in_grid(int GID);
    int GID_to_x(int GID);
    int GID_to_y(int GID);

public:


    int tot_num_Cities;
    int * cities_GID;
    int y_size;
    int my_rank;
    int mpi_comm_size;


    Rank(int x_world_size , int y_world_size);
    ~Rank();
    

    void City_Start( int * cities_, int leng);


    void Add_Roads( int *GIDs, int leng);

    void Run_Sim();

    void Send_Result();

    void Reset_Sim();
    
    void printCities();
    
    std::vector<City*> getCities();
    
    void runSearch()
    {
        MPI_Barrier(MPI_COMM_WORLD);
        unsigned long long start = GetTimeBase() ,finish;
        local_grid->find_routing();
        end = GetTimeBase();
        printf( "[%d] RunSeach took %lu\n", my_rank, end-start );
   
    }
    
    /* Think this is the same as Reset_Sim();
    void Road_Reset()
    {
        local_grid->Road_reset();
    }*/

};


#endif