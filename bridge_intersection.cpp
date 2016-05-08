#include "bridge_intersection.h"
#include "intersection.h"

void bridge_intersection::route_prep()
{
    link_sharing = new std::vector<bridge_intersection>();
    linked_cites = new std::vector<std::pair<City*,int> >();
    linked_bridges = new std::vector<std::pair<bridge_intersection*,int> >();
    visable_cities = new std::map<int,int>();
    ways_out = new std::map<int,std::pair<int,int> >();
}

void bridge_intersection::route_clean()
{
    delete link_sharing; 
    delete linked_cites;
    delete linked_bridges;
    delete visable_cities;
    delete ways_out;
}

void bridge_intersection::work_prep()
{
    for (int i = 0; i < linked_cites->size(); i++) 
    {
        (*visable_cities)[(*linked_cites)[i].first->GID] = (*linked_cites)[i].second;
    }
}

void bridge_intersection::add_cars(int* cars, int size)
{
    for(int i=0;i<size;i++)
    {
        LinkedNode<Car>* new_node = new LinkedNode<Car>();
        Car currCar = Car(cars[i]);
        new_node->data = currCar;
        if(head == NULL)
        {   
            this->head = new_node;
            this->tail = new_node;
            return;
        }
        if(tail != NULL)
            this->tail->next = new_node;
            new_node->prev = tail;
        this->tail = new_node;
    }
}

int bridge_intersection::count_cars()
{
    int out = 0;
    LinkedNode<Car> * targ = head;
    while(targ->next!=NULL)
    {
        out++;
        targ = targ->next;
    }
}
    
void bridge_intersection::process_cars()
{
    if(is_receiver()){
        
        int amount;
        MPI_Recv(&amount, 1, MPI_INT, share_rank, GID, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        int * cars = (int*)calloc(sizeof(int),amount);
        MPI_Recv(cars, amount, MPI_INT, share_rank, GID, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        add_cars(cars, amount);
        delete(cars);
        LinkedNode<Car> * targ = head;
        while(targ != NULL)
        {
            LinkedNode<Car> * currNode = targ;
            Car currCar = currNode->data;
            //int dest = send_lookup(currCar.destination);
            //outConnections[dest]->add_car(currCar);
            Road* next = directions[currCar.destination];
            next->add_car(currCar);
            targ = currNode->next;
            currNode->prev->next = currNode->next;
            delete(currNode);
        }
        return;
    } else {
        int * cars = prepare_cars();
        int count = count_cars();
        MPI_Send(cars, count, MPI_INT, share_rank, GID, MPI_COMM_WORLD);
        delete(cars);
    }
}

int * bridge_intersection::prepare_cars()
{
    int count = count_cars();
    int * out = (int*)calloc(sizeof(int),count);
    int i = 0;
    while(this->head!=NULL)
    {
        LinkedNode<Car> * temp = head;
        this->head = this->head->next;
        out[i] = temp->data.destination;
        delete(temp);
        i++;
    }
    head = NULL;
    tail = NULL;
    return out;
}

void bridge_intersection::swap_links()
{
    if( this->is_outgoing())
    {
        //POST MPI RECEIVE
        MPI_Status stat;  int count;
        MPI_Probe(share_rank, GID, MPI_COMM_WORLD,  &stat);
        
        
        MPI_Get_count(&stat, MPI_INT, &count);
        
        int *data = new int[count];
     
        MPI_Recv(data, count , MPI_INT, share_rank, GID, MPI_COMM_WORLD, &stat );
        for(int i =0 ; i<count ; i+=2)
        {
            std::map<int,int>::iterator it;
            it = visable_cities->find(data[i]);
            if( it != visable_cities->end() )
            {
                (*visable_cities)[data[i]] = (data[i+1] > (*visable_cities)[data[i]]) ? (*visable_cities)[data[i]] : data[i+1];
            }
            else (*visable_cities)[data[i]] = data[i+1];
        }
        
        delete data;
    }
    else if( this->is_incoming())
    {
        //POST MPI SEND
        MPI_Request request;
        int * data = new int[visable_cities->size()*2];
        int i =0;
        for (std::map<int,int>::iterator it=visable_cities->begin(); it!=visable_cities->end(); ++it)
        {
            data[i] = it->first;
            data[i+1] = it->second;
            i++;
        }
        
        MPI_Isend( data,linked_cites->size()*2,  MPI_INT, 
            share_rank, GID, MPI_COMM_WORLD, &request );
        delete data;
    }
    else
    {
        //THIS SHOULD NEVER HAPPEN
        fprintf(stderr, "Bad bridge intersection detected, shutting down");
        exit(1);
    }
}

void bridge_intersection::push_data()
{
    //push data to linked bridges
    
    for( int i = 0; i < linked_bridges->size() ; i++)
    {
        if( not (*linked_bridges)[i].first->is_incoming() ) continue;
        std::map<int, int> *ref;
        for (std::map<int,int>::iterator it=visable_cities->begin(); it!=visable_cities->end(); ++it)
        {
            std::map<int,int>::iterator fi;
            ref = (*linked_bridges)[i].first->visable_cities;
            fi = ref->find(it->first);
            if( fi != ref->end() )
            {
               (*ref)[it->first] = 
               ((*ref)[it->first] > (it->second + (*linked_bridges)[i].second) ) 
               ? (*ref)[it->first] : (it->second + (*linked_bridges)[i].second);
            }
            else (*ref)[it->first] = it->second + (*linked_bridges)[i].second;
        }
        
    }
    
}


void bridge_intersection::rank_sharing()
{
    if( not this->is_outgoing())
    {
        fprintf(stderr, "Shit hit fan");
        exit(1);
    }
    
    for(std::vector<std::pair<City*,int> >::iterator it = (*linked_cites).begin();
        it != (*linked_cites).end(); ++it)
        
    for (std::map<int,int>::iterator mit=visable_cities->begin(); mit!=visable_cities->end(); ++mit)
    {
        std::map<int,std::pair<int,int> >::iterator finder;
        finder = (*it->first->ways_out).find(mit->first);
        if(finder != (*it->first->ways_out).end())
        {
            if((*it->first->ways_out)[mit->first].first > it->second + mit->second)
            {
                (*it->first->ways_out)[mit->first] = 
                    std::make_pair( it->second + mit->second , this->GID);
            }
        }else (*it->first->ways_out)[mit->first] = 
            std::make_pair( it->second + mit->second , this->GID);
    }
    
    for(std::vector<std::pair<bridge_intersection*,int> >::iterator it = (*linked_bridges).begin();
        it != (*linked_bridges).end(); ++it)
    for (std::map<int,int>::iterator mit=visable_cities->begin(); mit!=visable_cities->end(); ++mit)
    {
        std::map<int,std::pair<int,int> >::iterator finder;
        finder = (*it->first->ways_out).find(mit->first);
        if(finder != (*it->first->ways_out).end())
        {
            if((*it->first->ways_out)[mit->first].first > it->second + mit->second)
            {
                (*it->first->ways_out)[mit->first] = 
                    std::make_pair( it->second + mit->second , this->GID);
            }
        }else (*it->first->ways_out)[mit->first] = 
            std::make_pair( it->second + mit->second , this->GID);
    }
    
}