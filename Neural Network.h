/* PMSIS includes */
#include "pmsis.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//define neural network parameters
#define INPUT_SIZE 2
#define OUTPUT_SIZE 1
#define LEARNING_RATE 0.01
#define EPOCHS 60000

#define RAN_NUM  3883838
#define RAN_MAX  RAN_NUM + 1378

// Performance Monitoring Event IDs
#define PI_PERF_CYCLES      0
#define PI_PERF_ACTIVE_CYCLES 1
#define PI_PERF_INSTR       2
#define PI_PERF_LD_STALL    3
#define PI_PERF_JR_STALL    4

double training_data[][INPUT_SIZE] = {
{1.96, 1.95, 1.95, 1.94, 1.94, 1.94, 1.93, 1.96, 1.94, 1.94, 1.93, 1.93, 1.94, 1.94, 1.95, 1.95, 1.95, 1.94, 1.93, 1.93, 1.94, 1.95, 1.95, 1.95, 1.94, 1.94, 1.94, 1.93, 1.94, 1.95, 1.95, 1.93, 1.94, 1.94, 1.94, 1.94, 1.96, 1.95, 1.96, 1.94, 1.94, 1.94, 1.94, 1.94, 1.95, 1.95, 1.96, 1.94, 1.95, 1.94, 1.94, 1.94, 1.94, 1.96, 1.96, 1.95, 1.94, 1.94, 1.94},



{1.83, 1.85, 1.85, 1.88, 1.86, 1.87, 1.83, 1.85, 1.85, 1.86, 1.86, 1.88, 1.85, 1.87, 1.88, 1.84, 1.86, 1.87, 1.84, 1.90, 1.90, 1.88, 1.88, 1.89, 1.89, 1.86, 1.91, 1.89, 1.92, 1.87, 1.87, 1.90, 1.86, 1.84, 1.89, 1.88, 1.87, 1.89, 1.89, 1.86, 1.92, 1.84, 1.87, 1.85, 1.85, 1.82, 1.80, 1.79, 1.81, 1.82, 1.86, 1.85, 1.82, 1.80, 1.81, 1.82, 1.79, 1.84, 1.84}, 



{1.74, 1.75, 1.75, 1.81, 1.78, 1.74, 1.78, 1.75, 1.77, 1.74, 1.73, 1.77, 1.75, 1.77, 1.77, 1.77, 1.75, 1.81, 1.81, 1.77, 1.74, 1.79, 1.73, 1.76, 1.78, 1.77, 1.82, 1.79, 1.77, 1.78, 1.77, 1.77, 1.74, 1.78, 1.78, 1.78, 1.79, 1.80, 1.76, 1.75, 1.82, 1.79, 1.76, 1.78, 1.79, 1.77, 1.75, 1.74, 1.78, 1.78, 1.78, 1.78, 1.78, 1.76, 1.82, 1.79, 1.81, 1.81, 1.78},



{1.60, 1.63, 1.63, 1.61, 1.68, 1.66, 1.66, 1.66, 1.64, 1.63, 1.60, 1.60, 1.62, 1.66, 1.64, 1.65, 1.63, 1.65, 1.60, 1.64, 1.63, 1.62, 1.64, 1.62, 1.62, 1.60, 1.59, 1.59, 1.63, 1.62, 1.61, 1.60, 1.63, 1.65, 1.59, 1.61, 1.65, 1.64, 1.60, 1.63, 1.61, 1.59, 1.63, 1.60, 1.60, 1.60, 1.60, 1.59, 1.60, 1.59, 1.58, 1.61, 1.62, 1.61, 1.59, 1.58, 1.61, 1.59, 1.59},



{1.55, 1.53, 1.53, 1.50, 1.53, 1.54, 1.53, 1.57, 1.55, 1.54, 1.55, 1.52, 1.51, 1.55, 1.57, 1.57, 1.56, 1.55, 1.55, 1.55, 1.53, 1.55, 1.54, 1.55, 1.53, 1.55, 1.53, 1.53, 1.52, 1.52, 1.57, 1.55, 1.56, 1.56, 1.51, 1.55, 1.57, 1.57, 1.56, 1.55, 1.57, 1.57, 1.55, 1.57, 1.59, 1.59, 1.55, 1.55, 1.56, 1.56, 1.58, 1.56, 1.59, 1.55, 1.55, 1.57, 1.56, 1.54, 1.54},



{1.41, 1.38, 1.40, 1.41, 1.40, 1.37, 1.39, 1.40, 1.41, 1.40, 1.40, 1.38, 1.39, 1.44, 1.44, 1.40, 1.41, 1.39, 1.39, 1.40, 1.39, 1.44, 1.40, 1.43, 1.41, 1.43, 1.47, 1.43, 1.38, 1.44, 1.40, 1.39, 1.39, 1.41, 1.41, 1.40, 1.37, 1.44, 1.43, 1.44, 1.39, 1.43, 1.47, 1.42, 1.44, 1.48, 1.47, 1.43, 1.43, 1.42, 1.44, 1.43, 1.44, 1.44, 1.49, 1.46, 1.44, 1.43, 1.44}



};
double targets[] = {0,20,40,60,80,90};
double test_data[][INPUT_SIZE] ={

{1.91, 1.92, 1.95, 1.94, 1.94, 1.94, 1.93, 1.96, 1.94, 1.94, 1.93, 1.93, 1.94, 1.94, 1.95, 1.95, 1.95, 1.94, 1.93, 1.93, 1.94, 1.95, 1.95, 1.95, 1.94, 1.94, 1.94, 1.93, 1.94, 1.95, 1.95, 1.93, 1.94, 1.94, 1.94, 1.94, 1.96, 1.95, 1.96, 1.94, 1.94, 1.94, 1.94, 1.94, 1.95, 1.95, 1.96, 1.94, 1.92, 1.90, 1.88, 1.94, 1.94, 1.96, 1.96, 1.95, 1.94, 1.94, 1.94},

{1.83, 1.85, 1.85, 1.88, 1.86, 1.87, 1.83, 1.85, 1.85, 1.86, 1.86, 1.88, 1.85, 1.87, 1.88, 1.84, 1.86, 1.87, 1.84, 1.90, 1.90, 1.88, 1.88, 1.89, 1.82, 1.81, 1.83, 1.89, 1.92, 1.87, 1.87, 1.90, 1.86, 1.84, 1.89, 1.88, 1.87, 1.89, 1.89, 1.86, 1.92, 1.84, 1.87, 1.85, 1.85, 1.82, 1.80, 1.79, 1.78, 1.69, 1.80, 1.85, 1.82, 1.80, 1.81, 1.82, 1.79, 1.84, 1.84}, 

{1.58, 1.67,1.75, 1.81, 1.78, 1.74, 1.78, 1.75, 1.77, 1.74, 1.73, 1.77, 1.75, 1.77, 1.77, 1.77, 1.69, 1.81, 1.81, 1.77, 1.74, 1.79, 1.73, 1.76, 1.78, 1.77, 1.82, 1.79, 1.77, 1.78, 1.77, 1.77, 1.74, 1.78, 1.78, 1.78, 1.79, 1.80, 1.76, 1.75, 1.66, 1.77, 1.72, 1.78, 1.79, 1.77, 1.75, 1.74, 1.78, 1.78, 1.78, 1.78, 1.78, 1.76, 1.82, 1.79, 1.81, 1.81, 1.78},

{1.58, 1.60, 1.63, 1.61, 1.68, 1.66, 1.66, 1.66, 1.64, 1.63, 1.60, 1.60, 1.62, 1.66, 1.64, 1.65, 1.67, 1.57, 1.60, 1.64, 1.63, 1.62, 1.64, 1.62, 1.58, 1.57, 1.59, 1.59, 1.63, 1.62, 1.61, 1.60, 1.63, 1.65, 1.59, 1.61, 1.65, 1.64, 1.60, 1.63, 1.61, 1.59, 1.63, 1.60, 1.60, 1.60, 1.60, 1.59, 1.46, 1.48, 1.66, 1.61, 1.62, 1.61, 1.59, 1.58, 1.61, 1.59, 1.59},

{1.55, 1.53, 1.53, 1.50, 1.53, 1.54, 1.53, 1.57, 1.55, 1.54, 1.55, 1.52, 1.51, 1.55, 1.57, 1.57, 1.56, 1.55, 1.55, 1.55, 1.53, 1.55, 1.54, 1.55, 1.53, 1.55, 1.49, 1.53, 1.52, 1.52, 1.57, 1.55, 1.56, 1.56, 1.51, 1.55, 1.57, 1.57, 1.56, 1.55, 1.57, 1.57, 1.55, 1.57, 1.59, 1.59, 1.55, 1.55, 1.49, 1.47, 1.50, 1.56, 1.59, 1.55, 1.55, 1.57, 1.56, 1.54, 1.54},

{1.22, 1.34, 1.40, 1.41, 1.40, 1.37, 1.39, 1.40, 1.41, 1.40, 1.40, 1.38, 1.39, 1.44, 1.44, 1.40, 1.41, 1.39, 1.39, 1.40, 1.39, 1.44, 1.40, 1.43, 1.41, 1.43, 1.33, 1.43, 1.38, 1.44, 1.40, 1.39, 1.39, 1.41, 1.41, 1.40, 1.37, 1.44, 1.43, 1.44, 1.39, 1.43, 1.47, 1.42, 1.44, 1.48, 1.47, 1.43, 1.43, 1.42, 1.44, 1.43, 1.44, 1.44, 1.49, 1.46, 1.44, 1.43, 1.44}

};
//define ReLU activated function

