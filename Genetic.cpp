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
        fprintf(stderr, "Index out of range!");
        throw "IndexOutOfRangeYouIdiotException";
    }
    // generate initial population if plausible
    while ( ind >= _roads.size() )
    {
        //fprintf(stderr, "GetGraph while loop\n");
        
        _roads.push_back(generateRoads());
        ++_comm_tag;
    }

    return _roads[ind];
};

std::vector<int> Genetic::generateRoads()
{
    std::vector<int> graph;
    std::pair<int,int> next_road;
    MPI_Request connection[2], confirm[2];
    int border_road[2][4], count, borderc[2] = {-1,-1};
    int success[3] = {0}, curr_send[2] = {0}, curr_recv[2] = {0};
    std::vector< std::pair<int,int> > points;
    std::map< std::pair<int,int>, int > endpoints;
    std::vector< std::vector<int> > connect(_cities.size());
    int i;
    std::vector<City*>::iterator it;
    
    //fprintf(stderr, "GenerateRoads init\n");
    
    MPI_Barrier(MPI_COMM_WORLD);

    for ( it = _cities.begin(), i = 0; it != _cities.end(); ++it, ++i )
    {
        std::vector<int> next_city;
        std::pair<int,int> loc((*it)->x_,(*it)->y_+_args.world_slice_pos);
        //fprintf( stderr, "[%d] city at (%d, %d), in [%d, %d]\n",
        //         _args.mpi_rank, loc.first, loc.second,
        //         _args.world_slice_pos, _args.world_slice_pos+_args.world_slice_size );
        if ( loc.second >= _args.world_slice_pos && loc.second < _args.world_slice_pos + _args.world_slice_size )
        {
            next_city.push_back(i);
            connect[i] = next_city;

            endpoints[loc] = i;
            points.push_back(loc);
        }
    }
    
    if ( _args.neighbors[0] >= 0 )
    {
        std::vector<int> border_city;
        borderc[0] = connect.size();
        border_city.push_back(borderc[0]);
        connect.push_back(border_city);
        
        //fprintf( stderr, "[%d] Attempting receive from [%d] with comm tag [%d]\n", 
        //         _args.mpi_rank, _args.neighbors[0], _comm_tag+curr_recv[0] );
        MPI_Irecv( border_road[0], 4, MPI_INT, _args.neighbors[0], _comm_tag+curr_recv[0], MPI_COMM_WORLD, &(connection[0]) );
        MPI_Irecv( &(success[0]), 1, MPI_INT, _args.neighbors[0], _comm_tag, MPI_COMM_WORLD, &(confirm[0]) );
    }
    else
    {
        success[0] = -1;
    }
    if ( _args.neighbors[1] >= 0 )
    {
        std::vector<int> border_city;
        borderc[1] = connect.size();
        border_city.push_back(borderc[1]);
        connect.push_back(border_city);

        //fprintf( stderr, "[%d] Attempting receive from [%d] with comm tag [%d]\n", 
        //         _args.mpi_rank, _args.neighbors[1], _comm_tag+curr_recv[1] );
        MPI_Irecv( border_road[1], 4, MPI_INT, _args.neighbors[1], _comm_tag+curr_recv[1], MPI_COMM_WORLD, &(connection[1]) );
        MPI_Irecv( &(success[1]), 1, MPI_INT, _args.neighbors[1], _comm_tag, MPI_COMM_WORLD, &(confirm[1]) );
    }
    else
    {
        success[1] = -1;
    }

    //fprintf(stderr, "GenerateRoads after first for loop\n");
    

    MPI_Barrier(MPI_COMM_WORLD);
    
    //fprintf(stderr, "[%d] Test! %d %d %d %d %d\n", _args.mpi_rank, success[0], success[1], success[2], curr_recv[0], curr_recv[1]);
    while ( !(success[0] && success[1] && success[2]) || curr_recv[0] < success[0] || curr_recv[1] < success[1] )
    {
        fprintf(stderr, "[%d] Status %d %d %d\n", _args.mpi_rank, success[2], success[0], success[1]);
        if ( !success[2] )
        {
            next_road = randomRoad(points, endpoints, connect, curr_send, success[2]);
            
            //fprintf(stderr, "[%d] GenerateRoads primary while loop After 'randomRoad'\n", _args.mpi_rank);
            if ( next_road.first != next_road.second )
            {
                graph.push_back(next_road.first);
                graph.push_back(next_road.second);
            }
        }
        if ( !(success[0]) || curr_recv[0] < success[0] )
        {
            MPI_Status st;
            count = 0;
            MPI_Request_get_status(connection[0], &count, &st);
            if ( count )
            {
                MPI_Get_count(&st, MPI_INT, &count);
                std::pair<int,int> entry(border_road[0][2], border_road[0][3]);
                fprintf ( stderr, "[%d] Received bridge (int[%d]) from (%d, %d) to (%d, %d) from rank %d with comm tag %d\n", 
                          _args.mpi_rank, count, border_road[0][0], border_road[0][1], border_road[0][2], 
                          border_road[0][3], _args.neighbors[0], _comm_tag+curr_recv[0] );
                ++curr_recv[0];
                graph.push_back(_args.GID_from_coord(border_road[0][0], border_road[0][1]));
                graph.push_back(_args.GID_from_coord(border_road[0][2], border_road[0][3]));
                // check to see if contained
                if ( endpoints.find( entry ) != endpoints.end() && endpoints[entry] >= 0 )
                {
                    //fprintf(stderr, "[%d] Adding inside stuff %d\n", _args.mpi_rank, endpoints[entry]);
                    int size = connect[endpoints[entry]].size();
                    //fprintf(stderr, "[%d] Created size\n", _args.mpi_rank);
                    if ( size > 1 &&
                         connect[endpoints[entry]].back() == borderc[1] &&
                         connect[endpoints[entry]][size-2] != borderc[0] )
                    {
                        //fprintf(stderr, "[%d] Weird case\n", _args.mpi_rank);
                        connect[endpoints[entry]].insert(--(connect[endpoints[entry]].end()), borderc[0]); 
                    }
                    else if ( connect[endpoints[entry]].back() != borderc[1] && 
                              connect[endpoints[entry]].back() != borderc[0] )
                    {
                        //fprintf(stderr, "[%d] Less weird case\n", _args.mpi_rank);
                        connect[endpoints[entry]].push_back(borderc[0]);
                    }
                    //fprintf(stderr, "[%d] Taken care of\n", _args.mpi_rank);
                    
                    if ( connect[endpoints[entry]].size() == connect.size() )
                    {  
                        MPI_Request req;
                        //fprintf(stderr, "[%d] Everything connected\n", _args.mpi_rank);
                        success[2] = 1;
                        MPI_Isend( &(curr_send[0]), 1, MPI_INT, _args.neighbors[0], _comm_tag, MPI_COMM_WORLD, &req );                        
                        MPI_Isend( &(curr_send[1]), 1, MPI_INT, _args.neighbors[1], _comm_tag, MPI_COMM_WORLD, &req );                        
                    }
                }
                else
                {
                    //fprintf(stderr, "[%d] Not adding inside stuff\n", _args.mpi_rank);
                    endpoints[entry] = borderc[0];
                }
                if ( !(success[0]) || curr_recv[0] < success[0] )
                {
                    MPI_Irecv( border_road[0], 4, MPI_INT, _args.neighbors[0], _comm_tag+curr_recv[0], MPI_COMM_WORLD, &(connection[0]) );
                }
            }
        }
        if ( !(success[1]) || curr_recv[1] < success[1] )
        {
            MPI_Status st;
            count = 0;
            MPI_Request_get_status(connection[1], &count, MPI_STATUS_IGNORE);
            if ( count )
            {
                MPI_Get_count(&st, MPI_INT, &count);
                std::pair<int,int> entry(border_road[1][2], border_road[1][3]);
                fprintf ( stderr, "[%d] Received bridge (int[%d]) from (%d, %d) to (%d, %d) from rank %d with comm tag %d\n", 
                          _args.mpi_rank, count, border_road[1][0], border_road[1][1], border_road[1][2], 
                          border_road[1][3], _args.neighbors[1], _comm_tag+curr_recv[1] );
                ++curr_recv[1];
                graph.push_back(_args.GID_from_coord(border_road[1][0], border_road[1][1]));
                graph.push_back(_args.GID_from_coord(border_road[1][2], border_road[1][3]));
                // check to see if contained
                if ( endpoints.find(entry) != endpoints.end() && 
                     endpoints[entry] >= 0 && connect[endpoints[entry]].back() != borderc[1] )
                {
                    connect[endpoints[entry]].push_back(borderc[1]);
                    if ( connect[endpoints[entry]].size() == connect.size() ) 
                    {
                        MPI_Request req[2];
                        //fprintf(stderr, "[%d] Everything connected\n", _args.mpi_rank);
                        success[2] = 1;
                        MPI_Isend( &(curr_send[0]), 1, MPI_INT, _args.neighbors[0], _comm_tag, MPI_COMM_WORLD, &(req[0]) );                        
                        MPI_Isend( &(curr_send[1]), 1, MPI_INT, _args.neighbors[1], _comm_tag, MPI_COMM_WORLD, &(req[1]) );                                                
                    }
                }
                else
                {
                    endpoints[entry] = borderc[1];
                }
                if ( !(success[1]) || curr_recv[1] < success[1] )
                {
                    MPI_Irecv( border_road[1], 4, MPI_INT, _args.neighbors[1], _comm_tag+curr_recv[1], MPI_COMM_WORLD, &(connection[1]) );
                }
            }
        }
        
        //MPI_Barrier(MPI_COMM_WORLD);
        //fprintf(stderr, "[%d] GenerateRoads primary while loop after if statements\n", _args.mpi_rank);
        
        
    }
    //fprintf(stderr, "GenerateRoads finished\n");

    return graph;
}


