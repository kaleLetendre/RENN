//gcc -o myprogram nodeTree.c GeneticAlgorithm.c; myprogram.exe
#include <stdio.h>
#include <stdlib.h>
#include "GeneticAlgorithm.h"

typedef struct Node{
    float value;
    float* wL;
    float* wR;
    Node** left;
    Node** right;
} Node;
typedef struct RecurrentNode{
    float value;
    float wL;
    float wR;
    Node** under;
} RecurrentNode;
typedef struct Network{
    Node* inputLayer;
    Node* outputLayer;
    Node** hiddenLayers;
    RecurrentNode** recurrentLayers;
} Network;
// every node has a conection to every node in the layer infront of it and behind it
// each nodes value will be the sum of the values of the nodes in the layer infront of it multiple by the weight of the connection
// wL weights are for back propagation and recurtion
// wR weights are for forward propagation
// the goal is for the wR weights to become tuned to leave cetain values at the nodes
// to modify the output not just depending on its current input, but also the previouse inputs
//
Network* createNetwork(int inputSize, int outputSize, int numhiddenLaylers){
    Network* network = (Network*)malloc(sizeof(Network));
    network->inputLayer = (Node*)malloc(sizeof(Node)*inputSize);//create the input layer
    network->outputLayer = (Node*)malloc(sizeof(Node)*outputSize);//create the output layer
    network->hiddenLayers = (Node**)malloc(sizeof(Node*)*numhiddenLaylers);//create the hidden layers
    network->recurrentLayers = (RecurrentNode**)malloc(sizeof(RecurrentNode*)*numhiddenLaylers);//create the recurrent layers
    //for recurrent think of these as sitting ontop its adjacent hidden layer
    //it will use wR weights to connect to the nodes in the hidden layer and copy the values from them
    //it will use wL weights to connect to the nodes in the hidden layer and push the values to them
    //pull hapens when hidden node is activated
    //push hapens when hidden node is deactivated


    //create the input nodes
    for (int i = 0; i < inputSize; i++)
    {
        network->inputLayer[i].value = 0;
        network->inputLayer[i].wL = (float*)malloc(sizeof(float)*inputSize);
        network->inputLayer[i].wR = (float*)malloc(sizeof(float)*inputSize);
        network->inputLayer[i].left = NULL;
        network->inputLayer[i].right = network->outputLayer;
    }
    //create the hidden layers
    for (int i = 0; i < numhiddenLaylers; i++)
    {
        //create the hidden layer
        network->hiddenLayers[i] = (Node*)malloc(sizeof(Node)*inputSize);
        //create the nodes in the hidden layer
        for (int j = 0; j < inputSize; j++)
        {
            network->hiddenLayers[i][j].value = 0;
            network->hiddenLayers[i][j].wL = (float*)malloc(sizeof(float)*inputSize);
            network->hiddenLayers[i][j].wR = (float*)malloc(sizeof(float)*inputSize);
            network->hiddenLayers[i][j].left = network->inputLayer;
            if(i == numhiddenLaylers-1)
                network->hiddenLayers[i][j].right = network->outputLayer;
            else
                network->hiddenLayers[i][j].right =  network->hiddenLayers[i+1];
        }
    }
    for (int i = 0; i < numhiddenLaylers; i++)
    {
        //create the hidden layer
        network->recurrentLayers[i] = (RecurrentNode*)malloc(sizeof(RecurrentNode)*inputSize);
        //create the nodes in the hidden layer
        for (int j = 0; j < inputSize; j++)
        {
            network->recurrentLayers[i][j].value = 0;
            network->recurrentLayers[i][j].wL = 0;
            network->recurrentLayers[i][j].wR = 0;
            network->recurrentLayers[i][j].under = network->hiddenLayers[i];
        }
    }
    //create the output nodes
    for (int i = 0; i < outputSize; i++)
    {
       network->outputLayer[i].wL = (float*)malloc(sizeof(float)*inputSize);
       network->outputLayer[i].wR = (float*)malloc(sizeof(float)*inputSize);
       network->outputLayer[i].left = network->inputLayer;
       network->outputLayer[i].right = NULL;
       network->outputLayer[i].value = 0;
    }
}
Network* loadNetwork(int inputSize, int outputSize, int numhiddenLaylers){
    //load the network from a file
}
Node* think(Network* network){

}
Network* train(Network* network){
    //math shit
    //sigmoid function
    //sigmod derivative
    //back propagation
    //gradient descent
    //update weights
    //i need to review python network
}

int fitnessCalculator(int* population, int size, int index){

    //translate the individual into a network

    int fitness = 0;
    for (int i = 0; i < size; i++)
    {
        if(*(population+index*size+i) == 1){
            fitness++;
        }
    }
    return fitness;   
}

int main(){
    int pop = 100;
    int minGeneValue = 0;
    int maxGeneValue = 9;
    int size = 7;
    int numParents = 3;
    int mutationRate = 100;
    int maxGens = 1000000;
    int maxFitness = size;
    int verbosity = 1;
    int (*calc_ptr)(int*population,int size, int index);
    calc_ptr = &fitnessCalculator;
    int * result = geneticAlgorithm(pop,size,maxFitness,maxGens,verbosity,numParents,mutationRate,calc_ptr,minGeneValue,maxGeneValue);
    printIndividual("Result:",result,size,0, calc_ptr,1);
}