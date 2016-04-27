/* Intersection.h */

#ifndef INTERSECTION_H
#define INTERSECTION_H

class intersection
{
    int * look_up;
    int x_;
    int y_;
public:
    virtual char type(){return 'i';}


    int GID; //Grid ID
    
    intersection();
    intersection(int x, int y);
    ~intersection(){delete look_up;}
    
    


    int get_GID() const{ return GID;} // should I not?
    int send_lookup (int dest) const;
    	
};




#endif