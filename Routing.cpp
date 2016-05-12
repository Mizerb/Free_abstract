#include "grid.h"
#include <climits>
#include <queue>
#include <set>

struct BFS_compare
{
    bool operator()( const Intersection * a, const Intersection *b ) const
    {
        return (a->dist < b->dist);
    }
};

void grid::BFS(Intersection* root)
{
    //std::priority_queue<Intersection*,std::vector<Intersection*>, BFS_compare> Q;
    
    std::multiset<Intersection*, BFS_compare> Q;
    std::multiset<Intersection*, BFS_compare>::iterator it;

    printf("Double check? %d , %d\n", (int)Intersections.size(), (int)Intersections[0].size());
    //printf("Setting everything to zero\n");
    for(int i = 0 ; i < Intersections.size(); i++)
      for(int j =0 ; j< Intersections[i].size(); j++)
    {
        Intersections[i][j]->dist = INT_MAX;
        Intersections[i][j]->prev = NULL;
        Q.insert(Intersections[i][j]);
    }

    printf("size change? %d\n", (int)Q.size());
    
    for(int i = 0; i < Bridges.size() ; i++ )
    {
        Bridges[i]->dist = INT_MAX;
        Bridges[i]->prev = NULL;
        Q.insert(dynamic_cast<Intersection*> (Bridges[i]));
    }
    
    it = Q.find( root);


    if( it == Q.end() )
    {
        printf( "Non zero top value \n");
        exit(1);
    }

    root->dist = 0;
    Q.erase(it); Q.insert(root);

    printf("Q size: %d , top GID %d root GID %d\n",(int)Q.size(), (*Q.begin())->GID, root->GID  );


    //exit(1);
    //printf("into the BFS\n");
    while( not Q.empty() )
    {
        Intersection* local = *(Q.begin()); Q.erase(local);
        for( int i=0 ; i < local->outConnections.size() ; i++)
        {
            Intersection* out = local->outConnections[i]->out;
            it = Q.find( out);
            if( it == Q.end() ){
                printf("Connection not found\n");
                continue;
            }
            else
            {
                root->dist = 0;
                //printf("Connection Found\n");
            } 
            if( out == NULL) printf("shit\n");
            //printf("out test %d\n" ,local->outConnections[i]->start);
            int alt = local->outConnections[i]->get_weight() + local->dist;
            if( alt < out->dist )
            {
                out -> dist = alt;
                out -> prev = local;
                it = Q.find( out);
                if( it == Q.end() )
                {
                  printf( "LInked to intersec not in Q...\n");
                  printf( "GID of OUT: %d\n" ,out->GID );
                  Q.insert(out);  
                } 
                else Q.erase(out);Q.insert(out);
            
            }
        }
    }

}

/*
void grid::BFS(Bridge_Intersection* root)
{
    
    /*
    std::priority_queue<Intersection*,std::vector<Intersection*>, BFS_compare> Q;
    for(int i = 0 ; i < Intersections.size(); i++)
      for(int j =0 ; j< Intersections[i].size(); j++)
    {
        Intersections[i][j].dist = INT_MAX;
        Intersections[i][j].prev = NULL;
        Q.push(Intersections[i][j].dist = INT_MAX);
    }
    
    for(int i = 0; i < Bridges.size() ; i++ )
    {
        Bridges[i].dist = INT_MAX; Bridges[i].prev = NULL;
        Q.push(dynamic_cast<Intersection*>(&Bridges[i]));
    }
    
    root->dist = 0;
    
    
    while( not Q.empty() )
    {
        Intersection* local = Q.top();
        for( int i=0 ; i < local->outConnections.size() ; i++)
        {
            Intersection* out = local.outConnections[i].get_Intersection();
            int alt = local.outConnections[i].get_weight() + local.dist;
            if( alt < out->dist )
            {
                out -> dist = alt;
                out -> prev = local;
            }
        }
    }
    
}
*/

void grid::trace_back(City* path_enda ) //CITY TO CITY
{
    Intersection* path_end = dynamic_cast<Intersection*> (path_enda);
    if( path_end->prev == NULL) return; //was not reached
    printf(" Got 1\n");
    int GID_to = path_end->GID;
    while( path_end->dist > 0)
    {
        Road * path = path_end->prev->find_road_to(path_end);
        path_end->prev->directions[GID_to] = path;
        path_end = path_end->prev;
        printf("moved back");
    }
}

