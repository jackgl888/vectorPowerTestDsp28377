#include "ss_minclude.h"

interrupt void xint_isr(void);

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

    ssSystem.fuction.bit.RDY = 4;
    ssSystem.fuction.bit.CHARGE = 1;
    flag = 1;

    while (1)
    {
#if 1
        if ((ssSystem.fuction.bit.RDY == 1) && (flag == 1))
        {
           // swicthToAynChargeMode();
           swicthToAynDisChargeMode();
            flag =0;
        }

        else if ((ssSystem.fuction.bit.RDY == 0) && (flag == 1))
        {
            swicthToSynDischargeMode();
            //    DELAY_US(40000);·
            //   swicthToSynChargeMode();   //ͬ���ŵ�

            flag = 0;
        }
        else if ((ssSystem.fuction.bit.RDY == 2) && (flag == 1))
        {
            EPwm1Regs.TZCLR.bit.OST = 1;
            EPwm1Regs.CMPA.bit.CMPA = 50;
            flag = 0;
        }
        else if ((ssSystem.fuction.bit.RDY == 3) && (flag == 1))
        {
            flag = 0;
            forcePwmOutputLow();

        }
        else if((ssSystem.fuction.bit.RDY == 4) && (flag == 1))
        {
           // swicthToAynChargeMode();
			//  DELAY_US(700000);
            swicthToSynChargeMode();   //ͬ�����?            flag = 0;
            flag = 0;

        }
        else if((ssSystem.fuction.bit.RDY == 6) && (flag == 1))
        {
            EPwm1Regs.CMPA.bit.CMPA = ssSystem.pwmPara.cmpValueHi;
            flag = 0;
        }
       
#endif
         GetConvertedValueOfAdc();


        //ServiceDog();

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

/*********************************************************************************
 *  name :interrupt void xint_isr(void)
 *  funtion :    adc invert  ok 
 *  inter para :
 *  out para :
 *  note:
 ********************************************************************************/

interrupt void xint_isr(void)
{

    static Uint16 temp = 0;

    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
    DELAY_US(1000);
    temp += 1;
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
