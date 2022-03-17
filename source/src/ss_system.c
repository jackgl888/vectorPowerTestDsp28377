
#include "ss_include.h"     // Device Headerfile and Examples Include File



SS_SYSTEM  ssSystem;

__interrupt void epwm1_tzint_isr(void);



/*********************************************************************************
 *  name :void GPIO_Setup(void)
 *  funtion : init all gpio
 *  inter para :
 *  out para :
 *  note:
 ********************************************************************************/
void GPIO_Setup(void)
{

	//InitGpio(); 


    GPIO_SetupPinMux(2, GPIO_MUX_CPU1, 0);
    GPIO_SetupPinOptions(2, GPIO_OUTPUT, GPIO_PUSHPULL);
    GPIO_SetupPinMux(3, GPIO_MUX_CPU1, 0);
    GPIO_SetupPinOptions(3, GPIO_OUTPUT, GPIO_PUSHPULL);
}



/*********************************************************************************
 *  name :void  paraInit(void)
 *  funtion : init all para
 *  inter para :
 *  out para :
 *  note:
 ********************************************************************************/
void  paraInit(void)
{
    
    ssSystem.piFunc.reset = pi_reset;
    ssSystem.piFunc.init =  pi_init;
    ssSystem.piFunc.clc = pi_clc;
    ssSystem.piFunc.calc =  pi_calc ;

	ssSystem.outCurInfo.Coeff= (0.0066*3000.0)/4096;      //
	ssSystem.batVolInfo.Coeff= (3.0*3.0)/4096;    //
	ssSystem.portVolInfo.Coeff = (3.0/4096);

	
}


/*********************************************************************************
 *  name :void  paraInit(void)
 *  funtion : init all para
 *  inter para :
 *  out para :
 *  note:
 ********************************************************************************/
 void   lcBoxInit(void)
 {

       GPIO_Setup();
	   
      /*ADC��ʼ��*/
       adcInit();

       /*filter init */
	   filterInit();
	 
      /*pwm init*/
	  epwmsInit();

      /*timer init*/
	  timerInit();

      /*goble para init */
      paraInit();

      /*pid para init */
	  buckBoostCtrlInit();

	  /*scic485init */
	  scicRs485Init( BAUDRATE);
 }



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
     //   PieVectTable.EPWM1_TZ_INT =    &epwm1_tzint_isr;
	  //  PieCtrlRegs.PIEIER2.bit.INTx1 = 1;
	   EALLOW;
	   CpuSysRegs.PCLKCR0.bit.TBCLKSYNC = 1;
	
	
	  /*enable int	group 1 and 13*/
	   IER |= M_INT1; //ADC
	   IER |= M_INT13;    //TIMER0
	   IER |=  M_INT8; //SCIC
	   EINT;   // Enable Global __interrupt INTM
	   ERTM;   // Enable Global realtime __interrupt DBGM

}



__interrupt void 
epwm1_tzint_isr(void)
{
     Uint16 a = 0 ;

	 a++;

    //
    // Leave these flags set so we only take this
    // interrupt once
    //
    // 	EALLOW;
    // 	EPwm1Regs.TZCLR.bit.OST = 1;
    // 	EPwm1Regs.TZCLR.bit.INT = 1;
    //	EDIS;

    //
    // Acknowledge this interrupt to receive more interrupts from group 2
    //
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP2;
}

