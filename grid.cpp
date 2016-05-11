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
            if(Bridges[i]->GID == GID) 
                return /*dynamic_cast<Intersection*>*/ (Bridges[i]);
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
        //printf("GID: %d:X %d, Y%d\n ", displace, x_counter, i);
        Intersections.push_back(local_ptrs);
    }

    /*Is there anything else?*/
}

void grid::add_city(int x, int y , int pop)
{
    int temp = this->Intersections[y][x]->get_GID();
    delete this->Intersections[y][x];
    Intersections[y][x] = /*dynamic_cast<Intersection*>*/(new City(temp , x , y,  pop));
    Cities.push_back(dynamic_cast<City*>(Intersections[y][x]));
    //printf("GID: %d, X: %d , Y: %d\n" ,Intersections[y][x]->get_GID(), Intersections[y][x]->x_ , Intersections[y][x]->y_ );
}


void grid::add_Road(int GID_f , int GID_t)
{
    
    GID_f -= rank_displace;
    GID_t -= rank_displace;
    //printf("crashing\n?");
    
    int x = GID_f%x_size;
    int y = GID_f/x_size;
    //printf("F: %d x:%d y:%d \n", GID_f ,  x , y );
    Road * local = new Road(GID_f, GID_t);
    //("HAHA\n");
    Roads.push_back(local);
    //printf("make sure\n");
    local->set_start( x, y );
    //printf("after me\n");


    Intersections[y][x]->add_out_Road(local);
    //printf("here\n");
    x = GID_t%x_size;
    y = GID_t/x_size;
    local->set_end(x,y);
    Intersections[y][x]->add_in_Road(local);
    Intersection* a = Intersections[y][x];
    //printf("%d\n", a->GID);
    //printf("F: %d x:%d y:%d \n", GID_t ,  x , y );
    //printf("Where do I crash?\n");
    local->set_connection(Intersections[y][x]);
    

/*
    x = GID_f%x_size;
    y = GID_f/x_size;

    if( Intersections[y][x]->outConnections.back()->out != a ) //Somehow this isn't being met.
    {
        printf("Steaming dogshit\n");
        exit(1);
    }
    if( Intersections[y][x]->outConnections.back()->end != GID_t+rank_displace)
    {
        printf("test failed GRID roading\n");
        exit(1);
    }
    if( Intersections[y][x]->outConnections.back()->start != GID_f+rank_displace)
    {
        printf("FUCK, road start is not equal to intersection\n");
        exit(1);
    }

    if( GID_t == 32767 || GID_f == 32767) printf("steaming shit\n");

    //printf("finally\n");
    for(int i = 0 ; i < Intersections.size() ; i++)
    for(int j = 0 ; j < Intersections[i].size() ; j++)
    {
        if( Intersections[i][j]->outConnections.size() ==0 ) continue;
        if( Intersections[i][j]->GID !=  Intersections[i][j]->outConnections[0]->start)
        {
            printf("Pissing Piles of Precum\n");
            printf( "X: %d Y: %d , end: %d\n" , j , i , Intersections[i][j]->outConnections[0]->end);
            
            printf("X: %d Y: %d\n" , Intersections[i][j]->outConnections[0]->endcoord.first ,Intersections[i][j]->outConnections[0]->endcoord.second);
            exit(1);
        }
        //else //printf("safe X :%d  Y: %d  end:%p \n", j,i ,Intersections[i][j]->outConnections[0]->out );
    }
*/ 
}
#include <climits>

Bridge_Intersection* grid::border_Road(int GID_f, int GID_t, int other_rank)
{
    int x = (((.5)*(((GID_f+GID_t)*(GID_f+GID_t+1))+GID_t)));
    x = (x % INT_MAX);
    x = x *-1;
    //printf("GIDt: %d  GIDf: %d  x: %d  other_rank %d\n",GID_f, GID_t, x, other_rank );
    Bridge_Intersection * local = new Bridge_Intersection(x, other_rank);

    Bridges.push_back(local);

    return local;
}

void grid::add_Road(Bridge_Intersection* from_, int GID_t, int GID_f)
{
    Road * local = new Road(from_->get_GID(),GID_t );
    local->set_start(-1,-1);
    
    int aGID_t = GID_t - rank_displace;
    
    Roads.push_back(local);
    
    from_->add_out_Road(local);

    if( GID_t == 32767 || from_->GID == 32767) printf("steaming shit\n");

    int x = aGID_t%x_size;
    int y = aGID_t/x_size;
    local->set_end(x,y);

    Intersections[y][x]->add_in_Road(local);
    Intersection * a = Intersections[y][x];
    local->set_connection(Intersections[y][x] );
    
    x = (GID_t%x_size) - (GID_f%x_size);
    y = (GID_t/x_size) - (GID_f/x_size);
    
    local->set_weight( sqrt( (x*x)+(y*y))/2 );


    if( from_->outConnections.back()->out != a ) //Somehow this isn't being met.
    {
        printf("Steaming dogshit\n");
        exit(1);
    }
    if( from_->outConnections.back()->end != aGID_t+rank_displace)
    {
        printf("test failed GRID roading\n");
        exit(1);
    }
    if( from_->outConnections.back()->start != from_->GID)
    {
        printf("FUCK, road start is not equal to intersection\n");
        exit(1);
    }

    return;
} 

void grid::add_Road(int GID_f, Bridge_Intersection* to, int GID_t)
{
    Road * local = new Road(GID_f, to->get_GID());
    Roads.push_back(local);

    local->set_end(-1,-1); // CAN BE CHANGED
    to->add_in_Road(local);

    int aGID_f= GID_f -  rank_displace;

    int x = aGID_f%x_size;
    int y = aGID_f/x_size;
    local->set_start(x,y);
    Intersections[y][x]->add_out_Road(local);
    
    local->set_connection( /*dynamic_cast<Intersection*>*/(to) );

    if( GID_f == 32767 || to->GID == 32767) printf("steaming shit\n");

    x = (GID_t%x_size) - (GID_f%x_size);
    y = (GID_t/x_size) - (GID_f/x_size);
    
    local->set_weight( sqrt( (x*x)+(y*y))/2 );

/*
    if( Intersections[y][x]->outConnections.back()->out != to ) //Somehow this isn't being met.
    {
        printf("Steaming dogshit\n");
        exit(1);
    }
    if( Intersections[y][x]->outConnections.back()->end != to->GID)
    {
        printf("test failed GRID roading\n");
        exit(1);
    }
    if( Intersections[y][x]->outConnections.back()->start != GID_f+rank_displace)
    {
        printf("FUCK, road start is not equal to intersection\n");
        exit(1);
    }
*/

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

    for( int i = 0 ; i<Roads.size() ; i++)
    {
        delete Roads[i];
    }

    for( int i = 0; i< Bridges.size() ; i++)
    {
        delete Bridges[i];
    }


    Bridges.clear();
    Roads.clear();

}


void grid::run_Tick()
{
    for(int i = 0;i<Roads.size();i++)
    {
        Roads[i]->process_cars();
    }
    for(int i = 0;i<Bridges.size();i++)
    {
        Bridges[i]->process_cars();
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