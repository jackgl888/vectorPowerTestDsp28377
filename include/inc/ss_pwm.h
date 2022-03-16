#ifndef   __SS_PWM_H__
#define  __SS_PWM_H__

#include "F28x_Project.h"     // Device Headerfile and Examples Include File

#define TB_CLK   100e6      //100*10^6

#define PWM_CLK   1e5         //5*10^3           100khz
#define SP             (TB_CLK/(2*PWM_CLK))

#define TBCTLVAL  0x200A              // Up-down cnt, timebase = SYSCLKOUT

#define DBTIME_FED      30 //涓婂崌娌挎鍖烘椂闂达紝T = TBCLK*50,杩欓噷澶х害涓�500ns
#define DBTIME_RED      30 //涓嬮檷娌挎鍖烘椂闂达紝T = TBCLK*50,杩欓噷澶х害涓�500ns

#define  DUTYCYCLEMAX        (SP  * 0.80f)  // (EPwm1Regs.TBPRD * 56 * 0.40f)    //  500*56*0.4= 11200
#define  DUTYCYCLEMIN        (SP   * 0.50f)   //(EPwm1Regs.TBPRD * 56 * 0.01f)    //    4650
#define  MEP_SCALE_FACTOR     56             //  MEP  steps    per epwmclk

#define abs(x)         (x > 0 ? x : -x)



typedef struct 
{

	Uint16 cmpValueHi;  //高16位
	Uint16 cmpValueLi;  //低16位
	float32 cmpValue;   //实际写入占空比
    float32  calDuty;  //pid计算值占空比
    float32  floatPart; 

}PWMPARA;





void  epwmsInit(void);

void dr_EpwmsSrcTZ(volatile struct EPWM_REGS *EPwmRegs);
void dr_EpwmsClrTZ(volatile struct EPWM_REGS *EPwmRegs);
void dr_Epwm1Upate(int32 PWM_DUTY_CMP);
void swicthToDischargeMode(void);
void swicthToChargeMode(void);
void swicthToSyncMode(void);
void  forcePwmOutputLow(void);


#endif

