#ifndef __SS_TP_H__
#define __SS_TP_H__

#include "F28x_Project.h"


/*通信状态结构体*/
typedef struct 
{
 
	Uint16 comOfflineCnt;	//累加设为通讯异常

	Uint16 commStatus;

	Uint16 commOffLine;   //离线标志

	Uint16 commErr;   //离线标志
  
}MCLC_COMMUNICATION;


/**/



#endif 