#include "grid.h"
#include <climits>
#include <queue>


class BFS_compare
{
public:
    bool operator()(Intersection * a, Intersection *b )
    {
        return (a->dist < b->dist);
    }
};

void grid::BFS(Intersection* root)
{
    std::priority_queue<Intersection*,std::vector<Intersection*>, BFS_compare> Q;
    
    
    for(int i = 0 ; i < Intersections.size(); i++)
      for(int j =0 ; j< Intersections[i].size(); j++)
    {
        Intersections[i][j]->dist = INT_MAX;
        Q.push(Intersections[i][j]);
    }
    
    for(int i = 0; i < Bridges.size() ; i++ )
    {
        Bridges[i].dist = INT_MAX;
        Q.push(dynamic_cast<Intersection*>(&Bridges[i]));
    }
    
    root->dist = 0;
    
    
    while( not Q.empty() )
    {
        Intersection* local = Q.top(); Q.pop();
        for( int i=0 ; i < local->outConnections.size() ; i++)
        {
            Intersection* out = this->find_GID(local->outConnections[i]->end_GID());
            int alt = local->outConnections[i]->get_weight() + local->dist;
            if( alt < out->dist )
            {
                out -> dist = alt;
                out -> prev = local;
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
    int GID_to = path_end->GID;
    while( path_end->dist > 0)
    {
        Road * path = path_end->prev->find_road_to(path_end->GID);
        path_end->prev->directions[GID_to] = path;
        path_end = path_end->prev;
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

void grid::find_routing() //yes?
{
    // Find paths to important points in the interior
    // Set the paths in place in all local Intersections
    
    for(int i=0 ; i < Bridges.size() ; i++) Bridges[i].route_prep();
    
    //First from all the cities to other cities, and outgoing Bridges
    for(int i = 0 ; i <Cities.size(); i++ )
    {
        BFS(dynamic_cast<Intersection*> (Cities[i]));
        for(int j = 0 ; j < Cities.size(); j++)
        {
            if(j==i) continue;
            trace_back( Cities[j] );
        }
        for(int j=0 ; j < Bridges.size(); j++)
        {
            if( not Bridges[j].is_outgoing() ) continue;
            trace_back( &Bridges[j] , Cities[i]);
        }
    }
    //Then from all outgoing Bridges to all cities and outoging Bridges
    for(int i =0 ; i<Bridges.size(); i++)
    {
        if( not Bridges[i].is_incoming()) continue;
        
        BFS(dynamic_cast<Intersection*> (&Bridges[i]));
        for(int j = 0 ; j < Cities.size(); j++)
        {
            trace_back(Cities[j], &Bridges[i]);
        }
        for(int j=0 ; j < Bridges.size(); j++)
        {
            if( not Bridges[j].is_outgoing() ) continue;
            trace_back( &Bridges[j] , &Bridges[i] );
        }
    }
    //Now that we know all the interior routes in each node,
    //  we tell other ranks how what the routes are. 
    for(int i=0 ; i < Bridges.size() ; i++) Bridges[i].work_prep();
    for( int i = 0; i< total_ranks ; i++ )
    {
        for( int j = 0 ; j < Bridges.size(); j++)
        {
            if( Bridges[j].is_incoming() )
            {
                Bridges[j].swap_links();
            }
        }
        for( int j = 0 ; j < Bridges.size(); j++)
        {
            if( Bridges[j].is_outgoing() )
            {
                Bridges[j].swap_links();
                Bridges[j].push_data();
            }
        }
    }
    
    for( int i = 0 ; i< Cities.size() ; i++) Cities[i]->Prep_way();
    
    for( int i = 0 ; i < Bridges.size() ; i++)
    {
        if( not Bridges[i].is_outgoing() ) continue;
        Bridges[i].rank_sharing();
    }
    
    for( int i = 0 ; i< Cities.size() ; i++)
    {
        write_routes( Cities[i]);
        //and incoming nodes
    }
    for( int i = 0 ; i< Bridges.size() ; i++)
    {
        if( not Bridges[i].is_incoming()) continue;
        write_routes( &Bridges[i]);
    }
    
    
    for (int i = 0; i < Cities.size(); i++) delete Cities[i]->ways_out;
    for(int i=0 ; i < Bridges.size() ; i++) Bridges[i].route_clean();
}

