/*ROAD.h */

#include "includer.h"
#include <utility>
#include "Grid_Object.h"
#include "LinkedNode.h"
#include <math.h>

#ifndef ROAD_H
#define ROAD_H

class intersection;

class Road : public Grid_Object
{
private:
    std::pair<int,int> srtcoord;
    std::pair<int,int> endcoord;
    LinkedNode<Car>* head;
    LinkedNode<Car>* tail;
    Grid_Object * outConnection;
    intersection * out;
    int start; // the crap do these do? // EVERYTHING
    int end;
    int capacity;
public:
    int count;

    int end_GID() const { return end; }
    int get_weight() const
    {
        return (int) sqrt(((srtcoord.first - endcoord.first)
            *(srtcoord.first - endcoord.first))
            + ((srtcoord.second - endcoord.second)
            *(srtcoord.second - endcoord.second)));
    }

    Road(int start_, int finish_): start(start_) , end(finish_) {}
    //~Road();
    
    void set_start(int x, int y){ srtcoord = std::make_pair(x,y); }
    void set_end  (int x, int y){ endcoord = std::make_pair(x,y); }
    void set_connection(intersection* mine){out = mine; }
    intersection * get_out() const { return out; }
    bool add_car(Car car)
    {
        if(count<capacity)
        {
            count++;
            LinkedNode<Car>* new_node = new LinkedNode<Car>(car);
            if(head == NULL)
            {
                head = new_node;
                tail = new_node;
                return true;
            }
            if(tail != NULL)
                this->tail->next = new_node;
                new_node->prev = tail;
            this->tail = new_node;
            return true;
        } else {
            return false;
        }
    }
    
    void process_cars()
    {
        LinkedNode<Car> * targ = head;
        while(targ != NULL)
        {
            count--;
            LinkedNode<Car> * currNode = targ;
            Car currCar = currNode->data;
            outConnection->add_car(currCar);
            targ = currNode->next;
            currNode->prev->next = currNode->next;
            delete(currNode);
        }
        return;
    }
    
    LinkedNode<Car>* getHead()
    {
        LinkedNode<Car> *head_ = this->head;
        this->head = head_->next;
        head_->next = NULL;
        return head_;
    }
    
    ~Road()
    {
        while(head!=NULL)
        {
            LinkedNode<Car>* temp = head;
            head = head->next;
            delete temp;
        }
    }
      
};

#endif