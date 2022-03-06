#ifndef __SS_SYSTEM_H__
#define __SS_SYSTEM_H__



#include "ss_include.h"     // Device Headerfile and Examples Include File




typedef enum
{
  FALSE =0,
  TRUE = 1
}Bool;


typedef struct
{



  DSP_ADC    dspAdc;  // 28337 adc数据结构
  PI_FUNC	 piFunc;


}SS_SYSTEM;







void  paraInit(void);

void lcBoxInit(void);


extern   SS_SYSTEM  ssSystem;
#endif





