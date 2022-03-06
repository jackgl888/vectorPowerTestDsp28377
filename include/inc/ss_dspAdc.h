#ifndef __SS_DSPADC_H__
#define __SS_DSPADC_H__
#include "F28x_Project.h"


#define RESULTS_BUFFER_SIZE 60






typedef struct 
{
    Uint16 filterMutex;    //滤波保护
	Uint16 resultsIndex;
	volatile Uint16 bufferFull;
    Uint16 curResult[RESULTS_BUFFER_SIZE];   //cur adc value 
    Uint16 volResult[RESULTS_BUFFER_SIZE];   //vol adc value 
}DSP_ADC;


void SetupADCEpwm(void);

void ConfigureADC(void);




#endif
