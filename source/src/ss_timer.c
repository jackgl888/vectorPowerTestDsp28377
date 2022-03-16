#include "ss_include.h"


interrupt void cpu_timer0_isr(void);
interrupt void cpu_timer1_isr(void);




/*********************************************************************************
 *  name :void timerInit(void)
 *  funtion : timer init 
 *  inter para :
 *  out para :
 *  note:
 ********************************************************************************/


void timerInit(void)
{
	 /*Register timer interrupt */
		 EALLOW;
	//	PieVectTable.TIMER0_INT=&cpu_timer0_isr;
	//   PieVectTable.TIMER1_INT=&cpu_timer1_isr;
		EDIS;
		/* only initialize the Cpu Timers*/
	 InitCpuTimers();	

     /*5ms timer  for  pid ctrl */
    ConfigCpuTimer(&CpuTimer0, 200, 5000);
	 /* 1 MS timer for rtos */
	ConfigCpuTimer(&CpuTimer1, 200, 1000);

    CpuTimer0Regs.TCR.all = 0x4000;
	CpuTimer1Regs.TCR.all = 0x4000;

    /*systimer init */
	sw_timer_init();


}




/*********************************************************************************
 *  name :void sw_timer_init(void)
 *  funtion : init  software timer 
 *  inter para :
 *  out para :
 *  note:
 ********************************************************************************/
void sw_timer_init(void)
{
   Uint32 i;
   //初始化软件定时器
	for(i=0;i<SWT_COUNT ;i++)	
	{
		ssSystem.swTimer[i].sta=SWT_STA_RELOAD ;
		ssSystem.swTimer[i].counter=0;
	}

	ssSystem.time=0;
}


/****************************************************************************
* 名    称：软件定时器
* 功    能：
* 入口参数： 1、-timer 定时器N  2、time 定时数据，3、重载方式(自动重载和手动加载)
* 出口参数：无
* 说    明：
****************************************************************************/
Uint16  sw_timer_call(Uint16  _timer,Uint32     time,Uint16  loadType)
{

	
	if(ssSystem.swTimer[_timer].sta==SWT_STA_RELOAD)
	{
		ssSystem.swTimer[_timer].counter=time;
		ssSystem.swTimer[_timer].sta=SWT_STA_TIMING;
	}

	else if(ssSystem.swTimer[_timer].sta==SWT_STA_TIMING)
	{
		if(ssSystem.swTimer[_timer].counter==0)
		{
			if(loadType==SWT_MODE_AUTO)
			{
				ssSystem.swTimer[_timer].counter=time;
			}
			else
			{
				ssSystem.swTimer[_timer].sta=SWT_STA_TIME_OUT;
			}
			return 1;
		}
	}

	else if(ssSystem.swTimer[_timer].sta==SWT_STA_TIME_OUT)
	{
		if(loadType==SWT_MODE_AUTO)
		{
			ssSystem.swTimer[_timer].counter=time;
			ssSystem.swTimer[_timer].sta=SWT_STA_TIMING;
		}
		else
		{
			return 1;
		}
	}
	else
	{
		if(loadType==SWT_MODE_AUTO)
		{
			ssSystem.swTimer[_timer].counter=time;
			ssSystem.swTimer[_timer].sta=SWT_STA_TIMING;
		}
		else
		{
			ssSystem.swTimer[_timer].sta=SWT_STA_RELOAD;
		}
	}
	return 0;
}









