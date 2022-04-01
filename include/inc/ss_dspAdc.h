#ifndef __SS_DSPADC_H__
#define __SS_DSPADC_H__
#include "F28x_Project.h"


#define ADCVAL_BUFFER_SIZE 100
#define  ARRAY_STARTING_POS   15
#define  ARRAY_ENDING_POS     45
#define  ARRAY_FILTER_SIZE    60

#define  ARRAY_AVG_LEN       (ARRAY_ENDING_POS- ARRAY_STARTING_POS )

#define RESULTS_BUFFER_SIZE 100


#define  ADCNUM              3
#define  OUT_CUR             0    //output current 
#define  BAT_VOL             1     //baterry  voltage
#define  PORT_VOL            2   //port   voltage 


//#define  INCURRENT         3


typedef struct 
{
  
	Uint16 resultsIndex;
	volatile Uint16 bufferFull;
	volatile Uint16 adcData0[RESULTS_BUFFER_SIZE];
    Uint16 adcValue[ADCNUM ][ ADCVAL_BUFFER_SIZE];   //cur adc value 
 
}DSP_ADC;



void dmaInit(void);

void SetupADCEpwm(void);

void  adcInit(void);  //test

void  GetConvertedValueOfAdc(void);




#endif
