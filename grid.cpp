#include "includer.h"
#include "grid.h"
grid::grid()
{
    return;
}

intersection * grid::find_GID(int GID)
{
    if( GID > 0 )
    {
        GID -= rank_displace;
        int x = GID % x_size;
        int y = GID / y_size;
        return intersections[y][x];
    }
    else
    {
        for(int i = 0; i< bridges.size(); i++)
        {
            if(bridges[i].GID == GID) 
                return dynamic_cast<intersection*> (&bridges[i]);
        }
    }
    fprintf(stderr, "OHHHH SHIT, a lost GID");
    exit(1);
}

grid::grid(int x, int y )
{
    x_size = x;
    y_size = y;
    
    int x_counter = 0;
    for( int i = 0 ; i < y_size ; i++)
    {
        std::vector<intersection*> local_ptrs(x_size , NULL);
        
        x_counter = 0;
        for (std::vector<intersection*>::iterator j = local_ptrs.begin(); j != local_ptrs.end(); ++j, x_counter++)
        {
            *j = new intersection(x_counter,i);             
        }
        intersections.push_back(local_ptrs);
    }

    /*Is there anything else?*/
}

void grid::add_city(int x, int y , int pop)
{
    int temp = this->intersections[y/*-1*/][x]->get_GID();
    delete this->intersections[y/*-1*/][x];
    intersections[y/*-1*/][x] = new City(temp , pop);
    Cities.push_back((City*)intersections[y/*-1*/][x]);
}


void grid::add_Road(int GID_f , int GID_t)
{
    GID_f -= rank_displace;
    GID_t -= rank_displace;

    int x = GID_f%x_size;
    int y = GID_f/y_size;

    Roads.push_back(Road(GID_f, GID_t));
    Roads.back().set_start( x, y );
    intersections[y][x]->add_out_Road(&(Roads.back()));

    x = GID_t%x_size;
    y = GID_t/y_size;
    Roads.back().set_end(x,y);
    intersections[y][x]->add_in_Road(&(Roads.back()));
    
    Roads.back().set_connection( find_GID(GID_t + rank_displace));
}

bridge_intersection* grid::border_Road(int GID_f, int GID_t, int other_rank)
{
    int x = (int) (-.5)*(((GID_f+GID_t)*(GID_f+GID_t+1))+GID_t);

    bridges.push_back(bridge_intersection(x, other_rank));

    return &(bridges.back());
}

void grid::add_Road(bridge_intersection* from_, int GID_t)
{
    Roads.push_back(Road(from_->get_GID(),GID_t));
    Roads.back().set_start(-1,-1); // CAN BE CHANGED
    from_->add_out_Road(&(Roads.back()));


    int x = GID_t%x_size;
    int y = GID_t/y_size;
    Roads.back().set_end(x,y);
    intersections[y][x]->add_in_Road(&(Roads.back()));
    
    Roads.back().set_connection(find_GID(GID_t ) );

    return;
} 

void grid::add_Road(int GID_f, bridge_intersection* to)
{
    Roads.push_back(Road(GID_f, to->get_GID()));
    Roads.back().set_end(-1,-1); // CAN BE CHANGED
    to->add_in_Road(&(Roads.back()));


    int x = GID_f%x_size;
    int y = GID_f/y_size;
    Roads.back().set_start(x,y);
    intersections[y][x]->add_out_Road(&(Roads.back()));
    
    Roads.back().set_connection( dynamic_cast<intersection*>(to) );

    return;
}


void grid::Road_reset()
{
    for(int i =0 ; i<intersections.size(); i++)
    {
        for(int j=0; j<intersections[i].size();i++)
        {
            intersections[i][j]->clear_connections();
        }
    }
    bridges.clear();
    Roads.clear();

}


void grid::run_Tick()
{
    for(int i = 0;i<Roads.size();i++)
    {
        Roads[i].process_cars();
    }
    for(int i = 0;i<bridges.size();i++)
    {
        bridges[i].process_cars();
    }
    
    for(int i = 0;i<Cities.size();i++)
    {
        Cities[i]->process_cars();
    }
    
    for(int i = 0;i<intersections.size();i++)
    {
        for(int j = 0; j<intersections[i].size();j++)
        {
            intersections[i][j]->process_cars();
        }
    }
}


//Trenton Debug Fucntion
void grid::printCities()
{
    printf("In the print function\n");
    for(int i = 0; i < Cities.size(); i++){
        printf("( %d , %d ) Pop: %d\n", Cities[i]->x_, Cities[i]->y_, Cities[i]->getPop());
    }
}

std::vector<City*> grid::getCities()
{
    return Cities;
}