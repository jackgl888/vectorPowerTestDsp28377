#ifndef   __SS_PWM_H__
#define  __SS_PWM_H__

#include "F28x_Project.h"     // Device Headerfile and Examples Include File

#define TB_CLK   100e6      //100*10^6

#define PWM_CLK   1e5         //5*10^3           100khz 
#define SP             TB_CLK/(2*PWM_CLK)
#define TBCTLVAL  0x200A              // Up-down cnt, timebase = SYSCLKOUT

#define DBTIME_FED      30 //上升沿死区时间，T = TBCLK*50,这里大约为500ns
#define DBTIME_RED      30 //下降沿死区时间，T = TBCLK*50,这里大约为500ns

#define  DUTYCYCLEMAX  (EPwm1Regs.TBPRD * 93 * 0.85f)    //最大占空比限制
 #define abs(x)         (x > 0 ? x : -x)

void  epwmsInit(void);


#endif

