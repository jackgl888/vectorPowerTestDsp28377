#ifndef __SS_FILTER_H__
#define __SS_FILTER_H__
#include "F28x_Project.h"




typedef struct 
{

  float32  channelValAvg[ADCNUM];  //鍚勯�氶亾骞冲潎鍊�
  float32  singleChannelVal[ADCVAL_BUFFER_SIZE];  //
  
}ADCFILTER;



void filterInit(void);

void  arrayBubbleSorting(Uint16 *array_addr,Uint16 len);

void  filterAdcChannels(void);

#endif
