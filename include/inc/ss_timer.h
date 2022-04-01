

#ifndef __SS_TIMER_H__
#define __SS_TIMER_H__
#include "F28x_Project.h"  



#define  SWT_COUNT   40

#define  SWT_STA_RELOAD     	0x01
#define  SWT_STA_TIMING       	0x02
#define  SWT_STA_TIME_OUT      	0x03

#define  SWT_MODE_AUTO          0x01
#define  SWT_MODE_MANU          0x00


typedef struct {

	//operating  mode 
    Uint16   mode ;
	
	// 
	Uint16 sta;
	
	//timer 1 as the benchmark
	volatile Uint32 counter;

}SOFTWARETIMER;



#define  sw_timer_reset(_timer)     		 ssSystem .swTimer[_timer].sta=SWT_STA_RELOAD

//需要手动立即加载的定时器
#define  sw_timer_load(_timer,time)      	 ssSystem.swTimer[_timer].sta=SWT_STA_TIMING;\
								  	        ssSystem.swTimer[_timer].counter=time

//禁止定时器的启动
#define  sw_timer_disable(_timer)           ssSystem.swTimer[_timer].sta=SWT_STA_TIME_OUT


//软件模拟定时器初始化
void sw_timer_init(void);


//软件定时器的使用接口
Uint16  sw_timer_call(Uint16  _timer,Uint32 time,Uint16  loadType);


void timerInit(void);


// .hhhh

Uint16	ssDelayMs(Uint16 time);



#endif
