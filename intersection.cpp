#include "includer.h"
#include "LinkedNode.h"

#include "intersection.h"

void intersection::add_car(Car car)
{
    LinkedNode<Car> new_node(car);
    this->tail->next = &new_node;
    this->tail = &new_node;
}

void intersection::process_cars()
{
    
    return;
}


intersection::intersection( int x, int y)
{
    x_ = x;
    y_ = y;
    return;
}

void intersection::add_out_road(road* new_road)
{
    outConnections.push_back(new_road);
}

void intersection::add_in_road(road* new_road)
{
    inConnections.push_back(new_road);
}