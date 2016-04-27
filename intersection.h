/* Intersection.h */

#include "includer.h"

#ifndef INTERSECTION_H
#define INTERSECTION_H

class intersection
{
    int * look_up;
public:
    virtual char type(){return 'i';}


    int GID; //Grid ID
    
    intersection();
    ~intersection();
    
    


    int get_GID() const{ return GID;} // should I not?
    int send_lookup (int dest) const;
    	
};

intersection::intersection()
{
    return;
}


#endif