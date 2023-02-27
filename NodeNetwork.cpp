//gcc -o myprogram nodeTree.c GeneticAlgorithm.c; myprogram.exe
#include <stdio.h>
#include <stdlib.h>
#include "GeneticAlgorithm.h"
#include <iostream>

using namespace std;
const int inputHeight = 3000;

typedef struct Node{
    float value;
    float wL;
    float wR;
    float bias;
    struct Node* left;
    struct Node* right;
} Node;
typedef struct RecurrentNode{
    float value;
    float wL;
    float wR;
    float bias;
    Node* under;
} RecurrentNode;
typedef struct Network{
    int inputSize;
    int outputSize;
    int numhiddenLaylers;
    int hiddenLayerSize;
    Node* inputLayer;
    Node* outputLayer;
    Node** hiddenLayers;
    RecurrentNode** recurrentLayers;
} Network;
float sigmoid(float x){
    return 1/(1+exp(-x));
}
float sigmoidPrime(float x){
    return sigmoid(x)*(1-sigmoid(x));
}
float randomFloat(float a, float b){
    return ((b - a) * ((float)rand() / RAND_MAX)) + a;
}
// every node has a conection to every node in the layer infront of it and behind it
// each nodes value will be the sum of the values of the nodes in the layer infront of it multiple by the weight of the connection
// wL weights are for back propagation and recurtion
// wR weights are for forward propagation
// the goal is for the wR weights to become tuned to leave cetain values at the nodes
// to modify the output not just depending on its current input, but also the previouse inputs
//
Network* createNetwork(int inputSize, int outputSize, int numhiddenLaylers,int hiddenLayerSize){
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

    network->inputSize = inputSize;
    network->outputSize = outputSize;
    network->numhiddenLaylers = numhiddenLaylers;
    network->hiddenLayerSize = hiddenLayerSize;

    srand(time(NULL));
    //create the input nodes
    for (int i = 0; i < inputSize; i++)
    {
        network->inputLayer[i].value = 0.0;
        network->inputLayer[i].wL = randomFloat(-1,1);
        network->inputLayer[i].wR = randomFloat(-1,1);
        network->inputLayer[i].bias = randomFloat(-1,1);
        network->inputLayer[i].left = NULL;
        network->inputLayer[i].right = NULL;
        cout<<(network->inputLayer[i].value);
    }
    cout<<("\n");
    //create the hidden layers
    for (int i = 0; i < numhiddenLaylers; i++)
    {
        //create the hidden layer
        network->hiddenLayers[i] = (Node*)malloc(sizeof(Node)*hiddenLayerSize);
        //create the nodes in the hidden layer
        for (int j = 0; j < hiddenLayerSize; j++)
        {
            network->hiddenLayers[i][j].value = 0.0;
            network->hiddenLayers[i][j].wL = randomFloat(-1,1);
            network->hiddenLayers[i][j].wR = randomFloat(-1,1);
            network->hiddenLayers[i][j].bias = randomFloat(-1,1);

            if(i == 0){
                network->hiddenLayers[i][j].left = network->inputLayer;
                network->inputLayer[i].right = network->hiddenLayers[i];
            }
            else
                network->hiddenLayers[i][j].left =  network->hiddenLayers[i-1];
            if(i == numhiddenLaylers-1)
                network->hiddenLayers[i][j].right = network->outputLayer;
            else
                network->hiddenLayers[i][j].right =  network->hiddenLayers[i+1];
            cout<<(network->hiddenLayers[i][j].value);
        }
        cout<<("\n");
    }
    for (int i = 0; i < numhiddenLaylers; i++)
    {
        //create the hidden layer
        network->recurrentLayers[i] = (RecurrentNode*)malloc(sizeof(RecurrentNode)*hiddenLayerSize);
        //create the nodes in the hidden layer
        for (int j = 0; j < hiddenLayerSize; j++)
        {
            network->recurrentLayers[i][j].value = 0.0;
            network->recurrentLayers[i][j].wL = randomFloat(-1,1);
            network->recurrentLayers[i][j].wR = randomFloat(-1,1);
            network->recurrentLayers[i][j].bias = randomFloat(-1,1);
            network->recurrentLayers[i][j].under = network->hiddenLayers[i];
        }
    }
    //create the output nodes
    for (int i = 0; i < outputSize; i++)
    {
       network->outputLayer[i].wL = randomFloat(-1,1);
       network->outputLayer[i].wR = randomFloat(-1,1);
       network->outputLayer[i].bias = randomFloat(-1,1);
       network->outputLayer[i].left = network->inputLayer;
       network->outputLayer[i].right = NULL;
       network->outputLayer[i].value = 0;
       cout<<(network->outputLayer[i].value);
    }
    cout<<("\n");
    return network;
}
Network* loadNetwork(){
    //load the network from a file
    return createNetwork(1,2,3,4);
}
Node* think(Network* network,int input[][inputHeight],int position,int inputs){
    printf("input#%d\n",position);
    //cout<<"think\n";
    //create the input nodes
    int inputSize = network->inputSize;
    int numhiddenLaylers = network->numhiddenLaylers;
    int hiddenLayerSize = network->hiddenLayerSize;
    int outputSize = network->outputSize;
    //_________MANUAL INPUT
    for (int i = 0; i < inputSize; i++)
    {
        network->inputLayer[i].value = input[position][i]; //get input from user
        ////cout<<("%d",network->inputLayer[i].value);
    }
    //_________________________________

    //cout<<("\n\n\n");
    for (int i = 0; i < inputSize; i++)
    {
        //cout<<("%d",network->inputLayer[i].value) << "\t";
    }
    //cout<<("\nHidden\n");
    for (int j = 0; j < hiddenLayerSize; j++)
        {
            for (int k = 0; k < inputSize; k++)
            {
                network->hiddenLayers[0][j].value += network->hiddenLayers[0][j].left[k].value * network->hiddenLayers[0][j].left[k].wR;
            }
            network->hiddenLayers[0][j].value += network->hiddenLayers[0][j].bias;
            network->hiddenLayers[0][j].value = sigmoid(network->hiddenLayers[0][j].value);

            //cout<< (network->hiddenLayers[0][j].value) << "\t";
        }
        //cout<<("\n");
    //pass to hidden layers
    for (int i = 1; i < numhiddenLaylers; i++)
    {
        network->hiddenLayers[i][0].value = 0.0;
        //pass through hidden layer
        for (int j = 0; j < hiddenLayerSize; j++)
        {
            for (int k = 0; k < hiddenLayerSize; k++)
            {
                network->hiddenLayers[i][j].value += network->hiddenLayers[i][j].left[k].value * network->hiddenLayers[i][j].left[k].wR;
                network->hiddenLayers[i][j].value += network->recurrentLayers[i][j].value * network->recurrentLayers[i][k].wL;
            }
            network->hiddenLayers[i][j].value += network->hiddenLayers[i][j].bias;
            network->hiddenLayers[i][j].value = sigmoid(network->hiddenLayers[i][j].value);

            //cout<< (network->hiddenLayers[i][j].value) << "\t";
        }
        //cout<<("\n");
    }
    //cout<<"Output\n";
    for (int i = 0; i < numhiddenLaylers; i++)
    {
        network->recurrentLayers[i][0].value = 0.0;
        //create the nodes in the hidden layer
        for (int j = 0; j < hiddenLayerSize; j++)
        {
            network->recurrentLayers[i][j].value = network->hiddenLayers[i][j].value * network->recurrentLayers[i][j].wR;
            network->recurrentLayers[i][j].value += network->recurrentLayers[i][j].bias;
            network->recurrentLayers[i][j].value = sigmoid(network->recurrentLayers[i][j].value);
        }
    }
    //create the output nodes
    for (int i = 0; i < outputSize; i++)
    {
        network->outputLayer[i].value = 0.0;   
        for (int j = 0; j < hiddenLayerSize; j++)
        {
            network->outputLayer[i].value += network->outputLayer[i].left[j].value * network->outputLayer[i].left[j].wR;
        }
        network->outputLayer[i].value += network->outputLayer[i].bias;
        network->outputLayer[i].value = sigmoid(network->outputLayer[i].value);
        cout<<(network->outputLayer[i].value) << "\t";
    }
    cout<<("\n");
    if(position < inputs-1){
        think(network,input,position+1,inputs);
    }
    return network->outputLayer;
}
Network* train(Network* network){

    //math shit
    //sigmoid function
    //sigmod derivative
    //back propagation
    //gradient descent
    //update weights
    //i need to review python network


    //or just use the GA :P
    return network;
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
    //randomly generate a 100x3 int array
    int input[100][inputHeight];
    for (int i = 0; i < 100; i++)
    {
        for (int j = 0; j < inputHeight; j++)
        {
            input[i][j] = rand();
        }
    }
    think(createNetwork(inputHeight,1,3,3),input,0,100);
    
    // int pop = 100;
    // int minGeneValue = 0;
    // int maxGeneValue = 100;
    // int size = 7;
    // int numParents = 3;
    // int mutationRate = 100;
    // int maxGens = 1000000;
    // int maxFitness = size;
    // int verbosity = 1;
    // int (*calc_ptr)(int*population,int size, int index);
    // calc_ptr = &fitnessCalculator;
    // int * result = geneticAlgorithm(pop,size,maxFitness,maxGens,verbosity,numParents,mutationRate,calc_ptr,minGeneValue,maxGeneValue);
    // printIndividual("Result:",result,size,0, calc_ptr,1);
}