#ifndef INFO_H
#define INFO_H

#include <string>
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <mpi.h>

#include <hwi/include/bqc/A2_inlines.h>


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

unsigned long long GetTimeBase()
{
    return (unsigned long long) 
}


#endif