void grid::trace_back(Bridge_Intersection * path_enda , City * from)  //OUTGOING BRIDGE TO CITY
{
    Intersection* path_end = dynamic_cast<Intersection*> (path_enda);
    if( path_end->prev == NULL) return; //was not reached
    
    int GID_to = path_end->GID;
    path_enda->linked_cites->push_back(std::make_pair<City*,int>(from,path_end->dist));
    while( path_end->dist > 0)
    {
        Road * path = path_end->prev->find_road_to(path_end->GID);
        path_end->prev->directions[GID_to] = path;
        path_end = path_end->prev;
    }
}

void grid::trace_back(Bridge_Intersection * path_enda , Bridge_Intersection * from) //INCOMING BRIDGE TO OUTGOING BRIDGE
{
    Intersection* path_end = dynamic_cast<Intersection*> (path_enda);
    if( path_end->prev == NULL) return; //was not reached
    
    int GID_to = path_end->GID;
    path_enda->linked_Bridges->push_back(std::make_pair<Bridge_Intersection*,int>(from,path_end->dist));
    while( path_end->dist > 0)
    {
        Road * path = path_end->prev->find_road_to(from->GID);
        path_end->prev->directions[GID_to] = path;
        path_end = path_end->prev;
    }
}

void grid::trace_back(City * path_enda , Bridge_Intersection * from) //CITY TO INCOMING BRIDGE
{
    Intersection* path_end = dynamic_cast<Intersection*> (path_enda);
    if( path_end->prev == NULL) return; //was not reached
    
    int GID_to = path_end->GID;
    from->linked_cites->push_back(std::make_pair<City*,int>(path_enda,path_end->dist));
    while( path_end->dist > 0)
    {
        Road * path = path_end->prev->find_road_to(path_end->GID);
        path_end->prev->directions[GID_to] = path;
        path_end = path_end->prev;
    }
}

void grid::write_routes(City* start)
{
    Intersection * curr; 
    
    for( std::map<int,std::pair<int,int> >::iterator it = (*start->ways_out).begin(); it!=(*start->ways_out).end(); ++it  )
    {
        curr = dynamic_cast<Intersection*> (start);
        while(curr->type() != 'b')
        {
            curr->directions[it->first] = curr->directions[it->second.second];
            Intersection * next = curr->directions[it->first]->get_out();
            curr = next;
        }
    }
}

void grid::write_routes( Bridge_Intersection* start)
{
    Intersection * curr; 
    
    for( std::map<int,std::pair<int,int> >::iterator it = (*start->ways_out).begin(); it!=(*start->ways_out).end(); ++it  )
    {
        curr = dynamic_cast<Intersection*> (start);
        while(curr->type() != 'b')
        {
            curr->directions[it->first] = curr->directions[it->second.second];
            Intersection * next = curr->directions[it->first]->get_out();
            curr = next;
        }
    }
}

void grid::safty_dance()
{
  for(int i = 0 ; i < Intersections.size() ; i++)
    for(int j = 0 ; j < Intersections[i].size() ; j++)
  {
    if( Intersections[i][j]->outConnections.size() ==0 ) continue;
    if( Intersections[i][j]->GID !=  Intersections[i][j]->outConnections[0]->start)
    {
        printf("Pissing Piles of Precum\n");
        printf( "X: %d Y: %d , end: %p\n" , j , i , Intersections[i][j]->outConnections[0]->out);
    }
    //else printf("safe X :%d  Y: %d  end:%p \n", j,i ,Intersections[i][j]->outConnections[0]->out );
  }
}

void grid::run_test()
{
    char filename[20];
    sprintf(filename, "Bridge_Check_%d.txt", rank_displace/(x_size)/y_size);


    FILE* a = fopen(filename, "w");

    for( int i = 0 ; i< Bridges.size() ; i++)
    {
        Bridges[i]->GID_testing(a);
        //Bridges[i]->Connection_testing();
    }

    printf("Bridge tests run\n");
    fclose(a);

    for( int i = 0 ; i< Bridges.size() ; i++)
    {
        //Bridges[i]->GID_testing(a);
        Bridges[i]->Connection_testing_S();
    }

    for( int i = 0 ; i< Bridges.size() ; i++)
    {
        //Bridges[i]->GID_testing(a);
        Bridges[i]->Connection_testing_R();
    }
}

void grid::Connection_test()
{
    char filename[128];
    sprintf(filename, "Connection_check_.txt" /*, rank_displace/(x_size)/y_size*/);

    FILE* a = fopen(filename, "w");

    for( int i= 0 ; i< Cities.size() ; i++)
    {
        fprintf( a ,"GID:%d Connects to %d \n", Cities[i]->GID, Cities[i]->Connections());
    }
    fclose(a);
}

