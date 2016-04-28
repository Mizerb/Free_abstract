/* Bridge Intersection .h */
#ifndef BRIDGE_INTER
#define BRIDGE_INTER

#include "intersection.h"

class bridge_intersection : public intersection
{
private:
    int share_rank;

public:
    bridge_intersection(int GID_ , int share_rank_): 
        intersection(GID_), share_rank(share_rank_) {}
    /*
    bridge_intersection(int x_, int y_ , int share_rank): 
        intersection(x_,y_) {}
    */
    //~bridge_intersection();

    char type(){ return 'b';}
    

    void swap_contents(int rank_swap);
};



#endif