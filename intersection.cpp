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

LinkedNode::getHead()
{
    LinkedNode<Car> *head = this.head;
    this.head = head->next;
    head->next = NULL;
    return head;
}

intersection::intersection( int x, int y)
{
    x_ = x;
    y_ = y;
    return;
}

intersection::add_out_road(road* new_road)
{
    outConnections.push_back(new_road);
}

intersection::add_in_road(road* new_road)
{
    inConnections.push_back(new_road);
}