void grid::find_routing() //yes?
{
    /* why test here */
    printf("runnings tests\n");
    run_test();
    printf("RANK %d done with tests\n", (rank_displace)/y_size/x_size);
    /* Why not */
MPI_Barrier(MPI_COMM_WORLD);
    // Find paths to important points in the interior
    // Set the paths in place in all local Intersections
    printf("basic prep\n");
    for(int i=0 ; i < Bridges.size() ; i++) Bridges[i]->route_prep();
    printf("basic prep done\n");
    //First from all the cities to other cities, and outgoing Bridges
    
    
    //for( int i = 0 ; i< Roads.size() ; i++) printf("ROADING %d\n", Roads[i].out->GID);
    
    safty_dance();

    //exit(1);


    printf("BFS time\n");
    for(int i = 0 ; i <Cities.size(); i++ )
    {
        //printf("Running BFS\n");
        BFS(dynamic_cast<Intersection*> (Cities[i]));
        //printf("completed BFS\n");
        for(int j = 0 ; j < Cities.size(); j++)
        {
            if(j==i) continue;
            //printf("Performing back trace\n");
            trace_back( Cities[j] );
            //printf("Back Trace worked\n");
        }
        for(int j=0 ; j < Bridges.size(); j++)
        {
            if( not Bridges[j]->is_outgoing() ) continue;
            trace_back( Bridges[j] , Cities[i]);
        }
    }
    //Then from all outgoing Bridges to all cities and outoging Bridges
    for(int i =0 ; i<Bridges.size(); i++)
    {
        if( not Bridges[i]->is_incoming()) continue;
        
        BFS(dynamic_cast<Intersection*> (Bridges[i]));
        for(int j = 0 ; j < Cities.size(); j++)
        {
            trace_back(Cities[j], Bridges[i]);
        }
        for(int j=0 ; j < Bridges.size(); j++)
        {
            if( not Bridges[j]->is_outgoing() ) continue;
            trace_back( Bridges[j] , Bridges[i] );
        }
    }
    //exit(1);
    printf("end of BFS time\n");
    //Now that we know all the interior routes in each node,
    //  we tell other ranks how what the routes are. 

    //exit(1);
     printf("swapping links\n");
    for(int i=0 ; i < Bridges.size() ; i++) Bridges[i]->work_prep();
    for( int i = 0; i< total_ranks ; i++ )
    {
        for( int j = 0 ; j < Bridges.size(); j++)
        {
            if( Bridges[j]->is_incoming() )
            {
               
                Bridges[j]->swap_links();
                //printf("sent out the links\n");
            }
        }
        //printf("send all links\n");
        for( int j = 0 ; j < Bridges.size(); j++)
        { 
            if( Bridges[j]->is_outgoing() )
            {
                printf( "RANK: %d ,GID %d\n", (rank_displace)/x_size/y_size, Bridges[j]->GID);
                Bridges[j]->swap_links();
                Bridges[j]->push_data();
            }
        }
    }
    printf("MPI messaged finished(correctly?)\n");
    for( int i = 0 ; i< Cities.size() ; i++) Cities[i]->Prep_way();
    
    for( int i = 0 ; i < Bridges.size() ; i++)
    {
        if( not Bridges[i]->is_outgoing() ) continue;
        printf("RUNNING CLUSTERFUCK\n");
        Bridges[i]->rank_sharing();
        printf("FINISHED RUNNING CLUSTERFUCK\n");
    }
    
    for( int i = 0 ; i< Cities.size() ; i++)
    {
        write_routes( Cities[i]);
        //and incoming nodes
    }
    for( int i = 0 ; i< Bridges.size() ; i++)
    {
        if( not Bridges[i]->is_incoming()) continue;
        write_routes( Bridges[i]);
    }
    
    
    for (int i = 0; i < Cities.size(); i++) delete Cities[i]->ways_out;
    for(int i=0 ; i < Bridges.size() ; i++) Bridges[i]->route_clean();
    for(int i = 0 ; i < Intersections.size() ; i++)
      for( int j = 0 ; j < Intersections[i].size() ; j++)
    {
        Intersections[i][j]->add_options();
    }
    
    printf("routed successfully\n");
    Connection_test();
}

void grid::path_testing( Intersection* start , int target_GID)
{

}