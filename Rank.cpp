#include "Rank.h"
#include <math.h>

Rank::Rank(int x_world_size_ , int y_world_size_)
{
    MPI_Comm_size( MPI_COMM_WORLD, &(mpi_comm_size) );
    MPI_Comm_rank( MPI_COMM_WORLD, &(my_rank) );

    //this that and the other 
    x_world_size = x_world_size_;
    y_world_size = y_world_size_;

    y_size = y_world_size/mpi_comm_size;

    /*
    if( my_rank != 0 &&  my_rank != mpi_comm_size -1) y_size += 1;
    else y_size+=2;
    */
    //Do I worry about crossing otherwise? How? 

    local_grid = new grid(x_world_size , y_size);
    local_grid->set_displacement( y_size*x_world_size*my_rank) /*MATH*/;
    local_grid->total_ranks = mpi_comm_size;
}

void Rank::City_Start(int * cities_, int leng)
{
    int x, y, pop;
    for(int i = 0 ; i < leng ; i +=3)
    {
        
        x = cities_[i]; y = cities_[i+1];
        pop = cities_[i+2];
        if( y/y_size == my_rank )
        {
            printf("%d %d %d\n", cities_[i], cities_[i+1], cities_[i+2]);
            local_grid->add_city(x, y%y_size , pop);
        }
    }
}

#define dist(x1,y1,x2,y2) (int)sqrt(((x1-x2)*(x1-x2))+((y1-y2)*(y1-y2)))

void Rank::Add_Roads(int * GIDs, int leng)
{
    int GID_f, GID_t;
    for(int i = 0 ; i< leng ; i+= 2)
    {
        if( in_grid(GIDs[i]) && in_grid(GIDs[i+1]))
        {
            //int x1 = GID_to_x(), y1=GID_to_y(), x2=GID_to_x(), y2=GID_to_y(); 
            local_grid->add_Road( GIDs[i], GIDs[i+1]);
        }
        else if(in_grid(GIDs[i]) )
        {
            //ADD border Road and stuff
            int other_rank= (GIDs[i+1]/x_world_size)/y_size; 
            bridge_intersection *a = local_grid->border_Road(GIDs[i], GIDs[i+1], other_rank);

            local_grid->add_Road(GIDs[i] , a);
        }
        else if(in_grid(GIDs[i+1]))
        {
            // Add 
            int other_rank= (GIDs[i+1]/x_world_size)/y_size; 
            bridge_intersection *a = local_grid->border_Road(GIDs[i], GIDs[i+1], other_rank);

            local_grid->add_Road(a , GIDs[i+1]);
        }
    }
}




void Rank::Run_Sim()
{
    //Take it away Aaron!!!!
    MPI_Barrier(MPI_COMM_WORLD);
    for(int x = 0; x<1000;x++)
    {
        local_grid->run_Tick();
        MPI_Barrier(MPI_COMM_WORLD);
    }
    
    
    
    return;
}

void Rank::Send_Result()
{
    //take it away Chris!!!!
    // THe hell am I taking away here?
    return;
}

void Rank::Reset_Sim()
{
    //Oh this is my job (Ben)
    local_grid->Road_reset();
    //Done
    return;
}

bool Rank::in_grid(int GID)
{
    int GID_rank = (GID/x_world_size)/y_size;

    return (GID_rank == my_rank);
}

int Rank::GID_to_x(int GID)
{
    return GID % x_world_size;
}

int Rank::GID_to_y(int GID)
{
    return GID/(x_world_size);
}

void Rank::printCities()
{
    local_grid->printCities();
}

std::vector<City*> Rank::getCities(){
    return local_grid->getCities();
}