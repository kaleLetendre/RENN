#ifndef GENETICALGORITHM_H
#define GENETICALGORITHM_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>

void printIndividual(char* pre, int* population, int GENE_SIZE, int i, int (*clac_ptr)(), int verbosity);
void printPopulation(int* population, int POPULATION_SIZE, int GENE_SIZE, int verbosity, int* fitness);
int* initialisePopulation(int* population,int POPULATION_SIZE, int GENE_SIZE , int minGeneValue, int maxGeneValue);
int* naturalSelection(int* primePopulation,int* population, int* fitness, int POPULATION_SIZE, int GENE_SIZE,int numParents);
int* getFitness(int* fitness, int* population, int POPULATION_SIZE, int GENE_SIZE, int numParents,int (*calc_ptr)());
int* reproduction(int* population,int* primePopulation, int POPULATION_SIZE, int GENE_SIZE,int numParents, int mutationRate, int minGeneValue, int maxGeneValue);
int* geneticAlgorithm(int POPULATION_SIZE, int GENE_SIZE, long int maxFitness, int maxGens, int verbosity, int numParents,int mutationRate, int (*calc_ptr)(),int minGeneValue, int maxGeneValue);
#endif
