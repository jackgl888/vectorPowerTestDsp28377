#include "ss_system.h"


SS_SYSTEM  ssSystem;




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


    GPIO_SetupPinMux(4, GPIO_MUX_CPU1, 0);
    GPIO_SetupPinOptions(4, GPIO_OUTPUT, GPIO_PUSHPULL);
    GPIO_SetupPinMux(5, GPIO_MUX_CPU1, 0);
    GPIO_SetupPinOptions(5, GPIO_OUTPUT, GPIO_PUSHPULL);
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
    Uint16  i ;

    ssSystem.piFunc.reset = pi_reset;
    ssSystem.piFunc.init =  pi_init;
    ssSystem.piFunc.clc = pi_clc;
    ssSystem.piFunc.calc =  pi_calc ;

    for( i = 0; i < RESULTS_BUFFER_SIZE;i++)
    {
        ssSystem.dspAdc.curResult [i ] = 0;
        ssSystem.dspAdc.volResult[i] = 0;
    }
    ssSystem.dspAdc.resultsIndex = 0;
    ssSystem.dspAdc. bufferFull = 0;


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
      ConfigureADC();

      /*pwm init*/
	  epwmsInit();

      /*timer init*/
	  timerInit();

      /*goble para init */
      paraInit();

	  /*scic485init */
	  scicRs485Init( BAUDRATE);
 }

