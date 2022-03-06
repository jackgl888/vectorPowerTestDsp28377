
#include "ss_include.h"


interrupt void adca1_isr(void);
interrupt void cpu_timer0_isr(void);
interrupt void cpu_timer1_isr(void);



/*********************************************************************************
 *  name :void dspSystemInit(void)
 *  funtion : init all peripheral
 *  inter para :
 *  out para :
 *  note:
 ********************************************************************************/
void dspSystemInit(void)
{
	/* Step 1. Initialize System Control, PLL, WatchDog, enable Peripheral Clocks*/
	   InitSysCtrl();
	
	/*This example function is found in the F2837xS_Gpio.c file and*/
	//	 InitEQep1Gpio();

	
	/*Clear all __interrupts and initialize PIE vector table:// Disable CPU __interrupts*/
	   DINT;
	
	/* Initialize the PIE control registers to their default state.*/
	   InitPieCtrl();
	
	/* Disable CPU __interrupts and clear all CPU __interrupt flags:*/
	   IER = 0x0000;
	   IFR = 0x0000;
	
	/* Initialize the PIE vector table with pointers to the shell Interrupt*/
	   InitPieVectTable();
	
	
	   /*init personal set*/
	   lcBoxInit();
	
	        /*int group 1and 7*/
	    PieCtrlRegs.PIEIER1.bit.INTx1 = 1;  //ADC
		PieCtrlRegs.PIEIER1.bit.INTx7 = 1;  //TIMER0
        PieCtrlRegs.PIEIER8.bit.INTx5= 1;     // SCIC
 
	
		EALLOW;
		CpuSysRegs.PCLKCR0.bit.TBCLKSYNC = 1;
	
	
	  /*enable int	group 1 and 13*/
	   IER |= M_INT1; //ADC
	   IER |= M_INT13;    //TIMER0
	   IER |=  M_INT8; //SCIC
	   EINT;   // Enable Global __interrupt INTM
	   ERTM;   // Enable Global realtime __interrupt DBGM

}



/*********************************************************************************
 *  ��������void main(void)
 *  ����: ������
 *  ����: ��
 *  ����: ��
 *  ����:
 *  �汾:
 ********************************************************************************/

void main(void)
{

    /* dsp   init */
	dspSystemInit();


	while(1)
	{
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
 *  name :interrupt void adca1_isr(void)
 *  funtion : adc interrupt handle 
 *  inter para :
 *  out para :
 *  note:
 ********************************************************************************/

interrupt void adca1_isr(void)
{
    static Uint16 index =0;

    ssSystem.dspAdc.curResult[index] = AdcaResultRegs.ADCRESULT0;
    ssSystem.dspAdc.volResult[index]=AdcaResultRegs.ADCRESULT1;

    if(RESULTS_BUFFER_SIZE <= index)
    {
       index =0;


        ssSystem.dspAdc.bufferFull = 1;
    }
   index++;
    AdcaRegs.ADCINTFLGCLR.bit.ADCINT1 = 1; //clear INT1 flag
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}





/*********************************************************************************
 *  name :interrupt void cpu_timer0_isr(void)
 *  funtion : timer0 interupt  handle 
 *  inter para :
 *  out para :
 *  note:
 ********************************************************************************/


interrupt void cpu_timer0_isr(void)
{
   CpuTimer0.InterruptCount++;

   // Acknowledge this interrupt to receive more interrupts from group 1
   PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;

   GpioDataRegs.GPATOGGLE.bit.GPIO4 = 1;
   GpioDataRegs.GPATOGGLE.bit.GPIO5 = 1;
}



/*********************************************************************************
 *   name : interrupt void cpu_timer1_isr(void)
 *   explain :timer for program
 *   enter para :
 *   out para::
 *   note:
 ********************************************************************************/


interrupt void cpu_timer1_isr(void)
{
      static Uint16  a = 0 ;
	  
    a++;  
}



//===========================================================================
// No more.
//===========================================================================

