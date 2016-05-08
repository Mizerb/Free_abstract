#include "grid.h"
#include <climits>
#include <queue>


class BFS_compare
{
public:
    bool operator()(intersection * a, intersection *b )
    {
        return (a->dist < b->dist);
    }
};

void grid::BFS(intersection* root)
{
    std::priority_queue<intersection*,std::vector<intersection*>, BFS_compare> Q;
    
    
    for(int i = 0 ; i < intersections.size(); i++)
      for(int j =0 ; j< intersections[i].size(); j++)
    {
        intersections[i][j]->dist = INT_MAX;
        Q.push(intersections[i][j]);
    }
    
    for(int i = 0; i < bridges.size() ; i++ )
    {
        bridges[i].dist = INT_MAX;
        Q.push(dynamic_cast<intersection*>(&bridges[i]));
    }
    
    root->dist = 0;
    
    
    while( not Q.empty() )
    {
        intersection* local = Q.top(); Q.pop();
        for( int i=0 ; i < local->outConnections.size() ; i++)
        {
            intersection* out = this->find_GID(local->outConnections[i]->end_GID());
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
void grid::BFS(bridge_intersection* root)
{
    
    /*
    std::priority_queue<intersection*,std::vector<intersection*>, BFS_compare> Q;
    for(int i = 0 ; i < intersections.size(); i++)
      for(int j =0 ; j< intersections[i].size(); j++)
    {
        intersections[i][j].dist = INT_MAX;
        intersections[i][j].prev = NULL;
        Q.push(intersections[i][j].dist = INT_MAX);
    }
    
    for(int i = 0; i < bridges.size() ; i++ )
    {
        bridges[i].dist = INT_MAX; bridges[i].prev = NULL;
        Q.push(dynamic_cast<intersection*>(&bridges[i]));
    }
    
    root->dist = 0;
    
    
    while( not Q.empty() )
    {
        intersection* local = Q.top();
        for( int i=0 ; i < local->outConnections.size() ; i++)
        {
            intersection* out = local.outConnections[i].get_intersection();
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
    intersection* path_end = dynamic_cast<intersection*> (path_enda);
    if( path_end->prev == NULL) return; //was not reached
    int GID_to = path_end->GID;
    while( path_end->dist > 0)
    {
        Road * path = path_end->prev->find_road_to(path_end->GID);
        path_end->prev->directions[GID_to] = path;
        path_end = path_end->prev;
    }
}

void grid::trace_back(bridge_intersection * path_enda , City * from)  //OUTGOING BRIDGE TO CITY
{
    intersection* path_end = dynamic_cast<intersection*> (path_enda);
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

void grid::trace_back(bridge_intersection * path_enda , bridge_intersection * from) //INCOMING BRIDGE TO OUTGOING BRIDGE
{
    intersection* path_end = dynamic_cast<intersection*> (path_enda);
    if( path_end->prev == NULL) return; //was not reached
    
    int GID_to = path_end->GID;
    path_enda->linked_bridges->push_back(std::make_pair<bridge_intersection*,int>(from,path_end->dist));
    while( path_end->dist > 0)
    {
        Road * path = path_end->prev->find_road_to(from->GID);
        path_end->prev->directions[GID_to] = path;
        path_end = path_end->prev;
    }
}

void grid::trace_back(City * path_enda , bridge_intersection * from) //CITY TO INCOMING BRIDGE
{
    intersection* path_end = dynamic_cast<intersection*> (path_enda);
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
    intersection * curr; 
    
    for( std::map<int,std::pair<int,int> >::iterator it = (*start->ways_out).begin(); it!=(*start->ways_out).end(); ++it  )
    {
        curr = dynamic_cast<intersection*> (start);
        while(curr->type() != 'b')
        {
            curr->directions[it->first] = curr->directions[it->second.second];
            intersection * next = curr->directions[it->first]->get_out();
            curr = next;
        }
    }
}

void grid::write_routes( bridge_intersection* start)
{
    intersection * curr; 
    
    for( std::map<int,std::pair<int,int> >::iterator it = (*start->ways_out).begin(); it!=(*start->ways_out).end(); ++it  )
    {
        curr = dynamic_cast<intersection*> (start);
        while(curr->type() != 'b')
        {
            curr->directions[it->first] = curr->directions[it->second.second];
            intersection * next = curr->directions[it->first]->get_out();
            curr = next;
        }
    }
}

void grid::find_routing() //yes?
{
    // Find paths to important points in the interior
    // Set the paths in place in all local intersections
    
    for(int i=0 ; i < bridges.size() ; i++) bridges[i].route_prep();
    
    //First from all the cities to other cities, and outgoing bridges
    for(int i = 0 ; i <Cities.size(); i++ )
    {
        BFS(dynamic_cast<intersection*> (Cities[i]));
        for(int j = 0 ; j < Cities.size(); j++)
        {
            if(j==i) continue;
            trace_back( Cities[j] );
        }
        for(int j=0 ; j < bridges.size(); j++)
        {
            if( not bridges[j].is_outgoing() ) continue;
            trace_back( &bridges[j] , Cities[i]);
        }
    }
    //Then from all outgoing Bridges to all cities and outoging bridges
    for(int i =0 ; i<bridges.size(); i++)
    {
        if( not bridges[i].is_incoming()) continue;
        
        BFS(dynamic_cast<intersection*> (&bridges[i]));
        for(int j = 0 ; j < Cities.size(); j++)
        {
            trace_back(Cities[j], &bridges[i]);
        }
        for(int j=0 ; j < bridges.size(); j++)
        {
            if( not bridges[j].is_outgoing() ) continue;
            trace_back( &bridges[j] , &bridges[i] );
        }
    }
    //Now that we know all the interior routes in each node,
    //  we tell other ranks how what the routes are. 
    for(int i=0 ; i < bridges.size() ; i++) bridges[i].work_prep();
    for( int i = 0; i< total_ranks ; i++ )
    {
        for( int j = 0 ; j < bridges.size(); j++)
        {
            if( bridges[j].is_incoming() )
            {
                bridges[j].swap_links();
            }
        }
        for( int j = 0 ; j < bridges.size(); j++)
        {
            if( bridges[j].is_outgoing() )
            {
                bridges[j].swap_links();
                bridges[j].push_data();
            }
        }
    }
    
    for( int i = 0 ; i< Cities.size() ; i++) Cities[i]->Prep_way();
    
    for( int i = 0 ; i < bridges.size() ; i++)
    {
        if( not bridges[i].is_outgoing() ) continue;
        bridges[i].rank_sharing();
    }
    
    for( int i = 0 ; i< Cities.size() ; i++)
    {
        write_routes( Cities[i]);
        //and incoming nodes
    }
    for( int i = 0 ; i< bridges.size() ; i++)
    {
        if( not bridges[i].is_incoming()) continue;
        write_routes( &bridges[i]);
    }
    
    
    for (int i = 0; i < Cities.size(); i++) delete Cities[i]->ways_out;
    for(int i=0 ; i < bridges.size() ; i++) bridges[i].route_clean();
}

