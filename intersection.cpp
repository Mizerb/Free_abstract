#include "includer.h"
#include "LinkedNode.h"

#include "intersection.h"

intersection::add_car(Car car)
{
    LinkedNode<Car> new_node = LinkedNode(car);
    this.tail->next = &new_node;
    this.tail = &new_node;
    
}

intersection::process_cars()
{
    
    
}

intersection::intersection( int x, int y)
{
    x_ = x;
    y_ = y;
    return;
}