/* Bridge Intersection .h */
#ifndef BRIDGE_INTER
#define BRIDGE_INTER

#include "intersection.h"

class bridge_intersection : public intersection
{
public:
    bridge_intersection();
    bridge_intersection(int x_, int y_): intersection(x_,y_) {}
    ~bridge_intersection();

    char type(){ return 'b';}
    

    void swap_contents(int rank_swap);
};



#endif