double activation(double x){
    return(x>0)?x:0.0;
}

//define neural network weights and bias

double weights[INPUT_SIZE];
double bias;



//neural networks forward propgation

double predict(double inputs[])
{
double output = 0; 
for(unsigned int i=0;i<INPUT_SIZE;i++)
	{
	output += weights[i] * inputs[i];
	}  
output +=  bias;
return activation(output);
}




void initialize(void)
{
int ran_inc = 0;
    for(unsigned int i=0; i<INPUT_SIZE;i++)
    {
	weights[i] = ((double) (RAN_NUM + ran_inc)/RAN_MAX) * 2-1; //initialize random weights
	ran_inc++;
    }  
    bias = ((double)(RAN_NUM + ran_inc)/RAN_MAX) * 2-1; //initialize random bias
    ran_inc++;
}



//Train a neural network

void train(double inputs[], double target)
{	
double prediction = (double)predict(inputs);
double error = target - prediction;
for(unsigned int i=0;i<INPUT_SIZE;i++)
    {
	weights[i]+= LEARNING_RATE* error * inputs[i];
    }
    bias += LEARNING_RATE* error;
}




void prediction_result(void)
{
uint32_t core_id = pi_core_id(), cluster_id = pi_cluster_id();
for(unsigned int i=0;i<sizeof(training_data)/sizeof(training_data[0]);i++)
	{ 	
	double prediction = predict(test_data[i]);
	printf("CORE[0x%x 0x%x]   INPUT:[%lf,%lf],    Target: %lf,    Prediction: %lf\n",core_id, cluster_id, training_data[i][0],training_data[i][1],targets[i],prediction);
	}
}



void training_process(void)
{
for (int epoch = 0; epoch < EPOCHS; epoch++)
    {
    for (unsigned int i = 0; i < sizeof(training_data) / sizeof(training_data[0]); i++)
        {
        train(training_data[i], targets[i]);
	}
     }
}





