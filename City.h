#include "includer.h"


#ifndef CITY_H
#define CITY_H

class City : public intersection{
private:
    char that;
public:
    City();
    City(int GID, int pop);
    ~City();
};

#endif