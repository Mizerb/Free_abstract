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
 * 
 * 
 */
 

#ifndef GENETIC_H
#define GENETIC_H

#include <mpi.h>
//#include <pthread.h>
#include <vector>
#include <set>
#include <map>
#include "City.h"
#include "Info.h"

class Genetic
{
public:
    /* constructor takes a communication tag to be used for mpi Isends and Irecvs,
     * a struct with input parameters to the world,
     * and the initial city data for the rank.
     */
    
    Genetic ( int tag, const Info &pargs, std::vector<City*> regionals ):
            _cities(regionals), _args(pargs), _comm_tag(tag), _first_gen(true), _roads() 
    {
        _gaussian_sigma = (int) sqrt(1.0*_args.world_x_size*_args.world_y_size/_cities.size())/2;
    }
    ~Genetic (){}
    /* takes the index of the requested individual and returns a vector of Roads
     */
    std::vector<int> getGraph (int ind);
    
    int numIndividuals() {return _args.ga_gen_size;}
    int numGenerations() {return _args.ga_num_gen;}
    /* takes all the fitnesses of the previous generation and uses them with
     * their corresponding genomic data to create the next generation
     */
    void newGeneration(std::vector< std::vector<int> > &fitnesses);
    
private:
    std::vector<int> generateRoads();
    std::pair<int,int> randomRoad
    (
        std::vector< std::pair<int,int> > &points, 
        std::map< std::pair<int,int>, int> &endpoints, 
        std::vector< std::vector<int> > &cities, 
        int (&curr_send)[2],
        int &connected
    );
    void selectData();
    void crossover(std::vector<int> &ind1, std::vector<int> &ind2);
    void mutate(std::vector<int> &ind);
    
    bool _first_gen;
    int _comm_tag;
    Info _args;
    int _gaussian_sigma;
    std::vector< std::vector<int> > _roads;
    std::vector<City*> _cities;
    std::vector< std::vector<int> > *_fit;
    //TPool<Genetic>* _tpool;
};


#endif

