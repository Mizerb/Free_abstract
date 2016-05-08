#include "Car.h"
#include "includer.h"


#ifndef LINKEDNODE_H
#define LINKEDNODE_H

template<class T>
class LinkedNode
{
public:
    T data;
    LinkedNode()
    {
        //default contructor
        next = NULL;
        prev = NULL;
    }
    
    LinkedNode(T indata)
    {
        data = indata;
        next = NULL;
        prev = NULL;
    }
    
    LinkedNode<T>* DeleteAfter()
    {
          // store the next Node in a temporary Node
        LinkedNode<T>* tempNode = next;
        // check if there is a next node
        if(next != NULL)
            next = next->next;

        return tempNode;
    }
    
    
    LinkedNode<T> * next;
    LinkedNode<T> * prev;

    
    
};

#endif