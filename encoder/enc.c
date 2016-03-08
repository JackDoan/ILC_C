/*
 * Copyright (c) 2013,
 * National Instruments Corporation.
 * All rights reserved.
 */

#include <stdio.h>
#include <time.h>
#include "Encoder.h"
#include "MyRio.h"

extern NiFpga_Session myrio_session;

#if !defined(LoopDuration)
#define LoopDuration   60  /* How long to monitor the signal, in seconds */
#endif


#if !defined(LoopStep)
#define LoopStep        5   /* How long to monitor the signal, in seconds */
#endif


/**
 * Overview:
 * Demonstrates using the encoder. Reads a step and direction signal from the
 * encoder on connector B. Prints the values to the console.
 *
 * Instructions:
 * 1. Connect a step and direction signal to encoder pins on connector B.
 * 2. Run this program.
 *
 * Output:
 * The program reads the encoder for 60 s. Encoder values are written to the
 * console every 5 s.
 *
 * Note:
 * The Eclipse project defines the preprocessor symbol for the NI myRIO-1900.
 * Change the preprocessor symbol to use this example with the NI myRIO-1950.
 */
MyRio_Encoder initEncoder(bool AorB) {

    MyRio_Encoder encA0;

    uint8_t selectReg;

    time_t currentTime;
    time_t printTime;
    time_t finalTime;

    uint32_t steps;
    const char* direction;

    /*
     * Initialize the encoder struct with registers from the FPGA personality.
     */

    if(AorB) {
	encA0.cnfg = ENCACNFG;
    	encA0.stat = ENCASTAT;
    	encA0.cntr = ENCACNTR;
    }
    else {
	encA0.cnfg = ENCBCNFG;
	encA0.stat = ENCBSTAT;
	encA0.cntr = ENCBCNTR;
    }

    /*
     * Encoder inputs are on pins shared with other onboard devices. To input
     * from a physical pin, select the encoder on the appropriate SELECT
     * register.
     *
     * Read the value of the SYSSELECTB register.
     */
    bool status = NiFpga_ReadU8(myrio_session, SYSSELECTB, &selectReg);
    //if not status, fail somehow

    /*
     * Set bit 5 of the SYSSELECTB register to enable ENCB functionality.
     * The functionality of these bits is specified in the documentation.
     */
    selectReg = selectReg | (1 << 5);

    /*
     * Write the updated value of the SYSSELECTB register.
     */
    status = NiFpga_WriteU8(myrio_session, SYSSELECTB, selectReg);
    //if not status, fail somehow

    /*
     * Enable the encoder and configure to read quadrature signals.
     */
    Encoder_Configure(&encA0, Encoder_Enable | Encoder_SignalMode,
            Encoder_Enabled | Encoder_QuadPhase);


return encA0;
}


int getEncoderCounter(MyRioEncoder encA0) {
	return Encoder_Counter(&encA0);
}

int getEncoderDirection(MyRioEncoder encA0) {
	if((Encoder_Status(&encA0) & Encoder_StDirection)== Encoder_Incrementing) {
		return 1;
	}
	else {
		return 0;
	}
}
