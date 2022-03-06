#ifndef __SS_PID_H__
#define __SS_PID_H__
#include "F28x_Project.h"
#include "IQmathLib.h"

typedef struct  {

    int32 T;          //cycle
    int32 Kp;         //pid   scale
    int32 Ti;         //pid     intergral
    
    int32 Ref;        //standard value
    int32 Fdb;        // feedback value

    int32 a0;         //cal :   a0 = Kp(1 + T/Ti)
    int32 a1;         //cal:    a1 = Kp

    int32 Ek_0;       // cal : Error[k],  error current 
    int32 Ek_1;       //cal :Error[k-1]

    int32 Inc;        //increment           pi[k] -pi[k-1]
    int32 Output;     //pi output 

    int32 OutMax;     //PImaxvalue 
    int32 OutMin;     //PIminvalue 

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


