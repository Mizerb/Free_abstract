#include "includer.h"

#ifndef RANK_H
#define RANK_H

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
    

    City_Start( int * cities_);
};



Rank::Rank(int x_world_size_ , int y_world_size_)
{
    MPI_Comm_size( MPI_COMM_WORLD, &(mpi_comm_size) );
    MPI_Comm_rank( MPI_COMM_WORLD, &(my_rank) );

    //this that and the other 
    x_world_size = x_world_size_;
    y_world_size = y_world_size_;

    y_size = y_world_size/mpi_comm_size;

    
    if( my_rank != 0 &&  my_rank != mpi_comm_size -1) y_size += 1;
    else y_size+=2;
    
    //Do I worry about crossing otherwise? How? 

    local_grid = new grid(x_world_size , y_size);
}

Rank::City_Start(int * cities_, int leng)
{
    int x, y, pop;
    for(int i = 0 ; i < leng ; i +=3)
    {
        x = cities_[i]; y = cities_[i+1];
        if( y/mpi_comm_size == my_rank )
        {
            local_grid.add_city(x, y%y_size , pop);
        }
    }
}

#endif