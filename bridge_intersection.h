/* Bridge Intersection .h */



#ifndef BRIDGE_INTER
#define BRIDGE_INTER

class bridge_intersection : public intersection
{
public:
    bridge_intersection();
    ~bridge_intersection();

    char type(){ return 'b';}
    

    void swap_contents(int rank_swap);
};



#endif