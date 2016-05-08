/*
 * Code by Chris C
 * 
 * Genetic algorithm:
 * 
 * 
 * MPI 0 devoted to storing result data
 * 
 * MPI 1-n store their own chromosome information.
 * 
 * MPI 0 runs t threads with a tournament based algorithm
 * 
 * Then MPI 0 sends results, replacements, mutation candidates, and crossover pairings to all ranks.
 * 
 * Then perform crossover on data from within rank.
 * 
 * Crossover will be a repeated cut-and-splice, with two resulting children.
 * 
 * mutations are allowed, and will simply be either the addition of new Roads to mutation candidates,
 * or subtraction of other Roads.
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 */

#include <cmath>
#include <utility>
#include <algorithm>
#include "Genetic.h"
#include "Random.h"
 
std::vector<int> Genetic::getGraph (int ind) {
    int i;
    // idiot checking
    if ( ind < 0 || ind >= _args.ga_gen_size ) {
        throw "IndexOutOfRangeYouIdiotException";
    }
    // generate initial population if plausible
    while ( ind >= _roads.size() )
    {
        _roads.push_back(generateRoads());
    }

    return _roads[ind];
};

std::vector<int> Genetic::generateRoads()
{
    std::vector<int> graph;
    std::pair<int,int> next_road;
    MPI_Request connection[2];
    int x_border, count;
    int success[3] = {false};
    std::vector< std::pair<int,int> > points;
    std::map< std::pair<int,int>, int > endpoints;
    std::vector< std::vector<int> > connect(_cities.size());
    std::set<int> visit_b;
    int i;
    std::vector<City*>::iterator it;
    for ( it = _cities.begin(), i = 0; it != _cities.end(); ++it, ++i )
    {
        std::vector<int> next_city;
        next_city.push_back(i);
        connect[i] = next_city;

        endpoints[std::make_pair((*it)->x_,(*it)->y_)] = i;
    }
    
    if ( _args.neighbors[0] < 0 )
    {
        success[0] = 1;
    }
    if ( _args.neighbors[1] < 0 )
    {
        success[1] = 1;
    }

    if ( !success[0] )
    {
        MPI_Irecv( &(success[0]), 1, MPI_INT, _args.neighbors[0], _comm_tag, MPI_COMM_WORLD, &(connection[0]) );
    }
    if ( !success[1] )
    {
        MPI_Irecv( &x_border, 1, MPI_INT, _args.neighbors[1], _comm_tag, MPI_COMM_WORLD, &(connection[1]) );
    }
    
    while ( !(success[0] && success[1] && success[2]) )
    {
        if ( !success[1] )
        {
            MPI_Request_get_status(connection[1], &count, MPI_STATUS_IGNORE);
            if ( count )
            {
                // check to see if contained
                if ( visit_b.find( x_border ) != visit_b.end() )
                {
                    success[1] = 1;
                    MPI_Isend( &(success[1]), 1, MPI_INT, _args.neighbors[1], _comm_tag, MPI_COMM_WORLD, &(connection[1]) );
                }
                else
                {
                    MPI_Irecv( &x_border, 1, MPI_INT, _args.neighbors[1], _comm_tag, MPI_COMM_WORLD, &(connection[1]) );
                }
            }
        }
        next_road = randomRoad(points, endpoints, connect, visit_b, success);
        graph.push_back(next_road.first);
        graph.push_back(next_road.second);
        
    }

    return graph;
}


