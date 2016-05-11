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

    local_grid = new grid(x_world_size , y_size, y_size*x_world_size*my_rank);
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
            
            //printf("[%d] City %d: %d %d\n", my_rank , i/3,cities_[i], cities_[i+1]);
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
            ////printf("start\n");
            if( GIDs[i] == GIDs[i+1])
            {
                //printf("Intersections can't lead to themselves\n");
                continue;
            }
            local_grid->add_Road( GIDs[i], GIDs[i+1]);
            //printf("finish\n");
        }
        else if(in_grid(GIDs[i]) )
        {
            //ADD border Road and stuff
            
            //printf("shit a GID:%d\n", GIDs[i+1]);
            int other_rank= (GIDs[i+1]/x_world_size)/y_size; 
            Bridge_Intersection *a = local_grid->border_Road(GIDs[i], GIDs[i+1], other_rank);

            local_grid->add_Road(GIDs[i] , a , GIDs[i+1]);
        }
        else if(in_grid(GIDs[i+1]))
        {
            // Add 
            //printf("shit b\n");
            //printf("shit a GID:%d\n", GIDs[i]);
            int other_rank= (GIDs[i]/x_world_size)/y_size; 
            Bridge_Intersection *a = local_grid->border_Road(GIDs[i], GIDs[i+1], other_rank);

            local_grid->add_Road(a , GIDs[i+1], GIDs[i]);
        }
    }

    printf( "RANK %d:BRIDGE NODE COUNT %d\n",my_rank, local_grid->get_bridge_size());
    printf( "RANK %d:ROAD COUNTING %d\n",my_rank, local_grid->get_road_count());
    //exit(1);

    //local_grid->safty_dance();
}




void Rank::Run_Sim()
{
    fprintf(stderr, "hello!\n");
    //Take it away Aaron!!!!
    MPI_Barrier(MPI_COMM_WORLD);
    for(int i=0; i<local_grid->Cities.size();i++)
    {
        local_grid->Cities[i]->make_Cars();
    }
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
    /*
    if( GID_rank != 0)
    {
        printf( "GID: %d , ysize: %d , xsize: %d\n", GID, y_size, x_world_size );
        printf( "f: %d  s: %d",GID/x_world_size, (GID/x_world_size)/y_size );
        exit(1);
    }
    */
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