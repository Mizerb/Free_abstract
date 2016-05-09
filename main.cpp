#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <vector>
#include <list>
#include <mpi.h>

#include "Car.h"
#include "Road.h"
#include "Intersection.h"
#include "City.h"
#include "grid.h"
#include "Rank.h"
#include "Info.h"
#include "Genetic.h"

/*
    
    Useage:
    ./[filename] [x-dim] [y-dim] [generationsize] [numgenerations] [inputfile] 
*/

Genetic* readAndSend(int numCities, Rank* rankMe, FILE* fptr, Info* worldInfo);

Genetic* getCities(Rank* rankMe, Info* worldInfo);

int main(int argc, char** argv){
    
    
     
     if(argc != 6){
        fprintf(stderr, " Usage:\n%s [x-dim] [y-dim] [generationsize] [numgenerations] [inputfile]\n", argv[0]);
        return false;
    }
    
    
    MPI_Init( &argc, &argv);
    Info *worldInfo = new Info(argc, argv);
    int numCities = 0;
    
   
    MPI_Barrier(MPI_COMM_WORLD);
    
    FILE* fptr = fopen(argv[5], "r");
    
    
    
    if(fptr == NULL){
        fprintf(stderr, "Could not open file.\n");
        return false;
    }
    
    fscanf(fptr, "%d\n", &numCities);
    
    if(numCities == 0){
        fprintf(stderr, "Can't build a graph without any cities, now can you?\n");
        return false;
    }
    
    Genetic *myGene;
    Rank *rankMe = new Rank(worldInfo->world_x_size, worldInfo->world_y_size );
    
    //printf("%d\n", rankMe->y_size);
    
    if(rankMe->my_rank == 0){
        myGene = readAndSend(numCities, rankMe, fptr, worldInfo);
    }
    else {
        myGene = getCities(rankMe, worldInfo);
    }
    
    MPI_Barrier(MPI_COMM_WORLD);
    
    printf("Got past the Genetic Creation\n");
    
    int gen;
    int member;
    
    for ( gen = 0; gen < worldInfo->ga_num_gen; ++gen )
    {
        for ( member = 0; member < worldInfo -> ga_gen_size; ++member )
        {
            fprintf(stderr, "Main in the for loops\n");
            //Generate roads
            std::vector<int> Road_data = myGene->getGraph(member);
            fprintf(stderr, "Main Got roads from genetic\n");
            rankMe->Add_Roads(Road_data.data(), Road_data.size());
            fprintf(stderr, "Main Added roads to grid\n");
        
            //Breadth first search
            rankMe->runSearch();
            fprintf(stderr, "Main Did the Search\n");
            
        
            //Run Sim
            rankMe->Run_Sim();
            fprintf(stderr, "Main Ran the Sim\n");
            
            rankMe->Reset_Sim();
            fprintf(stderr, "Main Reset the sim\n");
        }
    }
    
    
    MPI_Finalize();
    
    return EXIT_SUCCESS;
}




/*
This function reads the contents of the input file and sends out the the city information
To the corrosponding ranks

*/
Genetic* readAndSend(int numCities, Rank* rankMe, FILE* fptr, Info* worldInfo){
    
    int *buffer = (int*) calloc(3, 4);
    int *temp = (int*) calloc(3, 4);
    int *myBuf;
    int mySize = 0;
    
    int maxSize = 3;
    int current = 0;
    int curRank = 0;
    
    int i = 0;
    
    MPI_Request res1, res2;
    
    /* What I want this to do is read in each set of coordinates and population
    and when we hit the y-bounds, send that information to the desired MPI rank.  Right now
    I don't think it's reading the data correctly, maybe because I'm using a POSIX 
    file opener instead of an MPI one?
    
    */
    
    fscanf(fptr, "%d,%d,%d\n", buffer + current, buffer + current + 1, buffer + current + 2);
    //printf("Outside: %d %d %d %d\n", i, buffer[current], buffer[current +1], buffer[current+2]);
    i++;
    
    //int runs = 0;
    while (i < numCities /*&& runs++ < 10*/){
        //printf("Inside: %d %d %d %d\n", i, buffer[current], buffer[current + 1], buffer[current + 2]);
        if(current + 3>= maxSize)
        {
            maxSize *= 2;
            buffer = (int*)realloc(buffer, maxSize*4);
        }
        
        if(buffer[current + 1] > (rankMe -> y_size  * (curRank + 1))-1)
        {
            if(current == 0)
            {
                if(i == numCities){
                    break;
                }
                curRank++;
                continue;
            } 
            if(curRank == 0)
            {
                myBuf = (int*)calloc(current, 4);
                memcpy(myBuf, buffer, current*4);
                mySize = current;
                
            } else
            {
                MPI_Isend(&current, 1, MPI_INT, curRank, 0, MPI_COMM_WORLD, &res1);
                MPI_Isend(buffer, current, MPI_INT, curRank, 1, MPI_COMM_WORLD, &res2);
            }
            
            memcpy(temp, buffer+current, 12);
            buffer = (int*) calloc(maxSize, 4);
            memcpy(buffer, temp, 12);
            current = 0;
            curRank++;
        } else{
            
            current += 3;
            fscanf(fptr, "%d,%d,%d\n", buffer + current, buffer + current + 1, buffer + current + 2);
            i++;
        }
    }
    
    current +=3;
    if(curRank == 0)
    {
        myBuf = (int*)calloc(current, 4);
        memcpy(myBuf, buffer, current*4);
        mySize = current;
        
    } else
    {
        
        MPI_Isend(&current, 1, MPI_INT, curRank, 0, MPI_COMM_WORLD, &res1);
        MPI_Isend(buffer, current, MPI_INT, curRank, 1, MPI_COMM_WORLD, &res2);
    }
    
    
    printf("Size of rank 0 array: %d\n", mySize);
    /*for(int j = 0; j < mySize; j+=3){
        printf("Rank 0: %d %d %d\n", myBuf[j], myBuf[j+1], myBuf[j+2]);
    }*/
    
    rankMe->City_Start(myBuf, mySize);
    rankMe->printCities();
    
    Genetic *myGene = new Genetic(42, *worldInfo, rankMe->getCities());
    
    free(buffer);
    free(temp);
    free(myBuf);
    
    return myGene;
}




    /*Syntax for MPI_Irecv*/
	/*int MPI_Irecv(void *buf, int count, MPI_Datatype datatype,
	int source, int tag, MPI_Comm comm, 
	MPI_Request *request)
	
	int MPI_Recv(void *buf, int count, MPI_Datatype datatype,
    int source, int tag, MPI_Comm comm, MPI_Status *status)*/

Genetic* getCities(Rank* rankMe, Info* worldInfo)
{
    int size = 0;
    
    MPI_Status stat1, stat2;
    
    MPI_Recv(&size, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &stat1);
    
    printf("Size of rank %d array: %d\n", rankMe->my_rank, size);
    
    int *buffer = (int*)calloc(size, 4);
    
    MPI_Recv(buffer, size, MPI_INT, 0, 1, MPI_COMM_WORLD, &stat2);
    
    /*for(int j = 0; j < size; j+=3){
        printf("Rank %d: %d %d %d\n", rankMe->my_rank, buffer[j], buffer[j+1], buffer[j+2]);
    }*/
    
    rankMe->City_Start(buffer, size);
    rankMe->printCities();
    
    Genetic *myGene = new Genetic(42, *worldInfo, rankMe->getCities());
    
    free(buffer);
    
    return myGene;
}