
#ifndef LINKEDNODE_H
#define LINKEDNODE_H

#include "Car.h"
#include "includer.h"

template<class T>
class LinkedNode
{
public:
    T data;
    LinkedNode();
    
    LinkedNode(T indata);
    
    LinkedNode<T>* DeleteAfter();
    
    LinkedNode<T> * next;
    LinkedNode<T> * prev;

    
    
};


template<class T> LinkedNode<T>::LinkedNode()
{
    //default contructor
    next = NULL;
    prev = NULL;
}
    
    
template<class T> LinkedNode<T>::LinkedNode(T indata)
{
    data = indata;
    next = NULL;
    prev = NULL;
}
    
    
template<class T> LinkedNode<T>* LinkedNode<T>::DeleteAfter()
{
      // store the next Node in a temporary Node
    LinkedNode<T>* tempNode = next;
    // check if there is a next node
    if(next != NULL)
        next = next->next;

    return tempNode;
}

#endif