std::pair<int, int> Genetic::randomRoad
(
    std::vector< std::pair<int,int> > &points, 
    std::map< std::pair<int,int>, int> &endpoints, 
    std::vector< std::vector<int> > &cities,
    int (&curr_send)[2],
    int &connected
)
{
    std::pair<int,int> eptr, eptr2, test((int)gauss(0, _gaussian_sigma), (int)gauss(0, _gaussian_sigma));
    int rd[4], e1, e2;
    if ( abs(test.first) > _args.world_slice_size ) test.first = _args.world_slice_size;
    if ( abs(test.second) > _args.world_slice_size ) test.second = _args.world_slice_size;
    // first try to connect two random endpoints
    if ( points.size() > 0 )
    {
        eptr = points[(int) rand()%points.size()];
        eptr2 = points[(int) rand()%points.size()%cities.size()];
        /*
        for ( i = 0; i < cities.size() && 
              abs(eptr2.first-eptr.first) > abs(test.first) && 
              abs(eptr2.second-eptr.second) > abs(test.second); ++i )
        {
            if ( i != endpoints[eptr] )
            {
                eptr2 = points[i%points.size()];
            }
        }
        */
    }
    else
    {
        eptr2 = eptr = std::make_pair( (int) rand()%_args.world_x_size, (int) rand()%_args.world_slice_size + _args.world_slice_pos );
        endpoints[eptr] = -1;
        points.push_back(eptr);
    }
    // random walk and add roads
    
    //fprintf( stderr, "[%d] eptr %d eptr2 %d\n", _args.mpi_rank, endpoints[eptr], endpoints[eptr2] );
    //fprintf(stderr,"randomRoad after intitialisation\n");
    
    if ( eptr == eptr2 || abs(eptr.first-eptr2.first) > abs(test.first) || abs(eptr.first-eptr2.second) > abs(test.second) )
    {
        //fprintf(stderr, "randomRoad entering first if statement with %d, %d\n", eptr.first, eptr.second);
        
        int x = eptr.first + test.first;
        int y = eptr.second + test.second;
        
        //fprintf(stderr, "randomRoad After calls to 'gauss'\n");

        if ( x < 0 ) x = 0;
        else if ( x >= _args.world_x_size ) x = _args.world_x_size - 1;
        if ( y < 0 ) y = 0;
        else if ( y >= _args.world_y_size ) y = _args.world_y_size - 1;
        
        //fprintf(stderr, "randomRoad first if after initilization\n");
        
        eptr2 = std::make_pair(x, y);

        if ( y < _args.world_slice_pos )
        {
            MPI_Request req;
            rd[0] = eptr.first;
            rd[1] = eptr.second;
            rd[2] = eptr2.first;
            rd[3] = eptr2.second;
            MPI_Isend( rd, 4, MPI_INT, _args.neighbors[0], _comm_tag+curr_send[0], MPI_COMM_WORLD, &req );
            fprintf ( stderr, "[%d] Sent bridge from (%d, %d) to (%d, %d) to rank %d with comm tag %d\n", 
                      _args.mpi_rank, rd[0], rd[1], rd[2], rd[3], _args.neighbors[0], _comm_tag+curr_send[0] );
            ++curr_send[0];
            return std::pair<int,int>(_args.GID_from_coord(eptr.first, eptr.second), _args.GID_from_coord(eptr2.first, eptr2.second));
        }
        else if ( y >= _args.world_slice_pos+_args.world_slice_size )
        {
            MPI_Request req;
            rd[0] = eptr.first;
            rd[1] = eptr.second;
            rd[2] = eptr2.first;
            rd[3] = eptr2.second;
            MPI_Isend( rd, 4, MPI_INT, _args.neighbors[1], _comm_tag+curr_send[1], MPI_COMM_WORLD, &req );
            fprintf ( stderr, "[%d] Sent bridge from (%d, %d) to (%d, %d) to rank %d with comm tag %d\n", 
                      _args.mpi_rank, rd[0], rd[1], rd[2], rd[3], _args.neighbors[1], _comm_tag+curr_send[1] );
            ++curr_send[1];
            return std::pair<int,int>(_args.GID_from_coord(eptr.first, eptr.second), _args.GID_from_coord(eptr2.first, eptr2.second));
        }
        else if ( endpoints.find(eptr2) == endpoints.end() )
        {
            points.push_back(eptr2);
            endpoints[eptr2] = endpoints[eptr];
        }
    }
    fprintf( stderr, "[%d] New Road: (%d,%d)->(%d,%d)\n",
             _args.mpi_rank, eptr.first, eptr.second, eptr2.first, eptr2.second );
    if ( endpoints[eptr2] == -1 )
    {
        endpoints[eptr2] = endpoints[eptr];
    }
    else if ( endpoints[eptr] == -1 )
    {
        endpoints[eptr] = endpoints[eptr2];
    }
    else
    {
        e1 = cities[endpoints[eptr]].front();
        while ( e1 != cities[e1].front() ) e1 = cities[e1].front();
        e2 = cities[endpoints[eptr2]].front();
        while ( e2 != cities[e2].front() ) e2 = cities[e2].front();
        //fprintf( stderr, "[%d] Using %d %d\n", _args.mpi_rank, e1, e2 );
            
        if ( e1 != e2 )
        {
            std::vector<int> comb(cities[e1].size()+cities[e2].size());
            std::vector<int>::iterator itr = set_union
            (
                cities[e1].begin(),
                cities[e1].end(),
                cities[e2].begin(),
                cities[e2].end(),
                comb.begin()
            );
            comb.resize(itr-comb.begin());
            
            cities[e1] = comb;
            cities[e2] = comb;
            fprintf(stderr, "[%d] combined:", _args.mpi_rank);
            for ( int i = 0; i < comb.size(); ++i )
                fprintf(stderr, " %d", comb[i]);
            fprintf(stderr, "\n");
            
            if ( comb.size() == cities.size() )
            {
                MPI_Request req[2];
                fprintf(stderr, "[%d] connected graph!\n", _args.mpi_rank);
                connected = 1;
                MPI_Isend( &(curr_send[0]), 1, MPI_INT, _args.neighbors[0], _comm_tag, MPI_COMM_WORLD, &(req[0]) );
                MPI_Isend( &(curr_send[1]), 1, MPI_INT, _args.neighbors[1], _comm_tag, MPI_COMM_WORLD, &(req[1]) );
            }
        }
    }
    //fprintf(stderr, "[%d] randomRoad After second if statement, %d\n", _args.mpi_rank, connected);
    
    //fprintf( stderr, "randomRoad from GID %d to GID %d\n", _args.GID_from_coord(eptr.first, eptr.second), _args.GID_from_coord(eptr2.first, eptr2.second) );
    return std::pair<int,int>(_args.GID_from_coord(eptr.first, eptr.second), _args.GID_from_coord(eptr2.first, eptr2.second));
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
    //repeated shuffle
    int nshuffle = abs(gauss(0, 7)) +1, i;
    
    for ( i = 0; i < nshuffle; ++i )
    {
        ind1
    }
    
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
        //ffprintf(stderr, stderr, " Created pthread with id %d.\n", tptr->threadno);
    }
    
    //ffprintf(stderr, stderr, " Running function on self as thread 0\n");
    RunInstanceMethod(this);
    // make sure all threads have finished before exiting 
    for ( tptr = targs+1; tptr < targs+num_threads; ++tptr )
    {
        //ffprintf(stderr, stderr, " Attempting to join pthread with id %d.\n", tptr->threadno);
        pthread_join(tptr->_id, &ret);
    }

    delete[] targs;

    return;
}

*/