std::pair<int, int> Genetic::randomRoad
(
    std::vector< std::pair<int,int> > &points, 
    std::map< std::pair<int,int>, int> &endpoints, 
    std::vector< std::vector<int> > &cities, 
    std::set<int> &visit_b, 
    int (&connected)[3]
)
{
    std::pair<int,int> *eptr, *eptr2, p;
    // first try to connect two random endpoints
    eptr = &points[(int) (rand()*points.size())];
    eptr2 = &points[(int) (rand()*points.size())];
    // random walk and add roads
    if ( eptr += eptr2 || abs(eptr->first - eptr2->first) > abs(gauss(0, _gaussian_sigma)) || abs(eptr->second - eptr2->second) > abs(gauss(0, _gaussian_sigma)) )
    {
        int x = (int) gauss(eptr->first+0.5, _gaussian_sigma);
        int y = (int) gauss(eptr->second+0.5, _gaussian_sigma);
        if ( x < 0 ) x = 0;
        else if ( x >= _args.world_x_size ) x = _args.world_x_size - 1;
        if ( y < _args.world_slice_pos ) y = _args.world_slice_pos;
        else if ( y > _args.world_slice_pos+_args.world_slice_size ) y = _args.world_slice_pos + _args.world_slice_size;
        
        p = std::make_pair(x, y);

        if ( !connected[0] && y == _args.world_slice_pos )
        {
             MPI_Request req;
             MPI_Isend( &x, 1, MPI_INT, _args.neighbors[0], _comm_tag, MPI_COMM_WORLD, &req );
        }
        else if ( !connected[1] && y == _args.world_slice_pos+_args.world_slice_size )
        {
            visit_b.insert(x);
        }

        eptr2 = &p;
        if ( endpoints.find(p) == endpoints.end() )
        {
            points.push_back(p);
            endpoints[p] = endpoints[*eptr];
        }
    }
    if ( !connected[2] && endpoints[*eptr] != endpoints[*eptr2] )
    {
        std::vector<int> comb(cities[endpoints[*eptr]].size()+cities[endpoints[*eptr2]].size());
        std::vector<int>::iterator itr = set_union
        (
            cities[endpoints[*eptr]].begin(),
            cities[endpoints[*eptr]].end(),
            cities[endpoints[*eptr2]].begin(),
            cities[endpoints[*eptr2]].end(),
            comb.begin()
        );
        comb.resize(itr-comb.begin());
        
        cities[endpoints[*eptr]] = cities[endpoints[*eptr2]] = comb;
        
        if ( comb.size() == _cities.size() )
        {
            connected[2] = true;
        }
    }
    cities[endpoints[*eptr]];
    

    return std::pair<int,int>(_args.GID_from_coord(eptr->first, eptr->second), _args.GID_from_coord(eptr2->first, eptr2->second));
}




void Genetic::newGeneration ( std::vector< std::vector<int> > &fitnesses )
{
    /*
    int i;
    _fit = &fitnesses;
    
    for ( i = 0; i < _fit.size(); ++i )
    {
        
    }
    */
}

void Genetic::selectData()
{
    /*
    
    
    
    
    */
}

void Genetic::crossover(std::vector<int> &ind1, std::vector<int> &ind2)
{
    /*
    
    
    
    
    
    */
}

void mutate(std::vector<int> &ind)
{
    /*
    
    */
}
/*
class TPool<class T>
{
public:
    TPool ( T &instance, size_t pool_size ) : 
    _obj(instance), _nthreads(pool_size) {this->_id = pthread_self();}
    TPool ( TPool<class T> &old );
    void run( void (*func) (void *, int, int), void *arg );

private:
    static void* runInstanceMethod( void* args )
    
    void (*_func) (void *, int, int);
    void *_arg;
    size_t _nthreads;
    size_t _threadno;
    T _obj;
};

TPool<class T>::TPool ( TPool<class T> &old )
{
    this->_func = old.func;
    this->_arg = old._arg;
    this->_nthreads = old._nthreads;
    this->_threadno = old._threadno;
    this->obj = old.obj;
    this->_id = pthread_self();
}

static void* TPool<class T>::RunInstanceMethod( void* that )
{
    TPool<T> t = that;
    (t->obj)->*(t->_func) ( t->_args, t->_threadno, t->_nthreads );
}

// creates n-1 threads and runs a given function on itself and each thread 
void TPool<class T>::run( void (*func) (void *), void *arg )
{
    TPool<T> * targs= new TPool<T>(*this) [_nthreads-1];
    TPool* tptr;
    void * ret;
    
    size_t i;

    this->_threadno = 0;
    
    for ( i = 1, tptr = targs; i < _nthreads; ++i, ++tptr )
    {
        tptr->_threadno = i;
        pthread_create( &(tptr->_id), NULL, *(TPool<T>::RunInstanceMethod), tptr );
        //fprintf(stderr, " Created pthread with id %d.\n", tptr->threadno);
    }
    
    //fprintf(stderr, " Running function on self as thread 0\n");
    RunInstanceMethod(this);
    // make sure all threads have finished before exiting 
    for ( tptr = targs+1; tptr < targs+num_threads; ++tptr )
    {
        //fprintf(stderr, " Attempting to join pthread with id %d.\n", tptr->threadno);
        pthread_join(tptr->_id, &ret);
    }

    delete[] targs;

    return;
}

*/