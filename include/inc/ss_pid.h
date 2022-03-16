#ifndef __SS_PID_H__
#define __SS_PID_H__
#include "F28x_Project.h"
#include "IQmathLib.h"

typedef struct  {

    float32  T;          //cycle
    float32 Kp;         //pid   scale
    float32 Ti;         //pid     intergral
    
    float32 Ref;        //standard value
    float32 Fdb;        // feedback value

    float32 a0;         //cal :   a0 = Kp(1 + T/Ti)
    float32 a1;         //cal:    a1 = Kp

    float32 Ek_0;       // cal : Error[k],  error current 
    float32 Ek_1;       //cal :Error[k-1]

    float32 Inc;        //increment           pi[k] -pi[k-1]
    float32 Output;     //pi output 

    float32 OutMax;     //PImaxvalue 
    float32 OutMin;     //PIminvalue 

}PI_STRUCT;



typedef struct {

void (*reset) ( PI_STRUCT *p);
void (*init ) ( PI_STRUCT *p);
void (*clc  ) ( PI_STRUCT *p);
void (*calc ) ( PI_STRUCT *p);

}PI_FUNC;


void pi_reset( PI_STRUCT *p);
void pi_init ( PI_STRUCT *p);
void pi_clc  ( PI_STRUCT *p);
void pi_calc ( PI_STRUCT *p);


#endif


