/* Bridge Intersection .h */
#ifndef BRIDGE_INTER
#define BRIDGE_INTER

#include "Intersection.h"
#include "City.h"
#include "stdlib.h"

class Bridge_Intersection : public Intersection
{
public:
    int share_rank;
    
    std::vector<Bridge_Intersection> *link_sharing;
    
    std::vector<std::pair<City*,int> > *linked_cites;
    
    std::vector<std::pair<Bridge_Intersection*,int> > *linked_Bridges;

    std::map<int,int> *visable_cities;
    
    std::map<int,std::pair<int,int> > *ways_out;

    Bridge_Intersection(int GID_ , int share_rank_): 
        Intersection(GID_), share_rank(share_rank_) {}
    /*
    Bridge_Intersection(int x_, int y_ , int share_rank): 
        Intersection(x_,y_) {}
    */
    //~Bridge_Intersection();
    
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