

template<class T>
class LinkedNode
{
    public:
        T data;
        Node()
        {
            //default contructor
        }
        
        
        Node(T indata)
        {
            data = indata;
            next = NULL;
            
        }
        
        Node<T>* Node<T>::DeleteAfter()
        {
              // store the next Node in a temporary Node
            Node<T>* tempNode = next;
            // check if there is a next node
            if(next != NULL)
                next = next->next;

            return tempNode;
        }
        
        
    private:
        Node<T> * next;
        
    
    
    
}