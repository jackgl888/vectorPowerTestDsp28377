
#include "ss_include.h"






/*********************************************************************************
 *  name :void main(void)
 *  funtion : main 
 *  inter para :
 *  out para :
 *  note:
 ********************************************************************************/


void main(void)
{
   static Uint16 flag = 0;
    /* dsp   init */
	dspSystemInit();
	ssSystem.fuction.bit.RDY = 0;
	flag = 1;
	while(1)
	{
          #if 1
	       if((ssSystem.fuction.bit.RDY == 1 ) && (flag == 1))
		   	   swicthToChargeMode();
		   else if((ssSystem.fuction.bit.RDY == 0) && (flag == 1))
		   	{
		   	 	   swicthToDischargeMode();
				   DELAY_US(40000);
				   swicthToSyncMode();
				flag = 0;
		   	}
		    else if((ssSystem.fuction.bit.RDY == 2) && (flag == 1))
		   	{
		    	swicthToSyncMode();
			    flag =0;
		   	}

			else if((ssSystem.fuction.bit.RDY == 3) && (flag == 1))
			{

			   flag =0;
		      forcePwmOutputLow();
			 //  dr_EpwmsSrcTZ(&EPwm1Regs);//���ǿ��PWM1����͵��?	}
			/*�õ�ADCת����ֵ*/
	           GetConvertedValueOfAdc();  
           #endif

	//    	//
	//        EPwm1Regs.ETSEL.bit.SOCAEN = 1;  //浣胯�?SOCA
	//        EPwm1Regs.TBCTL.bit.CTRMODE = 0; //unfreeze, and enter up count mode
	//
	//    	//wait while ePWM causes ADC conversions, which then cause interrupts,
	//    	//which fill the results buffer, eventually setting the bufferFull
	//    	//flag
	//    	while(!ssSystem.dspAdc.bufferFull);
	//    	ssSystem.dspAdc.bufferFull = 0; //clear the buffer full flag
	//
	//    	//stop ePWM
	//        EPwm1Regs.ETSEL.bit.SOCAEN = 0;  //disable SOCA
	//       EPwm1Regs.TBCTL.bit.CTRMODE = 3; //freeze counter
            ;
	     }

    }

}















/*********************************************************************************
 *  name :interrupt void cpu_timer0_isr(void)
 *  funtion : timer0 interupt  handle 
 *  inter para :
 *  out para :
 *  note:
 ********************************************************************************/




/*********************************************************************************
 *   name : interrupt void cpu_timer1_isr(void)
 *   explain :timer for program
 *   enter para :
 *   out para::
 *   note:
 ********************************************************************************/






//===========================================================================
// No more.
//===========================================================================

