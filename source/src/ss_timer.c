#include "ss_timer.h"

interrupt void cpu_timer0_isr(void);
interrupt void cpu_timer1_isr(void);



/**
 *******************************************************************************
 *  鎻忚堪: 瀹氭椂鍣�0鍒濆鍖�
 *  杈撳叆: 鏃�
 *  杩斿洖: 鏃�
 *  浣滆��:
 *  鐗堟湰:
 *******************************************************************************
 */

void timerInit(void)
{
	 /*娉ㄥ唽瀹氭椂鍣ㄤ腑鏂�*/
		 EALLOW;//闁告劖鐟ょ换姘跺箮閿�?
		PieVectTable.TIMER0_INT=&cpu_timer0_isr;
	   PieVectTable.TIMER1_INT=&cpu_timer1_isr;
		EDIS;
		/* only initialize the Cpu Timers*/
	 InitCpuTimers();	

     /*5ms timer  for  pid ctrl */
    ConfigCpuTimer(&CpuTimer0, 200, 5000);
	 /* 1 MS timer for rtos */
	ConfigCpuTimer(&CpuTimer1, 200, 1000);

    CpuTimer0Regs.TCR.all = 0x4000;
	CpuTimer1Regs.TCR.all = 0x4000;



}










