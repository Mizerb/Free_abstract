#include "Car.h"


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
        }
        
        
        LinkedNode(T indata)
        {
            data = indata;
            next = NULL;
            
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
        
        
    private:
        LinkedNode<T> * next;
        
    
    
    
};

#endif