/*
 * Copyright (c) 2015,
 * National Instruments Corporation.
 * All rights reserved.
 */

#include <stdio.h>
#include <stdlib.h>
#include "MyRio.h"
#define MEM_WIDTH 10
#define MEM_DEPTH 5
#define MEM_ITER (int)memory[MEM_WIDTH+1]
#define MEM_VAL memory[MEM_WIDTH+2]

void initializeMemory(double**);
void freeTheMemory(double**);
int getIndex(int);

/**
 * Overview:
 * myRIO main function. This template contains basic code to open the myRIO
 * FPGA session. You can remove this code if you only need to use the UART.
 *
 * Code in myRIO example projects is designed for reuse. You can copy source
 * files from the example project to use in this template.
 *
 * Note:
 * The Eclipse project defines the preprocessor symbol for the myRIO-1900.
 * Change the preprocessor symbol to use this example with the myRIO-1950.
 */
int main(int argc, char **argv)
{
    NiFpga_Status status;

    /*
     * Open the myRIO NiFpga Session.
     * This function MUST be called before all other functions. After this call
     * is complete the myRIO target will be ready to be used.
     */
    status = MyRio_Open();
    if (MyRio_IsNotSuccess(status))
    {
        return status;
    }

    /*
     * Your application code goes here.
     */

    //we're gonna need lots of ram boss
    //layout of memory:
    //0-MEM_WIDTH: memory blocks of MEM_DEPTH size
    //MEM_ITER = MEM_WIDTH+1: a table of current indexes for each block
    //MEM_VAL = MEM_WIDTH+2: a table of current averages for each block

    double* memory[MEM_WIDTH+2];
    initializeMemory(memory);

    int keepGoing = 1;
    int index = getIndex(0);
    while(keepGoing) {
    	index = getIndex(index);
    	setOutput(MEM_VAL[index]);

    }



    /*
     * Close the myRIO NiFpga Session.
     * This function MUST be called after all other functions.
     */
    freeTheMemory(memory);
    status = MyRio_Close();

    return status;
}

double updateBlock(double** memory, int blockIndex, double value) {
	if(blockIndex >= MEM_WIDTH) {
		//error alerting code here
		return 0;
	}
	memory[blockIndex][MEM_ITER[blockIndex]++] = value;
	int i = 0;
	double valueHolder = 0.0;
	for(i = 0; i<MEM_DEPTH; i++) {
		valueHolder += memory[blockIndex][i];
	}
	valueHolder = (valueHolder/(double)MEM_DEPTH);
	MEM_VAL[blockIndex] = valueHolder;
	return valueHolder;

}

void initializeMemory(double** memory) {
	int i = 0;
	double memIterator[MEM_DEPTH] = {0.0};
	double memVal[MEM_WIDTH] = {0.0};
	for(i = 0; i < MEM_WIDTH; i++) {
		double * newBlock = malloc(MEM_DEPTH * sizeof(double));
		int j = 0;
		for(j = 0; j<MEM_DEPTH; j++) {
			newBlock[j] = 0.0;
		}
		memory[i] = newBlock;
	}
	memory[i+1] = memIterator;
	memory[i+2] = memVal;
}

void freeTheMemory(double** memory) {
	int i = 0;
	for(i = 0; i < MEM_WIDTH; i++) {
		free(memory[i]);
		}
}

int getIndex(int index) {
	return ++index % MEM_WIDTH;
}
