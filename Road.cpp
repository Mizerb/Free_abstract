#include Road.h


int Road::get_weight() const
{
    return (int) sqrt(((srtcoord.first - endcoord.first)
        *(srtcoord.first - endcoord.first))
        + ((srtcoord.second - endcoord.second)
        *(srtcoord.second - endcoord.second)));
}
    
bool Road::add_car(Car car)
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
    
void Road::process_cars()
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
    

Road::~Road()
{
    while(head!=NULL)
    {
        LinkedNode<Car>* temp = head;
        head = head->next;
        delete temp;
    }
}

Road::LinkedNode<Car>* getHead()
{
    LinkedNode<Car> *head_ = this->head;
    this->head = head_->next;
    head_->next = NULL;
    return head_;
}

void Road::set_connection(Intersection* mine)
{
    out = mine; 
    outConnection = (mine);
}