/* Bridge Intersection .h */
#ifndef BRIDGE_INTER
#define BRIDGE_INTER

#include "intersection.h"
#include "City.h"
#include "stdlib.h"

class bridge_intersection : public intersection
{
public:
    int share_rank;
    
    std::vector<bridge_intersection> *link_sharing;
    
    std::vector<std::pair<City*,int> > *linked_cites;
    
    std::vector<std::pair<bridge_intersection*,int> > *linked_bridges;

    std::map<int,int> *visable_cities;
    
    std::map<int,std::pair<int,int> > *ways_out;

    bridge_intersection(int GID_ , int share_rank_): 
        intersection(GID_), share_rank(share_rank_) {}
    /*
    bridge_intersection(int x_, int y_ , int share_rank): 
        intersection(x_,y_) {}
    */
    //~bridge_intersection();
    
    void add_cars(int* cars, int size);
    
    bool is_receiver(){ return (1==outConnections.size());}
    
    bool is_incoming(){ return is_receiver();}
    bool is_outgoing(){ return !(is_receiver());}
    
    int count_cars();
    
    void process_cars();
    
    int * prepare_cars();

    char type() const{ return 'b';}
    
    void route_prep();
    void route_clean();
    void work_prep();

    void swap_links();
    void push_data();
    void rank_sharing();
    //void swap_links_collect();
};


#endif