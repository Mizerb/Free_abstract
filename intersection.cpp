#include "includer.h"
#include "LinkedNode.h"

#include "intersection.h"

void intersection::add_car(Car car)
{
    LinkedNode<Car>* new_node = new LinkedNode<Car>(car);
    if(head == NULL)
    {
        head = new_node;
        tail = new_node;
        return;
    }
    if(tail != NULL)
    {
        this->tail->next = new_node;
        new_node->prev = tail;
    }
    this->tail = new_node;
}

void intersection::process_cars()
{
    LinkedNode<Car> * targ = head;
            while(targ != NULL)
            {
                LinkedNode<Car> * currNode = targ;
                Car currCar = currNode->data;
                bool added = directions[currCar.destination]->add_car(currCar);
                targ = currNode->next;
                if(added)
                {
                    currNode->prev->next = currNode->next;
                    delete(currNode);
                }
            }
    return;
}

LinkedNode<Car>* intersection::getHead()
{
    LinkedNode<Car> *head_ = this->head;
    this->head = head_->next;
    head_->next = NULL;
    return head_;
}


intersection::intersection( int x, int y)
{
    x_ = x;
    y_ = y;
    return;
}



void intersection::add_out_Road(Road* new_Road)
{
    outConnections.push_back(new_Road);
}

void intersection::add_in_Road(Road* new_Road)
{
    inConnections.push_back(new_Road);
}

Road * intersection::find_road_to(int GID)
{
    for(int i = 0; i< outConnections.size(); i++)
    {
        if( outConnections[i]->end_GID() == GID ) return outConnections[i];
    }
    fprintf(stderr,"Connection does not exist\n");
    exit(1);
}

void intersection::del_cars()
{
    while(head!=NULL)
    {
        LinkedNode<Car>* temp = head;
        head = head->next;
        delete temp;
    }
}

intersection::~intersection()
{
    while(head!=NULL)
    {
        LinkedNode<Car>* temp = head;
        head = head->next;
        delete temp;
    }
}