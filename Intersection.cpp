#include "Intersection.h"

void Intersection::add_car(Car car)
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

void Intersection::process_cars()
{
    LinkedNode<Car> * targ = head;
    fprintf(stderr, "i made it?");
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

LinkedNode<Car>* Intersection::getHead()
{
    LinkedNode<Car> *head_ = this->head;
    this->head = head_->next;
    head_->next = NULL;
    return head_;
}


Intersection::Intersection( int x, int y , int GID_)
{
    x_ = x;
    y_ = y;
    GID = GID_;
    head = NULL;
    return;
}

Intersection::Intersection( int x, int y )
{
    x_ = x;
    y_ = y;
    head = NULL;
    return;
}



void Intersection::add_out_Road(Road* new_Road)
{
    new_Road->start = GID;
    new_Road->set_start(x_,y_);
    outConnections.push_back(new_Road);
}

void Intersection::add_in_Road(Road* new_Road)
{
    new_Road->end = GID;
    new_Road->set_end(x_,y_);
    inConnections.push_back(new_Road);
}

Road * Intersection::find_road_to(int GID)
{
    for(int i = 0; i< outConnections.size(); i++)
    {
        if( outConnections[i]->end_GID() == GID ) return outConnections[i];
    }
    //fprintf(stderr,"Connection does not exist %d\n" , GID);
    //exit(1);
}

Road * Intersection::find_road_to( Intersection * me)
{
    for(int i = 0; i< outConnections.size(); i++)
    {
        if( outConnections[i]->out == me ) return outConnections[i];
    }
    //fprintf(stderr,"Connection does not exist\n");
    //exit(1);
}

void Intersection::del_cars()
{
    while(head!=NULL)
    {
        LinkedNode<Car>* temp = head;
        head = head->next;
        delete temp;
    }
}

Intersection::~Intersection()
{
    while(head!=NULL)
    {
        LinkedNode<Car>* temp = head;
        head = head->next;
        delete temp;
    }
}

void Intersection::add_options()
{
    for(std::map<int, Road*>::iterator it = directions.begin(); it!= directions.end(); ++it)
    {
        opt_directions[it->first].push_back(it->second);
        for( int i = 0 ; i < outConnections.size(); i++)
        {
            if( outConnections[i]->end == it->second->end)
                opt_directions[it->first].push_back(outConnections[i]);
        }
    }
}
