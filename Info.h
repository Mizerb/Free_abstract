#ifndef INFO_H
#define INFO_H

#include <string>
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <mpi.h>

class Info
{
public:

    Info(int argc, char* argv[]);

    int mpi_commsize;
    int mpi_rank;
    int world_x_size;
    int world_y_size;
    int world_slice_size;
    int world_slice_pos;
    int gaussian_sigma;
    int ga_gen_size;
    int ga_num_gen;
    int neighbors[2];
    
    int GID_from_coord( int x , int y);
};

// DO ERROR CHECKING BEFORE THIS POINT
Info::Info ( int argc, char* argv[] )
{
    int i;
    if(argc != 6){
        fprintf(stderr, " Usage:\n%s [x-dim] [y-dim] [generationsize] [numgenerations] [inputfile]", argv[0]);
    }
    
    world_x_size = atoi(argv[1]);
    world_y_size = atoi(argv[2]);
    ga_gen_size = atoi(argv[3]);
    ga_num_gen = atoi(argv[4]);
    
    
    // You might say to yourself, why are we doing this again?
    // Didn't we already do it in Rank.cpp?
    // To which I reply, "Why do we have a Rank.cpp? What function does it serve?"
    // After a long drawn out debate we agree to leave each other's code alone
    // and have our own redundant code because it's too difficult to deal with it.
    
    // When did we have this debate? 
    MPI_Comm_size( MPI_COMM_WORLD, &mpi_commsize);
    MPI_Comm_rank( MPI_COMM_WORLD, &mpi_rank);
    
    if ( world_y_size%mpi_commsize )
    {
        world_slice_pos = 0;
        for( i = 0; i < mpi_rank; ++i )
        {
            world_slice_pos += (world_y_size-world_slice_pos)/(mpi_commsize-i);
        }
        world_slice_size = (world_y_size-world_slice_pos)/(mpi_commsize-i);
    }
    else
    {
        world_slice_size = world_y_size/mpi_commsize;
        world_slice_pos = mpi_rank*world_slice_size;
    }
    
    neighbors[0] = mpi_rank-1;
    neighbors[1] = mpi_rank == mpi_commsize-1? -1 : mpi_rank+1;
}

int Info::GID_from_coord( int x , int y)
{
    int Ret;
    Ret = x + (y*world_x_size);
    return Ret;
}

#endif