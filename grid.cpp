#include "includer.h"
#include "grid.h"
grid::grid()
{
    return;
}

Intersection * grid::find_GID(int GID)
{
    if( GID > 0 )
    {
        GID -= rank_displace;
        int x = GID % x_size;
        int y = GID / y_size;
        return Intersections[y][x];
    }
    else
    {
        for(int i = 0; i< Bridges.size(); i++)
        {
            if(Bridges[i].GID == GID) 
                return dynamic_cast<Intersection*> (&Bridges[i]);
        }
    }
    fprintf(stderr, "OHHHH SHIT, a lost GID");
    exit(1);
}

grid::grid(int x, int y , int displace)
{
    x_size = x;
    y_size = y;
    int GIDa = displace;
    int x_counter = 0;
    int y_count = displace/x_size;
    for( int i = 0 ; i < y_size ; i++)
    {
        std::vector<Intersection*> local_ptrs(x_size , NULL);
        
        x_counter = 0;
        for (std::vector<Intersection*>::iterator j = local_ptrs.begin(); j != local_ptrs.end(); ++j, x_counter++)
        {
            *j = new Intersection(x_counter,y_count++ , displace++);
             //printf("GID: %d:X %d, Y%d\n ", x_counter, i, displace);
        }
        //printf("GID: %d:X %d, Y%d\n ", x_counter, i, displace)
        Intersections.push_back(local_ptrs);
    }

    /*Is there anything else?*/
}

void grid::add_city(int x, int y , int pop)
{
    int temp = this->Intersections[y][x]->get_GID();
    delete this->Intersections[y][x];
    Intersections[y][x] = dynamic_cast<Intersection*> (new City(temp , x , y,  pop));
    Cities.push_back(dynamic_cast<City*>(Intersections[y][x]));
    //printf("GID: %d, X: %d , Y: %d\n" ,Intersections[y][x]->get_GID(), Intersections[y][x]->x_ , Intersections[y][x]->y_ );
}


void grid::add_Road(int GID_f , int GID_t)
{
    GID_f -= rank_displace;
    GID_t -= rank_displace;
    printf("crashing\n?");
    
    int x = GID_f%x_size;
    int y = GID_f/y_size;
    printf("F: %d x:%d y:%d \n", GID_f ,  x , y );
    Road local(GID_f, GID_t);
    ("HAHA\n");
    Roads.push_back(local);
    printf("make sure\n");
    Roads.back().set_start( x, y );
    printf("after me\n");
    Intersections[y][x]->add_out_Road(&(Roads.back()));
    printf("here\n");
    x = GID_t%x_size;
    y = GID_t/y_size;
    Roads.back().set_end(x,y);
    Intersections[y][x]->add_in_Road(&(Roads.back()));
    printf("Where do I crash?\n");
    Roads.back().set_connection( find_GID(GID_t + rank_displace));
    
    
    printf("finally\n");
}

Bridge_Intersection* grid::border_Road(int GID_f, int GID_t, int other_rank)
{
    int x = (int) (-.5)*(((GID_f+GID_t)*(GID_f+GID_t+1))+GID_t);

    Bridges.push_back(Bridge_Intersection(x, other_rank));

    return &(Bridges.back());
}

void grid::add_Road(Bridge_Intersection* from_, int GID_t)
{
    Roads.push_back(Road(from_->get_GID(),GID_t));
    Roads.back().set_start(-1,-1); // CAN BE CHANGED
    from_->add_out_Road(&(Roads.back()));


    int x = GID_t%x_size;
    int y = GID_t/y_size;
    Roads.back().set_end(x,y);
    Intersections[y][x]->add_in_Road(&(Roads.back()));
    
    Roads.back().set_connection(find_GID(GID_t ) );

    return;
} 

void grid::add_Road(int GID_f, Bridge_Intersection* to)
{
    Roads.push_back(Road(GID_f, to->get_GID()));
    Roads.back().set_end(-1,-1); // CAN BE CHANGED
    to->add_in_Road(&(Roads.back()));


    int x = GID_f%x_size;
    int y = GID_f/y_size;
    Roads.back().set_start(x,y);
    Intersections[y][x]->add_out_Road(&(Roads.back()));
    
    Roads.back().set_connection( dynamic_cast<Intersection*>(to) );

    return;
}


void grid::Road_reset()
{
    for(int i =0 ; i<Intersections.size(); i++)
    {
        for(int j=0; j<Intersections[i].size();i++)
        {
            Intersections[i][j]->clear_connections();
        }
    }
    Bridges.clear();
    Roads.clear();

}


void grid::run_Tick()
{
    for(int i = 0;i<Roads.size();i++)
    {
        Roads[i].process_cars();
    }
    for(int i = 0;i<Bridges.size();i++)
    {
        Bridges[i].process_cars();
    }
    
    for(int i = 0;i<Cities.size();i++)
    {
        Cities[i]->process_cars();
    }
    
    for(int i = 0;i<Intersections.size();i++)
    {
        for(int j = 0; j<Intersections[i].size();j++)
        {
            Intersections[i][j]->process_cars();
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